function(humoto_add_module)
    set(oneValueArgs MODULE MODULE_DIR)
    cmake_parse_arguments("HUMOTO" "" "${oneValueArgs}" "" ${ARGN} )


    include_directories("${HUMOTO_MODULE_DIR}/include")
    set(HUMOTO_MODULE_${HUMOTO_MODULE}_INCLUDE_DIR "${HUMOTO_MODULE_DIR}/include" PARENT_SCOPE)


    if (HUMOTO_BUILD_TESTS OR HUMOTO_BUILD_REGRESSION_TESTS)
        enable_testing()

        include(humoto_copy_extra_test_files)
        include(humoto_define_test)
        include(humoto_define_regression_test)
        include(humoto_parse_test_dependencies)

        if (HUMOTO_BUILD_WITH_QI)
            set (HUMOTO_RUNTIME_OUTPUT_PREFIX   "${CMAKE_BINARY_DIR}/sdk/bin/")
        else(HUMOTO_BUILD_WITH_QI)
            set (HUMOTO_RUNTIME_OUTPUT_PREFIX   "${PROJECT_BINARY_DIR}/tests/")
        endif(HUMOTO_BUILD_WITH_QI)

        if (EXISTS "${HUMOTO_MODULE_DIR}/tests")
            set (CMAKE_RUNTIME_OUTPUT_DIRECTORY     "${HUMOTO_RUNTIME_OUTPUT_PREFIX}/${HUMOTO_MODULE}/")
            add_custom_target(  "${HUMOTO_MODULE}" )
            add_subdirectory (  "${HUMOTO_MODULE_DIR}/tests"
                                "${PROJECT_BINARY_DIR}/humoto_modules/${HUMOTO_MODULE}")
        endif()
    endif()

    if (EXISTS "${HUMOTO_MODULE_DIR}/include")
        install (DIRECTORY "${HUMOTO_MODULE_DIR}/include/humoto/"
                 DESTINATION "${humoto_INCLUDE_DIR}/")
    endif()

    if (EXISTS "${HUMOTO_MODULE_DIR}/config")
        install (DIRECTORY "${HUMOTO_MODULE_DIR}/config/"
                 DESTINATION "${humoto_CONFIG_DIR}/${HUMOTO_MODULE}/")
    endif()
endfunction(humoto_add_module)
