#ifdef _MSC_BUILD
#pragma once
#endif

/*
Programmed by: Thomas Kim
First Created: Nov. 21, 2017
Last Modified: Nov. 21, 2017
Fixed: Feb. 03, 2019 - compatibility for GNU C++ and clang++

http://www.TalkPlayFun.com

*/

#ifndef _TPF_RANDOM_H
#define _TPF_RANDOM_H

#include <chrono>
#include <random>

#include "tpf_stop_watch.h"

namespace tpf
{
template <typename T>
using uniform_integer_dist_t = std::uniform_int_distribution<T>;

template <typename T>
using uniform_real_dist_t = std::uniform_real_distribution<T>;

template <typename T>
using normal_dist_t = std::normal_distribution<T>;

using uniform_signed_dist_t = uniform_integer_dist_t<signed_t>;
using uniform_unsigned_dist_t = uniform_integer_dist_t<unsigned_t>;
using uniform_int_dist_t = uniform_integer_dist_t<int>;
using uniform_unsigned_int_dist_t = uniform_integer_dist_t<unsigned int>;

template <typename T, typename TDistribution,
		  typename TEngine = std::default_random_engine>
class random_base
{

  public:
	typedef T value_type;
	typedef TDistribution distribution_t;
	typedef TEngine engine_t;

  protected:
	distribution_t m_distribution;
	engine_t m_engine;
	T m_from, m_end;

  protected:
	random_base(T from, T to, bool reset = true) :
	 	m_distribution{from, to},
		m_engine{reset ? (unsigned)get_now_tick<T>() : 0}
	{
		// if (reset)
		// 	 m_engine.((unsigned)get_now_tick<T>());
	}

  public:
	T operator()()
	{
		return this->m_distribution(this->m_engine);
	}
};

template <typename TypeInt = signed_t,
		  typename TDistribution = std::uniform_int_distribution<TypeInt>,
		  typename TEngine = std::default_random_engine>
class integer_random : public random_base<TypeInt, TDistribution, TEngine>
{

  public:
	integer_random(TypeInt from = 0, TypeInt to = 100, bool reset = true) :
		random_base<TypeInt, TDistribution, TEngine>{from, to, reset} {}
};

template <typename TypeReal = bigreal_t,
		  typename TDistribution = std::uniform_real_distribution<TypeReal>,
		  typename TEngine = std::default_random_engine>
class real_random : public random_base<TypeReal, TDistribution, TEngine>
{

  public:
	real_random(TypeReal from = 0, TypeReal to = 1, bool reset = true) : random_base<TypeReal, TDistribution, TEngine>{from, to, reset} {}
};

using random_signed_t = integer_random<signed_t>;
using random_unsigned_t = integer_random<unsigned_t>;
using random_int_t = integer_random<int>;
using random_unsigned_int_t = integer_random<unsigned int>;

using random_bigreal_t = real_random<bigreal_t>;
using random_real_t = real_random<real_t>;

namespace hide
{
template <typename T>
struct random_st
{
	using type = integer_random<long long>;
};

template <>
struct random_st<int>
{
	using type = integer_random<int>;
};

template <>
struct random_st<unsigned>
{
	using type = integer_random<unsigned>;
};

template <>
struct random_st<long long>
{
	using type = integer_random<long long>;
};

template <>
struct random_st<unsigned long long>
{
	using type = integer_random<unsigned long long>;
};

template <>
struct random_st<float>
{
	using type = real_random<float>;
};

template <>
struct random_st<double>
{
	using type = real_random<double>;
};
} // namespace hide

template <typename T>
using random_t = typename hide::random_st<T>::type;
} // namespace tpf

#endif // _TPF_RANDOM_H