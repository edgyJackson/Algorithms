#ifdef _MSC_BUILD
#pragma once
#endif

/*
	Programmed by: Thomas Kim
	First Created: Nov. 21, 2017
	Last Modified: Nov. 21, 2017

	http://www.TalkPlayFun.com
*/

#ifndef _TPF_STOP_WATCH_H
#define _TPF_STOP_WATCH_H

#include <chrono>
#include <cstdint>
#include <locale>
#include <iostream>
#include <algorithm>

#include "tpf_types.h"
#include "tpf_string.h"


namespace tpf
{
	using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using high_resolution_clock_t = std::chrono::high_resolution_clock;

	using nanoseconds = std::chrono::nanoseconds;
	using microseconds = std::chrono::microseconds;
	using milliseconds = std::chrono::milliseconds;
	using seconds = std::chrono::seconds;
	using minutes = std::chrono::minutes;
	using hours = std::chrono::hours;

	class stop_watch
	{
	public:
		enum class DispMode
		{
			smart = 0, nano, micro, milli,
			seconds, minutes, hours
		};

	protected:
		time_point_t m_start_time;
		time_point_t m_end_time;
		std::string m_tag;
		bool m_auto;
		DispMode m_mode;
	public:

		stop_watch(const std::string& tag = "Elapsed: ", bool auto_run = true,
			DispMode mode = DispMode::smart) :
			m_tag(tag), m_auto(auto_run), m_mode(mode)
		{
			if (auto_run) reset();
		}

		~stop_watch()
		{
			if (m_auto)
			{
				stop();

				std::cout.imbue(std::locale(""));

				if (this->m_tag != "")
					std::cout << this->m_tag;

				switch (m_mode)
				{
				case DispMode::nano:
					std::cout << this->elapsed_nano() << " nano-seconds.\n"; break;

				case DispMode::micro:
					std::cout << this->elapsed_micro() << " micro-seconds.\n"; break;

				case DispMode::milli:
					std::cout << this->elapsed_micro() << " milli-seconds.\n"; break;

				case DispMode::seconds:
					std::cout << this->elapsed_seconds() << " seconds.\n"; break;

				case DispMode::minutes:
					std::cout << this->elapsed_minutes() << " minutes.\n"; break;

				case DispMode::hours:
					std::cout << this->elapsed_hours() << " hours.\n"; break;

				case DispMode::smart:
				default:
					std::cout << FormatTime() << "\n";
				}
			}
		}

		std::string FormatTime()
		{
			signed_t hour = this->elapsed_hours();
			signed_t e_minute = hour * 60;
			signed_t minute = this->elapsed_minutes() - e_minute;
			signed_t e_second = (e_minute + minute) * 60;
			signed_t second = this->elapsed_seconds() - e_second;
			signed_t e_milli = (e_second + second) * 1000;
			signed_t milli = this->elapsed_milli() - e_milli;
			signed_t e_micro = (e_milli + milli) * 1000;
			signed_t micro = this->elapsed_micro() - e_micro;
			signed_t e_nano = (e_micro + micro) * 1000;
			signed_t nano = this->elapsed_nano() - e_nano;

			std::ostringstream os;

			os.imbue(std::locale(""));

			if (hour > 0) os << hour << " hours ";
			if (minute > 0) os << minute << " minutes ";
			if (second > 0) os << second << " seconds ";
			if (milli > 0) os << milli << " milli-seconds ";
			if (micro > 0) os << micro << " micro-seconds ";
			if (nano > 0) os << nano << " nano-seconds ";

			os << " (total: " << this->elapsed_nano() << " nano-seconds)\n";

			std::string msg = os.str();

			return ltrim(rtrim(msg));
		}

		void reset()
		{
			m_start_time = high_resolution_clock_t::now();
			m_end_time = m_start_time;
		}

		time_point_t get_start_time() const
		{
			return this->m_start_time;
		}

		template<typename T = nanoseconds>
		signed_t get_start_time_tick() const
		{
			return std::chrono::duration_cast<T>
				(this->m_start_time.time_since_epoch()).count();
		}

		time_point_t get_end_time() const { return this->m_end_time; }

		template<typename T = nanoseconds>
		signed_t get_end_time_tick() const
		{
			return std::chrono::duration_cast<T>
				(this->m_end_time.time_since_epoch()).count();
		}

		time_point_t get_now() const { return high_resolution_clock_t::now(); }

		template<typename T = nanoseconds>
		signed_t get_now_tick() const
		{
			return std::chrono::duration_cast<T>
				(high_resolution_clock_t::now().time_since_epoch()).count();
		}

		stop_watch& capture_end_time()
		{
			this->m_end_time = high_resolution_clock_t::now();
			return *this;
		}

		stop_watch& stop()
		{
			return capture_end_time();
		}

		stop_watch& capture()
		{
			return capture_end_time();
		}

		template<typename T = nanoseconds>
		T get_elapsed() const
		{
			return std::chrono::duration_cast<T>(this->m_end_time - this->m_start_time);
		}

		template<typename T = nanoseconds>
		signed_t get_elapsed_tick() const
		{
			return std::chrono::duration_cast<T>(this->m_end_time - this->m_start_time).count();
		}

		signed_t elapsed_time(DispMode mode = DispMode::nano) const
		{
			switch (mode)
			{
			case DispMode::hours:
				return get_elapsed_tick<hours>();

			case DispMode::minutes:
				return get_elapsed_tick<minutes>();

			case DispMode::seconds:
				return get_elapsed_tick<seconds>();

			case DispMode::milli:
				return get_elapsed_tick<milliseconds>();

			case DispMode::micro:
				return get_elapsed_tick<microseconds>();

			case DispMode::nano:
			default:
				return get_elapsed_tick<nanoseconds>();
			}
		}

		signed_t elapsed_nano() const
		{
			return get_elapsed_tick<tpf::nanoseconds>();
		}

		signed_t elapsed_micro() const
		{
			return get_elapsed_tick<tpf::microseconds>();
		}

		signed_t elapsed_milli() const
		{
			return get_elapsed_tick<tpf::milliseconds>();
		}

		signed_t elapsed_seconds() const
		{
			return get_elapsed_tick<tpf::seconds>();
		}

		signed_t elapsed_minutes() const
		{
			return get_elapsed_tick<tpf::minutes>();
		}

		signed_t elapsed_hours() const
		{
			return get_elapsed_tick<tpf::hours>();
		}
	};

	template<typename TypeRtn = signed_t, typename T = nanoseconds>
	TypeRtn get_now_tick()
	{
		return TypeRtn(std::chrono::duration_cast<T>(high_resolution_clock_t::now().time_since_epoch()).count());
	}
}


#endif // end of _TPF_STOP_WATCH_H
