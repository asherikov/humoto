humoto_copy_file_if_exists( TGT_humoto_copy_headers                             "obstacle_avoidance.h"
                            "${CMAKE_CURRENT_LIST_DIR}/obstacle_avoidance.h"    "${HUMOTO_INCLUDE_DIR}")
humoto_copy_dir_if_exists(  TGT_humoto_copy_headers                             "obstacle_avoidance_dir"
                            "${CMAKE_CURRENT_LIST_DIR}/obstacles"               "${HUMOTO_INCLUDE_DIR}/obstacles")


set (TGT_OBSTACLE_AVOIDANCE           "${BRIDGE_TARGET_PREFIX}_${HUMOTO_BRIDGE}")

set(HUMOTO_BRIDGE_${HUMOTO_BRIDGE}_NAMESPACE "obstacle_avoidance" PARENT_SCOPE)


if(${HUMOTO_BRIDGE_${HUMOTO_BRIDGE}} STREQUAL "BUILD")
    set(HUMOTO_BRIDGE_${HUMOTO_BRIDGE}_INCLUDES "" PARENT_SCOPE)
    set(HUMOTO_BRIDGE_${HUMOTO_BRIDGE}_LIBS "" PARENT_SCOPE)
else()
    message(FATAL_ERROR "Only BUILD mode is supported for '${HUMOTO_BRIDGE}' bridge.")
endif()
