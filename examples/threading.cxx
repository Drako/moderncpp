// low level threading
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

// high level abstraction
#include <future>

#include <chrono>
using namespace std::literals::chrono_literals;

#include <iostream>
#include <string>
#include <vector>
#include <numeric>

/**
 * @brief calculate the average of [numbers] in a parallel manner
 * @param numbers the input numbers, potentially a lot
 * @param numWorkers the number of worker threads to use
 * @result The average of the numbers
 */
double average(std::vector<double> const& numbers, int numWorkers) {
	std::mutex ioMutex;

	int chunkSize = numbers.size() / numWorkers;
	int lastChunkSize = chunkSize + (numbers.size() % numWorkers);

	std::vector<std::future<double>> workers;
	workers.reserve(numWorkers);

	for (int n = 0; n < numWorkers; ++n) {
		int const currentChunkSize = (n == numWorkers - 1) ? lastChunkSize : chunkSize;
		workers.emplace_back(std::async(
			std::launch::async,
			[&numbers, start=n*chunkSize, currentChunkSize, &ioMutex] {
				{
					std::lock_guard<std::mutex> lock{ioMutex};
					// ----------------------------
					std::cout << "Worker #"
						<< std::this_thread::get_id()
						<< " working on "
						<< currentChunkSize
						<< " elements."
						<< std::endl;
				}
				double sum{0.0};
				for (int idx = start; idx < start + currentChunkSize; ++idx) {
					sum += numbers[idx];
				}
				return sum;
			}
		));
	}

	return std::accumulate(
		workers.begin(),
		workers.end(),
		0.0,
		[](double acc, std::future<double>& current) {
			return acc + current.get();
		}
	) / numbers.size();
}

#include <array>

void dining_philosophers() {
	std::mutex ioMutex;
	using fork = std::mutex;
	using philosopher = std::thread;

	auto const behaviour = [&ioMutex](fork& left, fork& right) {
		for (int n = 0; n < 5; ++n) {
			/*
			std::lock(left, right);
			std::lock_guard ll1{ left, std::adopt_lock };
			std::lock_guard ll2{ right, std::adopt_lock };

			// semi new
			std::scoped_lock l1{ left, std::defer_lock };
			std::scoped_lock l2{ right, std::defer_lock };
			std::lock(l1, l2);
			*/

			// new
			std::scoped_lock lock{ left, right };

			{
				std::lock_guard ioLock{ ioMutex };
				std::cout << std::this_thread::get_id() << " is eating now." << std::endl;
			}

			std::this_thread::sleep_for(200ms);

			{
				std::lock_guard ioLock{ ioMutex };
				std::cout << std::this_thread::get_id() << " is done eating for now." << std::endl;
			}
		}
	};

	std::array<fork, 5> forks{};
	std::array<philosopher, 5> philosophers{
		philosopher { behaviour, std::ref(forks[0]), std::ref(forks[1]) },
		philosopher { behaviour, std::ref(forks[1]), std::ref(forks[2]) },
		philosopher { behaviour, std::ref(forks[2]), std::ref(forks[3]) },
		philosopher { behaviour, std::ref(forks[3]), std::ref(forks[4]) },
		philosopher { behaviour, std::ref(forks[0]), std::ref(forks[4]) },
	};

	for (auto & p : philosophers) {
		p.join();
	}
}

enum class CurrentThread {
	Sender,
	Receiver
};

void echo() {
	std::atomic<CurrentThread> current = CurrentThread::Sender;
	std::mutex mutex;
	std::condition_variable cv;

	std::string data;

	std::thread receiver{ [&] {
		for (int n = 0; n < 5; ++n) {
			std::unique_lock lock{ mutex };
			cv.wait(lock, [&current] { return current == CurrentThread::Receiver; });

			std::cout << "Received: " << data << std::endl;

			current = CurrentThread::Sender;
			cv.notify_one();
		}
	} };

	std::thread sender{ [&] {
		for (int n = 0; n < 5; ++n) {
			std::unique_lock lock{ mutex };
			cv.wait(lock, [&current] { return current == CurrentThread::Sender; });

			data = "Current iteration = " + std::to_string(n);

			current = CurrentThread::Receiver;
			cv.notify_one();
		}
	} };

	sender.join();
	receiver.join();
}

#include <deque>
#include <initializer_list>

struct WriteChannel {
	virtual ~WriteChannel() = default;

	virtual void send_values(std::initializer_list<int> values) = 0;
};

struct ReadChannel {
	virtual ~ReadChannel() = default;

	virtual int receive_value() = 0;
};

class Channel
	: public WriteChannel
	, public ReadChannel
{
public:
	void send_values(std::initializer_list<int> values) override {
		{
			std::scoped_lock lock{ mutex };

			for (int const v : values) {
				data.push_back(v);
			}
		}

		cv.notify_all();
	}

	int receive_value() override {
		std::unique_lock lock{ mutex };
		cv.wait(lock, [this] { return !data.empty(); });

		int const currentNumber = data.front();
		data.pop_front();

		return currentNumber;
	}

private:
	std::mutex mutex;
	std::condition_variable cv;

	std::deque<int> data;
};

void producer_subscriber() {
	Channel chan;
	ReadChannel * reader = &chan;
	WriteChannel * writer = &chan;

	std::thread producer{[writer]{
		for (int n = 0; n < 25; ++n) {
			std::cout << std::this_thread::get_id() << ": Producing 4 numbers..." << std::endl;
			writer->send_values({ n * 4, n * 4 + 1, n * 4 + 2, n * 4 + 3 });

			std::this_thread::sleep_for(50ms);
		}
	} };

	std::vector<std::thread> subscribers;
	subscribers.reserve(5);

	for (int n = 0; n < 5; ++n) {
		subscribers.emplace_back([reader] {
			for (int m = 0; m < 20; ++m) {
				int const currentNumber = reader->receive_value();
				std::cout << std::this_thread::get_id() << ": Got number " << currentNumber << std::endl;

				std::this_thread::sleep_for(200ms);
			}
		});
	}

	producer.join();
	for (auto & s : subscribers) {
		s.join();
	}
}

int main() {
	echo();
	dining_philosophers();

	auto const result = average({ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 }, 4);
	std::cout << "The average is: "
		<< result
		<< std::endl;

	/*
	std::future<int> answer = std::async(std::launch::async, []() {
		std::cout << "calculating..." << std::endl;
		std::this_thread::sleep_for(1s);
		std::cout << "done." << std::endl;
		return 42;
	});

	std::cout << "Waiting for result..." << std::endl;
	int const result = answer.get();
	std::cout << "The answer to the ultimate question of life, the universe and everything is: " << result << std::endl;
	*/

	producer_subscriber();

	std::cin.get();
	return 0;
}
