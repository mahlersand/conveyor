#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <ranges>
#include <iostream>
#include <functional>

#include <conveyor/enumerate>

// Shitty test case for demo, please ignore
TEST_CASE("Enumerate view")
{
	using std::vector;
	using std::cerr;
	using std::endl;

	std::vector<int> range = {0, 1, 2, 3, 4, 5, 10, 26, 69, 420};

	auto filter_secondValueIsEven = [](auto value) -> bool {
		return value.second % 2 == 0;
	};

	auto switchTupleOrder = [](auto const value) {
		return std::pair<int &, std::size_t>(value.second, value.first);
	};

	for(auto x : range
	                | conveyor::enumerate
	                | std::views::filter(filter_secondValueIsEven)
	                | std::views::transform(switchTupleOrder)
	                | std::views::reverse
	    ) {
		cerr << x.first << "\t" << x.second << endl;
	}
}
