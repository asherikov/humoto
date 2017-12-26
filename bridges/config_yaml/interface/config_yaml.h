/**
    @file
    @author Alexander Sherikov
    @author Jan Michalczyk
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once

#ifdef HUMOTO_CONFIG_INCLUDED
    #error "This header must be included before humoto.h"
#endif


#include "humoto_helpers.h"
#include "config/reader_base.h"

#include "yaml-cpp/yaml.h"

#include "config_yaml/reader.h"
#include "config_yaml/writer.h"


#define HUMOTO_CONFIG_YAML_NAMESPACE yaml


// If something is stupid but it works, it is not stupid (c)
#ifndef HUMOTO_CONFIG_NAMESPACE_0
#   define HUMOTO_CONFIG_NAMESPACE_0 HUMOTO_CONFIG_YAML_NAMESPACE
#else
#   ifndef HUMOTO_CONFIG_NAMESPACE_1
#       define HUMOTO_CONFIG_NAMESPACE_1 HUMOTO_CONFIG_YAML_NAMESPACE
#   else
#       ifndef HUMOTO_CONFIG_NAMESPACE_2
#           define HUMOTO_CONFIG_NAMESPACE_2 HUMOTO_CONFIG_YAML_NAMESPACE
#       else
#           ifndef HUMOTO_CONFIG_NAMESPACE_3
#               define HUMOTO_CONFIG_NAMESPACE_3 HUMOTO_CONFIG_YAML_NAMESPACE
#           else
#               ifndef HUMOTO_CONFIG_NAMESPACE_4
#                   define HUMOTO_CONFIG_NAMESPACE_4 HUMOTO_CONFIG_YAML_NAMESPACE
#               else
#                   ifndef HUMOTO_CONFIG_NAMESPACE_5
#                       define HUMOTO_CONFIG_NAMESPACE_5 HUMOTO_CONFIG_YAML_NAMESPACE
#                   else
#                       ifndef HUMOTO_CONFIG_NAMESPACE_6
#                           define HUMOTO_CONFIG_NAMESPACE_6 HUMOTO_CONFIG_YAML_NAMESPACE
#                       else
#                           ifndef HUMOTO_CONFIG_NAMESPACE_7
#                               define HUMOTO_CONFIG_NAMESPACE_7 HUMOTO_CONFIG_YAML_NAMESPACE
#                           else
#                               error "Too many config namespaces."
#                           endif
#                       endif
#                   endif
#               endif
#           endif
#       endif
#   endif
#endif


namespace humoto
{
    namespace config
    {
        /**
         * @brief YAML bridge namespace.
         */
        namespace yaml
        {
        }
    }
}
