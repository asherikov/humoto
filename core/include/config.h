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
    /**
     * @brief Namespace of classes related to configuration handling
     */
    namespace config
    {
    }
}

#define HUMOTO_CONFIG_DEFINE_ACCESSORS  "humoto/config/define_accessors.h"


#ifdef HUMOTO_CONFIG_NAMESPACE_0
#   define HUMOTO_USE_CONFIG

#   ifndef HUMOTO_CONFIG_NAMESPACE_1
#       define HUMOTO_CONFIG_NAMESPACE_LIST \
                    HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_0)
#   else
#       ifndef HUMOTO_CONFIG_NAMESPACE_2
#           define HUMOTO_CONFIG_NAMESPACE_LIST \
                        HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_0) \
                        HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_1)
#       else
#           ifndef HUMOTO_CONFIG_NAMESPACE_3
#               define HUMOTO_CONFIG_NAMESPACE_LIST \
                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_0) \
                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_1) \
                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_2)
#           else
#               ifndef HUMOTO_CONFIG_NAMESPACE_4
#                   define HUMOTO_CONFIG_NAMESPACE_LIST \
                                HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_0) \
                                HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_1) \
                                HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_2) \
                                HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_3)
#               else
#                   ifndef HUMOTO_CONFIG_NAMESPACE_5
#                       define HUMOTO_CONFIG_NAMESPACE_LIST \
                                    HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_0) \
                                    HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_1) \
                                    HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_2) \
                                    HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_3) \
                                    HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_4)
#                   else
#                       ifndef HUMOTO_CONFIG_NAMESPACE_6
#                           define HUMOTO_CONFIG_NAMESPACE_LIST \
                                        HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_0) \
                                        HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_1) \
                                        HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_2) \
                                        HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_3) \
                                        HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_4) \
                                        HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_5)
#                       else
#                           ifndef HUMOTO_CONFIG_NAMESPACE_7
#                               define HUMOTO_CONFIG_NAMESPACE_LIST \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_0) \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_1) \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_2) \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_3) \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_4) \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_5) \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_6)
#                           else
#                               define HUMOTO_CONFIG_NAMESPACE_LIST \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_0) \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_1) \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_2) \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_3) \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_4) \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_5) \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_6) \
                                            HUMOTO_CONFIG_NAMESPACE_WRAPPER(HUMOTO_CONFIG_NAMESPACE_7)
#                           endif
#                       endif
#                   endif
#               endif
#           endif
#       endif
#   endif



    #define HUMOTO_CONFIG_WRITE_PARENT_CLASS(parent_class)  parent_class::writeConfigEntries(writer);
    #define HUMOTO_CONFIG_WRITE_MEMBER_CLASS(member, name)  member.writeNestedConfig(writer, name);

    #define HUMOTO_CONFIG_WRITE_COMPOUND_(entry)    writer.writeCompound(entry##_, #entry);
    #define HUMOTO_CONFIG_WRITE_COMPOUND(entry)     writer.writeCompound(entry, #entry);

    #define HUMOTO_CONFIG_WRITE_SCALAR_(entry)  writer.writeScalar(entry##_, #entry);
    #define HUMOTO_CONFIG_WRITE_SCALAR(entry)   writer.writeScalar(entry, #entry);

    #define HUMOTO_CONFIG_WRITE_ENUM_(entry)    writer.writeEnum(entry##_, #entry);
    #define HUMOTO_CONFIG_WRITE_ENUM(entry)     writer.writeEnum(entry, #entry);


    #define HUMOTO_CONFIG_READ_PARENT_CLASS(parent_class)  parent_class::readConfigEntries(reader, crash_on_missing_entry);
    #define HUMOTO_CONFIG_READ_MEMBER_CLASS(member, name)  reader.readNestedConfig(member, name, crash_on_missing_entry);

    #define HUMOTO_CONFIG_READ_COMPOUND_(entry)     reader.readEntry(entry##_, #entry, crash_on_missing_entry);
    #define HUMOTO_CONFIG_READ_COMPOUND(entry)      reader.readEntry(entry, #entry, crash_on_missing_entry);

    #define HUMOTO_CONFIG_READ_SCALAR_(entry)   reader.readEntry(entry##_, #entry, crash_on_missing_entry);
    #define HUMOTO_CONFIG_READ_SCALAR(entry)    reader.readEntry(entry, #entry, crash_on_missing_entry);

    #define HUMOTO_CONFIG_READ_ENUM_(entry)     reader.readEnum(entry##_, #entry, crash_on_missing_entry);
    #define HUMOTO_CONFIG_READ_ENUM(entry)      reader.readEnum(entry, #entry, crash_on_missing_entry);

    // ----------------------------

    namespace humoto
    {
        namespace config
        {
            /**
             * @brief Configurable base class.
             */
            class HUMOTO_LOCAL CommonConfigurableBase
            {
                protected:
                    /**
                     * @brief Protected destructor: prevent destruction of the child
                     * classes through a base pointer.
                     */
                    ~CommonConfigurableBase() {}
                    CommonConfigurableBase() {}


                    /**
                     * @brief Return the default name of a configuration node
                     * corresponding to this class
                     *
                     * @return the name
                     *
                     * @attention Implementation of this method is added
                     * automatically upon inclusion of define_accessors.h if
                     * HUMOTO_CONFIG_SECTION_ID is defined.
                     */
                    virtual const std::string & getConfigSectionID() const = 0;


                    /**
                     * @brief Get number of entries in the corresponding
                     * configuration node.
                     *
                     * @return number of entries
                     */
                    virtual std::size_t getNumberOfEntries() const = 0;


                    /// @{
                    /**
                     * These functions are always defined automatically.
                     */
                    #define HUMOTO_CONFIG_NAMESPACE_WRAPPER(config_namespace) \
                        virtual void writeConfigEntries(humoto::config::config_namespace::Writer &) const = 0;
                    HUMOTO_MACRO_SUBSTITUTE(HUMOTO_CONFIG_NAMESPACE_LIST)
                    #undef HUMOTO_CONFIG_NAMESPACE_WRAPPER
                    /// @}

                    virtual bool getCrashOnMissingEntryFlag() = 0;


                public:
                    /**
                     * @brief This function is called automaticaly after reading
                     * a configuration file. Does nothing by default.
                     */
                    virtual void finalize() {};


                    /**
                     * @brief Set members to their default values.
                     */
                    virtual void setDefaults() = 0;


                    // ------------------------------------------


                    /**
                     * @brief Write nested configuration node
                     *
                     * @param[in,out] writer configuration writer
                     */
                    template <class t_Writer>
                        void writeNestedConfig( t_Writer& writer) const
                    {
                        writeNestedConfig(writer, getConfigSectionID());
                    }


                    /**
                     * @brief Write nested configuration node
                     *
                     * @param[in,out] writer configuration writer
                     * @param[in] node_name   node name, the default is used if empty
                     */
                    template <class t_Writer>
                        void writeNestedConfig( t_Writer& writer,
                                                const std::string &node_name) const
                    {
                        writer.descend(node_name, getNumberOfEntries());
                        writeConfigEntries(writer);
                        writer.ascend();
                    }


                    /**
                     * @brief Write configuration
                     *
                     * @param[in,out] writer configuration writer
                     */
                    template <class t_Writer>
                        void writeConfig(t_Writer& writer) const
                    {
                        writeConfig(writer, getConfigSectionID());
                    }


                    /**
                     * @brief Write configuration
                     *
                     * @param[in,out] writer configuration writer
                     * @param[in] node_name   node name, the default is used if empty
                     */
                    template <class t_Writer>
                        void writeConfig(t_Writer& writer,
                                         const std::string &node_name) const
                    {
                        writer.initRoot();
                        writeNestedConfig(writer, node_name);
                        writer.flush();
                    }


                    /**
                     * @brief Write configuration.
                     *
                     * @param[in] file_name file name
                     */
                    template <class t_Writer>
                        void writeConfig(const std::string &file_name) const
                    {
                        t_Writer writer(file_name);
                        writeConfig(writer);
                    }


                    /**
                     * @brief Write configuration.
                     *
                     * @param[in] file_name file name
                     * @param[in] node_name   node name, the default is used if empty
                     */
                    template <class t_Writer>
                        void writeConfig(const std::string &file_name,
                                         const std::string &node_name) const
                    {
                        t_Writer writer(file_name);
                        writeConfig(writer, node_name);
                    }
            };



            /// Default configurable base is strict
            class HUMOTO_LOCAL ConfigurableBase : public humoto::config::CommonConfigurableBase
            {
                protected:
                    bool getCrashOnMissingEntryFlag() {return (true);}


                protected:
                    /**
                     * @brief Protected destructor: prevent destruction of the child
                     * classes through a base pointer.
                     */
                    ~ConfigurableBase() {}
                    ConfigurableBase() {}
            };


            class HUMOTO_LOCAL StrictConfigurableBase : public humoto::config::CommonConfigurableBase
            {
                protected:
                    bool getCrashOnMissingEntryFlag() {return (true);}
                    static const bool default_crash_on_missing_entry_ = true;


                protected:
                    /**
                     * @brief Protected destructor: prevent destruction of the child
                     * classes through a base pointer.
                     */
                    ~StrictConfigurableBase() {}
                    StrictConfigurableBase() {}
            };


            class HUMOTO_LOCAL RelaxedConfigurableBase : public humoto::config::CommonConfigurableBase
            {
                protected:
                    bool getCrashOnMissingEntryFlag() {return (false);}


                protected:
                    /**
                     * @brief Protected destructor: prevent destruction of the child
                     * classes through a base pointer.
                     */
                    ~RelaxedConfigurableBase() {}
                    RelaxedConfigurableBase() {}
            };
        }
    }

#else

    namespace humoto
    {
        namespace config
        {
            // Some classes may inherit from this
            class HUMOTO_LOCAL ConfigurableBase
            {
                protected:
                    /**
                     * @brief Protected destructor: prevent destruction of the
                     * child classes through a base pointer.
                     */
                    ~ConfigurableBase() {}
            };


            // Some classes may inherit from this
            class HUMOTO_LOCAL StrictConfigurableBase
            {
                protected:
                    /**
                     * @brief Protected destructor: prevent destruction of the
                     * child classes through a base pointer.
                     */
                    ~StrictConfigurableBase() {}
            };


            // Some classes may inherit from this
            class HUMOTO_LOCAL RelaxedConfigurableBase
            {
                protected:
                    /**
                     * @brief Protected destructor: prevent destruction of the
                     * child classes through a base pointer.
                     */
                    ~RelaxedConfigurableBase() {}
            };
        }
    }
#endif //HUMOTO_CONFIG_NAMESPACE_0


#define HUMOTO_CONFIG_INCLUDED
