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
    namespace wpg04
    {
        class HUMOTO_LOCAL TaskCoMAcceleration: public humoto::TaskAB
        {
            #define HUMOTO_CONFIG_ENTRIES \
                HUMOTO_CONFIG_PARENT_CLASS(TaskAB)
            #include HUMOTO_CONFIG_DEFINE_ACCESSORS


            public:
                explicit TaskCoMAcceleration(const double gain = 1.0) : TaskAB("TaskCoMAcceleration", gain)
                {
                }


                /// @copydoc humoto::TaskBase::form
                void form(  const humoto::SolutionStructure &sol_structure,
                            const humoto::Model &model_base,
                            const humoto::ControlProblem &control_problem)
                {
                    const humoto::wpg04::MPCforWPG  &mpc = dynamic_cast <const humoto::wpg04::MPCforWPG &> (control_problem);

                    getA().noalias() = getGain() * (mpc.acceleration_selector_ * mpc.S_); // Sa
                    getB().noalias() = -getGain() * (mpc.acceleration_selector_ * mpc.s_); // sa
                }


                virtual void setDefaults()
                {
                    TaskAB::setDefaults();
                    setGain(1.0);
                }


                virtual void finalize()
                {
                    TaskAB::finalize();
                }
        };
    }
}
