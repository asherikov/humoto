/**
    @file
    @author  Alexander Sherikov
    @copyright 2017   Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#define HUMOTO_GLOBAL_LOGGER_ENABLED

#include "humoto/config_yaml.h"
#include "humoto/config_msgpack.h"

#include "humoto/humoto.h"
#include "humoto/qpoases.h"
#include "humoto/qpmad.h"
#include "humoto/quadprogpp.h"
#include "humoto/rbdl.h"
#include "humoto/rigid_body.h"
#include "humoto/walking.h"

HUMOTO_INITIALIZE_GLOBAL_LOGGER(std::cout);

void noop();

int main()
{
    noop();

    return (0);
}
