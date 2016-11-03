#include <muse_amcl/plugin_factories/propagation_function_factory.h>

using namespace muse_amcl;

Propagation::Ptr PropagationFunctionFactory::create(const std::string& plugin_name,
                                                    const std::string& class_name)
{
    Propagation::Ptr propagation = PluginFactory::create(class_name);
    if(propagation) {
        propagation->setup(plugin_name, nh_private_);
    }
    return propagation;
}
