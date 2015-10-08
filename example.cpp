#include <iostream>
#include <vector>
#include "cpp_unroll.h"

using namespace cpp_unroll;

int main()
{
	int n;
	std::cin >> n;
	unrolled_for<8>(n, [] {
		std::cout << "no\n";
	});

	unrolled_for(std::integral_constant<int, 6>{}, n, [] {
		std::cout << "elo\n";
	});

    std::vector<int> numbers = { 1, 2, 3, 4, 5 };
    unrolled_for<4>(numbers, [] (int x) {
        std::cout << x << ' ';
    });
    
    unrolled_for(std::integral_constant<int, 2>{}, {3.4, 54.3, -1.3}, [] (float x) {
        std::cout << ' ' << x;
    });
}
