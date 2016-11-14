#pragma once

#include <string>
#include <muse_amcl/data_types/data.hpp>

namespace muse_amcl {

struct MockData : public Data
{
    MockData() :
        Data("this_frame_is_mocky")
    {
    }

    typedef std::shared_ptr<MockData const> ConstPtr;

    std::string value;

};
}
