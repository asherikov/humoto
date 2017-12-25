/**
    @file
    @author  Alexander Sherikov

    @copyright 2014-2017 Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once

#include "eigen_decl.h"
#include "vector_decl.h"

namespace humoto
{
    namespace config
    {
        namespace reader
        {
            /**
             * @brief Read configuration entry
             *
             * @tparam t_Entry type of the entry
             *
             * @param[out] entry     configuration parameter
             * @param[in] crash_on_missing_entry
             */
            template <class t_Reader>
                void readBody(   t_Reader  & reader,
                                        humoto::config::CommonConfigurableBase & entry,
                                        const bool crash_on_missing_entry = false)
            {
                entry.setDefaults();
                entry.readConfigEntries(reader, crash_on_missing_entry);
            }


            /**
             * @brief Read configuration entry (an enum)
             * This function is necessary since an explicit casting to integer is needed.
             *
             * @tparam t_Entry type of the entry
             *
             * @param[out] entry     configuration parameter
             * @param[in] crash_on_missing_entry
             */
            template <  class t_Reader,
                        typename t_Enumeration>
                void readBody(   t_Reader & reader,
                                        t_Enumeration &entry,
                                        const bool crash_on_missing_entry = false,
                                        // disable this function for CommonConfigurableBase
                                        HUMOTO_CONFIG_IS_BASE_OF_DISABLER_TYPE(
                                            humoto::config::CommonConfigurableBase, t_Enumeration) *dummy_base = NULL,
                                        // ENABLE this function for enums
                                        HUMOTO_CONFIG_IS_ENUM_ENABLER_TYPE(t_Enumeration) *dummy_enum = NULL)
            {
                int tmp_value = 0;
                reader.readElement(tmp_value);
                entry = static_cast<t_Enumeration> (tmp_value);
            }


            /**
             * @brief Read configuration entry (scalar template)
             *
             * @tparam t_Entry type of the entry
             *
             * @param[out] entry     configuration parameter
             * @param[in] crash_on_missing_entry
             */
            template <  class t_Reader,
                        typename t_Entry>
                void readBody(   t_Reader & reader,
                                        t_Entry &entry,
                                        const bool crash_on_missing_entry = false,
                                        // disable this function for CommonConfigurableBase
                                        HUMOTO_CONFIG_IS_BASE_OF_DISABLER_TYPE(
                                            humoto::config::CommonConfigurableBase, t_Entry) *dummy_base = NULL,
                                        // disable this function for enums
                                        HUMOTO_CONFIG_IS_ENUM_DISABLER_TYPE(t_Entry) *dummy_enum = NULL)
            {
                reader.readElement(entry);
            }



            /**
             * @brief Read configuration entry
             *
             * @tparam t_Entry type of the entry
             *
             * @param[out] entry      configuration parameter
             * @param[in]  entry_name name of the configuration parameter
             * @param[in]  crash_on_missing_entry
             */
            template <  class t_Reader,
                        class t_Entry>
                void readEntry(  t_Reader & reader,
                                        t_Entry &entry,
                                        const std::string & entry_name,
                                        const bool crash_on_missing_entry = false)
            {
                if (reader.descend(entry_name))
                {
                    try
                    {
                        readBody(reader, entry, crash_on_missing_entry);
                    }
                    catch(const std::exception &e)
                    {
                        HUMOTO_THROW_MSG(   std::string("Failed to parse entry <")
                                            + entry_name
                                            + "> in the configuration file: "
                                            + e.what());
                    }

                    reader.ascend();
                }
                else
                {
                    if (crash_on_missing_entry)
                    {
                        HUMOTO_THROW_MSG(std::string("Configuration file does not contain entry '") + entry_name + "'.");
                    }
                }
            }
        }
    }
}

#include "eigen_impl.h"
#include "vector_impl.h"
