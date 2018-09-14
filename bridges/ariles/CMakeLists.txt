set (TGT_ARILES             "${BRIDGE_TARGET_PREFIX}_${HUMOTO_BRIDGE}")

set(HUMOTO_BRIDGE_${HUMOTO_BRIDGE}_NAMESPACE "ariles" PARENT_SCOPE)

if(${HUMOTO_BRIDGE_${HUMOTO_BRIDGE}} STREQUAL "BUILD")
    set (ARILES_SOURCE_DIR      "${CMAKE_CURRENT_LIST_DIR}/ariles")
    set (ARILES_CXX_FLAGS       "-fPIC")


    set(ARILES_ARGS
        ${BRIDGES_ARGS}
        "-DARILES_BRIDGES_DEFAULT_MODE=OFF"
        "-DARILES_INSTALL_BRIDGES=ON"
        "-DARILES_BRIDGE_octave=BUILD"
        "-DARILES_BRIDGE_yaml_cpp03=BUILD"
        "-DARILES_CPP_STANDARD=c++03"
        "-DARILES_BUILD_REGRESSION_TESTS=OFF"
        "-DARILES_DEFAULT_CONFIGURABLE_FLAGS=ariles::ConfigurableFlags::FORCE_EXPLICIT_MATRIX_SIZE"
        "-DCMAKE_CXX_FLAGS=${ARILES_CXX_FLAGS}")

    if(HUMOTO_COPY_CXX_FLAGS_TO_C_FLAGS)
        set(ARILES_ARGS
            ${ARILES_ARGS}
            "-DCMAKE_C_FLAGS=${ARILES_CXX_FLAGS}")
    endif(HUMOTO_COPY_CXX_FLAGS_TO_C_FLAGS)

    humoto_add_external_git_project(TARGET_NAME "${TGT_ARILES}"
                                    PROJECT_DIR "${ARILES_SOURCE_DIR}"
                                    CMAKE_ARGS   ${ARILES_ARGS})


    set(HUMOTO_BRIDGE_${HUMOTO_BRIDGE}_INCLUDES "" PARENT_SCOPE)
    set(HUMOTO_BRIDGE_${HUMOTO_BRIDGE}_LIBS
        "${HUMOTO_BRIDGES_LIBDIR}/${CMAKE_SHARED_MODULE_PREFIX}yaml-cpp${CMAKE_STATIC_LIBRARY_SUFFIX}" PARENT_SCOPE)
else()
    find_package(ariles REQUIRED)
    find_package(ariles-yaml_cpp REQUIRED)
    find_package(ariles-octave REQUIRED)

    set(HUMOTO_BRIDGE_${HUMOTO_BRIDGE}_INCLUDES "${ariles_INCLUDE_DIRS};${ariles-yaml_cpp_INCLUDE_DIRS};${ariles-octave_INCLUDE_DIRS}" PARENT_SCOPE)
    set(HUMOTO_BRIDGE_${HUMOTO_BRIDGE}_LIBS "${ariles_LIBRARIES};${ariles-yaml_cpp_LIBRARIES};${ariles-octave_LIBRARIES}" PARENT_SCOPE)
endif()
