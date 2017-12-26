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
            /**
             * @brief Read configuration entry (std::vector)
             *
             * @tparam t_VectorEntryType type of the entry of std::vector
             *
             * @param[out] entry      configuration parameter
             * @param[in]  crash_on_missing_entry
             */
            template <  class t_Reader,
                        typename t_VectorEntryType>
                void readBody(   t_Reader & reader,
                                        std::vector<t_VectorEntryType> & entry,
                                        const bool crash_on_missing_entry)
            {
                entry.resize(reader.startArray());
                for(std::size_t i = 0; i < entry.size(); ++i)
                {
                    readBody(reader, entry[i], crash_on_missing_entry);
                    reader.shiftArray();
                }
                reader.endArray();
            }
        }
    }
}

