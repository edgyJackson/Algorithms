#ifdef _MSC_BUILD
#pragma once
#endif

#ifndef _TPF_NCR_NPR_H
#define _TPF_NCR_NPR_H

/*
Author: Thomas Kim
First Edit: Feb. 20, 2017
Last Edit: Nov. 27, 2017

http://www.TalkPlayFun.com

*/

#include <cstdint>
#include <sstream>
#include <exception>
#include <type_traits>

#include "tpf_rational_number.h"

namespace tpf
{
	// if fails, either return 0 or throws range_error
	template<typename T> inline T combi(T n, T r, bool bThrow = false)
	{
		if (r == 0 && n>=0)
		{
			return 1;
		}
		else if (n == r && n > 0)
		{
			return 1;
		}
		else if (r == (n - 1) || r == 1)
		{
			return n;
		}
		else if (r >= 0 && r <= n)
		{
			if (r > (n - r)) r = n - r;

			UnsignedRational<T> rlt(n, r);

			for (T i = 1; i < r; ++i)
				rlt *= UnsignedRational<T>(n - i, r - i);

			return rlt.p();
		}
		else
		{
			if (bThrow)
			{
				std::ostringstream os;
				os << "ranage error at combi(n=" << n << ", r="
					<< r << ") - where r should be r <= n";
				throw std::range_error(os.str());
			}
			else
				return 0;
		}
	}

	// if fails, either returns 0 or throws range error
	template<typename T> inline T ncr(T n, T r, bool bThrow = false)
	{
		return combi(n, r, bThrow);
	}

	// if fails, either returns 0 or throws range error
	template<typename T> inline T fact(T n, bool bThrow)
	{
		if (n == 0 || n == 1)
			return 1;
		else if (n > 1)
		{
			T rlt = 1;

			for (T i = 1; i <= n; ++i)
				rlt *= i;

			return rlt;
		}
		else
		{
			if (bThrow)
			{
				std::ostringstream os;
				os << :"range error at fact(n=" << n < "), where n >= 0";
				throw std::range_error(os.str());
			}
			else
				return 0;
		}
	}

	// if fails, either returns 0 or throws range error
	template<typename T> inline T permu(T n, T r, bool bThrow = false)
	{
		if (r == 0 && n > 0)
			return 1;
		else if (r == 1 && n > 0)
			return n;
		else if (r > 0 && r <= n)
		{
			T rlt = 1;

			for (T i = 0; i < r; ++i)
				rlt *= (n - i);

			return rlt;
		}
		else
		{
			if (bThrow)
			{
				std::ostringstream os;
				os << "range error at permu(n=" << n << ", r=" << r << ")";
				throw std::range_error(os.str());
			}
			else
				return 0;
		}
	}

	// if fails, either returns 0 or throw range_error
	template<typename T> inline T npr(T n, T r, bool bThrow = false)
	{
		return permu(n, r, bThrow);
	}

	template<typename T> class Combi
	{
	protected:
		mutable T m_combi, m_n, m_r;

		void reset()
		{
			this->m_n = T(0); this->m_r = T(0);
			this->m_combi = T(1);
		}

		void offset_nr(intmax_t offset)
		{
			//std::cout << __FUNCSIG__ << ", offset: " << offset << std::endl;
			
			if (offset < 0)
			{
				while (offset < 0)
				{
					this->m_combi =
						UnsignedRational<T>(m_r--, m_n--)*this->m_combi;

					++offset;
				}
			}
			else if (offset > 0)
			{
				while (offset > 0)
				{
					this->m_combi =
						UnsignedRational<T>(++m_n, ++m_r)*this->m_combi;

					--offset;
				}
			}
		}

		void offset_n(intmax_t offset)
		{
			//std::cout << __FUNCSIG__ << ", offset: " << offset << std::endl;

			if (offset > 0)
			{
				while (offset > 0)
				{
					this->m_combi =
						UnsignedRational<T>(m_n + 1, m_n - m_r + 1)*this->m_combi;
					++m_n; --offset;
				}
			}
			else if (offset < 0)
			{
				while (offset < 0)
				{
					this->m_combi =
						UnsignedRational<T>(m_n - m_r, m_n)*this->m_combi;
					--m_n; ++offset;
				}
			}
		}

		void offset_r(intmax_t offset)
		{
			//std::cout << __FUNCSIG__ << ", offset: " << offset << std::endl;
			
			if (offset > 0)
			{
				while (offset > 0)
				{
					this->m_combi =
						UnsignedRational<T>(m_n - m_r, m_r + 1)*this->m_combi;
					++m_r; --offset;
				}
			}
			else if (offset < 0)
			{
				while (offset < 0)
				{
					this->m_combi =
						UnsignedRational<T>(m_r, m_n - m_r + 1)*this->m_combi;
					--m_r; ++offset;
				}
			}
		}

	public:
		Combi() { reset();  }

		template<typename X, typename Y>
		Combi(X n, Y r, bool bThrow=false)
		{
			reset(); // in case throws
			
			this->m_combi = combi(T(n), T(r), bThrow);

			// in case of throw, the following won't be executed
			this->m_n = T(n); this->m_r = T(r);
		}

		Combi(const Combi& rhs) = default;
		Combi& operator=(const Combi& rhs) = default;
		
		const T& operator()()const { return this->m_combi; }
		
		const T& DecreaseNR()
		{
			this->m_combi =
				UnsignedRational<T>(m_r--, m_n--)*this->m_combi;
			
			return this->m_combi;
		}

		const T& IncreaseNR()
		{
			this->m_combi =
				UnsignedRational<T>(++m_n, ++m_r)*this->m_combi;

			return this->m_combi;
		}
			
		const T& IncreaseR()
		{
			this->m_combi =
				UnsignedRational<T>(m_n - m_r, m_r + 1)*this->m_combi;
			
			// we should do this, because compiler
			// may evaluate on the right handside first
			++m_r; 

			return this->m_combi;
		}

		const T& DecreaseR()
		{
			this->m_combi =
				UnsignedRational<T>(m_r, m_n - m_r + 1)*this->m_combi;

			// we should do this, because compiler
			// may evaluate on the right handside first
			--m_r;

			return this->m_combi;
		}

		const T& IncreaseN()
		{
			this->m_combi =
				UnsignedRational<T>(m_n + 1, m_n - m_r + 1)*this->m_combi;
			
			// we should do this, because compiler
			// may evaluate on the right handside first
			++m_n;

			return this->m_combi;
		}

		const T& DecreaseN()
		{
			this->m_combi =
				UnsignedRational<T>(m_n - m_r, m_n)*this->m_combi;
			
			// we should do this, because compiler
			// may evaluate on the right handside first
			--m_n;

			return this->m_combi;
		}

		// if error either returns 0 or throw range_error
		template<typename X, typename Y>
		T operator() (X n, Y r, bool bThrow=false)
		{
			// no change
			if (m_n == T(n) && m_r == T(r))
			{
				return this->m_combi;
			}
			else if (r >= 0 && r <= n)
			{
				if (r == 0 || n == 0)
				{
					reset();
				}
				else if (r == n && n > 0)
				{
					this->m_n = T(n); this->m_r = T(r);
					this->m_combi = T(1);
				}
				else if (r == (n - 1) || r == 1)
				{
					this->m_n = T(n); this->m_r = T(r);
					this->m_combi = T(n);
				}
				else
				{
					T nr = T(n) - T(r);
					T sr = T(r) > nr ? nr : T(r);

					constexpr intmax_t offset = 6;

					if (sr < offset) // for small r, direct compute
					{
						this->m_n = T(n); this->m_r = T(r);

						UnsignedRational<T> rlt(T(n), sr);

						for (T i = 1; i < sr; ++i)
							rlt *= UnsignedRational<T>(T(n - i), T(sr - i));

						this->m_combi = T(rlt.p());

						//std::cout << "Here 1" << std::endl;
					}
					else
					{
						intmax_t dn = intmax_t(n) - intmax_t(this->m_n);
						intmax_t dr = intmax_t(r) - intmax_t(this->m_r);

						if (dn == 0 && dr != 0)
							offset_r(dr);
						else if (dr == 0 && dn != 0)
							offset_n(dn);
						else if (dn == dr && dr != 0)
							offset_nr(dn);
						else // dn != dr && dn != 0 && dr != 0
						{
							this->m_n = T(n); this->m_r = T(r);

							UnsignedRational<T> rlt(T(n), sr);

							for (T i = 1; i < sr; ++i)
								rlt *= UnsignedRational<T>(T(n - i), sr - i);

							this->m_combi = T(rlt.p());
						}
					}
				}

				return this->m_combi;
			}
			else // error condition
			{
				// do not change
				// reset();

				if (bThrow)
				{
					std::ostringstream os;
					os << "ranage error at combi(n=" << n << ", r="
						<< r << ") - where r should be r <= n";
					throw std::range_error(os.str());
				}
				else
					return 0;
			}
		}
		
		operator const T& () const { return this->m_combi; }

		template<typename S>
		friend std::ostream& operator<<(std::ostream& os, const Combi<S>& cb);
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Combi<T>& cb)
	{
		os.imbue(std::locale(""));
		os << cb.m_n << "C" << cb.m_r << "=" << cb.m_combi;
		return os;
	}

	template<typename T> class Permu
	{
	protected:
		mutable T m_n, m_r;
		mutable T m_permu;
				
		void reset()
		{
			this->m_n = T(0); this->m_r = (0);
			this->m_permu = T(1);
		}
	public:
		Permu() { reset(); }

		template<typename X, typename Y>
		Permu(X n, Y r, bool bThrow=false)
		{
			reset(); // in case of throws
			
			this->m_permu = permu(T(n), T(r), bThrow);
			
			// in case of throw, the following won't be executed
			this->m_n = T(n); this->m_r = T(r);
		}

		Permu(const Permu& rhs) = default;
		Permu& operator=(const Permu& rhs) = default;

		const T& operator()() const { return this->m_permu;}
		
		// if fails, either returns 0 or throw range_error
		template<typename X, typename Y>
		T operator()(X n, Y r, bool bThrow = false)
		{
			// no change
			if (m_n == T(n) && m_r == T(r))
			{
				return this->m_permu;
			}
			else if (r == 0 && n > 0)
			{
				this->m_n = T(n); this->m_r = T(0);
				this->m_permu = 1;

				return this->m_permu;
			}
			else if (r == 1 && n > 0)
			{
				this->m_n = T(n); this->m_r = T(1);
				this->m_permu = T(n);

				return this->m_n;
			}
			else if (r > 0 && r <= n)
			{
				constexpr T offset = 6;
				
				if (r < offset) // direct calculation
				{
					this->m_n = n; this->m_r = r;
					this->m_permu = 1;

					for (T i = 0; i < r; ++i)
						this->m_permu *= (n - i);
				}
				else
				{
					T dr = T(r) - T(this->m_r);
					T dn = T(n) - T(this->m_n);
					
					if (dn == 0)
					{
						if (dr > 0) // dn == 0, dr > 0
						{
							T last = this->m_n - this->m_r + 1;
							T end = this->m_n - r + 1; this->m_r = r;

							while (last > end)
							{
								this->m_permu *= --last;
							}
						}
						else if (dr < 0) // dn == 0, dr < 0
						{
							T last = this->m_n - this->m_r + 1;
							T end = this->m_n - r + 1; this->m_r = r;

							while (last < end)
							{
								this->m_permu /= last++;
							}
						}
						// else dn==0, dr==0, no need to recompute
					}
					else if (dr == 0)
					{
						if (dn > 0)
						{
							T last = this->m_n - r;

							while (n > this->m_n)
							{
								this->m_permu /= ++last;
								this->m_permu *= ++this->m_n;
							}
						}
						else if (dn < 0)
						{
							T last = this->m_n - r;

							while (n < this->m_n)
							{
								this->m_permu /= this->m_n--;
								this->m_permu *= last++;
							}
						}
						// else dr == 0, dn == 0, no need to recompute
					}
					else if (dr == dn)
					{
						if (dn > 0)
						{
							while (n > this->m_n)
							{
								++this->m_r;
								this->m_permu *= ++this->m_n;
							}
						}
						else if (dn < 0)
						{
							while (n < this->m_n)
							{
								--this->m_r;
								this->m_permu /= this->m_n--;
							}
						}
						// else, dr == 0, dn == 0, no need to recompute
					}
					else // dn != 0, dr != 0, dn != dr
					{
						this->m_n = n; this->m_r = r;
						this->m_permu = 1;

						for (T i = 0; i < r; ++i)
							this->m_permu *= (n - i);
					}
				}

				return this->m_permu;
			}
			else
			{
				// do not change
				// reset();

				if (bThrow)
				{
					std::ostringstream os;
					os << "range error at Permu()(n=" << n << ", r=" << r << ")";
					throw std::range_error(os.str());
				}
				else
					return 0;
			}
		}

		operator const T& () const { return this->m_permu; }

		template<typename S>
		friend std::ostream& operator<<(std::ostream& os, const Permu<S>& p);
	};
	
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const Permu<T>& p)
	{
		os.imbue(std::locale(""));
		os << p.m_n << "P" << p.m_r << "=" << p.m_permu;
		return os;
	}
}

#endif // _TPF_NCR_NPR_H