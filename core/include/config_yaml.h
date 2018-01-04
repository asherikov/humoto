/**
    @file
    @author  Alexander Sherikov

    @brief
*/

#pragma once

#ifndef ARILES_VISIBILITY_ATTRIBUTE
#   include "build_config.h"
#   include "export_import.h"

#   define ARILES_VISIBILITY_ATTRIBUTE      HUMOTO_LOCAL
#   define ARILES_DEFAULT_CONFIG_PREFIX     HUMOTO_DEFAULT_CONFIG_PREFIX
#endif

#include "ariles/config_yaml.h"

namespace humoto
{
    namespace config = ::ariles;
}
