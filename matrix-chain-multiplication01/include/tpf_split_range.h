#ifdef _MSC_BUILD
#pragma once
#endif

/*
Programmed by: Thomas Kim
First Version: Nov. 12, 2017
Last Version:  Nov. 21, 2017

http://www.TalkPlayFun.com

if you want to re-define default index_t to something else
for example, int, then

#define index_t int

before including this file to stdafx.h, such as

#define index_t int
#include "tmp_split_ranges.h"

Do NOT include "tpf_split_range.h" to multiple files
*/

#ifndef _TPF_SPLIT_RANGE_H
#define _TPF_SPLIT_RANGE_H

#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <vector>
#include <thread>
#include <functional>
#include <future>
#include <utility>
#include <string>

#include "tpf_types.h"

namespace tpf
{
	template<typename T>
	using range = std::pair<T, T>;

	typedef range<index_t> range_t;

	template<typename T>
	using rngvctr = std::vector<range<T>>;

	typedef rngvctr<index_t> range_vctr_t;

	template<typename T> std::string DisplayRange(const range<T>& rg, bool thousands = true)
	{
		std::ostringstream os;

		if (thousands)
			os.imbue(std::locale(""));
		os << "[" << rg.first << ", " << rg.second << ") - count: " << (rg.second - rg.first);

		return os.str();
	}

	/*
	st                              ed
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11

	minimum span = 3; [1, 5), [5, 8), [8, 11)

	1, 2,  3,  4, 5		-> [1, 5) : 4
	5, 6,  7,  8			-> [5, 8) : 3
	8, 9, 10,  11			-> [8, 11): 3
	*/
	template<typename T>
	rngvctr<T> SplitRangeSpan(T min_span, T st, T ed)
	{
		T dist = ed - st;
		T rnd = dist % min_span;
		T count = dist / min_span;

		rngvctr<T> rng;
		if (rnd == 0)
		{
			T prev = st;
			for (T i = 0; i < count; ++i)
			{
				rng.emplace_back(range<T>(prev, prev + min_span));
				prev += min_span;
			}

			return rng;
		}
		else
		{
			if (count >= rnd)
			{
				T span1 = min_span + 1;

				T prev = st;
				for (T i = 0; i < rnd; ++i)
				{
					rng.emplace_back(range<T>(prev, prev + span1));
					prev += span1;
				}

				count = count - rnd;
				for (T i = 0; i < count; ++i)
				{
					rng.emplace_back(range<T>(prev, prev + min_span));
					prev += min_span;
				}

				return rng;
			}
			else
			{
				return SplitRangeCount(count, st, ed);
			}
		}
	}

#define SPLIT_RANGE_SPAN(span, start, end) tpf::SplitRangeSpan<tpf::index_t>((span), (start), (end))

	template<typename T>
	rngvctr<T> SplitRangeCount(size_t count, T st, T ed)
	{
		T dist = ed - st;
		T span = dist / (T)count;
		T rnd = dist % (T)count;

		rngvctr<T> rng;

		if (rnd == 0)
		{
			T prev = st;
			for (T i = 0; i < (T)count; ++i)
			{
				rng.push_back(range<T>(prev, prev + span));
				prev += span;
			}

			return rng;
		}
		else
		{
			T span1 = span + 1;

			T prev = st;
			for (T i = 0; i < rnd; ++i)
			{
				rng.emplace_back(range<T>(prev, prev + span1));
				prev += span1;
			}
			// rnd = dist % count;
			// rnd cannot be greater than count
			count = count - (size_t)rnd;
			for (T i = 0; i < (T)count; ++i)
			{
				rng.emplace_back(range<T>(prev, prev + span));
				prev += span;
			}

			return rng;
		}
	}

#define SPLIT_RANGE_COUNT(count, start, end) tpf::SplitRangeCount<tpf::index_t>((count), (start), (end))

	template<typename T>
	std::string DisplayRanges(const rngvctr<T>& rngs, int width = 9, bool thousands = true)
	{
		if (!rngs.empty())
		{
			std::ostringstream os;

			if (thousands)
				os.imbue(std::locale(""));

			os << "Range count: " << rngs.size() << std::endl <<
				"Max range: " << (rngs[0].second - rngs[0].first) <<
				" elements, Min range: " << (rngs.back().second - rngs.back().first)
				<< " elements." << std::endl << std::endl;

			index_t count = (index_t)rngs.size();

			for (index_t i = 0; i < count; ++i)
			{
				os << std::setw(3) << i << ": [" << std::setw(width)
					<< rngs[(size_t)i].first << ", " << std::setw(width)
					<< rngs[(size_t)i].second << ") " << std::setw(width)
					<< (rngs[(size_t)i].second - rngs[(size_t)i].first)
					<< " elements" << std::endl;
			}

			return os.str();
		}
		else
			return std::string("Range empty.");
	}

	inline index_t get_cpu_thread_count()
	{
		return (index_t)std::thread::hardware_concurrency();
	}
}

#endif // end of _TPF_SPLIT_RANGE_H