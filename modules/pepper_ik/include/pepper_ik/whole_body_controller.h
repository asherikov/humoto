/**
    @file
    @author  Alexander Sherikov
    @author  Jan Michalczyk 
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once

namespace humoto
{
    namespace pepper_ik
    {
        /**
         * @brief Whole body controller
         *
         * @tparam t_features features identifying the model
         */
        template <int   t_features>
            class HUMOTO_LOCAL WholeBodyController : public humoto::ControlProblem
        {
            private:
                bool                             solution_is_parsed_;
                humoto::pepper_ik::WBCParameters wbc_parameters_;

            public:
                humoto::pepper_ik::MotionParameters    motion_parameters_;
                std::map<std::string, etools::Vector6> tag_velocity_;


            public:
                /**
                 * @brief Get tag velocity
                 *
                 * @param[in, out] tag_velocity
                 * @param[in]      tag_name
                 */
                void getTagRefVelocity(etools::Vector6& tag_velocity, const std::string& tag_name) const
                {
                    if((tag_velocity_.find(tag_name) != tag_velocity_.end()))
                    {
                        tag_velocity = tag_velocity_.at(tag_name);
                    }
                    else
                    {
                        tag_velocity.setZero();
                    }
                }
                
                
                /**
                 * @brief Set tag(s) velocity
                 *
                 * @param[in] tag_velocity
                 */
                void setTagRefVelocity(const std::map<std::string, etools::Vector6>& tag_velocity)
                {
                    tag_velocity_ = tag_velocity;
                }


                /**
                 * @brief Get velocity in global frame
                 *
                 * @param[in] model
                 * @param[in] tag_name
                 * @param[in] spatial_type
                 * @return    velocity in global frame
                 */
                Eigen::VectorXd getTagVelocityInGlobal(const humoto::pepper_ik::Model<t_features>& model, 
                                                       const std::string&                          tag_name, 
                                                       const rbdl::SpatialType::Type&              spatial_type) const
                {
                    rbdl::TagLinkPtr tag = model.getLinkTag(tag_name);
                    
                    etools::Vector6 tag_ref_velocity;
                    getTagRefVelocity(tag_ref_velocity, tag_name);

                    Eigen::VectorXd velocity_in_global;
                    velocity_in_global.resize(rbdl::SpatialType::getNumberOfElements(spatial_type));

                    std::size_t part_size = 3;
                    switch(spatial_type)
                    {
                        case rbdl::SpatialType::ROTATION:
                            velocity_in_global.head(part_size) = model.getTagOrientation(tag) *
                                                    tag_ref_velocity.tail(part_size);
                            break;
                        
                        case rbdl::SpatialType::TRANSLATION:
                            velocity_in_global.tail(part_size) = model.getTagPosition(tag).cross(model.getTagOrientation(tag)
                                                    * tag_ref_velocity.tail(part_size))
                                                    + model.getTagOrientation(tag) *
                                                      tag_ref_velocity.head(part_size);
                            break;

                        case rbdl::SpatialType::COMPLETE:
                            velocity_in_global.head(part_size) = model.getTagOrientation(tag) *
                                                    tag_ref_velocity.tail(part_size);
                        
                            velocity_in_global.tail(part_size) = model.getTagPosition(tag).cross(model.getTagOrientation(tag)
                                                    * tag_ref_velocity.tail(part_size))
                                                    + model.getTagOrientation(tag) *
                                                      tag_ref_velocity.head(part_size);
                            break;
                        
                        default:
                            HUMOTO_THROW_MSG("Unsupported velocity type.");
                    }

                    return(velocity_in_global);
                }


                /**
                 * @brief Get tag pose error in global frame
                 *
                 * @param[in] model
                 * @param[in] tag_name
                 * @param[in] spatial_type
                 * @return    error in pose in global frame
                 */
                etools::Vector6 getTagPoseErrorInGlobal(const humoto::pepper_ik::Model<t_features>& model,
                                                        const std::string&                          tag_name, 
                                                        const rbdl::SpatialType::Type&              spatial_type) const
                {
                    rbdl::TagLinkPtr tag = model.getLinkTag(tag_name);
                    
                    etools::Vector6 tag_pose_error;

                    Eigen::VectorXd pose_increment =
                            wbc_parameters_.control_interval_ * getTagVelocityInGlobal(model, tag_name, rbdl::SpatialType::COMPLETE);
                    
                    std::size_t part_size = 3;
                    tag_pose_error.head(part_size) = rigidbody::getRotationErrorAngleAxis(
                                                        model.getTagOrientation(tag),
                                                        rigidbody::convertEulerAnglesToMatrix(           
                                                            rigidbody::convertMatrixToEulerAngles(model.getTagOrientation(tag),
                                                            rigidbody::EulerAngles::RPY) +
                                                        pose_increment.head(part_size), rigidbody::EulerAngles::RPY));
                    
                    tag_pose_error.tail(part_size) = pose_increment.tail(part_size); 

                    return(tag_pose_error);
                }


                /**
                 * @brief Constructor
                 */
                WholeBodyController()
                {
                    solution_is_parsed_ = false;
                }
                
                
                /**
                 * @brief Constructor
                 *
                 * @param[in] control_interval
                 */
                WholeBodyController(const humoto::pepper_ik::WBCParameters& wbc_parameters) : wbc_parameters_(wbc_parameters)
                {
                    solution_is_parsed_ = false;
                }


                /**
                 * @brief Update control problem
                 *
                 * @param[in] model     model of the system
                 * @param[in] motion_parameters
                 *
                 * @return ControlProblemStatus::OK/ControlProblemStatus::STOPPED
                 */
                ControlProblemStatus::Status
                    update( humoto::pepper_ik::Model<t_features>          &model,
                            const humoto::pepper_ik::MotionParameters     &motion_parameters)
                {
                    humoto::ControlProblemStatus::Status    control_status = ControlProblemStatus::OK;
                    solution_is_parsed_ = false;

                    // decision variables
                    sol_structure_.reset();
                    sol_structure_.addSolutionPart(ROOT_VARIABLES_ID  , ModelDescription<t_features>::ROOT_DOF_NUMBER);
                    sol_structure_.addSolutionPart(JOINTS_VARIABLES_ID, ModelDescription<t_features>::JOINTS_DOF_NUMBER);

                    motion_parameters_ = motion_parameters;

                    return (control_status);
                }


                /**
                 * @brief Process solution
                 *
                 * @param[in] solution  solution
                 */
                void    parseSolution(const humoto::Solution &solution)
                {
                    solution_is_parsed_ = true;
                }


                /**
                 * @brief Get next model state.
                 *
                 * @param[in] solution  solution
                 * @param[in] model model
                 *
                 * @return next model state.
                 */
                humoto::pepper_ik::GeneralizedCoordinates<t_features>
                    getNextGeneralizedCoordinates(
                        const humoto::Solution                      &solution,
                        const humoto::pepper_ik::Model<t_features>  &model)
                {
                    HUMOTO_ASSERT(solution_is_parsed_ == false, "The solution is parsed.");
                    parseSolution(solution);

                    humoto::pepper_ik::GeneralizedCoordinates<t_features> next_generalized_coordinates;
                    next_generalized_coordinates = model.getState();
                    next_generalized_coordinates.root_pose_ += solution.getSolutionPart(ROOT_VARIABLES_ID);
                    next_generalized_coordinates.joint_angles_ += solution.getSolutionPart(JOINTS_VARIABLES_ID);

                    return(next_generalized_coordinates);
                }


                /**
                 * @brief Get next model state.
                 *
                 * @param[in] model model
                 *
                 * @return next model state.
                 */
                humoto::pepper_ik::GeneralizedCoordinates<t_features>
                    getNextGeneralizedCoordinates(
                        const humoto::pepper_ik::Model<t_features>                      &model) const
                {
                    HUMOTO_ASSERT(solution_is_parsed_ == true, "This function can be called only after the solution is parsed.");
                    return(model.state_);
                }


                /**
                 * @brief Log
                 *
                 * @param[in,out] logger logger
                 * @param[in] parent parent
                 * @param[in] name name
                 */
                void log(   humoto::Logger & logger HUMOTO_GLOBAL_LOGGER_IF_DEFINED,
                            const LogEntryName &parent = LogEntryName(),
                            const std::string & name = "wbc") const
                {
                }
        };
    }
}

