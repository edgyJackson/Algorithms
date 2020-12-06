#ifdef _MSC_BUILD
#pragma once
#endif

#ifndef _TPF_PRIME_FACTORIZATION_H
#define _TPF_PRIME_FACTORIZATION_H

/*
Programmed by: Thomas Kim
First Created: Nov. 27, 2017
http://www.TalkPlayFun.com

*/

#include <set>
#include <vector>
#include <iostream>

#include "tpf_prime_numbers.h"

namespace tpf
{
	template<typename T>
	class Term
	{
	public:
		Term() { }

		Term(T base, T power = T(1)) :
			m_base(base), m_power(power) { }

		Term(const Term&) = default;
		Term& operator=(const Term&) = default;

		Term(Term&&) = default;
		Term& operator=(Term&&) = default;

		bool operator<(const Term& term) const
		{
			return this->m_base < term.m_base;
		}

		size_t divisors(std::set<T>& rlt) const
		{
			if (m_base != 0 && m_power != 0)
			{
				for (T idx = 0; idx <= m_power; ++idx)
					rlt.emplace((T)pow(double(m_base), double(idx)));
			}

			return rlt.size();
		}

		size_t divisors(std::vector<T>& rlt) const
		{
			if (m_base != 0 && m_power != 0)
			{
				for (T idx = 0; idx <= m_power; ++idx)
					rlt.emplace_back((T)pow(double(m_base), double(idx)));
			}

			return rlt.size();
		}

		operator std::string() const
		{
			std::ostringstream os;

			if (this->m_power == 1)
				os << this->m_base;
			else
				os << this->m_base << "^" << this->m_power;

			return os.str();
		}

	public:
		T base() const { return this->m_base; }
		void base(T b) { this->m_base = b; }

		T power() const { return this->m_power; }
		void power(T p) { this->m_power = p; }

	private:
		T m_base, m_power;

	};

	template<typename T>
	using TermSet = std::set<Term<T>>;

	template<typename T>
	using TermVctr = std::vector<Term<T>>;

	template<typename T> std::set<T> get_bases(const TermSet<T>& cntr)
	{
		std::set<T> rlt;
		if (!cntr.empty())
		{
			for (const auto& i : cntr)
				rlt.emplace(i.base());
		}

		return rlt;
	}

	template<typename T> std::vector<T> get_bases(const TermVctr<T>& cntr)
	{
		std::vector<T> rlt;

		if (!cntr.empty())
		{
			for (const auto& i : cntr)
				rlt.emplace_back(i.base());
		}

		return rlt;
	}

	template<typename T> std::set<T> get_powers(const TermSet<T>& cntr)
	{
		std::set<T> rlt;
		if (!cntr.empty())
		{
			for (const auto& i : cntr)
				rlt.emplace(i.power());
		}

		return rlt;
	}

	template<typename T> std::vector<T> get_powers(const TermVctr<T>& cntr)
	{
		std::vector<T> rlt;

		if (!cntr.empty())
		{
			for (const auto& i : cntr)
				rlt.emplace_back(i.power());
		}

		return rlt;
	}

	// val = 2*2*2*5, div = 2
	// return value pwr = 3, power of div
	// q = 5, val = (div^pwr) * q
	template<typename T>
	T factor(T div, T val, T& q)
	{
		T d = div, pwr = 0;
		q = val;

		while (d <= val)
		{
			if (val % d == 0)
			{
				q = val / d; ++pwr;
			}

			d *= div;
		}

		return pwr;
	}

	template<typename T>
	size_t factorize(T value, TermSet<T>& terms, const Set<T>& primes)
	{
		T val = value, pwr, q;

		for (const auto& p : primes)
		{
			if (p > val) break;

			pwr = factor(p, val, q);

			if (pwr >= 1)
			{
				terms.emplace(Term<T>(p, pwr));

				val = q;
			}
		}

		return terms.size();
	}

	template<typename T>
	size_t factorize(T value, TermVctr<T>& terms, const Vctr<T>& primes)
	{
		if (value == T(1))
		{
			terms.emplace_back(Term<T>(1, 1));
			return 1;
		}

		T val = value, pwr, q;

		for (const auto& p : primes)
		{
			if (p > val) break;

			pwr = factor(p, val, q);

			if (pwr >= 1)
			{
				terms.emplace_back(Term<T>(p, pwr));
				val = q;
			}
		}

		return terms.size();
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Term<T>& term)
	{
		os << (std::string)term;
		return os;
	}

	template<typename T>
	Set<T> product_merge(const Set<T>& A, const Set<T>& B)
	{
		Set<T> rlt;

		size_t count_A = A.size();
		size_t count_B = B.size();

		do
		{
			if (count_A != 0 && count_B != 0)
			{
				for (const auto& a : A)
					for (const auto& b : B)
						rlt.insert(a*b);
				break;
			}
			else if ((count_A == 0 && count_B != 0) ||
				(count_A != 0 && count_B == 0))
			{
				rlt = (count_A != 0) ? A : B;
				break;
			}
			else // (count_A == 0 && count_B == 0)
				break;

		} while (true);

		return rlt;
	}

	template<typename T>
	Vctr<T> product_merge(const Vctr<T>& A, const Vctr<T>& B)
	{
		Vctr<T> rlt;

		size_t count_A = A.size();
		size_t count_B = B.size();

		do
		{
			if (count_A != 0 && count_B != 0)
			{
				for (const auto& a : A)
					for (const auto& b : B)
						rlt.emplace_back(a*b);
				break;
			}
			else if ((count_A == 0 && count_B != 0) ||
				(count_A != 0 && count_B == 0))
			{
				rlt = (count_A != 0) ? A : B;
				break;
			}
			else // (count_A == 0 && count_B == 0)
				break;

		} while (true);

		return rlt;
	}
	
	template<typename T>
	size_t get_divisors(Set<T>& factors, const TermSet<T>& terms)
	{
		Set<T> tmp;

		size_t count = terms.size();

		for (const auto& item : terms)
		{
			tmp.clear(); item.divisors(tmp);

			factors = product_merge(factors, tmp);
		}

		return factors.size();
	}

	template<typename T>
	size_t get_divisors(Vctr<T>& factors, const TermVctr<T>& terms)
	{
		Vctr<T> tmp;

		size_t count = terms.size();

		for (const auto& item : terms)
		{
			tmp.clear(); item.divisors(tmp);

			factors = product_merge(factors, tmp);
		}

		std::sort(factors.begin(), factors.end());

		return factors.size();
	}

	template<typename T>
	Vctr<T> get_divisors(T num, const Vctr<T>& primes)
	{
		Vctr<T> cntr;

		TermVctr<T> terms;

		if (factorize(num, terms, primes))
			get_divisors(cntr, terms);

		return cntr;
	}

	template<typename T>
	Set<T> get_divisors(T num, const Set<T>& primes)
	{
		Set<T> cntr;

		TermSet<T> terms;

		if (factorize(num, terms, primes))
			get_divisors(cntr, terms);

		return cntr;
	}

	/*template<typename T>
	std::ostream& operator<<(std::ostream& os, const TermVctr<T>& tv)
	{
		if (tv.empty()) return os;

		auto last_element = --tv.cend();
		os << "{";
		for (auto itr = tv.cbegin(); itr != last_element; ++itr)
			os << *itr << ", ";

		os << *last_element << "}";
		return os;
	}*/
}

#endif // end of _TPF_PRIME_FACTORIZATION_H
