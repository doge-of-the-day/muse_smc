#pragma once

#include <muse_amcl/plugins/update.hpp>

namespace muse_amcl {
class MockUpdate : public Update
{
public:
    typedef std::shared_ptr<MockUpdate> Ptr;

    MockUpdate();

    virtual double apply(ParticleSet::WeightIterator set) override;

    double  first_parameter;
    int     second_parameter;

protected:
    virtual void loadParameters(ros::NodeHandle &nh) override;


};
}
