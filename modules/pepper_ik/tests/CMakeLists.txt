if (HUMOTO_BUILD_TESTS)
    # Test model with floating base
    humoto_define_test(${HUMOTO_MODULE} "000" "qpOASES;rbdl")

    # Test model with planar root joint
    humoto_define_test(${HUMOTO_MODULE} "001" "rbdl")

    # Test IK + MPC
    humoto_define_test(${HUMOTO_MODULE} "002" "rbdl;qpOASES")

    # Feasibility test
    humoto_define_test(${HUMOTO_MODULE} "003" "rbdl;qpOASES")

    # Test MPC + IK head control using cartesion angular velocity command
    humoto_define_test(${HUMOTO_MODULE} "004" "rbdl;qpOASES")
endif()

if (HUMOTO_BUILD_REGRESSION_TESTS)
    # Crosscheck RBDL, rmt, and RBDL interface functions
    #   computation of the CoM position
    #   CoM Jacobians
    humoto_define_regression_test(${HUMOTO_MODULE} "000" "rbdl")

    # Test IK
    humoto_define_regression_test(${HUMOTO_MODULE} "001" "rbdl")
endif()
