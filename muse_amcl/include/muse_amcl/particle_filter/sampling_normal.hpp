#ifndef POSE_GENERATION_NORMAL_HPP
#define POSE_GENERATION_NORMAL_HPP

#include <memory>
#include <vector>
#include <tf/transform_listener.h>
#include <ros/node_handle.h>

#include <muse_amcl/particle_filter/particle_set.hpp>
#include <muse_amcl/data_sources/map_provider.hpp>
#include <muse_amcl/data_sources/tf_provider.hpp>
#include <muse_amcl/math/covariance.hpp>

namespace muse_amcl {
class NormalSampling {
public:
    typedef std::shared_ptr<NormalSampling> Ptr;

    using MapProviders = std::map<std::string, MapProvider::Ptr>;

    NormalSampling()
    {
    }

    virtual ~NormalSampling()
    {
    }

    inline const static std::string Type()
    {
        return "muse_amcl::NormalPoseGeneration";
    }

    inline std::string getName() const
    {
        return name_;
    }

    void setup(const std::string                             &name,
               ros::NodeHandle                               &nh_private,
               const std::map<std::string, MapProvider::Ptr> &map_providers,
               const TFProvider::Ptr                         &tf_provider)
    {
        double sampling_timeout;
        double tf_timeout;
        name_              = name;
        sample_size_       = nh_private.param(parameter("sample_size"), 500);
        sampling_timeout   = nh_private.param(parameter("timeout"), 10.0);
        tf_timeout         = nh_private.param(parameter("tf_timeout"), 0.1);

        sampling_timeout_ = ros::Duration(sampling_timeout);
        tf_timeout_       = ros::Duration(tf_timeout);
        tf_provider_       = tf_provider;

        doSetup(nh_private);
        doSetupMapProviders(nh_private, map_providers);
    }

    /**
     * @brief Generate a multivariate Gaussian distributed particle set with
     *        a mean given by a desired initialization pose and the covariance
     *        matrix.
     * @param pose          - 6 dimensional pose vector (x,y,z,roll,pitch,yaw)
     * @param covariance    - 6 dimensnioal covariance matrix
     */
    virtual void apply(const math::Pose       &pose,
                       const math::Covariance &covariance,
                       ParticleSet            &particle_set) = 0;

protected:
    std::string                   name_;
    std::size_t                   sample_size_;
    std::vector<MapProvider::Ptr> map_providers_;
    ros::Duration                 sampling_timeout_;
    ros::Duration                 tf_timeout_;
    TFProvider::Ptr               tf_provider_;

    virtual void doSetup(ros::NodeHandle &nh_private) = 0;
    virtual void doSetupMapProviders(ros::NodeHandle    &nh_private,
                                     const MapProviders &map_providers)
    {
        std::vector<std::string> map_provider_ids;
        nh_private.getParam(parameter("maps"), map_provider_ids);

        for(auto m : map_provider_ids) {
            map_providers_.emplace_back(map_providers.at(m));
        }
    }

    inline std::string parameter (const std::string &name)
    {
        return name_ + "/" + name;
    }

};
}
#endif // POSE_GENERATION_NORMAL_HPP