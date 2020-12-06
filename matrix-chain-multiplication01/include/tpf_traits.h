#ifdef _MSC_BUILD
#pragma once
#endif

/*
Programmed by: Thomas Kim
First Created: Nov. 21, 2017
Last Modified: Nov. 21, 2017

http://www.TalkPlayFun.com

*/

#ifndef _TPF_TRAITS_H
#define _TPF_TRAITS_H

#include <vector>
#include <deque>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <string>
#include <locale>
#include <deque>
#include <iomanip>
#include <utility>
#include <type_traits>
#include <iterator>

namespace tpf
{
	template<typename T>
	struct reverse_adaptor
	{
		T& m_cntr;
	};

	template<typename T>
	auto begin(reverse_adaptor<T> cnt)
	{
		return std::rbegin(cnt.m_cntr);
	}

	template<typename T>
	auto end(reverse_adaptor<T> cnt)
	{
		return std::rend(cnt.m_cntr);
	}

	template<typename T>
	reverse_adaptor<T> reverse(T&& cntr) { return{ cntr }; }

	template <typename T> struct container_traits: 
		public std::false_type 
	{
		static bool const value = false;
	};

	template <typename... Args>
	struct container_traits<std::vector<Args...>>:
		public std::true_type
	{
		static bool const value = true;
	};

	template <typename... Args>
	struct container_traits<std::deque<Args...>> :
		public std::true_type 
	{
		static bool const value = true;
	};

	template <typename... Args>
	struct container_traits<std::list<Args...>>:
		public std::true_type
	{
		static bool const value = true;
	};

	template <typename... Args>
	struct container_traits<std::set<Args...>> :
		public std::true_type 
	{
		static bool const value = true;
	};

	template <typename... Args>
	struct container_traits<std::map<Args...>> :
		public std::true_type 
	{
		static bool const value = true;
	};

	template <typename... Args>
	struct container_traits<std::unordered_set<Args...>> :
		public std::true_type 
	{
		static bool const value = true;
	};

	template <typename... Args>
	struct container_traits<std::unordered_map<Args...>> :
		public std::true_type 
	{
		static bool const value = true;
	};

	template<typename T>
	struct is_container: public container_traits<T> 
	{
		static constexpr bool const value = container_traits<T>::value;
	};

	template<typename T>
	using Vctr = std::vector<T>;
	
	template<typename T>
	using RefVctr = std::vector<std::reference_wrapper<T>>;

	template<typename T>
	using CRefVctr = std::vector<std::reference_wrapper<const T>>;
	
	template<typename T>
	using Set = std::set<T>;

	template<typename Tkey, typename Tval>
	using Map = std::map<Tkey, Tval>;

	template<typename T>
	using Deque = std::deque<T>;
}

#endif // end of _TPF_TRAITS_H
