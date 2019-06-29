#ifndef __ITIMIZE_H__
#define __ITIMIZE_H__

#include <iostream>
#include <ctime>
#include <chrono>
#include <map>


#define STR(X) #X
#define STR1(X) STR(X)

#define COMBINE1(X,Y) X##Y 
#define COMBINE(X,Y) COMBINE1(X,Y)

#define LOCATION std::string(__FUNCTION__) +  ":" + STR1(__LINE__)

#define TIME_NAMED_BLOCK(label) itimize::Ticker COMBINE(_timer_,__LINE__)(std::string(label))
#define TIME_BLOCK() itimize::Ticker COMBINE(_timer_,__LINE__)( LOCATION )

namespace itimize
{
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using Duration = double; // in ms.

	TimePoint Now()
	{
		return std::chrono::high_resolution_clock::now();
	}

	struct TimingEntry
	{
			TimingEntry(const std::string &l) : label_(l) {}
			TimingEntry(const std::string &l, const TimePoint &s) : label_(l), start_(s){}

			void compute_duration()
			{ 
				std::chrono::duration<double, std::milli> duration = end_ - start_;
				duration_ = duration.count();
			}

			std::string label_;
			TimePoint start_;
			TimePoint end_;
			Duration duration_;
	};

	class Ticker
	{
		public:
			Ticker(const Ticker&) = delete;

		 	Ticker(const std::string &label) : entry_(label)
			{ 
				entry_.start_ = Now();
			}

			~Ticker()
			{
				entry_.end_ = Now();
				entry_.compute_duration();
				std::cout << entry_.label_ << " " << entry_.duration_ << std::endl;
			}

		private:
			TimingEntry entry_;
	};

	class TimeCentral
	{
		public:
			static TimeCentral* getInstance()
			{
				if(!instance)
					instance = new TimeCentral();

				return instance;
			}

		private:
			TimeCentral(){}
			
			std::map<std::string, TimingEntry> records;

			static TimeCentral* instance;
	};

	TimeCentral *TimeCentral::instance = nullptr;

};


#endif