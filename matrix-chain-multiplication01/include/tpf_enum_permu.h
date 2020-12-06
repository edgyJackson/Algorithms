#ifdef _MSC_BUILD
#pragma once
#endif

#ifndef _TPF_ENUM_PERMU_H
#define _TPF_ENUM_PERMU_H

#include "tpf_rational_number.h"
#include "tpf_ncr_npr.h"
#include "tpf_smart_output.h"
#include "tpf_traits.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <exception>
#include <cstdint>
#include <iomanip> 
#include <string>

namespace tpf
{
	// this is the fastest and most efficient
	template<typename T>
	std::vector<T> enum_permutation(T n, T r, T mth)
	{
		std::vector<T> v;

		if (!(r < 1 || n<1 || r > n))
		{
			v.reserve((size_t)r);

			std::vector<T> e((size_t)n);

			// initialize set e
			// e = { 0, 1, 2, ..., n-1}
			for (size_t i = 0; i < size_t(n); ++i)
				e[i] = T(i);

			T npr = permu(T(e.size() - 1), T(r - 1));

			while (r) // r != 0
			{
				// compute index
				size_t s = size_t(mth / npr);
				mth %= npr;

				// move s-th element of e from e to v
				v.emplace_back(e[s]);

				// remove s-th element from e
				auto itr = e.begin() + s;
				e.erase(itr); --r;

				if (e.size() > 1) npr /= T(e.size());
			}
		}

		return v;
	}

	template<typename T, typename S, typename U>
	RefVctr<T> enum_permutation(Vctr<T>& cntr, S r, U mth)
	{
		RefVctr<T> rlt; rlt.reserve((size_t)r);

		for (auto& idx : enum_permutation(cntr.size(), size_t(r), size_t(mth)))
			rlt.emplace_back(cntr[idx]);

		return rlt;
	}

	// using Vctr = std::vector<T>
	template<typename T, typename S, typename U>
	Vctr<T> enum_permutation(Vctr<T>&& cntr, S r, U mth)
	{
		Vctr<T> rlt; rlt.reserve((size_t)r);

		for (auto& idx : enum_permutation(cntr.size(), size_t(r), size_t(mth)))
			rlt.emplace_back(std::move(cntr[idx]));

		return rlt;
	}
}

#endif // _TPF_ENUM_PERMU_H