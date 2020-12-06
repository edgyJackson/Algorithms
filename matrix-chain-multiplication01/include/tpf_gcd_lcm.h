#ifdef _MSC_BUILD
#pragma once
#endif

#ifndef _TPF_GCD_LCM_H
#define _TPF_GCD_LCM_H

/*
	Programmed by: Thomas Kim
	First Created: Nov. 26, 2017
	
	http://www.TalkPlayFun.com
*/

#include <utility>
#include <deque>
#include <functional>
#include <vector>
#include <type_traits>

namespace tpf
{
	// returns positive remainder
	template<typename T>
	inline T modulo_positive(T a, T m)
	{
		a %= m;
		return ((a < 0) ? a + m : a);
	}

	// make positive remainder
	template<typename T>
	inline void modulo_make_positive(T& a, T m)
	{
		a %= m; if (a < 0) a += m;
	}
	
	// returns negative remainder
	template<typename T>
	inline T modulo_negative(T a, T m)
	{
		a %= m;
		return ((a < 0) ? a : a - m);
	}

	// make negative remainder
	template<typename T>
	inline void modulo_make_negative(T& a, T m)
	{
		a %= m; if (a > 0) a -= m;
	}
	
	// returns addative inverse
	template<typename T>
	inline T modulo_addative_inverse(T a, T m)
	{
		// make positive and normalize
		a %= m; if (a < 0) a += m;
		return m - a;
	}

	// Eulidean Algorithm
	// returns Greatest Common Divisor
	template<typename T>
	T gcd(T a, T b)
	{
		if (b > a) std::swap(a, b);

		T r; // remainder
		
		while (r = a % b) { a = b; b = r; }

		return b;
	}

	namespace hide
	{
		// https://goo.gl/o1mKYM
		// https://goo.gl/NTdu2w
		// a * x + b * y = gcd(a , b)
		// eea for Extended Euclidean Algorithm
		// do not call it directly,
		// it is helper function
		// 
		// precondition: a > b, otherwise 
		// if (b > a) std::swap(a, b);
		// if fails, return 0;
		template<typename T>
		T _eea_gcd_(T a, T b, T& x, T& y)
		{
			constexpr size_t max_count = 100;
			T q[max_count];

			size_t cache_count = 0;

			T r; // remainder

			while (r = a % b)
			{
				q[cache_count++] = -(a / b);

				if (cache_count == max_count)
					return 0;

				a = b; b = r;
			}

			// 10 x + 5 y = gcd(10, 5) = 5
			// if gcd != 1, then we cannot solve
			// 10 x = 1 (mod 5) or 5 x = 1 (mod 10)
			// 10 x + 5 y = 5
			// 10 0 + 5 1 = 5
			// x = 0, y = 1
			if (cache_count == 0)
			{
				x = 0; y = 1;
				return b;
			}

			// initial condition

			x = 1; y = q[cache_count - 1];

			for (int i = (int)cache_count - 2; i >= 0; --i)
			{
				T tmp = x; x = y;
				y = tmp + y*q[i];
			}

			return b;
		}

		// https://goo.gl/o1mKYM
		// https://goo.gl/NTdu2w
		// helper for extended euclidean algorithm using recursion
		// do not call this function directly
		// 
		// precondition: a > b
		//
		template<typename T>
		T _eea_gcd_recursion_(T a, T b, T& x, T& y)
		{
			// r is remainder of a mod b
			T r = a % b;

			if (r)
			{
				T gcd = _eea_gcd_recursion_(b, r, x, y);

				T q = -(a / b);

				if (x)
				{
					T tmp = x; x = y;
					y = tmp + y * q;
				}
				else // x == 0
				{
					// initial condition
					x = 1; y = q;
				}

				return gcd;
			}
			else // r == 0
			{
				// if gcd (=b) != 1, then failure
				// 10 x + 5 y = gcd(10, 5) = 5
				// if gcd (a, b) != 1, so, we cannot solve
				// 10 x = 1 (mod 5) or 5 x = 1 (mod 10)
				// 10 x + 5 y = 5
				// 10 0 + 5 1 = 5
				// x = 0, y = 1
				// more about this
				x = 0;  y = 1;
				return b;
			}
		}
	}

	// https://goo.gl/o1mKYM
	// https://goo.gl/NTdu2w
	// a x + b y = gcd(a, b)
	// return gcd(a, b), x and y
	template<typename T>
	T eea_gcd(T a, T b, T& x, T& y)
	{
		T a1 = a, b1 = b;

		// remember we have swapped a and b
		if (b1 > a1) std::swap(a1, b1);

		T g = hide::_eea_gcd_(a1, b1, x, y);
		
		// fail safe routine
		if (g == 0)
		{
			//std::cout << "Failed with: a=" << a1 << ", b=" << b << std::endl;
			
			g = hide::_eea_gcd_recursion_(a1, b1, x, y);
		}

		// if x and y does not satisfy
		// a * x + b * y = gcd(a, b), i.e.,
		// a * x + b * y != gcd(a, b)
		// then we have to swap x and y
		if (a * x + b * y != g)
			std::swap(x, y);

		return g;
	}

	template<typename T, typename... TArgs>
	T gcd(T a, const TArgs&... args)
	{
		return gcd(a, gcd(args...));
	}

	template<typename T>
	T lcm(T a, T b)
	{
		T g = gcd(a, b);
		
		return (a / g) * b;
	}

	template<typename T, typename... TArgs>
	T lcm(T a, const TArgs&... args)
	{
		return lcm(a, lcm(args...));
	}

	// https://goo.gl/o1mKYM
	// https://goo.gl/NTdu2w
	// a x + b y = gcd(a, b)
	// return gcd(a, b)
	// extended euclidean algorithm using loop
	template<typename T>
	T eea_gcd_recursion(T a, T b, T& x, T& y)
	{
		T a1 = a, b1 = b;

		if (b1 > a1) std::swap(a1, b1);

		T g = _eea_gcd_recursion_(a1, b1, x, y);

		// if x and y does not satisfy
		// a * x + b * y = gcd(a, b), i.e.,
		// a x + b y != gcd(a, b)
		// then we have to swap x and y
		if (a * x + b * y != gcd)
			std::swap(x, y);
		
		return g;
	}

	namespace hide
	{
		// By Cathy Frey
		// Solving Linear Congruences with One Unique Solution Solution
		// https://www.youtube.com/watch?v=HklhuO79U3s
		// Solving Linear Congruences with Multiple Solutions
		// https://www.youtube.com/watch?v=mN30Og87Vyg
		// Finding Modular Inverses
		// https://www.youtube.com/watch?v=FnQNbFl72LY

		// By Maths with Jay
		// Solve a Linear Congruence using Euclid's Algorithm
		// https://www.youtube.com/watch?v=4-HSjLXrfPs

		// By John Bowers
		// Extended Euclidean Algorithm Example 
		// https://www.youtube.com/watch?v=6KmhCKxFWOs

		// Solve a x = k (mod n)
		// if fail, returns 0
		// if success, the value of x is returned
		// modulo_make_positive(k, n);
		template<typename T>
		T _linear_congruence_(T a, T k, T n)
		{
			//
			// a * x + n * y = gcd(a, n) == 1
			//
			// a * x = 1 (mod n), multiply k to both side of the equation
			//
			// k * a * x = k (mod n)
			// 
			T x, y;

			// if gcd(a, n) != 1, failure
			if (eea_gcd(a, n, x, y) != T(1))
				return 0;

			// make x positive
			modulo_make_positive(x, n);

			return (x * k) % n;
		}

		// x = a (mod n)
		// gcd(a, n) == 1 is assumed
		template<typename T>
		T _modulo_inverse_(T a, T n)
		{
			using sgn_t = std::make_signed_t<T>;

			//sgn_t a_t = a, n_t = n;

			return modulo_positive(hide::_linear_congruence_(sgn_t(a), sgn_t(1), sgn_t(n)), sgn_t(n));
		}
	}

	namespace disable
	{
		// if fails returns 0
		// a * x = b (mod n) and gcd(a, n) = 1
		// returns x,
		// if b == 1, then x is inverse of a (mod n)
		// b should be postive 
		// call modulo_make_positive(b, n)
		// before calling this function
		//
		// Do not use this function
		// this function is for testing purpose only
		//
		template<typename T>
		T _linear_congruence_bt_(T a, T b, T n)
		{
			//modulo_make_positive(b, n);

			T k, v;
			for (k = 0; k < a; ++k)
			{
				v = n*k + b;

				if (v % a == 0) break;
			}

			return (v / a);
		}

		// if fails returns 0
		// a * x = b (mod n) and gcd(a, n) = 1
		// returns x,
		// if b == 1, then x is inverse of a (mod n)
		template<typename T>
		std::vector<T> linear_congruence_bt(T a, T b, T n)
		{
			modulo_make_positive(b, n);

			T divisor = gcd_loop(a, n);

			std::vector<T> rtn;

			// one and only one solution
			if (divisor == 1)
			{
				rtn.emplace_back(_linear_congruence_bt_(a, b, n));
			}
			else if ((b%divisor) == 0) //multiple solutions
			{
				T a1 = a / divisor, b1 = b / divisor, n1 = n / divisor;
				T x = _linear_congruence_bt_(a1, b1, n1);
				rtn.emplace_back(x);

				for (T i = 1; i < divisor; ++i)
				{
					x += n1; rtn.emplace_back(x);
				}
			}

			return rtn;
		}
	}

	// x = a (mod n)
	// if gcd(a, n) != 1
	// returns 0
	// else returns x
	template<typename T>
	T modulo_inverse(T a, T n)
	{
		using sgn_t = std::make_signed_t<T>;

		sgn_t a_t = a, n_t = n;

		sgn_t g = gcd(a_t, n_t);

		if (g != 1)
			return 0;
		else
			return modulo_positive(hide::_linear_congruence_(a_t, sgn_t(1), n_t), n_t);
	}
	
	// a x = b (mod n)
	// if gcd(a, n) != 1
	// returns 0
	// else returns x
	template<typename T>
	T solve_linear_congruence(T a, T b, T n)
	{
		using sgn_t = std::make_signed_t<T>;

		sgn_t a_t = a, b_t = b, n_t = n;

		sgn_t g = gcd(a_t, n_t);

		if (g != 1)
			return 0;
		else
			return modulo_positive(hide::_linear_congruence_(a_t, b_t, n_t), n_t);
	}
	// if fails returns 0
	// a * x = b (mod n) and gcd(a, n) = 1
	// returns x,
	// if b == 1, then x is inverse of a (mod n)
	template<typename T>
	std::vector<T> linear_congruence(T a, T b, T n)
	{
		using sgn_t = std::make_signed_t<T>;

		sgn_t a_t = a, b_t = b, n_t = n;

		modulo_make_positive(b_t, n_t);

		sgn_t g = gcd(a_t, n_t);

		std::vector<T> rtn;

		// one and only one solution
		if (g == 1)
		{
			rtn.emplace_back(hide::_linear_congruence_(a_t, b_t, n_t));
		}
		else if ((b_t % g) == 0) //multiple solutions
		{
			sgn_t a1 = a_t / g;
			sgn_t b1 = b_t / g;
			sgn_t n1 = n_t / g;
			
			sgn_t x = hide::_linear_congruence_(a1, b1, n1);

			rtn.emplace_back(T(x));

			for (sgn_t i = 1; i < g; ++i)
			{
				x += n1; rtn.emplace_back(T(x));
			}
		}

		return rtn;
	}

	// Prof. Jeff Suzuki
	// chinese remainder problems 1
	// https://www.youtube.com/watch?v=oKMYNKbFHBE
	// chinese remainder problems 2
	// https://www.youtube.com/watch?v=TnJPNRxu8mM
	//
	// Cathy Frey
	// https://www.youtube.com/watch?v=Y5RcMWiUyyE
	//
	// if fails, returns 0
	//
	// chinese_remainder(a0, n1, a1, n1, a2, n2)
	// x = a0 (mod n0)
	// x = a1 (mod n1)
	// x = a2 (mod n2)
	template<typename T>
	T chinese_remainder(std::initializer_list<T> args)
	{
		// odd number of arguments
		if (args.size() % 2)
		{
			throw std::range_error("range error at"
				" ChineseRemainderTheorem(std::initializer_list<T> args)"
				", args.size() should be even");
		}

		tpf::Vctr<T> a, n;

		// STEP 0. split arguments to a's and n's
		T index = 0;
		for (auto& i : args)
		{
			// odd
			if (index % 2)
				n.emplace_back(i);
			else // even
				a.emplace_back(i);

			++index;
		}

		T cmb = tpf::combi(T(n.size()), T(2));

		// STEP 1. make sure n's are coprime
		for (T i = 0; i < cmb; ++i)
		{
			RefVctr<T> mds = tpf::enum_combination(n, 2, i);

			if (gcd(mds[0].get(), mds[1].get()) != 1)
				return 0;
		}

		T m = 1;
		for (auto i : n) m *= i;

		std::vector<T> ms(n.size());
		std::vector<T> y(n.size());

		T amy = 0;

		for (size_t i = 0; i < n.size(); ++i)
		{
			ms[i] = m / n[i];

			y[i] = modulo_inverse(ms[i], n[i]);

			amy += a[i] * ms[i] * y[i];
		}

		return modulo_positive(amy, m);
	}

	namespace hide
	{
		// Jeff Suzuki
		// Fast Powering Algorithm
		// https://goo.gl/S5RpwL
		// make a positive modulo
		// before calling this function
		// modulo_make_positive(a, n);
		// a^p (mod n)
		template<typename T> T _fast_modulo_(T a, T p, T n)
		{
			T b = 1;

			while (p)
			{
				// if last digit of p is 1
				if ((p & 1))
				{
					b *= a; b %= n;
				}

				p >>= 1;

				a *= a; a %= n;
			}

			return b;
		}
	}

	// use Fermat' Little Theorem
	// where n is prime
	// a^p (mod n)
	template<typename T> T fast_modulo_fermat(T a, T p, T n)
	{
		return  hide::_fast_modulo_(a, p%(n-1), n);
	}

	// use Fermat' Little Theorem
	// where n is prime
	// a^b (mod p*q)
	template<typename T> T fast_modulo_euler(T a, T b, T p, T q)
	{
		T pq = p*q; b %= (p - 1)* (q - 1);

		return hide::_fast_modulo_(a, b, pq);
	}

	// Prof. Jeff Suzuki
	// chinese remainder problems 1
	// https://www.youtube.com/watch?v=oKMYNKbFHBE
	// chinese remainder problems 2
	// https://www.youtube.com/watch?v=TnJPNRxu8mM

	// Cathy Frey
	// https://www.youtube.com/watch?v=Y5RcMWiUyyE

	// if fails, 
	// i.e., gcd(m1, m2) != 1, then returns 0
	// else returns solution
	// x = a1 (mod m1)
	// x = a2 (mod m2)
	template<typename T>
	T chinese_remainder(T a1, T m1, T a2, T m2)
	{
		//if (gcd(m1, m2) != 1) return 0;

		T y1 = modulo_inverse(m2, m1);
		T y2 = modulo_inverse(m1, m2);
		
		//T amy = a1 * m2 * modulo_inverse(m2, m1) + 
			//a2 * m1 * modulo_inverse(m1, m2);
		
		return (a1 * m2 * y1 + a2 * m1 * y2)%(m1*m2);
	}

	// use Chinese Remainder Theorem
	// where p and q are primes
	// x = a^b (mod p*q)
	template<typename T> T fast_modulo_chinese(T a, T b, T p, T q)
	{
		return chinese_remainder(tpf::fast_modulo_fermat(a, b, p), p,
			tpf::fast_modulo_fermat(a, b, q), q);
	}
}

#endif // end of _TPF_GCD_LCM_H