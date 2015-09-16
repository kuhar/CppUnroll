#include <iostream>

#include "cpp_unroll.h"

using namespace cpp_unroll;

int main()
{
	int n;
	std::cin >> n;
	unrolled_for<8>(n, [] {
		std::cout << "elo\n";
	});
}