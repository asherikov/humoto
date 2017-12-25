/**
    @file
    @author  Alexander Sherikov

    @copyright 2017 Licensed under the Apache License, Version 2.0.
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
        namespace writer
        {
            /**
             * @brief Write a configuration entry (enum)
             *
             * @tparam t_EnumType type of the enum
             *
             * @param[in] entry      data
             * @param[in] entry_name name
             */
            template <  class t_Writer,
                        typename t_Enumeration>
                void writeBody( t_Writer & writer,
                                const t_Enumeration  entry,
                                HUMOTO_CONFIG_IS_BASE_OF_DISABLER_TYPE(
                                        humoto::config::CommonConfigurableBase, t_Enumeration) *dummy_base = NULL,
                                HUMOTO_CONFIG_IS_ENUM_ENABLER_TYPE(t_Enumeration) *dummy_enum = NULL)
            {
                int tmp_value = entry;
                writer.writeElement(tmp_value);
            }


            template <  class t_Writer>
                void writeBody( t_Writer & writer,
                                const humoto::config::CommonConfigurableBase & entry)
            {
                writer.startMap(entry.getNumberOfEntries());
                entry.writeConfigEntries(writer);
                writer.endMap();
            }


            template <  class t_Writer,
                        typename t_Entry>
                void writeBody( t_Writer & writer,
                                const t_Entry & entry,
                                HUMOTO_CONFIG_IS_BASE_OF_DISABLER_TYPE(
                                        humoto::config::CommonConfigurableBase, t_Entry) *dummy_base = NULL,
                                HUMOTO_CONFIG_IS_ENUM_DISABLER_TYPE(t_Entry) *dummy_enum = NULL)
            {
                writer.writeElement(entry);
            }



            template <  class t_Writer,
                        typename t_Entry>
                void writeEntry( t_Writer & writer,
                                const t_Entry & entry,
                                const std::string  & entry_name)
            {
                writer.descend(entry_name);
                writeBody(writer, entry);
                writer.ascend();
            }
        }
    }
}

#include "eigen_impl.h"
#include "vector_impl.h"
