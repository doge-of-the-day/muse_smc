#include <muse_mcl_2d_ndt/providers/occupancy_gridmap_provider.h>

#include <cslibs_ndt_2d/serialization/dynamic_maps/occupancy_gridmap.hpp>
#include <fstream>
#include <yaml-cpp/yaml.h>

#include <class_loader/class_loader_register_macro.h>
CLASS_LOADER_REGISTER_CLASS(muse_mcl_2d_ndt::OccupancyGridmapProvider, muse_mcl_2d::MapProvider2D)

namespace muse_mcl_2d_ndt {
OccupancyGridmapProvider::OccupancyGridmapProvider() :
    loading_(false)
{
}

OccupancyGridmapProvider::state_space_t::ConstPtr OccupancyGridmapProvider::getStateSpace() const
{
    std::unique_lock<std::mutex> l(map_mutex_);
    if (!map_ && blocking_)
        map_loaded_.wait(l);

    return map_;
}

void OccupancyGridmapProvider::setup(ros::NodeHandle &nh)
{
    auto param_name = [this](const std::string &name){return name_ + "/" + name;};

    path_     = nh.param<std::string>(param_name("path"), "");
    frame_id_ = nh.param<std::string>(param_name("frame_id"), "/world");
    blocking_ = nh.param<bool>(param_name("blocking"), false);

    loadMap();
}

void OccupancyGridmapProvider::loadMap()
{
    if (!loading_ && !map_) {
        loading_ = true;

        auto load = [this]() {
            std::ifstream map_in_yaml(path_);
            if (!map_in_yaml.is_open())
                std::cout << "[OccupancyGridmapProvider]: Could not open file '" << path_ << "'" << std::endl;
            else
                std::cout << "[OccupancyGridmapProvider]: Loading file '" << path_ << "'..." << std::endl;

            YAML::Node n = YAML::LoadFile(path_);
            cslibs_ndt_2d::dynamic_maps::OccupancyGridmap::Ptr map =
                n.as<cslibs_ndt_2d::dynamic_maps::OccupancyGridmap::Ptr>();
            std::unique_lock<std::mutex> l(map_mutex_);
            map_.reset(new OccupancyGridmap(map, frame_id_));
            loading_ = false;
        };
        auto load_blocking = [this]() {
            std::ifstream map_in_yaml(path_);
            if (!map_in_yaml.is_open())
                std::cout << "[OccupancyGridmapProvider]: Could not open file '" << path_ << "'" << std::endl;
            else
                std::cout << "[OccupancyGridmapProvider]: Loading file '" << path_ << "'..." << std::endl;

            YAML::Node n = YAML::LoadFile(path_);
            cslibs_ndt_2d::dynamic_maps::OccupancyGridmap::Ptr map =
                n.as<cslibs_ndt_2d::dynamic_maps::OccupancyGridmap::Ptr>();
            std::unique_lock<std::mutex> l(map_mutex_);
            map_.reset(new OccupancyGridmap(map, frame_id_));
            loading_ = false;
            map_loaded_.notify_one();
        };

        if (blocking_)
            worker_ = std::thread(load_blocking);
        else
            worker_ = std::thread(load);
    }
}
}
