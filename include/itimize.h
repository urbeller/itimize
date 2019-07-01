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

  TimePoint now()
  {
    return std::chrono::high_resolution_clock::now();
  }

  struct Entry
  {
      Entry(const std::string &l) : label(l) {}
      Entry(const std::string &l, const TimePoint &s) : label(l), start(s){}

			// Set the endtime and compute the duration.
      void set_endtime(const TimePoint &e)
      { 
				end = e;
        std::chrono::duration<double, std::milli> dur= end - start;
        duration = dur.count();
      }

      std::string label;
      TimePoint   start;
      TimePoint   end;
      Duration    duration;
  };

  class Ticker
  {
    public:
      Ticker(const Ticker&) = delete;
      Ticker(const std::string &label) : entry_(label, now()) { }

      ~Ticker()
      {
        entry_.set_endtime( now() );
        std::cout << entry_.label << " " << entry_.duration << std::endl;
      }

    private:
      Entry entry_;
  };
}
#endif