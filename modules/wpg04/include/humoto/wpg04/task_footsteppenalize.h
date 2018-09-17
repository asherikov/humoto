/**
    @file
    @author  Alexander Sherikov
    @copyright 2014-2017 INRIA, 2014-2015 CNRS. Licensed under the Apache
    License, Version 2.0. (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

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
        class HUMOTO_LOCAL TaskFootstepPenalize : public humoto::TaskZeroVariables
        {
            #define HUMOTO_CONFIG_ENTRIES \
                HUMOTO_CONFIG_PARENT_CLASS(TaskZeroVariables)
            #include HUMOTO_CONFIG_DEFINE_ACCESSORS


            public:
                explicit TaskFootstepPenalize(const double gain = 1.0)
                    : TaskZeroVariables(gain, "TaskFootstepPenalize", FOOTPOS_VARIABLES_ID)
                {
                    // nothing to do
                }


                virtual void setDefaults()
                {
                    TaskZeroVariables::setDefaults();
                    TaskZeroVariables::setVariablesID(FOOTPOS_VARIABLES_ID);
                    setGain(1.0);
                }


                virtual void finalize()
                {
                    TaskZeroVariables::finalize();
                }
        };
    }
}
