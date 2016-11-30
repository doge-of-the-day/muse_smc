#include "uniform_mm_aa_3d.h"

#include <muse_amcl/pose_generators/uniform.hpp>

using namespace muse_amcl;

using M   = muse_amcl::pose_generation::Metric;
using R   = muse_amcl::pose_generation::Radian;
using RNG = muse_amcl::pose_generation::Uniform<M, M, M, R, R, R>;

void UniformMainMapAA3D::apply(ParticleSet &particle_set)
{

}

void UniformMainMapAA3D::doSetup(const std::map<std::string, MapProvider::Ptr> &map_providers,
                                 ros::NodeHandle &nh_private)
{

}
