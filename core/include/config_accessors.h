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

#ifdef HUMOTO_CONFIG_ENTRIES
#   define ARILES_ENTRIES HUMOTO_CONFIG_ENTRIES
#endif


#include "humoto/ariles/define_accessors.h"


#undef HUMOTO_CONFIG_ENTRIES

#undef HUMOTO_CONFIG_CONSTRUCTOR

#undef HUMOTO_CONFIG_SECTION_ID
