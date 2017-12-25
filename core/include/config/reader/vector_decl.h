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
                        typename t_VectorEntryType>
                void readBody(   t_Reader & reader,
                                        std::vector<t_VectorEntryType> & entry,
                                        const bool crash_on_missing_entry = false);
        }
    }
}

