#include "map_provider_distance_gridmap.h"

#include <class_loader/class_loader_register_macro.h>
CLASS_LOADER_REGISTER_CLASS(muse_amcl::MapProviderDistanceGridMap, muse_amcl::MapProvider)

using namespace muse_amcl;

Map::ConstPtr MapProviderDistanceGridMap::getMap() const
{
    std::unique_lock<std::mutex> l(map_mutex_);
    return map_;
}

void MapProviderDistanceGridMap::doSetup(ros::NodeHandle &nh_private)
{
    topic_ = nh_private.param<std::string>(privateParameter("topic"), "/map");
    binarization_threshold_ = nh_private.param<double>(privateParameter("threshold"), 0.5);
    kernel_size_ = std::max(nh_private.param<int>(privateParameter("kernel_size"), 5), 5);
    kernel_size_ += 1 - (kernel_size_ % 2);

    source_= nh_private.subscribe(topic_, 1, &MapProviderDistanceGridMap::callback, this);
}

void MapProviderDistanceGridMap::callback(const nav_msgs::OccupancyGridConstPtr &msg)
{
    /// conversion can take time
    /// we allow concurrent loading, this way, the front end thread is not blocking.
    if(!loading_) {
        if(!map_ || msg->info.map_load_time > map_->getStamp()) {
            loading_ = true;
            auto load = [this, msg]() {
                maps::DistanceGridMap::Ptr map(new maps::DistanceGridMap(*msg, binarization_threshold_, kernel_size_));
                std::unique_lock<std::mutex>l(map_mutex_);
                map_ = map;
                loading_ = false;
            };
            worker_ = std::thread(load);
            worker_.detach();
        }
    }
}
