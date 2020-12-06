#ifdef _MSC_BUILD
#pragma once
#endif

#ifndef _TPF_RATIONAL_NUMBER_H
#define _TPF_RATIONAL_NUMBER_H

/*
Programmed by: Thomas Kim
First Created: Nov. 27, 2017

http://www.TalkPlayFun.com

*/

#include <exception>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <limits>
#include <type_traits>

#include "tpf_gcd_lcm.h"
#include "tpf_unsigned_rational.h"

namespace tpf
{
	template<typename TypeValue, typename TypeSign>
	inline TypeValue sign_value(TypeSign& sign, const TypeValue& v)
	{
		sign = (v == 0) ? TypeSign(0) : ((v > 0) ? TypeSign(1) : TypeSign(-1));
		return (v >= 0) ? v : -v;
	}

	template<typename T>
	class Rational
	{
	public:

		using value_type = std::make_signed_t<T>;
		using unsigned_type = std::make_unsigned_t<T>;

	private:
		value_type m_p; // numerator
		value_type m_q; // denominator
		
		inline void reduce()
		{
			// signs for m_p, m_q
			int sp, sq;

			value_type p = sign_value(sp, this->m_p);
			value_type q = sign_value(sq, this->m_q);

			int spq = sp*sq;

			// either numerator or denominator is zero
			if (spq == 0)
			{
				this->m_p = 0; this->m_q = 1;
				return;
			}

			value_type d = gcd(p, q);

			// both are either positive or negative
			if (spq > 0)
			{
				this->m_p = p / d; this->m_q = q / d;
			}
			else
			{
				this->m_p = -p / d; this->m_q = q / d;
			}
		}

		inline void reduce_sign()
		{
			int sp, sq;

			value_type p = sign_value(sp, this->m_p);
			value_type q = sign_value(sq, this->m_q);

			int spq = sp * sq;

			// either numerator or denominator is zero
			if (spq == 0)
			{
				this->m_p = 0; this->m_q = 1;
				return;
			}

			// either both positive or negative
			if (spq > 0)
			{
				this->m_p = p; this->m_q = q;
				return;
			}
			else // make numerator negative
			{
				this->m_p = -p; this->m_q = q;
				return;
			}
		}
	public:
		Rational() { }
		Rational(T p) : m_p(value_type(p)), m_q(value_type(1)) { }
		Rational(T p, T q) : m_p(value_type(p)), m_q(value_type(q)) { reduce(); }
		Rational(T p, T q, bool) : m_p(p), m_q(q) { 	}

		Rational(const Rational& r) = default;
		Rational& operator=(const Rational& r) = default;

		Rational(Rational&& r) = default;
		Rational& operator=(Rational&& r) = default;
		
		~Rational() = default;
		
		// numerator
		const value_type& p() const { return this->m_p; }
		// denomiator
		const value_type& q() const { return this->m_q; }

		template<typename C> operator C() const
		{
			return C((double)this->m_p / (double)this->m_q);
		}

		void operator *= (const Rational& r)
		{
			// m_p     r.m_p     m_p   * r.m_p
			// ---  x  ----- =  -------------
			// m_q     r.m_q     r.m_q *  m_q

			Rational r1(this->m_p, r.m_q);
			Rational r2(r.m_p, this->m_q);

			this->m_p = r1.m_p * r2.m_p;
			this->m_q = r1.m_q * r2.m_q;

			// important
			reduce_sign();
		}

		Rational operator *(const Rational& r) const
		{
			// m_p     r.m_p     m_p   * r.m_p
			// ---  x  ----- =  -------------
			// m_q     r.m_q     r.m_q *  m_q

			Rational r1(this->m_p, r.m_q);
			Rational r2(r.m_p, this->m_q);

			return Rational(r1.m_p * r2.m_p, r1.m_q * r2.m_q);
		}

		void operator /= (const Rational& r)
		{
			// m_p     r.m_q     m_p   * r.m_q
			// ---  /  ----- =  -------------
			// m_q     r.m_p     r.m_p *  m_q

			Rational r1(this->m_p, r.m_p), r2(r.m_q, this->m_q);

			this->m_p = r1.m_p * r2.m_p;
			this->m_q = r1.m_q * r2.m_q;

			// important
			this->reduce_sign();
		}

		Rational operator / (const Rational& r) const
		{
			// m_p     r.m_q     m_p   * r.m_q
			// ---  /  ----- =  -------------
			// m_q     r.m_p     r.m_p *  m_q

			Rational r1(this->m_p, r.m_p), r2(r.m_q, this->m_q);

			return Rational(r1.m_p * r2.m_p, r1.m_q * r2.m_q);
		}

		void operator += (const Rational& r)
		{
			// m_p     r.m_p     m_p * r.m_q + r.m_p * m_q
			// ---  +  ----- =  -------------------------------
			// m_q     r.m_q           m_q * r.m_q 

			// m_p   * r.m_q   r.m_p *  m_q
			// ------------ + ---------------
			// r.m_q * m_q     m_q   * r.m_q


			Rational p1(this->m_p, r.m_q), p2(r.m_q, this->m_q);
			Rational p3(r.m_p, m_q), p4(this->m_q, r.m_q);

			this->m_p = p1.m_p * p2.m_p + p3.m_p * p4.m_p;
			this->m_q = p1.m_q * p2.m_q;

			// important
			reduce_sign();
		}

		Rational operator + (const Rational& r) const
		{
			// m_p     r.m_p     m_p * r.m_q + r.m_p * m_q
			// ---  +  ----- =  -------------------------------
			// m_q     r.m_q           m_q * r.m_q 

			// m_p   * r.m_q   r.m_p *  m_q
			// ------------ + ---------------
			// r.m_q * m_q     m_q   * r.m_q


			Rational p1(this->m_p, r.m_q), p2(r.m_q, this->m_q);
			Rational p3(r.m_p, m_q), p4(this->m_q, r.m_q);

			return Rational(p1.m_p * p2.m_p + p3.m_p * p4.m_p, p1.m_q * p2.m_q);
		}

		void operator -= (const Rational& r)
		{
			// m_p     r.m_p     m_p * r.m_q - r.m_p * m_q
			// ---  +  ----- =  -------------------------------
			// m_q     r.m_q           m_q * r.m_q 

			// m_p   * r.m_q   r.m_p *  m_q
			// ------------ + ---------------
			// r.m_q * m_q     m_q   * r.m_q


			Rational p1(this->m_p, r.m_q), p2(r.m_q, this->m_q);
			Rational p3(r.m_p, m_q), p4(this->m_q, r.m_q);

			this->m_p = p1.m_p * p2.m_p - p3.m_p * p4.m_p;
			this->m_q = p1.m_q * p2.m_q;

			// important
			reduce_sign();
		}

		Rational operator - (const Rational& r) const
		{
			// m_p     r.m_p     m_p * r.m_q - r.m_p * m_q
			// ---  +  ----- =  -------------------------------
			// m_q     r.m_q           m_q * r.m_q 

			// m_p   * r.m_q   r.m_p *  m_q
			// ------------ + ---------------
			// r.m_q * m_q     m_q   * r.m_q


			Rational p1(this->m_p, r.m_q), p2(r.m_q, this->m_q);
			Rational p3(r.m_p, m_q), p4(this->m_q, r.m_q);

			return Rational(p1.m_p * p2.m_p - p3.m_p * p4.m_p, p1.m_q * p2.m_q);
		}

		friend std::ostream& operator<<(std::ostream& os, const Rational& r)
		{
			if (r.m_q == value_type(1))
				os << r.m_p;
			else
				os << r.m_p << "/" << r.m_q;

			return os;
		}
	};

	template<typename T> auto to(const Rational<T>& r)
	{
		return UnsignedRational<std::make_unsigned_t<T>>(r.p(), r.q());
	}

	template<typename T> auto to(const UnsignedRational<T>& r)
	{
		return Rational<std::make_signed_t<T>>(r.p(), r.q());
	}
}

#endif // end of _TPF_RATIONAL_NUMBER_H
