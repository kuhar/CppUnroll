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
}
```
