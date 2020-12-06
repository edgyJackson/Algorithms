#ifdef _MSC_BUILD
#pragma once
#endif

/*
	Programmed by: Thomas Kim
	First Created: Nov. 21, 2017
	Last Modified: Nov. 21, 2017

	http://www.TalkPlayFun.com
*/

#ifndef _TPF_TYPES_H
#define _TPF_TYPES_H

#ifndef TPF_TYPES_INTEGER
#define TPF_TYPES_INTEGER

#include <cstdint>

namespace tpf
{
	/// default integer type
	// intmax_t is long long on Windows
	using signed_t = intmax_t;

	/// default index type
	/// on Win32 (or x86) - index_t is 4 bytes (32 bits) - unsigned int
	/// on Win64 (or x64) - index_t is 8 bytes (64 bits) - unsigned long long or unsigned __int64 
	using index_t = size_t;

	/// default unsigned integer type
	/// uintmax_t is unsigned long long on Windows
	using unsigned_t = uintmax_t;
}
#endif // end of TPF_TYPES_INTEGER

#ifndef TPF_TYPES_REAL
#define TPF_TYPES_REAL
	namespace tpf
	{
		/// default real number type
		using real_t = double;

		/// for big real
		using bigreal_t = long double;

		/// default short real
		using shtreal_t = float;
	}
#endif // end of TPF_TYPES_REAL

#endif // end of _TPF_TYPES_H