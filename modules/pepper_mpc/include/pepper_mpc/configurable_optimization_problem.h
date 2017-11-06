/**
    @file
    @author Alexander Sherikov
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once


namespace humoto
{
    namespace pepper_mpc
    {
        /**
         * @brief Class representing the hierarchy of the problem
         */
        class HUMOTO_LOCAL ConfigurableOptimizationProblem
            :   public humoto::ConfigurableOptimizationProblem
        {
            protected:
                /**
                 * @brief Fill map with all pointers to all tasks for given module
                 */
                humoto::TaskSharedPointer getTask(const std::string &string_id) const
                {
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskBaseAccelerationBounds, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskBaseJerkMinimization, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskBasePositionReference, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskBaseVelocityBounds, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskBaseVelocityReference, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskBodyJerkMinimization, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskBodyPositionBounds, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskBodyPositionReference, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskCoPCentering, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskCoPPositionBounds, string_id);

                    return(humoto::ConfigurableOptimizationProblem::getTask(string_id));
                }
        };
    }
}
