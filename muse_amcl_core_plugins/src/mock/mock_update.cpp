#include "mock_update.h"

#include <class_loader/class_loader_register_macro.h>

#include <iostream>

CLASS_LOADER_REGISTER_CLASS(muse_amcl::MockUpdate, muse_amcl::Update)

using namespace muse_amcl;

MockUpdate::MockUpdate()
{

}

double MockUpdate::apply(ParticleSet::WeightIterator set)
{
    std::cout << "Hello, I am a mock update - Greetings Traveller!" << std::endl;
    return 0.0;
}

void MockUpdate::loadParameters(ros::NodeHandle &nh)
{
    first_parameter  = nh.param(param("first"), 0.5);
    second_parameter = nh.param(param("second"),1);
}
