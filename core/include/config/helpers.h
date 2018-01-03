/**
    @file
    @author  Alexander Sherikov

    @copyright 2017 Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once


#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_enum.hpp>


#define HUMOTO_CONFIG_IS_ENUM_ENABLER_TYPE(Enum) \
    const typename boost::enable_if_c< (boost::is_enum<Enum>::value) >::type


#define HUMOTO_CONFIG_BASIC_TYPES_LIST \
    HUMOTO_CONFIG_BASIC_TYPE(std::string) \
    HUMOTO_CONFIG_BASIC_TYPE(float) \
    HUMOTO_CONFIG_BASIC_TYPE(double) \
    HUMOTO_CONFIG_BASIC_TYPE(int) \
    HUMOTO_CONFIG_BASIC_TYPE(unsigned int) \
    HUMOTO_CONFIG_BASIC_TYPE(short) \
    HUMOTO_CONFIG_BASIC_TYPE(unsigned short) \
    HUMOTO_CONFIG_BASIC_TYPE(long) \
    HUMOTO_CONFIG_BASIC_TYPE(unsigned long) \
    HUMOTO_CONFIG_BASIC_TYPE(char) \
    HUMOTO_CONFIG_BASIC_TYPE(unsigned char) \
    HUMOTO_CONFIG_BASIC_TYPE(bool)
