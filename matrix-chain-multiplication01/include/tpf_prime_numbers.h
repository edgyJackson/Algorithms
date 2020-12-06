#ifdef _MSC_BUILD
#pragma once
#endif

#ifndef _TPF_PRIME_NUMBERS_H
#define _TPF_PRIME_NUMBERS_H

/*
	Programmed by: Thomas Kim
	First Created: Feb. 20, 2017
	Last Modified: Nov. 26, 2017

	http://www.TalkPlayFun.com
*/

#include <vector>
#include <set>
#include <cmath>
#include <utility>
#include <algorithm>
#include <functional>
#include <thread>
#include <iostream>
#include <fstream>
#include <exception>
#include <atomic>
#include <ios>
#include <iterator>

#include "tpf_types.h"
#include "tpf_stop_watch.h"
#include "tpf_split_range.h"
#include "tpf_smart_output.h"
#include "tpf_random.h"
#include "tpf_gcd_lcm.h"

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

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Term<T>& term)
	{
		os << (std::string)term;
		return os;
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

	enum class PrimeState { No = -1, Unknown = 0, Yes = 1};
		
	std::ostream& operator<<(std::ostream& os, const PrimeState& ispr)
	{
		switch (ispr)
		{
		case PrimeState::No:
			os << "Not Prime"; break;
		case PrimeState::Yes:
			os << "Yes Prime"; break;

		case PrimeState::Unknown:
		default:
			os << "Unknown"; break;
		}

		return os;
	}

	// approximate count
	template<typename T> T estimate_prime_count(T num)
	{
		/* 
		
		refer to  https://primes.utm.edu/howmany.html

		The prime number theorem clearly implies that you can use
		x / (log x - a) (with any constant a) to approximate pi(x).
		The prime number theorem was stated with a = 0,
		but it has been shown that a = 1 is the best choice :
		There are longer tables below and (of pi(x) only) above.

		Example : Someone recently e - mailed me and asked for
		a list of all the primes with at most 300 digits.
		Since the prime number theorem implies this list
		would have about 1.4 * 10297 entries we know that there can be no such list!
		Note that Pierre Dusart[Dusart99] showed that if x>598 then
		(x / log x)(1 + 0.992 / log x) < pi(x) <(x / log x)(1 + 1.2762 / log x)
		(The upper bound holds for all x > 1.)
		This gives a tight bound for larger x.
		Note x / log x < pi(x) for x > 10.
		*/

		double x = double(num);
		double inv_logx = 1.0 / log(x);
		double xlogx = x * inv_logx;

		return T(xlogx*(1.0 + 1.2762*inv_logx));
	}
			
	template<typename T>
	class CPrimes
	{
	public:
		std::vector<T> m_primes;
		T m_prime_limit;
		std::atomic_bool m_StopThread;
	private:
		
		T factor(T div, T val, T& q)
		{
			T d = div, pwr = 0; q = val;

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

		PrimeState is_prime(const tpf::range_t& rnge, T n)
		{
			PrimeState is_prime = PrimeState::Unknown;

			T sqrt_n = (T)sqrt((double)n);

			for (size_t i = rnge.first; i < rnge.second; ++i)
			{
				if (this->m_StopThread.load()) break;

				if (n % this->m_primes[i] == 0)
				{
					this->m_StopThread.store(true);
					is_prime = PrimeState::No; break;
				}

				if (this->m_primes[i] > sqrt_n)
				{
					this->m_StopThread.store(true);
					is_prime = PrimeState::Yes; break;
				}
			}

			return is_prime;
		}

		void build_single_thread(T bound)
		{
			T sqrt_n, n; // start number to determine primality

			typename std::vector<T>::iterator start;
			typename std::vector<T>::iterator end;

			size_t reserve_count = size_t(estimate_prime_count(bound) + 10);

			// new build
			if (this->m_prime_limit == 0)
			{
				if (m_primes.capacity() < reserve_count)
					m_primes.reserve(reserve_count);
				
				m_primes.emplace_back(2);
				m_primes.emplace_back(3);

				n = 5;
			}
			else // build up
			{
				if (bound <= this->m_prime_limit) return;

				// make n odd
				n = (this->m_prime_limit % 2) ?
					this->m_prime_limit : this->m_prime_limit + 1;

				if (m_primes.capacity() < reserve_count)
					m_primes.reserve(reserve_count);
			}

			start = ++m_primes.begin();
			end = m_primes.end();

			while (n < bound)
			{
				sqrt_n = T(sqrt(double(n)));

				// if perfect square,
				// it is not prime
				if (sqrt_n * sqrt_n != n)
				{
					for (auto p = start; p != end; ++p)
					{
						if (n % (*p) == 0) // not prime
							break;
						else if ((*p) > sqrt_n) // prime
						{
							m_primes.emplace_back(n);
							end = m_primes.end();
							break;
						}
					}
				}
				
				n += 2;
			}
		}

		// for parallel processing, thread count >= 2
		size_t build_primes_parallel_task(std::vector<T>& new_primes, tpf::range<T> rng)
		{
			auto start = ++m_primes.begin();
			auto end = m_primes.end();

			T sqrt_n;

			// rng.first is even number, make it odd number
			T n = (rng.first % 2) ?
				rng.first : rng.first + 1;

			while (n < rng.second)
			{
				sqrt_n = (T)(sqrt(double(n)));

				// if perfect square
				// it is not prime
				if (sqrt_n * sqrt_n != n)
				{
					for (auto p = start; p != end; ++p)
					{
						if (n % (*p) == 0) // not prime
							break;
						else if ((*p) > sqrt_n) // prime
						{
							new_primes.emplace_back(n);
							break;
						}
					}
				}

				n += 2;
			}

			return new_primes.size();
		}

		void build_primes_parallel(size_t thread_count, T lower_limit, T prime_limit)
		{
			rngvctr<T> rnge = tpf::SplitRangeCount(thread_count, lower_limit, prime_limit);
			
			std::unique_ptr<std::vector<T>[]> new_prime(new std::vector<T>[thread_count]);
			std::unique_ptr<std::future<size_t>[]> ftr(new std::future<size_t>[thread_count]);

			T est1, est2, dist;
			for (size_t i = 0; i < thread_count; ++i)
			{
				est2 = estimate_prime_count(rnge[i].second);
				est1 = estimate_prime_count(rnge[i].first);

				dist = (est2 - est1) + 10;

				new_prime[i].reserve(size_t(dist));

				ftr[i] = std::async(&CPrimes::build_primes_parallel_task,
					this, std::ref(new_prime[i]), rnge[i]);
			}

			std::unique_ptr<size_t[]> prime_size(new size_t[thread_count]);

			for (size_t i = 0; i < thread_count; ++i)
				prime_size[i] = ftr[i].get();

			std::vector<T>::const_iterator start;
			std::vector<T>::const_iterator end;

			for (size_t i = 0; i < thread_count; ++i)
			{
				start = new_prime[i].cbegin();
				end = start + prime_size[i];

				for (auto p = start; p != end; ++p)
					m_primes.push_back(*p);
			}
		}

		// thread_count >= 2
		void build_multi_threads(T prime_limit, size_t thread_count)
		{
			if (prime_limit > this->m_prime_limit)
			{
				size_t reserve_count =
					(size_t)estimate_prime_count(prime_limit) + 10;

				if (m_primes.capacity() < reserve_count)
					m_primes.reserve(reserve_count);

				T lower_limit = (T)(sqrt((double)prime_limit) * thread_count);

				if (lower_limit >= prime_limit) lower_limit = prime_limit;

				build_single_thread(lower_limit);

				if (lower_limit < prime_limit)
					build_primes_parallel(thread_count, lower_limit, prime_limit);
			}
		}

		
		public:

		CPrimes() : m_prime_limit{}, m_primes{} { 	}

		std::string Report()
		{
			std::ostringstream os;
			
			os.imbue(std::locale(""));
			os << "Number of primes: " << this->m_primes.size() << std::endl;

			if (!this->m_primes.empty())
			{

				intmax_t max_cached_prime = this->m_primes.back();
				intmax_t max_test_prime = max_cached_prime * max_cached_prime;
				os << "Max cached prime: " << max_cached_prime << std::endl;
				os << "Max testable prime: " << max_test_prime << std::endl;
			}
			
			return os.str();
		}

		std::pair<size_t, size_t> FindRange(T lbound, T ubound)
		{
			auto lb_itr = std::find_if(this->m_primes.cbegin(), this->m_primes.cend(),
				[=](T v) { return v > lbound; });

			auto ub_itr = std::find_if(this->m_primes.crbegin(), this->m_primes.crend(),
				[=](T v) { return v < ubound; });

			return std::make_pair(size_t(lb_itr - this->m_primes.cbegin()),
				size_t(this->m_primes.crend() - ub_itr - 1));
		}

		//  returns a prime between lbound and ubound
		T GetOnePrime(T lbound, T ubound)
		{
			std::pair<size_t, size_t> bnd = 
				FindRange(lbound, ubound);
			
			return this->m_primes[integer_random<size_t>(bnd.first, bnd.second)];
		}

		// returns two primes betweeen lbound and ubound
		// first is the smaller of the two
		std::pair<T, T> GetTwoPrimes(T lbound, T ubound)
		{
			std::pair<size_t, size_t> bnd = FindRange(lbound, ubound);

			integer_random<size_t> random(bnd.first, bnd.second);

			size_t first = random(); size_t second = random();

			while (first == second) second = random();

			// make sure first is less than the second
			if (first > second ) std::swap(first, second);

			return std::make_pair(this->m_primes[first], this->m_primes[second]);
		}

		// returns a prime between lbound and ubound
		// that is co-prime with phi
		T GetCoPrime(T lbound, T ubound, T phi)
		{
			std::pair<size_t, size_t> 
				bnd = FindRange(lbound, ubound);

			integer_random<size_t> random(bnd.first, bnd.second);

			T e = this->m_primes[random()];

			while (gcd(e, phi) != 1)
				e = this->m_primes[random()];

			return e;
		}

		T GetPrivateKey(T e, T phi)
		{
			return tpf::hide::_modulo_inverse_(e, phi);
		}

		std::vector<T>& GetPrimes()
		{
			return m_primes;
		}
		
		void Save(std::string filename = "primes")
		{
			filename += ".txt";

			std::ofstream ofs(filename);
			ofs << this->m_primes.size()<< " ";

			for (auto& e : this->m_primes)
				ofs << e<<" ";
			
		}

		void Load(std::string filename = "primes")
		{
			try
			{
				T value; intmax_t size;

				if (!this->m_primes.empty())
					this->m_primes.clear();

				filename += ".txt";

				std::ifstream ifs(filename, std::ios::in);
				ifs >> size;
				this->m_primes.reserve((size_t)size);

				while (!ifs.eof() && size > 0)
				{
					ifs >> value; --size;
					this->m_primes.emplace_back(value);
				}
			}
			catch (...)
			{
				std::cerr << "Failed to read prime numbers." << std::endl;
			}
		}

		bool BuildPrimes(T prime_limit, size_t thread_count = 0)
		{
			try
			{
				if (thread_count == 0)
					thread_count = get_cpu_thread_count();

				if (thread_count < 2)
					build_single_thread(prime_limit);
				else
					build_multi_threads(prime_limit, thread_count);

				this->m_prime_limit = prime_limit;

				m_primes.shrink_to_fit();

				return true;
			}
			catch (...)
			{
				this->m_prime_limit = 0;

				this->m_primes.clear();
				this->m_primes.shrink_to_fit();

				return false;
			}
		}
		

		// if n is prime, PrimeState::Yes (1)
		// if n is composite number, PrimeState::No (0)
		// if cannot determine, PrimeState::Unknown (-1)
		PrimeState IsPrimeNumber(T n)
		{
			T last_cashed = this->m_primes.back();

			if (n <= last_cashed)
			{
				return
					std::binary_search(this->m_primes.begin(), this->m_primes.end(), n)
					? PrimeState::Yes : PrimeState::No;
			}
			else
			{
				if (this->m_prime_limit < 10)
				{
					T sqrt_n = (T)sqrt((double)n);

					if (sqrt_n < last_cashed)
					{
						for (auto p : this->m_primes)
						{
							if (n%p == 0) return PrimeState::No; // not prime
							if (p > sqrt_n) return PrimeState::Yes; // prime
						}

						return PrimeState::Yes; // prime 
					}
					else
						return PrimeState::Unknown; // unknown
				}
				else
				{
					typedef std::future<PrimeState> IsPrimeFuture;
					size_t cpu_count = get_cpu_thread_count();

					std::unique_ptr<IsPrimeFuture[]> ftr(new IsPrimeFuture[cpu_count]);
					range_vctr_t rngs = tpf::SplitRangeCount<index_t>(cpu_count, 0, m_primes.size());

					this->m_StopThread.store(false);

					for (size_t i = 0; i < cpu_count; ++i)
					{
						ftr[i] = std::async(&CPrimes::is_prime, this, rngs[i], n);
					}

					PrimeState isprm;

					for (size_t i = 0; i < cpu_count; ++i)
					{
						isprm = ftr[i].get();

						if (isprm == PrimeState::Unknown)
							continue;
						else
							return isprm;
					}

					return PrimeState::Unknown;
				}
			}
		}

		size_t factorize(T value, TermVctr<T>& terms)
		{
			if (value == T(1))
			{
				terms.emplace_back(Term<T>(1, 1));
				return 1;
			}

			T val = value, pwr, q;

			for (const auto& p : this->m_primes)
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

		Vctr<T> get_divisors(T num)
		{
			Vctr<T> cntr;

			TermVctr<T> terms;

			if (factorize(num, terms))
				get_divisors(cntr, terms);

			return cntr;
		}
	};
}

#endif // end of _TPF_PRIME_NUMBERS_H