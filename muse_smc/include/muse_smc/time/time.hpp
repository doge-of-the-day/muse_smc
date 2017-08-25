#ifndef TIME_HPP
#define TIME_HPP

#include <muse_smc/time/duration.hpp>
#include <ctime>

namespace muse_smc {
class Time {
public:
    using clock_t = std::chrono::high_resolution_clock;
    using time_t  = clock_t::time_point;
    using duration_t = clock_t::duration;

    Time() :
        time_(duration_t(0))
    {
    }

    Time(const double seconds) :
        time_(std::chrono::nanoseconds(static_cast<int64_t>(seconds * 1e9)))
    {
    }

    Time(const int64_t &nanoseconds) :
        time_(duration_t(nanoseconds))
    {
    }

    Time(const uint64_t &nanoseconds) :
        time_(duration_t(static_cast<int64_t>(nanoseconds)))
    {
    }

    Time(const time_t &time) :
        time_(time)
    {
    }

    inline time_t const & time() const
    {
        return time_;
    }

    inline double seconds() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(time_.time_since_epoch()).count() * 1e-9;
    }

    inline int64_t nanoseconds() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(time_.time_since_epoch()).count();
    }

    inline bool isZero() const
    {
        return time_ == time_t(duration_t(0));
    }

    inline Time static now()
    {
        return Time(clock_t::now());
    }

    inline bool operator == (const Time &other) const
    {
        return time_ == other.time_;
    }

    inline bool operator != (const Time &other) const
    {
        return time_ != other.time_;
    }

    inline bool operator <= (const Time &other) const
    {
        return time_ <= other.time_;
    }

    inline bool operator >= (const Time &other) const
    {
         return time_ >= other.time_;
    }

    inline bool operator > (const Time &other) const
    {
         return time_ > other.time_;
    }

    inline bool operator < (const Time &other) const
    {
         return time_ < other.time_;
    }

    inline Duration operator - (const Time &other) const
    {
        return Duration(time_ - other.time_);
    }

    inline Time operator - (const Duration &d) const
    {
        return Time(time_ - d.duration());
    }

    inline Time operator + (const Duration &d) const
    {
        return Time(time_ + d.duration());
    }

    inline std::string string() const
    {
        const int64_t ns = nanoseconds();
        const int64_t s = ns / static_cast<int64_t>(1e9);
        const int64_t ms = (ns % static_cast<int64_t>(1e9));
        return std::to_string(s) + "." + std::to_string(ms);
    }

private:
    time_t time_;
};
}

#endif // TIME_HPP
