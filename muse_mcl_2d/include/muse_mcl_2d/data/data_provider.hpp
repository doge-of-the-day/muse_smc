#ifndef DATA_PROVIDER_2D_HPP
#define DATA_PROVIDER_2D_HPP

#include <muse_smc/data/data.hpp>

#include <muse_smc/data/data.hpp>
#include <muse_smc/data/data_provider.hpp>

#include <muse_mcl_2d/samples/sample_2d.hpp>
#include <muse_mcl_2d/tf/tf_provider.hpp>

namespace muse_mcl_2d {
class DataProvider : public muse_smc::DataProvider<Pose2D>
{
public:
    inline virtual void setup(const TFProvider::Ptr &tf_provider,
                              ros::NodeHandle       &nh_private) = 0;
};
}

#endif // DATA_PROVIDER_2D_HPP
