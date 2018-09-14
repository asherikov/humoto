humoto_copy_file_if_exists( TGT_humoto_copy_headers                     "qpmad.h"
                            "${${HUMOTO_BRIDGE}_INTERFACE_DIR}/qpmad.h" "${HUMOTO_INCLUDE_DIR}")
humoto_copy_dir_if_exists(  TGT_humoto_copy_headers                     "qpmad_dir"
                            "${${HUMOTO_BRIDGE}_INTERFACE_DIR}/qpmad"   "${HUMOTO_INCLUDE_DIR}/qpmad")


set (TGT_QPMAD             "${BRIDGE_TARGET_PREFIX}_${HUMOTO_BRIDGE}")

set(HUMOTO_BRIDGE_${HUMOTO_BRIDGE}_NAMESPACE "qpmad" PARENT_SCOPE)

if(${HUMOTO_BRIDGE_${HUMOTO_BRIDGE}} STREQUAL "BUILD")
    set (QPMAD_SOURCE_DIR      "${CMAKE_CURRENT_LIST_DIR}/qpmad")
    set (QPMAD_CXX_FLAGS       "-fPIC")


    set(QPMAD_ARGS
        ${BRIDGES_ARGS}
        "-DQPMAD_BUILD_TESTS=OFF"
        "-DQPMAD_ENABLE_TRACING=OFF"
        "-DQPMAD_USE_HOUSEHOLDER=OFF"
        "-DCMAKE_CXX_FLAGS=${QPMAD_CXX_FLAGS}")

    if(HUMOTO_COPY_CXX_FLAGS_TO_C_FLAGS)
        set(QPMAD_ARGS
            ${QPMAD_ARGS}
            "-DCMAKE_C_FLAGS=${QPMAD_CXX_FLAGS}")
    endif(HUMOTO_COPY_CXX_FLAGS_TO_C_FLAGS)

    humoto_add_external_git_project(TARGET_NAME "${TGT_QPMAD}"
                                    PROJECT_DIR "${QPMAD_SOURCE_DIR}"
                                    CMAKE_ARGS   ${QPMAD_ARGS})


    set(HUMOTO_BRIDGE_${HUMOTO_BRIDGE}_INCLUDES "" PARENT_SCOPE)
    set(HUMOTO_BRIDGE_${HUMOTO_BRIDGE}_LIBS "" PARENT_SCOPE)
else()
    find_package(qpmad REQUIRED)

    set(HUMOTO_BRIDGE_${HUMOTO_BRIDGE}_INCLUDES "${qpmad_INCLUDE_DIRS}" PARENT_SCOPE)
    set(HUMOTO_BRIDGE_${HUMOTO_BRIDGE}_LIBS "" PARENT_SCOPE)
endif()
