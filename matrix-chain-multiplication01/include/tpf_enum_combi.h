#ifdef _MSC_BUILD
#pragma once
#endif

#ifndef _TPF_ENUM_COMBI_H
#define _TPF_ENUM_COMBI_H

#include "tpf_smart_output.h"
#include "tpf_rational_number.h"
#include "tpf_ncr_npr.h"
#include "tpf_traits.h"

namespace tpf
{
	namespace disable
	{
		// do not use this method
		// use enum_combination(T n, T r, T mth) is faster
		template<typename T>
		std::vector<T> enum_combination_deque(T n, T r, T mth)
		{
			std::deque<T> ele((size_t)n);
			for (size_t i = 0; i < size_t(n); ++i)
				ele[i] = T(i);

			std::vector<T> v;
			v.reserve((size_t)r);

			Combi<T> combination(ele.size() - 1, r - 1);

			do
			{
				if (r <= 0)
					break;
				else if (size_t(r) == ele.size())
				{
					for (auto& e : ele)
						v.emplace_back(e);

					break;
				}
				else
				{
					if (mth < combination())
					{
						v.emplace_back(ele[0]);
						ele.pop_front(); --r;

						combination.DecreaseNR();
					}
					else
					{
						mth -= combination();
						ele.pop_front();

						combination.DecreaseN();
					}
				}

			} while (true);

			return v;
		}
	}

	// highly optimized
	template<typename T>
	std::vector<T> enum_combination(T n, T r, T mth)
	{
		std::vector<T> ele((size_t)n);

		for (long i = 0; i < long(n); ++i)
			ele[size_t(i)] = T(n-i-1);

		std::vector<T> v;
		v.reserve((size_t)r);

		Combi<T> combination(ele.size() - 1, r - 1);

		do
		{
			if (r <= 0)
				break;
			else if (size_t(r) == ele.size())
			{
				//for (auto& e : tpf::reverse(ele))
					//v.emplace_back(e);

				for (auto itr = ele.rbegin(); itr != ele.rend();
					++itr) v.emplace_back(*itr);

				break;
			}
			else
			{
				if (mth < combination())
				{
					v.emplace_back(ele.back());
					ele.pop_back(); --r;

					combination.DecreaseNR();
				}
				else
				{
					mth -= combination();
					ele.pop_back();

					combination.DecreaseN();
				}
			}

		} while (true);

		return v;
	}

	// using Vctr = std::vector<T>
	// using RefVctr = std::vector<std::reference_wrapper<T>>
	template<typename T, typename S, typename U>
	RefVctr<T> enum_combination(Vctr<T>& cntr, S r, U mth)
	{
		RefVctr<T> rlt; rlt.reserve((size_t)r);

		for (auto& idx : enum_combination(cntr.size(), size_t(r), size_t(mth)))
			rlt.emplace_back(cntr[idx]);

		return rlt;
	}

	// using Vctr = std::vector<T>
	template<typename T, typename S, typename U>
	Vctr<T> enum_combination(Vctr<T>&& cntr, S r, U mth)
	{
		Vctr<T> rlt; rlt.reserve((size_t)r);

		for (auto& idx : enum_combination(cntr.size(), size_t(r), size_t(mth)))
			rlt.emplace_back(std::move(cntr[idx]));

		return rlt;
	}
}

#endif // _TPF_ENUM_COMBI_H
