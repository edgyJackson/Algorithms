#ifdef _MSC_BUILD
#pragma once
#endif

#ifndef _TPF_SET_OPERATION_H
#define _TPF_SET_OPERATION_H

/*
	Author: Thomas Kim
	First Edit: Feb. 20, 2017
	Last Edit: Nov. 26, 2017

	http://www.TalkPlayFun.com

*/

#include "tpf_smart_output.h"

namespace tpf
{
	template<typename T>
	Set<T> set_union(Set<T>&& A) { return A; }

	template<typename T, typename... Ts>
	Set<T> set_union(Set<T>&& A, Ts&&... B)
	{
		Set<T> S = A;

		for (const auto& item : set_union(std::forward<Ts>(B)...))
			S.insert(item);

		return S;
	}

	template<typename T>
	Set<T> set_union(const Set<T>& A) { return A; }

	template<typename T, typename... Ts>
	Set<T> set_union(const Set<T>& A, Ts&... B)
	{
		Set<T> S = A;

		for (const auto& item : set_union(B...))
			S.insert(item);

		return S;
	}

	template<typename T>
	Vctr<T> set_union(const Vctr<T>& A) { return A; }

	template<typename T, typename... Ts>
	Vctr<T> set_union(const Vctr<T>& A, Ts&... B)
	{
		Vctr<T> S = A;

		for (const auto& item : set_union(B...))
			S.emplace_back(item);

		std::sort(S.begin(), S.end());
		S.erase(std::unique(S.begin(), S.end()), S.end());

		return S;
	}

	template<typename T>
	Set<T> set_intersection(Set<T>&& A) { return A; }

	// should A and B both sorted
	template<typename T, typename... Ts>
	Set<T> set_intersection(Set<T>&& A, Ts&&...B)
	{
		Set<T> S;

		for (const auto& a : A)
		{
			for (const auto& b : set_intersection(std::forward<Ts>(B)...))
			{
				if (a == b)
				{
					S.insert(a); break;
				}
				else if (a < b) break;
			}
		}

		return S;
	}

	template<typename T>
	Set<T> set_intersection(const Set<T>& A) { return A; }

	template<typename T, typename... Ts>
	Set<T> set_intersection(const Set<T>& A, const Ts&...B)
	{
		Set<T> S;

		for (const auto& a : A)
		{
			for (const auto& b : set_intersection(B...))
			{
				if (a < b)
					break;
				else if (a == b)
				{
					S.insert(a); break;
				}
			}
		}

		return S;
	}

	template<typename T>
	Vctr<T> set_intersection(const Vctr<T>& A) { return A; }

	// set A, B, C should be sorted
	template<typename T, typename... Ts>
	Vctr<T> set_intersection(const Vctr<T>& A, const Ts&...B)
	{
		Vctr<T> S;

		for (const auto& a : A)
		{
			for (const auto& b : set_intersection(B...))
			{
				// set A, B, C should be sorted
				if (a < b)
					break;
				else if (a == b)
				{
					S.emplace_back(a); break;
				}
			}
		}

		return S;
	}

	template<typename T>
	Set<T> set_difference(Set<T>&& A, Set<T>&& B)
	{
		Set<T> S; T ele_b;

		for (const auto& a : A)
		{
			for (const auto& b : B)
			{
				ele_b = b;

				if (a == ele_b) break;
				else if (a < b) break;
			}

			if (a != ele_b) S.emplace(a);
		}

		return S;
	}

	template<typename T>
	Set<T> set_difference(const Set<T>& A, const Set<T>& B)
	{
		Set<T> S; T ele_b;

		for (const auto& a : A)
		{
			for (const auto& b : B)
			{
				ele_b = b;

				if (a == ele_b) break;
				else if (a < b) break;
			}

			if (a != ele_b) S.emplace(a);
		}

		return S;
	}

	// possibly should be sorted
	template<typename T>
	Vctr<T> set_difference(const Vctr<T>& A, const Vctr<T>& B)
	{
		Vctr<T> S; T ele_b;

		for (const auto& a : A)
		{
			for (const auto& b : B)
			{
				ele_b = b;

				if (a == ele_b) break;
				else if (a < b) break;
			}

			if (a != ele_b) S.emplace_back(a);
		}

		return S;
	}
}

#endif // SILINERS_SET_OPERATION_H
