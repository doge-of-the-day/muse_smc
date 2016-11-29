#ifndef POSE_GENERATION_2D_H
#define POSE_GENERATION_2D_H

#include "../particle_filter/pose_generation.hpp"

namespace muse_amcl {
namespace pose_generation {
class PoseGeneration2D : public PoseGeneration
{
public:
    void normal(const math::Pose       &pose,
                const math::Covariance &covariance,
                ParticleSet            &particle_set) override;

    void uniform(ParticleSet &particle_set);

};
}
}

#endif /* POSE_GENERATION_2D_H */
