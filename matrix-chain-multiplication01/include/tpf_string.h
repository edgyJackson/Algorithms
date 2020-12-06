#ifdef _MSC_BUILD
#pragma once
#endif

/*
	Programmed by: Thomas Kim
	First Created: Nov. 21, 2017
	Last Modified: Nov. 21, 2017

	http://www.TalkPlayFun.com

*/

#ifndef _TPF_STRING_H
#define _TPF_STRING_H

#include <string>
#include <sstream>
#include <algorithm>
#include <locale>

namespace tpf
{
	template<typename Tstr>
	Tstr& ltrim(Tstr& str)
	{
		auto it2 = std::find_if(str.begin(), str.end(),
			[](auto ch) { return !std::isspace<decltype(ch)>(ch, std::locale::classic()); });

		str.erase(str.begin(), it2);
		return str;
	}

	template<typename Tstr>
	Tstr& rtrim(Tstr& str)
	{
		auto it1 = std::find_if(str.rbegin(), str.rend(),
			[](auto ch) { return !std::isspace<decltype(ch)>(ch, std::locale::classic()); });
		str.erase(it1.base(), str.end());
		return str;
	}
}

#endif // end of _TPF_STRING_H