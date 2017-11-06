/**
    @file
    @author Alexander Sherikov
    @author Jan Michalczyk
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once

#define HUMOTO_CONFIGURABLE_HIERARCHY_GET_PEPPER_IK_TASK_IF_ID_MATCHES(task_name, id)\
                if (#task_name == id) \
                {\
                    return (humoto::TaskSharedPointer(new task_name<t_features>)); \
                }

namespace humoto
{
    namespace pepper_ik
    {
        /**
         * @brief Class representing the hierarchy of the problem
         */
        template <int t_features>
            class HUMOTO_LOCAL ConfigurableOptimizationProblem
                :   public humoto::ConfigurableOptimizationProblem
        {
            protected:
                /**
                 * @brief Fill map with all pointers to all tasks for given module
                 */
                humoto::TaskSharedPointer getTask(const std::string &string_id) const
                {
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_PEPPER_IK_TASK_IF_ID_MATCHES(TaskBaseCoMTracking, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_PEPPER_IK_TASK_IF_ID_MATCHES(TaskBaseOrientation, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_PEPPER_IK_TASK_IF_ID_MATCHES(TaskBodyCoMTracking, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_PEPPER_IK_TASK_IF_ID_MATCHES(TaskFixArms, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_PEPPER_IK_TASK_IF_ID_MATCHES(TaskFixHead, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_PEPPER_IK_TASK_IF_ID_MATCHES(TaskJointsBounds, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_PEPPER_IK_TASK_IF_ID_MATCHES(TaskJointsReference, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_PEPPER_IK_TASK_IF_ID_MATCHES(TaskTagOrientation, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_PEPPER_IK_TASK_IF_ID_MATCHES(TaskTagAngularVelocity, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_PEPPER_IK_TASK_IF_ID_MATCHES(TaskTagCompleteVelocity, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_PEPPER_IK_TASK_IF_ID_MATCHES(TaskTagPose, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_PEPPER_IK_TASK_IF_ID_MATCHES(TaskTagPose3Dof, string_id);

                    return(humoto::ConfigurableOptimizationProblem::getTask(string_id));
                }
        };
    }
}
