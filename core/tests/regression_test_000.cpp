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
// walking-related classes
#include "humoto/walking.h"

//testing
#include "gtest/gtest.h"

HUMOTO_INITIALIZE_GLOBAL_LOGGER(std::cout);

//#undef HUMOTO_GLOBAL_LOGGER_ENABLED


class ConfigHierarchyMatchTest : public ::testing::Test
{
    protected:
        template<class t_Reader, class t_Writer>
            void test()
        {
            humoto::ConfigurableOptimizationProblem             opt_problem_out;

            humoto::TaskSharedPointer   task1 (new humoto::TaskInfeasibleInequality(1.0, "task1"));
            humoto::TaskSharedPointer   task2 (new humoto::TaskInfeasibleInequality(1.0, "task2"));
            humoto::TaskSharedPointer   task3 (new humoto::TaskZeroVariables(1.0, "task3"));
            humoto::TaskSharedPointer   task4 (new humoto::TaskInfeasibleInequality(1.0, "task4"));

            opt_problem_out.reset(3);

            opt_problem_out.pushTask(task1, 0, "TaskInfeasibleInequality");
            opt_problem_out.pushTask(task2, 0, "TaskInfeasibleInequality");

            opt_problem_out.pushTask(task3, 1, "TaskZeroVariables");

            opt_problem_out.pushTask(task4, 2, "TaskInfeasibleInequality");

            opt_problem_out.writeConfig<t_Writer>("hierarchy_match.cfg");

            // -------

            humoto::ConfigurableOptimizationProblem             opt_problem_in;
            opt_problem_in.readConfig<t_Reader>("hierarchy_match.cfg");

            // -------

            EXPECT_EQ(opt_problem_out.getNumberOfLevels(),  opt_problem_in.getNumberOfLevels());
            if (opt_problem_out.getNumberOfLevels() == opt_problem_in.getNumberOfLevels())
            {
                for (std::size_t i = 0; i < opt_problem_in.getNumberOfLevels(); ++i)
                {
                    EXPECT_EQ(opt_problem_out[i].getNumberOfConstraints(),  opt_problem_in[i].getNumberOfConstraints());
                    EXPECT_EQ(opt_problem_out[i].tasks_.size(),             opt_problem_in[i].tasks_.size());

                    if (opt_problem_out[i].tasks_.size() == opt_problem_in[i].tasks_.size())
                    {
                        std::list<humoto::HierarchyLevel::TaskInfo>::const_iterator it_out = opt_problem_out[i].tasks_.begin();
                        std::list<humoto::HierarchyLevel::TaskInfo>::const_iterator it_in = opt_problem_in[i].tasks_.begin();

                        for (; it_out != opt_problem_out[i].tasks_.end(); ++it_out, ++it_in)
                        {
                            EXPECT_EQ(it_out->ptr_->getDescription(), it_in->ptr_->getDescription());
                        }
                    }
                }
            }
        }
};



//---------------

TEST_F(ConfigHierarchyMatchTest, ConfigMatchHierarchyYAML)
{
    test<humoto::config::yaml, humoto::config::yaml>();
}



/**
 * @brief main
 *
 * @param[in] argc number of args
 * @param[in] argv args
 *
 * @return status
 */
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
