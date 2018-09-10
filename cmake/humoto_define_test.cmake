# parameters
#   HUMOTO_MODULE               module
#   TEST_ID                     test id, number starting from 000
#   DEPENDENCIES                list of dependencies
#   [OPTIONAL_DEPENDENCIES]     list of optional dependencies, can be omitted
#
function(humoto_define_test HUMOTO_MODULE TEST_ID_SOURCES DEPENDENCIES)
    # pop test id
    list(GET TEST_ID_SOURCES 0 TEST_ID)
    list(REMOVE_AT TEST_ID_SOURCES 0)

    set(TEST_NAME test_${TEST_ID})
    set(TGT_NAME "${HUMOTO_MODULE}_${TEST_NAME}")

    list(LENGTH TEST_ID_SOURCES NUM_SOURCE_FILES)
    if (NUM_SOURCE_FILES GREATER "0")
        set (SOURCES    ${TEST_ID_SOURCES})
    else()
        set (SOURCES    "${TEST_NAME}.cpp")
    endif()



    include_directories("${HUMOTO_CORE_DIR}/tests_include/")

    set(OPTIONAL_DEPENDENCIES   "${ARGV3}")
    set(LINK_TO_LIBRARIES       "${HUMOTO_MANDATORY_LIBRARIES}")
    set(TGT_DEPENDS             "")


    humoto_parse_test_dependencies("${DEPENDENCIES};ariles" "${LINK_TO_LIBRARIES}" "${TGT_DEPENDS}")
    if (MISSING_DEPENDENCY)
        return()
    endif()
    # optional dependencies
    humoto_parse_test_dependencies("${OPTIONAL_DEPENDENCIES}" "${LINK_TO_LIBRARIES}" "${TGT_DEPENDS}")


    set(TGT_NAME "${HUMOTO_MODULE}_${TEST_NAME}")

    if(HUMOTO_BUILD_WITH_QI)
        #qibuild modif
        qi_create_bin(${TGT_NAME} ${SOURCES} SUBFOLDER ${HUMOTO_MODULE})
    else()
        add_executable(${TGT_NAME} ${SOURCES})
    endif()
    add_dependencies("${HUMOTO_MODULE}" "${TGT_NAME}")


    set_target_properties(${TGT_NAME} PROPERTIES OUTPUT_NAME "${TEST_NAME}")

    add_dependencies(${TGT_NAME} TGT_humoto_copy_headers ${TGT_DEPENDS})

    target_link_libraries(${TGT_NAME} ${LINK_TO_LIBRARIES})

    #if(HUMOTO_BUILD_WITH_QI)
    #    qi_use_lib(${TGT_NAME} BOOST EIGEN3)
    #endif()


    humoto_copy_extra_test_files(${TGT_NAME} "copy_cfg" "${CMAKE_CURRENT_LIST_DIR}" "${TEST_NAME}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endfunction()
