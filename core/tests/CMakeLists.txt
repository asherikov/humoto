if (HUMOTO_BUILD_TESTS)
    # Simple demo
    humoto_define_test(${HUMOTO_MODULE} "000" "config_yaml")

    # Two translation units
    humoto_define_test(${HUMOTO_MODULE} "001;test_001_unit_0.cpp;test_001_unit_1.cpp" "" "config_yaml;config_msgpack;rbdl;QuadProgpp;qpOASES;qpmad")
endif()


if (HUMOTO_BUILD_REGRESSION_TESTS)
    # Basic test of reading and writing of configuration files.
    humoto_define_regression_test(${HUMOTO_MODULE} "000" "config_yaml;config_msgpack")

    # Math tests:
    #   Block Matrix tests: creation, initialization, etc.
    #   Euler angles
    #   Matrix concatenation
    #   various triple integrator versions
    humoto_define_regression_test(${HUMOTO_MODULE} "001" "")

    # Tests of condensing functions
    humoto_define_regression_test(${HUMOTO_MODULE} "005" "config_yaml")
endif()
