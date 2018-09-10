/**
    @file
    @author  Alexander Sherikov

    @copyright 2017. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)


    @brief
*/

#pragma once


#ifdef HUMOTO_DOXYGEN_PROCESSING
#   define ARILES_DOXYGEN_PROCESSING
#endif

#ifndef ARILES_VISIBILITY_ATTRIBUTE
#   define ARILES_VISIBILITY_ATTRIBUTE  HUMOTO_LOCAL
#endif

#include "ariles/ariles_all.h"

namespace ariles
{
    typedef ::ariles::yaml_cpp03 yaml;
}

namespace humoto
{
    /**
     * @brief Namespace of classes related to configuration handling
     */
    namespace config = ::ariles;
}


#ifdef ARILES_ENABLED
#   define HUMOTO_USE_CONFIG
#endif


#define HUMOTO_CONFIG_DEFINE_ACCESSORS  "humoto/config_accessors.h"


#ifdef HUMOTO_USE_CONFIG

// deprecated
    #define HUMOTO_CONFIG_READ_COMPOUND_(entry)     ARILES_READ_ENTRY_(entry)
    #define HUMOTO_CONFIG_READ_COMPOUND(entry)      ARILES_READ_ENTRY(entry)

    #define HUMOTO_CONFIG_READ_SCALAR_(entry)       ARILES_READ_ENTRY_(entry)
    #define HUMOTO_CONFIG_READ_SCALAR(entry)        ARILES_READ_ENTRY(entry)

    #define HUMOTO_CONFIG_READ_ENUM_(entry)         ARILES_READ_ENTRY_(entry)
    #define HUMOTO_CONFIG_READ_ENUM(entry)          ARILES_READ_ENTRY(entry)


    #define HUMOTO_CONFIG_WRITE_COMPOUND_(entry)    ARILES_WRITE_ENTRY_(entry)
    #define HUMOTO_CONFIG_WRITE_COMPOUND(entry)     ARILES_WRITE_ENTRY(entry)

    #define HUMOTO_CONFIG_WRITE_SCALAR_(entry)      ARILES_WRITE_ENTRY_(entry)
    #define HUMOTO_CONFIG_WRITE_SCALAR(entry)       ARILES_WRITE_ENTRY(entry)

    #define HUMOTO_CONFIG_WRITE_ENUM_(entry)        ARILES_WRITE_ENTRY_(entry)
    #define HUMOTO_CONFIG_WRITE_ENUM(entry)         ARILES_WRITE_ENTRY(entry)


    #define HUMOTO_CONFIG_COMPOUND_(entry)     ARILES_ENTRY_(entry)
    #define HUMOTO_CONFIG_COMPOUND(entry)      ARILES_ENTRY(entry)

    #define HUMOTO_CONFIG_SCALAR_(entry)       ARILES_ENTRY_(entry)
    #define HUMOTO_CONFIG_SCALAR(entry)        ARILES_ENTRY(entry)

    #define HUMOTO_CONFIG_ENUM_(entry)         ARILES_ENTRY_(entry)
    #define HUMOTO_CONFIG_ENUM(entry)          ARILES_ENTRY(entry)


    #define HUMOTO_CONFIG_WRITE_MEMBER_CLASS(member, name)  ARILES_WRITE_NAMED_ENTRY(member, name)
    #define HUMOTO_CONFIG_READ_MEMBER_CLASS(member, name)   ARILES_READ_NAMED_ENTRY(member, name)
    #define HUMOTO_CONFIG_MEMBER_CLASS(entry, name)         ARILES_NAMED_ENTRY(entry, name)


// aliases
    #define HUMOTO_CONFIG_ENTRY_(entry)         ARILES_ENTRY_(entry)
    #define HUMOTO_CONFIG_ENTRY(entry)          ARILES_ENTRY(entry)

    #define HUMOTO_CONFIG_TYPED_ENTRY_(entry, type)     ARILES_TYPED_ENTRY_(entry, type)
    #define HUMOTO_CONFIG_TYPED_ENTRY(entry, type)      ARILES_TYPED_ENTRY(entry, type)


    #define HUMOTO_CONFIG_WRITE_NAMED_ENTRY(entry, name)    ARILES_WRITE_NAMED_ENTRY(entry, name)

    #define HUMOTO_CONFIG_WRITE_ENTRY_(entry)   ARILES_WRITE_ENTRY_(entry)
    #define HUMOTO_CONFIG_WRITE_ENTRY(entry)    ARILES_WRITE_ENTRY(entry)

    #define HUMOTO_CONFIG_WRITE_PARENT_CLASS(parent_class)  ARILES_WRITE_PARENT_CLASS(parent_class)


    #define HUMOTO_CONFIG_READ_NAMED_ENTRY(entry, name)     ARILES_READ_NAMED_ENTRY(entry, name)

    #define HUMOTO_CONFIG_READ_ENTRY_(entry)    ARILES_READ_ENTRY_(entry)
    #define HUMOTO_CONFIG_READ_ENTRY(entry)     ARILES_READ_ENTRY(entry)

    #define HUMOTO_CONFIG_READ_PARENT_CLASS(parent_class)  ARILES_READ_PARENT_CLASS(parent_class)

    #define HUMOTO_CONFIG_PARENT_CLASS(entry)               ARILES_PARENT(entry)
#endif
