#ifdef _MSC_BUILD
#pragma once
#endif

#ifndef _TPF_SMART_OUTPUT_H
#define _TPF_SMART_OUTPUT_H

/*
Author: Thomas Kim
First Edit: Feb. 20, 2017

http://www.TalkPlayFun.com

*/

#include <iostream>
#include <sstream>
#include <string>
#include <locale>
#include <iomanip>
#include <utility>

#include "tpf_stop_watch.h"
#include "tpf_traits.h"

namespace tpf
{
	namespace io
	{
		template<typename S, typename T>
		std::ostream& operator<<(std::ostream& os, const std::pair<S, T>& p)
		{
			os << "<: " << p.first << ", " << p.second << " :>";
			return os;
		}

		template<typename T>
		std::ostream& operator<<(std::ostream& os, const std::vector<T>& cntr)
		{
			if (cntr.empty()) return os;

			os << "{ ";

			auto last_element = --cntr.cend();
			for (auto itr = cntr.cbegin();
				itr != last_element; ++itr)
			{
				os << *itr << ", ";
			}

			os << *last_element << " }";

			return os;
		}

		template<typename T>
		std::ostream& operator<<(std::ostream& os, 
			const std::vector<std::reference_wrapper<T>>& cntr)
		{
			if (cntr.empty()) return os;

			os << "{ ";

			auto last_element = --cntr.cend();
			for (auto itr = cntr.cbegin();
				itr != last_element; ++itr)
			{
				os << itr->get() << ", ";
			}

			os << last_element->get() << " }";

			return os;
		}

		template<typename T>
		std::ostream& operator<<(std::ostream& os, const std::deque<T>& cntr)
		{
			if (cntr.empty()) return os;

			os << "{ ";

			auto last_element = --cntr.cend();
			for (auto itr = cntr.cbegin();
				itr != last_element; ++itr)
			{
				os << *itr << ", ";
			}

			os << *last_element << " }";

			return os;
		}

		template<typename T>
		std::ostream& operator<<(std::ostream& os, 
			const std::deque<std::reference_wrapper<T>>& cntr)
		{
			if (cntr.empty()) return os;

			os << "{ ";

			auto last_element = --cntr.cend();
			for (auto itr = cntr.cbegin();
				itr != last_element; ++itr)
			{
				os << itr->get() << ", ";
			}

			os << last_element->get() << " }";

			return os;
		}

		template<typename T>
		std::ostream& operator<<(std::ostream& os, const std::set<T>& cntr)
		{
			if (cntr.empty()) return os;

			os << "{ ";

			auto last_element = --cntr.cend();
			for (auto itr = cntr.cbegin();
				itr != last_element; ++itr)
			{
				os << *itr << ", ";
			}

			os << *last_element << " }";

			return os;
		}
	}
}

#endif // end of _TPF_SMART_OUTPUT_H