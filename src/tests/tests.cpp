#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ranges>
#include <iostream>
#include <functional>
#include <string>

#include <conveyor/enumerate>

template <typename ...F>
auto compose(F ...);

template <typename F>
auto compose(F&& f)
{
	return std::forward<F>(f);
}

// Shitty test case for demo, please ignore
TEST_CASE("Enumerate view")
{
	using std::vector;
	using std::cerr;
	using std::endl;

	std::vector<int> range = {100, 1, 2, 3, 4, 5, 10, 26, 69, 420};

	auto filter_secondValueIsEven = [](auto value) -> bool {
		return value.second % 2 == 0;
	};

	auto switchTupleOrder = [](auto const value) {
		return std::pair<int &, std::size_t>(value.second, value.first);
	};

	using namespace std::string_literals;

	std::string s = "range";

	for(auto x : range
	                //| std::views::filter([](auto value){return value % 2 == 0;})
	                //| std::views::transform([](auto value){if(value % 2 == 0) return value; return 2*value;})
	                //| std::views::transform([](char value){return char(value + ('A' - 'a'));})
	                | conveyor::enumerate
	                //| std::views::filter(filter_secondValueIsEven)
	                //| std::views::transform(switchTupleOrder)
	                //| std::views::transform([](auto x){return x;})
	                | std::views::reverse
	                //| std::views::drop(3)
	                //| std::views::take(3)
	    ) {
		cerr << x.first << "\t" << x.second << endl;
		//cerr << x << endl;
	}
}
