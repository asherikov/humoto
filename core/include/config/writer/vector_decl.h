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
        namespace writer
        {
            /**
             * @brief Read configuration entry (std::vector)
             *
             * @tparam t_VectorEntryType type of the entry of std::vector
             *
             * @param[in] entry      data
             * @param[in] entry_name name
             */
            template <  class t_Writer,
                        typename t_VectorEntryType>
                void writeBody( t_Writer & writer,
                                const std::vector<t_VectorEntryType> & entry);
        }
    }
}
