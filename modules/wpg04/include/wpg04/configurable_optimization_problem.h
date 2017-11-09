/**
    @file
    @author Jan Michalczyk
    @author Alexander Sherikov
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once


namespace humoto
{
    namespace wpg04
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
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskCoMVelocity, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskCoPPosition, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskCoPVelocity, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskFootstepBounds, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskCoPBounds, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskTerminalConstraint, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskReferenceSolution, string_id);
                    HUMOTO_CONFIGURABLE_HIERARCHY_GET_TASK_IF_ID_MATCHES(TaskFootstepPenalize, string_id);

                    return(humoto::ConfigurableOptimizationProblem::getTask(string_id));
                }
        };
    }
}
