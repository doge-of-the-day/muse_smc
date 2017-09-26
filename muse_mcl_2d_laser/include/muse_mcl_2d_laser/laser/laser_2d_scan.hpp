#ifndef LASER_SCAN_2D_HPP
#define LASER_SCAN_2D_HPP

#include <muse_mcl_2d/math/point_2d.hpp>
#include <muse_smc/data/data.hpp>

#include <limits>

namespace muse_mcl_2d_laser {
class LaserScan2D : public muse_smc::Data
{
public:
    using point_t = muse_mcl_2d::math::Point2D;
    using time_frame_t = muse_smc::TimeFrame;

    struct Ray {
        const double  angle;
        const double  range;
        const point_t point;

        inline Ray(const double angle,
                   const double range) :
            angle(angle),
            range(range),
            point(point_t(cos(angle) * range,
                          sin(angle) * range))
        {
        }

        inline Ray(const point_t &pt) :
                   angle(atan2(pt.y(), pt.x())),
                   range(hypot(pt.y(), pt.x())),
                   point(pt)
        {
        }

        inline Ray() :
            angle(0.0),
            range(0.0),
            point(point_t())
        {
        }

        inline Ray(const Ray &other) :
            angle(other.angle),
            range(other.range),
            point(other.point)
        {
        }

        inline Ray(Ray &&other) :
            angle(other.angle),
            range(other.range),
            point(std::move(other.point))
        {
        }

        inline bool valid() const
        {
            return range != 0.0;
        }

    };

    using Ptr  = std::shared_ptr<LaserScan2D>;
    using Rays = std::vector<Ray>;

    LaserScan2D(const std::string &frame,
                const time_frame_t &time_frame) :
        Data(frame, time_frame),
        range_min_(0.0),
        range_max_(std::numeric_limits<double>::max()),
        angle_min_(-M_PI),
        angle_max_(+M_PI)
    {
    }

    inline void setRangeInterval(const double range_min,
                                 const double range_max)
    {
        range_min_ = range_min;
        range_max_ = range_max;
    }

    inline void setAngleInterval(const double angle_min,
                                  const double angle_max)
    {
        angle_min_ = angle_min;
        angle_max_ = angle_max;
    }

    inline double getRangeMin() const
    {
        return range_min_;
    }

    inline double getRangeMax() const
    {
        return range_max_;
    }

    inline double getAngleMin() const
    {
        return angle_min_;
    }

    inline double getAngleMax() const
    {
        return angle_max_;
    }

    inline void insert(const double angle,
                       const double range)
    {
        rays_.emplace_back(Ray(angle, range));
    }

    inline void insert(const point_t &pt)
    {
        rays_.emplace_back(Ray(pt));
    }

    inline void insertInvalid()
    {
        rays_.emplace_back(Ray());
    }

    inline const Rays& getRays() const
    {
        return rays_;
    }

private:
    Rays   rays_;         /// only valid rays shall be contained here

    double range_min_;
    double range_max_;
    double angle_min_;
    double angle_max_;
};
}

#endif // LASER_SCAN_2D_HPP
