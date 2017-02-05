#include "differential_drive.h"

#include <class_loader/class_loader_register_macro.h>
CLASS_LOADER_REGISTER_CLASS(muse_amcl::DifferentialDrive, muse_amcl::PredictionModel)

using namespace muse_amcl;

DifferentialDrive::DifferentialDrive()
{
}

PredictionModel::Result DifferentialDrive::predict(const Data::ConstPtr &data,
                                                   const ros::Time &until,
                                                   ParticleSet::Poses set)
{

}

void DifferentialDrive::doSetup(ros::NodeHandle &nh_private)
{
    alpha_1_ = nh_private.param<double>(privateParameter("alpha1"), alpha_1_);
    alpha_2_ = nh_private.param<double>(privateParameter("alpha1"), alpha_2_);
    alpha_3_ = nh_private.param<double>(privateParameter("alpha1"), alpha_3_);
    alpha_4_ = nh_private.param<double>(privateParameter("alpha1"), alpha_4_);
    alpha_5_ = nh_private.param<double>(privateParameter("alpha1"), alpha_5_);
}
