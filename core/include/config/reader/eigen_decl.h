/**
    @file
    @author  Alexander Sherikov

    @copyright 2017 Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once


namespace humoto
{
    namespace config
    {
        namespace reader
        {
            template <  class t_Reader,
                        typename t_Scalar,
                        int t_rows,
                        int t_flags>
                void readBody(  t_Reader &reader,
                                Eigen::Matrix<t_Scalar, t_rows, 1, t_flags> &entry,
                                const bool crash_on_missing_entry = false);

            template <  class t_Reader,
                        typename t_Scalar,
                        int t_rows,
                        int t_cols,
                        int t_flags>
                void readBody(  t_Reader & reader,
                                Eigen::Matrix<t_Scalar, t_rows, t_cols, t_flags> &entry,
                                const bool crash_on_missing_entry = false);
        }
    }
}

