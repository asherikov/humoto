/**
    @file
    @author  Alexander Sherikov
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
         * @brief
         */
        class HUMOTO_LOCAL TaskReferenceSolution : public humoto::TaskAB
        {
            #define HUMOTO_CONFIG_ENTRIES \
                HUMOTO_CONFIG_PARENT_CLASS(TaskAB)
            #include HUMOTO_CONFIG_DEFINE_ACCESSORS


            protected:
                void setDefaults()
                {
                    TaskAB::setDefaults();
                }


            public:
                explicit TaskReferenceSolution(const double gain = 1.0) : TaskAB("TaskReferenceSolution", gain)
                {
                }



                /// @copydoc humoto::TaskBase::form
                void form(  const humoto::SolutionStructure &sol_structure,
                            const humoto::Model &model_base,
                            const humoto::ControlProblem &control_problem)
                {
                    const humoto::wpg04::MPCforWPG  &mpc = dynamic_cast <const humoto::wpg04::MPCforWPG &> (control_problem);

                    Eigen::MatrixXd     &A = getA();
                    Eigen::VectorXd     &b = getB();


                    EigenIndex  num_rows_cstate_profile = mpc.S_.rows();
                    EigenIndex  num_rows_footpos_profile = mpc.Vfp_.rows();

                    EigenIndex  num_rows = num_rows_cstate_profile + num_rows_footpos_profile;
                    EigenIndex  num_cols = mpc.S_.cols();


                    A.resize(num_rows, num_cols);
                    b.resize(num_rows);


                    A.topRows(num_rows_cstate_profile).noalias() = getGain()*mpc.S_;
                    b.head(num_rows_cstate_profile).noalias() = getGain()*(mpc.cstate_profile_ - mpc.s_);

                    A.bottomRows(num_rows_footpos_profile).setZero();
                    sol_structure.getMatrixPart(FOOTPOS_VARIABLES_ID, A).bottomRows(num_rows_footpos_profile) = getGain()*mpc.Vfp_;
                    b.tail(num_rows_footpos_profile).noalias() = getGain()*(mpc.footpos_profile_ - mpc.vfp_);
                }
        };
    }
}
