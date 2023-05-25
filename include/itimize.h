#ifndef __ITIMIZE_H__
#define __ITIMIZE_H__
  
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <unordered_map>
#include <map>
#include <vector>
#include <limits>

#define STR(X) #X
#define STR1(X) STR(X)

#define COMBINE1(X,Y) X##Y 
#define COMBINE(X,Y) COMBINE1(X,Y)

#define LOCATION std::string(__FUNCTION__) +  ":" + STR1(__LINE__)

// Use these macros for a single block timing.
#define TIME_NAMED_BLOCK(label) itimize::Ticker COMBINE(_timer_,__LINE__)(std::string(label))
#define TIME_BLOCK() itimize::Ticker COMBINE(_timer_,__LINE__)( LOCATION )

// Use these macros for cumulative timing.
#define START_TIMER(label) (TimeCenter::Instance()->StartTimer(label))
#define END_TIMER(label) (TimeCenter::Instance()->EndTimer(label))
#define PRINT_TIMER(...) (TimeCenter::Instance()->PrintTimer(__VA_ARGS__))
#define RESET_TIMER(label) (TimeCenter::Instance()->ResetTimer(label))

namespace itimize
{
  class TimeCenter;

  using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
  using Duration = double; // in ms.
  using TimeCenterPtr = std::unique_ptr<TimeCenter>;

  TimePoint now()
  {
    return std::chrono::high_resolution_clock::now();
  }

  struct Entry
  {
      Entry(const std::string &l) : label(l), stopped(false) {}
      Entry(const std::string &l, const TimePoint &s) : label(l), start(s), stopped(false){}

      // Set the endtime and compute the duration.
      void set_endtime(const TimePoint &e)
      { 
        end = e;
        std::chrono::duration<double, std::milli> dur= end - start;
        duration = dur.count();
        stopped = true;
      }

      std::string label;
      TimePoint   start;
      TimePoint   end;
      Duration    duration = -1;
      bool stopped;
  };

  class Ticker
  {
    public:
      Ticker(const Ticker&) = delete;
      Ticker(const std::string &label) : entry_(label, now()) { }

      ~Ticker()
      {
        entry_.set_endtime( now() );
        std::cout << "[" << entry_.label << "]" << ": " << entry_.duration << "ms" << std::endl;
      }

    private:
      Entry entry_;
  };

  class TimeCenter
  {
    public:

      static TimeCenterPtr& Instance()
      {
        static TimeCenterPtr center(new TimeCenter);
        return center;
      }

      bool HasKey(std::string label)
      {
        return entries.find(label) != entries.end();
      }

      void StartTimer(std::string label)
      {
        entries[label].push_back(Entry(label, now()));
      }

      void EndTimer(std::string label)
      {
        
        // Record end time now to avoid waisting time :-)
        TimePoint end = now();

        // Check if this label exists.
        if(!HasKey(label)) return;

        auto& e = entries[label].back();

        // Check if this timer hasnt been stopped already.
        if(e.stopped) return;

        e.set_endtime(end);
      }

      void ResetTimer(std::string label)
      {
        if(HasKey(label))
          entries.erase(label);
      }

      void PrintTimer(std::string label, std::string unit = "ms")
      {
        // Check if this label exists.
        if(!HasKey(label))
        {
          std::cout << "["<<label << "]: Invalid" << std::endl;
          return;
        }

        Duration min = 0, max = 0, total;
        std::vector<Entry> timers = entries[label];
        if(timers.size() == 0)
        {
          std::cout << "["<<label << "]: Invalid." << std::endl;
        }


        double factor = 0;
        std::string unitStr = "";

        if(unit == "us")
        {
          // Micro-secs
          factor = 1.0;
          unitStr = unit;
        }
        else if(unit == "s")
        {
          // Seconds
          factor = 1E-6;
          unitStr = unit;
        }
        else
        {
          // Default to milli-secs
          factor = 1E-3;
          unitStr = "ms";
        }

        min = std::numeric_limits<float>::max();
        max = 0;
        total = 0;
        int n = 0;
        for(auto e: timers)
        {
          if(!e.stopped) continue;
    
          std::chrono::duration<double, std::micro> diff = e.end - e.start;
          double duration = diff.count() * factor;

          min = std::min(min, duration);
          max = std::max(max, duration);
          total += duration;

          ++n;
        }

        if(n == 0)
        {
          std::cout << "["<<label << "]: Invalid." << std::endl;
          return;
        }

        double avg = total / double(n);


        // Save cout format.
        std::ios cout_state(nullptr);
        cout_state.copyfmt(std::cout);

        std::cout << std::fixed << std::showpoint;
        std::cout << std::setprecision(2);

        std::cout << "["<<label << "("<< n <<")]: " 
                  <<  total << unitStr
                  << " avg-min-max=" << avg
                  << "/" << min <<"/"
                  <<max<<unitStr<<std::endl;

        // Restore cout format.
        std::cout.copyfmt(cout_state);
      }

    private:
      TimeCenter(){};

      std::unordered_map<std::string, std::vector<itimize::Entry>> entries;
  };
}
#endif
