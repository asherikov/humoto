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

#   include "config/process_namespaces.h"

    #define HUMOTO_CONFIG_ENTRY_(entry)     HUMOTO_CONFIG_NAMED_ENTRY(entry##_, #entry)
    #define HUMOTO_CONFIG_ENTRY(entry)      HUMOTO_CONFIG_NAMED_ENTRY(entry, #entry)

    #define HUMOTO_CONFIG_TYPED_ENTRY_(entry, type) HUMOTO_CONFIG_TYPED_NAMED_ENTRY(type, entry##_, #entry)
    #define HUMOTO_CONFIG_TYPED_ENTRY(entry, type)  HUMOTO_CONFIG_TYPED_NAMED_ENTRY(type, entry, #entry)


    #define HUMOTO_CONFIG_WRITE_NAMED_ENTRY(entry, name)    humoto::config::writer::writeEntry(writer, entry, name);

    #define HUMOTO_CONFIG_WRITE_ENTRY_(entry)   HUMOTO_CONFIG_WRITE_NAMED_ENTRY(entry##_, #entry)
    #define HUMOTO_CONFIG_WRITE_ENTRY(entry)    HUMOTO_CONFIG_WRITE_NAMED_ENTRY(entry, #entry)

    #define HUMOTO_CONFIG_WRITE_PARENT_CLASS(parent_class)  parent_class::writeConfigEntries(writer);


    #define HUMOTO_CONFIG_READ_NAMED_ENTRY(entry, name)  humoto::config::reader::readEntry(reader, entry, name, crash_on_missing_entry);

    #define HUMOTO_CONFIG_READ_ENTRY_(entry)    HUMOTO_CONFIG_READ_NAMED_ENTRY(entry##_, #entry);
    #define HUMOTO_CONFIG_READ_ENTRY(entry)     HUMOTO_CONFIG_READ_NAMED_ENTRY(entry, #entry);

    #define HUMOTO_CONFIG_READ_PARENT_CLASS(parent_class)  parent_class::readConfigEntries(reader, crash_on_missing_entry);

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


                    virtual bool getCrashOnMissingEntryFlag() = 0;


                public:
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
                     * @brief This function is called automaticaly after reading
                     * a configuration file. Does nothing by default.
                     */
                    virtual void finalize() {};


                    /**
                     * @brief Set members to their default values.
                     */
                    virtual void setDefaults() = 0;


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
                    #define HUMOTO_CONFIG_NAMESPACE(config_namespace) \
                        virtual void writeConfigEntries(humoto::config::config_namespace::Writer &) const = 0; \
                        virtual void readConfigEntries( humoto::config::config_namespace::Reader & reader, \
                                                        const bool crash_flag) = 0;
                    HUMOTO_MACRO_SUBSTITUTE(HUMOTO_CONFIG_NAMESPACE_LIST)
                    #undef HUMOTO_CONFIG_NAMESPACE
                    /// @}
            };


            class HUMOTO_LOCAL StrictConfigurableBase : public humoto::config::CommonConfigurableBase
            {
                protected:
                    bool getCrashOnMissingEntryFlag() {return (true);}


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



            /// Default configurable base is strict
            class HUMOTO_LOCAL ConfigurableBase : public humoto::config::StrictConfigurableBase
            {
                protected:
                    /**
                     * @brief Protected destructor: prevent destruction of the child
                     * classes through a base pointer.
                     */
                    ~ConfigurableBase() {}
                    ConfigurableBase() {}
            };
        }
    }

#   include "config/helpers.h"
#   include "config/reader/all.h"
#   include "config/writer/all.h"

#else

    namespace humoto
    {
        namespace config
        {
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


            // Some classes may inherit from this
            class HUMOTO_LOCAL ConfigurableBase : humoto::config::StrictConfigurableBase
            {
                protected:
                    /**
                     * @brief Protected destructor: prevent destruction of the
                     * child classes through a base pointer.
                     */
                    ~ConfigurableBase() {}
            };
        }
    }

#endif //HUMOTO_CONFIG_NAMESPACE_0


#define HUMOTO_CONFIG_INCLUDED
