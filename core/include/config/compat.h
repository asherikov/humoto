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


    #define HUMOTO_CONFIG_WRITE_COMPOUND_(entry)    HUMOTO_CONFIG_WRITE_ENTRY_(entry)
    #define HUMOTO_CONFIG_WRITE_COMPOUND(entry)     HUMOTO_CONFIG_WRITE_ENTRY(entry)

    #define HUMOTO_CONFIG_WRITE_SCALAR_(entry)      HUMOTO_CONFIG_WRITE_ENTRY_(entry)
    #define HUMOTO_CONFIG_WRITE_SCALAR(entry)       HUMOTO_CONFIG_WRITE_ENTRY(entry)

    #define HUMOTO_CONFIG_WRITE_ENUM_(entry)        HUMOTO_CONFIG_WRITE_ENTRY_(entry)
    #define HUMOTO_CONFIG_WRITE_ENUM(entry)         HUMOTO_CONFIG_WRITE_ENTRY(entry)


    #define HUMOTO_CONFIG_COMPOUND_(entry)     HUMOTO_CONFIG_ENTRY_(entry)
    #define HUMOTO_CONFIG_COMPOUND(entry)      HUMOTO_CONFIG_ENTRY(entry)

    #define HUMOTO_CONFIG_SCALAR_(entry)       HUMOTO_CONFIG_ENTRY_(entry)
    #define HUMOTO_CONFIG_SCALAR(entry)        HUMOTO_CONFIG_ENTRY(entry)

    #define HUMOTO_CONFIG_ENUM_(entry)         HUMOTO_CONFIG_ENTRY_(entry)
    #define HUMOTO_CONFIG_ENUM(entry)          HUMOTO_CONFIG_ENTRY(entry)


    #define HUMOTO_CONFIG_WRITE_MEMBER_CLASS(member, name)  HUMOTO_CONFIG_WRITE_NAMED_ENTRY(member, name)
    #define HUMOTO_CONFIG_READ_MEMBER_CLASS(member, name)   HUMOTO_CONFIG_READ_NAMED_ENTRY(member, name)
    #define HUMOTO_CONFIG_MEMBER_CLASS(entry, name)         HUMOTO_CONFIG_NAMED_ENTRY(entry, name)
#endif
