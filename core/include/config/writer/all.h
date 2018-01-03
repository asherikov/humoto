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



            #define HUMOTO_CONFIG_BASIC_TYPE(type) \
                    template <class t_Writer> \
                        void writeBody( t_Writer &  writer,\
                                        const type & entry) \
                    {\
                        writer.writeElement(entry);\
                    }

            HUMOTO_MACRO_SUBSTITUTE(HUMOTO_CONFIG_BASIC_TYPES_LIST)

            #undef HUMOTO_CONFIG_BASIC_TYPE



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
