#ifdef _MSC_BUILD
#pragma once
#endif

#ifndef _TPF_UNSIGNED_RATIONAL_H
#define _TPF_UNSIGNED_RATIONAL_H

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

namespace tpf
{
	template<typename T>
	class UnsignedRational
	{
	public:
		
		using value_type = std::make_unsigned_t<T>;
		using signed_type = std::make_signed_t<T>;
				
	private:
		value_type m_p; // numerator
		value_type m_q; // denominator

		inline void reduce()
		{
			// either numerator or denominator is zero
			if (this->m_p == 0 || this->m_q==0)
			{
				this->m_p = 0; this->m_q = 1;
				return;
			}

			value_type d = gcd(m_p, m_q);

			this->m_p /= d; this->m_q /= d; return;
		}

	public:
		UnsignedRational() { }
		UnsignedRational(T p) : m_p(value_type(p)), m_q(value_type(1)) { }
		UnsignedRational(T p, T q) : m_p(value_type(p)), m_q(value_type(q)) { reduce(); }
		UnsignedRational(T p, T q, bool) : m_p(p), m_q(q) { 	}

		UnsignedRational(const UnsignedRational& r) = default;
		UnsignedRational& operator=(const UnsignedRational& r) = default;

		UnsignedRational(UnsignedRational&& r) = default;
		UnsignedRational& operator=(UnsignedRational&& r) = default;
		~UnsignedRational() = default;
		
		// numerator
		const value_type& p() const { return this->m_p; }
		// denomiator
		const value_type& q() const { return this->m_q; }

		template<typename C> operator C() const
		{
			return C((double)this->m_p / (double)this->m_q);
		}

		void operator *= (const UnsignedRational& r)
		{
			// m_p     r.m_p     m_p   * r.m_p
			// ---  x  ----- =  -------------
			// m_q     r.m_q     r.m_q *  m_q

			UnsignedRational r1(this->m_p, r.m_q);
			UnsignedRational r2(r.m_p, this->m_q);

			this->m_p = r1.m_p * r2.m_p;
			this->m_q = r1.m_q * r2.m_q;
		}

		UnsignedRational operator *(const UnsignedRational& r) const
		{
			// m_p     r.m_p     m_p   * r.m_p
			// ---  x  ----- =  -------------
			// m_q     r.m_q     r.m_q *  m_q

			UnsignedRational r1(this->m_p, r.m_q);
			UnsignedRational r2(r.m_p, this->m_q);

			// no need to factor out
			return UnsignedRational(r1.m_p * r2.m_p, r1.m_q * r2.m_q, false);
		}

		void operator /= (const UnsignedRational& r)
		{
			// m_p     r.m_q     m_p   * r.m_q
			// ---  /  ----- =  -------------
			// m_q     r.m_p     r.m_p *  m_q

			UnsignedRational r1(this->m_p, r.m_p);
			UnsignedRational r2(r.m_q, this->m_q);

			this->m_p = r1.m_p * r2.m_p;
			this->m_q = r1.m_q * r2.m_q;
		}

		UnsignedRational operator / (const UnsignedRational& r) const
		{
			// m_p     r.m_q     m_p   * r.m_q
			// ---  /  ----- =  -------------
			// m_q     r.m_p     r.m_p *  m_q

			UnsignedRational r1(this->m_p, r.m_p);
			UnsignedRational r2(r.m_q, this->m_q);

			return UnsignedRational(r1.m_p * r2.m_p, r1.m_q * r2.m_q, false);
		}

		void operator += (const UnsignedRational& r)
		{
			// m_p     r.m_p     m_p * r.m_q + r.m_p * m_q
			// ---  +  ----- =  -------------------------------
			// m_q     r.m_q           m_q * r.m_q 

			// m_p   * r.m_q   r.m_p *  m_q
			// ------------ + ---------------
			// r.m_q * m_q     m_q   * r.m_q


			UnsignedRational p1(this->m_p, r.m_q);
			UnsignedRational p2(r.m_q, this->m_q);
			UnsignedRational p3(r.m_p, m_q);
			UnsignedRational p4(this->m_q, r.m_q);

			this->m_p = p1.m_p * p2.m_p + p3.m_p * p4.m_p;
			this->m_q = p1.m_q * p2.m_q;
			
			reduce();
		}

		UnsignedRational operator + (const UnsignedRational& r) const
		{
			// m_p     r.m_p     m_p * r.m_q + r.m_p * m_q
			// ---  +  ----- =  -------------------------------
			// m_q     r.m_q           m_q * r.m_q 

			// m_p   * r.m_q   r.m_p *  m_q
			// ------------ + ---------------
			// r.m_q * m_q     m_q   * r.m_q


			UnsignedRational p1(this->m_p, r.m_q);
			UnsignedRational p2(r.m_q, this->m_q);
			UnsignedRational p3(r.m_p, m_q);
			UnsignedRational p4(this->m_q, r.m_q);

			return UnsignedRational(p1.m_p * p2.m_p + p3.m_p * p4.m_p, p1.m_q * p2.m_q);
		}

		void operator -= (const UnsignedRational& r)
		{
			// m_p     r.m_p     m_p * r.m_q - r.m_p * m_q
			// ---  +  ----- =  -------------------------------
			// m_q     r.m_q           m_q * r.m_q 

			// m_p   * r.m_q   r.m_p *  m_q
			// ------------ + ---------------
			// r.m_q * m_q     m_q   * r.m_q


			UnsignedRational p1(this->m_p, r.m_q), p2(r.m_q, this->m_q);
			UnsignedRational p3(r.m_p, m_q), p4(this->m_q, r.m_q);

			this->m_p = p1.m_p * p2.m_p - p3.m_p * p4.m_p;
			this->m_q = p1.m_q * p2.m_q;
			
			reduce();
		}

		UnsignedRational operator - (const UnsignedRational& r) const
		{
			// m_p     r.m_p     m_p * r.m_q - r.m_p * m_q
			// ---  +  ----- =  -------------------------------
			// m_q     r.m_q           m_q * r.m_q 

			// m_p   * r.m_q   r.m_p *  m_q
			// ------------ + ---------------
			// r.m_q * m_q     m_q   * r.m_q


			UnsignedRational p1(this->m_p, r.m_q);
			UnsignedRational p2(r.m_q, this->m_q);
			UnsignedRational p3(r.m_p, m_q);
			UnsignedRational p4(this->m_q, r.m_q);

			return UnsignedRational(p1.m_p * p2.m_p - p3.m_p * p4.m_p, p1.m_q * p2.m_q);
		}

		friend std::ostream& operator<<(std::ostream& os, const UnsignedRational& r)
		{
			if (r.m_q == value_type(1))
				os << r.m_p;
			else
				os << r.m_p << "/" << r.m_q;

			return os;
		}
	};

}
#endif // _TPF_UNSIGNED_RATIONAL_H