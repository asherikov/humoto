/**
    @file
    @author  Alexander Sherikov

    @brief
*/


#ifdef HUMOTO_CONFIG_SECTION_ID
#   define ARILES_SECTION_ID HUMOTO_CONFIG_SECTION_ID
#endif

#ifdef HUMOTO_CONFIG_CONSTRUCTOR
#   define ARILES_CONSTRUCTOR HUMOTO_CONFIG_CONSTRUCTOR
#endif

#ifndef HUMOTO_CONFIG_ENTRIES
//#   error "HUMOTO_CONFIG_ENTRIES must be defined"
#else
#   define ARILES_ENTRIES HUMOTO_CONFIG_ENTRIES
#endif


//#include "humoto/ariles/define_accessors.h"
#include ARILES_INITIALIZE


#undef HUMOTO_CONFIG_ENTRIES

#undef HUMOTO_CONFIG_CONSTRUCTOR

#undef HUMOTO_CONFIG_SECTION_ID
