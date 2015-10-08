# CppUnroll
Tiny C++(14) loop unrolling library

### Examples:

```cpp
#include <iostream>
#include <boost/hana> // optional for the 3rd example
#include "cpp_unroll.h"

int main() {
	using namespace cpp_unroll;
	using namespace boost::hana;
	
	int n;
	std::cin >> n;
	
	unrolled_for<8>(n, [] {
		std::cout << "no\n";
	});

	unrolled_for(std::integral_constant<int, 6>{}, n, [] {
		std::cout << "elo\n";
	});
	
	unrolled_for(16_c, n, [] { // hana's integral constant
		std::cout << "!\n";
	});
	
	// range for support!
	std::vector<int> numbers = {1, 2, 3, 4, 5};
	unrolled_for<4>(numbers, [] (int x) {
		std::cout << x << ' ';
	});
    
	unrolled_for(2_c, {3.14, -2.0, 12.56}, [] (double x) {
		std::cout << ' ' << x;
	});
}
```
