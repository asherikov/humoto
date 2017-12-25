/**
    @file
    @author  Alexander Sherikov

    @copyright 2017 Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once


#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_enum.hpp>


#define HUMOTO_CONFIG_IS_BASE_OF_DISABLER_TYPE(Base, Derived) \
    const typename boost::enable_if_c< !(boost::is_base_of<Base, Derived>::value) >::type


#define HUMOTO_CONFIG_IS_ENUM_ENABLER_TYPE(Enum) \
    const typename boost::enable_if_c< (boost::is_enum<Enum>::value) >::type

#define HUMOTO_CONFIG_IS_ENUM_DISABLER_TYPE(Enum) \
    const typename boost::enable_if_c< !(boost::is_enum<Enum>::value) >::type
