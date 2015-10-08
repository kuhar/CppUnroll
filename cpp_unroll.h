#ifndef CPP_UNROLL_H
#define CPP_UNROLL_H

#include <type_traits>
#include <iterator>
#include <utility>
#include <cstdint>
#include <cstddef>
#include <initializer_list>

namespace cpp_unroll_helper 
{

using IntegerType = std::conditional_t<
	(sizeof(ptrdiff_t) == sizeof(int64_t)), int64_t, int>;

template<IntegerType N>
struct unrolled_for_inner
{
	template<typename F>
	static void run(F& f)
	{
		f();
		unrolled_for_inner<N - 1>::run(f);
	}
};

template<>
struct unrolled_for_inner<0>
{
	template<typename F>
	static void run(F&) {}
};

#define CPP_UNROLL_REM_4(FUNCTOR_N) \
		case 3:  FUNCTOR_N(); \
		case 2:  FUNCTOR_N(); \
		case 1:  FUNCTOR_N()

#define CPP_UNROLL_REM_8(FUNCTOR_N) \
		case 7:  FUNCTOR_N(); \
		case 6:  FUNCTOR_N(); \
		case 5:  FUNCTOR_N(); \
		case 4:  FUNCTOR_N(); \
		CPP_UNROLL_REM_4(FUNCTOR_N)

#define CPP_UNROLL_REM_16(FUNCTOR_N) \
		case 15:  FUNCTOR_N(); \
		case 14:  FUNCTOR_N(); \
		case 13:  FUNCTOR_N(); \
		case 12:  FUNCTOR_N(); \
		case 11:  FUNCTOR_N(); \
		case 10:  FUNCTOR_N(); \
		case 9:   FUNCTOR_N(); \
		case 8:   FUNCTOR_N(); \
		CPP_UNROLL_REM_8(FUNCTOR_N)

template<IntegerType N, typename = void>
struct unrolled_for_remainder
{
	template<typename F>
	static void run(F& f, IntegerType times)
	{
		for (IntegerType i = 0; i < times % N; ++i)
			f();
	}
};

template<IntegerType N>
struct unrolled_for_remainder<N, std::enable_if_t<(N <= 4)>>
{
	template<typename F>
	static void run(F& f, IntegerType times)
	{
		switch (times % N)
		{
			CPP_UNROLL_REM_4(f);
		}
	}
};

template<IntegerType N>
struct unrolled_for_remainder<N, std::enable_if_t<(N > 4 && N <= 8)>>
{
	template<typename F>
	static void run(F& f, IntegerType times)
	{
		switch (times % N)
		{
			CPP_UNROLL_REM_8(f);
		}
	}
};

template<IntegerType N>
struct unrolled_for_remainder<N, std::enable_if_t<(N > 8 && N <= 16)>>
{
	template<typename F>
	static void run(F& f, IntegerType times)
	{
		switch (times % N)
		{
			CPP_UNROLL_REM_16(f);
		}
	}
};

template<IntegerType N>
struct unrolled_for_runner
{
	template<typename F>
	static void run(F& f, IntegerType times)
	{
		for (IntegerType i = 0; i < times / N; ++i)
			unrolled_for_inner<N>::run(f);
		if (times > 0)
			unrolled_for_remainder<N>::run(f, times);
	}
};

} // cpp_unroll_helper

namespace cpp_unroll
{

using cpp_unroll_helper::IntegerType;

template<IntegerType N, typename F>
void unrolled_for(IntegerType times, F f)
{
	static_assert(N > 0, "");
	cpp_unroll_helper::unrolled_for_runner<N>::run(f, times);
}

template<typename N, typename F>
void unrolled_for(N n, IntegerType times, F f)
{
	constexpr IntegerType Factor(n);
	static_assert(Factor > 0, "");
	cpp_unroll_helper::unrolled_for_runner<Factor>::run(f, times);
}

template<IntegerType N, typename R, typename F>
auto unrolled_for(R&& range, F f) -> decltype(
        std::begin(std::declval<std::remove_reference_t<R>>()),
        std::end(std::declval<std::remove_reference_t<R>>()),
        (void) 0)
{
	static_assert(N > 0, "");
    auto it = std::begin(range);
    auto e = std::end(range);
    const auto n = std::distance(it, e);

    auto functor = [&it, &f] {
            f(*it);
            ++it;
    };
    cpp_unroll_helper::unrolled_for_runner<N>::run(functor, n);  
}

template<typename  N, typename R, typename F>
auto unrolled_for(N n, R&& range, F f) -> decltype(
        std::begin(std::declval<std::remove_reference_t<R>>()),
        std::end(std::declval<std::remove_reference_t<R>>()),
        (void) 0)
{
    constexpr IntegerType Factor = IntegerType(n);
	static_assert(Factor > 0, "");
    unrolled_for<Factor>(std::forward<R>(range), f);  
}

template<IntegerType N, typename T, typename F>
void unrolled_for(std::initializer_list<T> list, F f)
{
	static_assert(N > 0, "");
    auto it = list.begin();
    auto e = list.end();
    const auto n = std::distance(it, e);

    auto functor = [&it, &f] {
            f(*it);
            ++it;
    };
    cpp_unroll_helper::unrolled_for_runner<N>::run(functor, n);  
}

template<typename  N, typename T, typename F>
void unrolled_for(N n, std::initializer_list<T> list, F f)
{
    constexpr IntegerType Factor = IntegerType(n);
	static_assert(Factor > 0, "");
    unrolled_for<Factor>(std::move(list), f);  
}

} // namespace cpp_unroll

#endif // CPP_UNROLL_H
