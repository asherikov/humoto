/**
    @file
    @author  Alexander Sherikov

    @copyright 2017. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)


    @brief
*/

#pragma once


#ifdef HUMOTO_USE_CONFIG
    #define HUMOTO_CONFIG_READ_COMPOUND_(entry)     HUMOTO_CONFIG_READ_ENTRY_(entry)
    #define HUMOTO_CONFIG_READ_COMPOUND(entry)      HUMOTO_CONFIG_READ_ENTRY(entry)

    #define HUMOTO_CONFIG_READ_SCALAR_(entry)       HUMOTO_CONFIG_READ_ENTRY_(entry)
    #define HUMOTO_CONFIG_READ_SCALAR(entry)        HUMOTO_CONFIG_READ_ENTRY(entry)

    #define HUMOTO_CONFIG_READ_ENUM_(entry)         HUMOTO_CONFIG_READ_ENTRY_(entry)
    #define HUMOTO_CONFIG_READ_ENUM(entry)          HUMOTO_CONFIG_READ_ENTRY(entry)
#endif
