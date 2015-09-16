#ifndef CPP_UNROLL_H
#define CPP_UNROLL_H

#include <type_traits>
#include <utility>

namespace cpp_unroll_helper 
{

template<int N>
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

template<int N, typename = void>
struct unrolled_for_remainder
{
	template<typename F>
	static void run(F& f, int times)
	{
		for (int i = 0; i < times % N; ++i)
			f();
	}
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

template<int N>
struct unrolled_for_remainder<N, std::enable_if_t<(N <= 4)>>
{
	template<typename F>
	static void run(F& f, int times)
	{
		switch (times % N)
		{
			CPP_UNROLL_REM_4(f);
		}
	}
};

template<int N>
struct unrolled_for_remainder<N, std::enable_if_t<(N > 4 && N <= 8)>>
{
	template<typename F>
	static void run(F& f, int times)
	{
		switch (times % N)
		{
			CPP_UNROLL_REM_8(f);
		}
	}
};

template<int N>
struct unrolled_for_remainder<N, std::enable_if_t<(N > 8 && N <= 16)>>
{
	template<typename F>
	static void run(F& f, int times)
	{
		switch (times % N)
		{
			CPP_UNROLL_REM_16(f);
		}
	}
};

template<int N>
struct unrolled_for_runner
{
	template<typename F>
	static void run(F& f, int times)
	{
		for (int i = 0; i < times / N; ++i)
			unrolled_for_inner<N>::run(f);
		if (times > 0)
			unrolled_for_remainder<N>::run(f, times);
	}
};

} // cpp_unroll_helper

namespace cpp_unroll
{

template<int N, typename F>
void unrolled_for(int times, F f)
{
	static_assert(N > 0, "");
	cpp_unroll_helper::unrolled_for_runner<N>::run(f, times);
}

}

#endif // CPP_UNROLL_H