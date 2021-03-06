/**
    @file
    @author  Alexander Sherikov
    @author  Jan Michalczyk
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#include <iostream>
#include <limits>
#include <iomanip>

#define HUMOTO_GLOBAL_LOGGER_ENABLED

// common & abstract classes
#include "humoto/humoto.h"
// specific solver (many can be included simultaneously)
#include "humoto/kktsolver.h"
#include "humoto/qpoases.h"
// specific control problem (many can be included simultaneously)
#include "humoto/pepper_mpc.h"
#include "humoto/pepper_ik.h"

#include "utilities.h"

#define MODEL_FEATURES humoto::pepper_ik::ModelFeatures::FIXED_WHEELS | humoto::pepper_ik::ModelFeatures::ROOT_PLANAR


HUMOTO_INITIALIZE_GLOBAL_LOGGER(std::cout);
//HUMOTO_INITIALIZE_GLOBAL_LOGGER("test_001.m");

/**
 * @param[in] argc number of arguments
 * @param[in] argv arguments
 *
 * @return status
 */
int main(int argc, char **argv)
{
    std::string config_path = humoto_tests::getConfigPath(argc, argv, 2);
    std::string log_file_name = humoto_tests::getLogFileName(argc, argv, 3);

    std::string config_file_name = std::string(argv[0]) + ".yaml";

    std::string mpc_motion_param_config_file_name;
    std::string mpc_motion_param_config_entry_id;
    if (argc > 1)
    {
        mpc_motion_param_config_file_name = argv[1];
        mpc_motion_param_config_entry_id = "";
    }
    else
    {
        mpc_motion_param_config_file_name = config_file_name;
        mpc_motion_param_config_entry_id = "MPCMotionParameters";
    }


    humoto::config::yaml::Reader    config_reader(config_file_name);

    double sampling_interval_difference = 0.001;


    try
    {
        humoto::Logger       logger(log_file_name);
        humoto::LogEntryName prefix;

        // -----------------ik--------------------------------

        // optimization problem (a stack of tasks / hierarchy)
        humoto::pepper_ik::ConfigurableOptimizationProblem<MODEL_FEATURES>  ik_opt_problem;
        ik_opt_problem.readConfig(config_reader, "IKOptimizationProblem");

        // parameters of the solver
        humoto::kktsolver::SolverParameters   ik_solver_parameters;
        ik_solver_parameters.solution_method_ = humoto::kktsolver::SolverParameters::CONSTRAINT_ELIMINATION_LLT;
        // a solver which is giong to be used
        humoto::kktsolver::Solver             ik_solver(ik_solver_parameters);

        // solution
        humoto::Solution           ik_solution;
        // parameters of the control problem
        humoto::pepper_ik::WBCParameters           ik_wbc_parameters(config_reader);
        // control problem, which is used to construct an optimization problem
        humoto::pepper_ik::WholeBodyController<MODEL_FEATURES>     ik_wbc;
        // model representing the controlled system
        humoto::pepper_ik::Model<MODEL_FEATURES>                   ik_model;
        ik_model.loadParameters(config_path + "pepper_fixedwheels_roottibia_planar.urdf");
        // options for walking
        humoto::pepper_ik::MotionParameters     ik_motion_parameters(config_reader, "IKMotionParameters", true);
        //humoto::pepper_ik::GeneralizedCoordinates<MODEL_FEATURES>   ik_generalized_coordinates;
        humoto::pepper_ik::GeneralizedCoordinates<MODEL_FEATURES>   ik_generalized_coordinates;
        ik_generalized_coordinates.readConfig<humoto::config::yaml>(config_path + "initial_state_pepper_ik_planar.yaml", true);
        ik_model.updateState(ik_generalized_coordinates);

        // -----------------ik--------------------------------


        // -----------------mpc--------------------------------

        // optimization problem (a stack of tasks / hierarchy)
        humoto::pepper_mpc::ConfigurableOptimizationProblem     mpc_opt_problem;
        mpc_opt_problem.readConfig(config_reader, "MPCOptimizationProblem");


        // parameters of the solver
        humoto::qpoases::SolverParameters   mpc_solver_parameters;
        //solver_parameters.crash_on_any_failure_ = false;
        // a solver which is giong to be used
        humoto::qpoases::Solver             mpc_solver(mpc_solver_parameters);
        // solution
        humoto::qpoases::Solution           mpc_solution;
        humoto::pepper_mpc::RobotParameters         mpc_robot_parameters(config_reader);
        // model representing the controlled system
        humoto::pepper_mpc::Model                   mpc_model(mpc_robot_parameters);
        // parameters of the control problem
        humoto::pepper_mpc::MPCParameters           mpc_mg_parameters(config_reader);
        // control problem, which is used to construct an optimization problem
        humoto::pepper_mpc::MPCforMG                mpc_mg(mpc_mg_parameters);

        humoto::pepper_mpc::ModelState              mpc_model_state;


        // options for walking
        humoto::pepper_mpc::MotionParameters        mpc_motion_parameters_1;
        mpc_motion_parameters_1.readConfig<humoto::config::yaml>(mpc_motion_param_config_file_name,
                                                                        mpc_motion_param_config_entry_id);

        humoto::pepper_mpc::MotionParameters        mpc_motion_parameters_2;
        mpc_motion_parameters_2.duration_ms_    = 1500;

        /*
        humoto::pepper_mpc::MotionParameters        mpc_motion_parameters_3 = mpc_motion_parameters_1;
        mpc_motion_parameters_3.base_velocity_ = - mpc_motion_parameters_1.base_velocity_;

        humoto::pepper_mpc::MotionParameters        mpc_motion_parameters_4;
        mpc_motion_parameters_4.duration_ms_    = 1500;
        */

        std::deque<humoto::pepper_mpc::MotionParameters>    mpc_motion_param_deque;

        mpc_motion_param_deque.push_back(mpc_motion_parameters_2);
        mpc_motion_param_deque.push_back(mpc_motion_parameters_1);
        mpc_motion_param_deque.push_back(mpc_motion_parameters_2);
        //mpc_motion_param_deque.push_back(mpc_motion_parameters_3);
        //mpc_motion_param_deque.push_back(mpc_motion_parameters_4);

        // -----------------mpc--------------------------------




        humoto::Timer                   timer;
        humoto::pepper_ik::RobotCommand command;

        for (std::size_t i = 0;; ++i)
        {
            // -----------------feedback--------------------------------
            if (i > 0)
            {
                // in the perfect case
                //  sampling_interval_difference = 0.0

                //ik_model.correct(command,
                //                 sampling_interval_difference);
                ik_model.correct(ik_model.getState(),
                                 command,
                                 sampling_interval_difference);

                mpc_mg.shift(   mpc_motion_param_deque,
                                mpc_mg_parameters.subsampling_time_ms_
                                + humoto::convertSecondToMillisecond(sampling_interval_difference));
            }
            // -----------------feedback--------------------------------

            // -----------------sync-models--------------------------------
            mpc_model_state.update( ik_model.getBaseMass(),
                                    ik_model.getBodyMass(),
                                    ik_model.getBaseCoM(),
                                    ik_model.getBodyCoM(),
                                    ik_model.getBaseYaw());
            //mpc_model_state.log(logger, prefix, "current_state");

            mpc_model.updateState(mpc_model_state);
            // -----------------sync-models--------------------------------


            prefix = humoto::LogEntryName("humoto").add(i);

            timer.start();
            // prepare control problem for new iteration
            //if (mpc_mg.update(  mpc_motion_parameters,
            if (mpc_mg.update(  mpc_motion_param_deque,
                                mpc_model) != humoto::ControlProblemStatus::OK)
            {
                break;
            }

            // form an optimization problem
            mpc_opt_problem.form(mpc_solution, mpc_model, mpc_mg);

            // solve an optimization problem
            mpc_solver.solve(mpc_solution, mpc_opt_problem);
            //timer.stop();
            //HUMOTO_LOG_RAW(timer);

            mpc_mg.parseSolution(mpc_solution);

            //========================
            // logging
            //mpc_opt_problem.log(logger, prefix);
            //mpc_solver.log(logger, prefix);
            //mpc_solution.log(logger, prefix);
            //mpc_mg.log(logger, prefix);
            //mpc_model.log(logger, prefix);
            //========================

            //mpc_model_state.log(logger, prefix, "current_state");
            //logger.log(humoto::LogEntryName(prefix).add("current_cop"), mpc_model.getCoP(mpc_model_state));

            // extract next model state from the solution and update model
            mpc_model_state = mpc_mg.getModelState(mpc_model, mpc_mg_parameters.subsampling_time_ms_);

            ik_motion_parameters.base_com_position_.x()    = mpc_model_state.base_state_.position_.x();
            ik_motion_parameters.base_com_position_.y()    = mpc_model_state.base_state_.position_.y();
            ik_motion_parameters.base_com_position_.z()    = mpc_model_state.base_state_.position_.z();
            ik_motion_parameters.base_orientation_rpy_.x() = 0.0;
            ik_motion_parameters.base_orientation_rpy_.y() = 0.0;
            ik_motion_parameters.base_orientation_rpy_.z() = mpc_model_state.base_state_.rpy_.z();
            ik_motion_parameters.body_com_position_.x()    = mpc_model_state.body_state_.position_.x();
            ik_motion_parameters.body_com_position_.y()    = mpc_model_state.body_state_.position_.y();
            ik_motion_parameters.body_com_position_.z()    = mpc_model_state.body_state_.position_.z();

            //mpc_model_state.log(logger, prefix, "next_state");
            //logger.log(humoto::LogEntryName(prefix).add("next_cop"), mpc_model.getCoP(mpc_model_state));

            //ik_model.log(logger, prefix);

            HUMOTO_LOG_RAW("===================");
            HUMOTO_LOG("iter = ", i);
            ik_model.saveCurrentState();
            for (std::size_t j = 0; ; ++j)
            {
                if (j == ik_wbc_parameters.maximal_number_of_iterations_)
                {
                    HUMOTO_THROW_MSG("Maximal number of IK iterations reached.");
                }
                //timer.start();
                // prepare control problem for new iteration
                if (ik_wbc.update(ik_model, ik_motion_parameters) != humoto::ControlProblemStatus::OK)
                {
                    HUMOTO_THROW_MSG("Control problem could not be updated.");
                }

                // form an optimization problem
                ik_opt_problem.form(ik_solution, ik_model, ik_wbc);

                // solve an optimization problem
                ik_solver.solve(ik_solution, ik_opt_problem);
                //timer.stop();
                //HUMOTO_LOG_RAW(timer);

                //========================
                // logging
                prefix = humoto::LogEntryName("ik_loop").add(i).add(j);

                //ik_opt_problem.log(logger, prefix);
                //ik_solver.log(logger, prefix);
                //ik_solution.log(logger, prefix);
                //ik_wbc.log(logger, prefix);
                ik_model.log(logger, prefix);
                //========================

                // extract next model state from the solution and update model
                ik_generalized_coordinates = ik_wbc.getNextGeneralizedCoordinates(ik_solution, ik_model);
                ik_model.updateState(ik_generalized_coordinates);

                humoto::pepper_ik::MotionParameters ik_motion_parameters_errors;
                ik_model.getStateError(ik_motion_parameters_errors, ik_motion_parameters);

                HUMOTO_LOG_RAW("---");

                HUMOTO_LOG_RAW(ik_motion_parameters_errors.base_com_position_);
                HUMOTO_LOG_RAW(ik_motion_parameters_errors.base_orientation_rpy_);
                HUMOTO_LOG_RAW(ik_motion_parameters_errors.body_com_position_);

                /*
                HUMOTO_LOG("Error", ik_motion_parameters_errors.body_com_position_);
                HUMOTO_LOG("Reference", ik_motion_parameters.body_com_position_);
                */

                if (    ik_motion_parameters_errors.base_com_position_.norm()
                        + ik_motion_parameters_errors.base_orientation_rpy_.norm()
                        + ik_motion_parameters_errors.body_com_position_.norm()  <  ik_wbc_parameters.motion_parameters_tolerance_)
                {
                    break;
                }


                if (ik_solution.x_.lpNorm<Eigen::Infinity>() < ik_wbc_parameters.joint_angle_error_tolerance_)
                {
                    break;
                }
            }

            ik_model.getRobotCommand(command, ik_wbc_parameters.control_interval_);

            prefix = humoto::LogEntryName("control_loop").add(i);
            command.log(logger, prefix);
            ik_model.getTorsoRootPose().log(logger, prefix, "torso_root_pose");
//            ik_model.log(logger, prefix);
        }
    }
    catch (const std::exception &e)
    {
        HUMOTO_LOG_RAW(e.what());
        exit(-1);
    }

    return (0);
}
