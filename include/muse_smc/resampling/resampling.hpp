#ifndef MUSE_SMC_RESAMPLING_HPP
#define MUSE_SMC_RESAMPLING_HPP

#include <iostream>
#include <memory>

namespace muse_smc {
template <typename SampleSet_T, typename UniformSampling_T,
          typename NormalSampling_T>
class Resampling {
 public:
  using Ptr = std::shared_ptr<Resampling>;

  inline Resampling() = default;
  virtual ~Resampling() = default;

  virtual void setup(
      const typename UniformSampling_T::Ptr &uniform_pose_sampler,
      const typename NormalSampling_T::Ptr &normal_pose_sampler,
      const double recovery_alpha_fast = 0.0,
      const double recovery_alpha_slow = 0.0,
      const double variance_threshold = 0.0) {
    uniform_pose_sampler_ = uniform_pose_sampler;
    normal_pose_sampler_ = normal_pose_sampler;
    recovery_alpha_fast_ = recovery_alpha_fast;
    recovery_alpha_slow_ = recovery_alpha_slow;
    variance_treshold_ = variance_threshold;
  }

  inline void apply(SampleSet_T &sample_set) {
    if (sample_set.getWeightSum() == 0.0) {
      std::cerr << "[MuseSMC]: All particle weights are zero. \n";
      return;
    }

    if (sample_set.getWeightVariance() < variance_treshold_) {
      std::cerr << "[MuseSMC]: Variance not high enough for resampling. \n";
      return;
    }

    auto do_apply = [&sample_set, this]() { doApply(sample_set); };
    auto do_apply_recovery = [&sample_set, this]() {
      doApplyRecovery(sample_set);
      resetRecovery();
    };

    recovery_random_pose_probability_ == 0.0 ? do_apply() : do_apply_recovery();
  }

  inline void resetRecovery() {
    recovery_fast_ = 0.0;
    recovery_slow_ = 0.0;
  }

  inline void updateRecovery(SampleSet_T &particle_set) {
    const double weight_average = particle_set.getAverageWeight();
    if (recovery_slow_ == 0.0) {
      recovery_slow_ = weight_average;
    } else {
      recovery_slow_ +=
          recovery_alpha_slow_ * (weight_average - recovery_slow_);
    }

    if (recovery_fast_ == 0.0) {
      recovery_fast_ = weight_average;
    } else {
      recovery_fast_ +=
          recovery_alpha_fast_ * (weight_average - recovery_fast_);
    }

    if (recovery_slow_ != 0.0) {
      recovery_random_pose_probability_ =
          std::max(0.0, 1.0 - recovery_fast_ / recovery_slow_);
    }
  }

 protected:
  double recovery_alpha_fast_{0.0};
  double recovery_alpha_slow_{0.0};
  double recovery_fast_{0.0};
  double recovery_slow_{0.0};
  double recovery_random_pose_probability_{0.0};
  double variance_treshold_{0.0};
  typename UniformSampling_T::Ptr uniform_pose_sampler_;
  typename NormalSampling_T::Ptr normal_pose_sampler_;

  virtual void doApply(SampleSet_T &sample_set) = 0;
  virtual void doApplyRecovery(SampleSet_T &sample_set) = 0;
};
}  // namespace muse_smc

#endif  // MUSE_SMC_RESAMPLING_HPP
