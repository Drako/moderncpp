#include "list.hxx"

int bar() {
	workshop::List<int> empty{};
	empty.push_front(23);
	return empty.size();
}
