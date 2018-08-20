/**
    @file
    @author  Alexander Sherikov
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief Inclusion of this file results in generation of functions which
    read and write entries 'HUMOTO_CONFIG_ENTRIES' defined in the including
    header from / to a configuration file.
*/

#ifndef HUMOTO_DOXYGEN_PROCESSING

#ifdef HUMOTO_USE_CONFIG
// Generic stuff
    protected:
        // Define read and write methods
        #ifdef HUMOTO_CONFIG_ENTRIES
            #define HUMOTO_CONFIG_COMPOUND_(entry)       HUMOTO_CONFIG_WRITE_COMPOUND_(entry)
            #define HUMOTO_CONFIG_COMPOUND(entry)        HUMOTO_CONFIG_WRITE_COMPOUND(entry)

            #define HUMOTO_CONFIG_SCALAR_(entry)       HUMOTO_CONFIG_WRITE_SCALAR_(entry)
            #define HUMOTO_CONFIG_SCALAR(entry)        HUMOTO_CONFIG_WRITE_SCALAR(entry)

            #define HUMOTO_CONFIG_ENUM_(entry)         HUMOTO_CONFIG_WRITE_ENUM_(entry)
            #define HUMOTO_CONFIG_ENUM(entry)          HUMOTO_CONFIG_WRITE_ENUM(entry)

            #define HUMOTO_CONFIG_PARENT_CLASS(entry)          HUMOTO_CONFIG_WRITE_PARENT_CLASS(entry)
            #define HUMOTO_CONFIG_MEMBER_CLASS(entry, name)    HUMOTO_CONFIG_WRITE_MEMBER_CLASS(entry, name)

            template <class t_Writer>
                void writeConfigEntriesTemplate(t_Writer & writer) const
            {
                HUMOTO_MACRO_SUBSTITUTE(HUMOTO_CONFIG_ENTRIES)
            }

            #undef HUMOTO_CONFIG_COMPOUND_
            #undef HUMOTO_CONFIG_COMPOUND

            #undef HUMOTO_CONFIG_SCALAR_
            #undef HUMOTO_CONFIG_SCALAR

            #undef HUMOTO_CONFIG_ENUM_
            #undef HUMOTO_CONFIG_ENUM

            #undef HUMOTO_CONFIG_PARENT_CLASS
            #undef HUMOTO_CONFIG_MEMBER_CLASS



            #define HUMOTO_CONFIG_COMPOUND_(entry)       HUMOTO_CONFIG_READ_COMPOUND_(entry)
            #define HUMOTO_CONFIG_COMPOUND(entry)        HUMOTO_CONFIG_READ_COMPOUND(entry)

            #define HUMOTO_CONFIG_SCALAR_(entry)       HUMOTO_CONFIG_READ_SCALAR_(entry)
            #define HUMOTO_CONFIG_SCALAR(entry)        HUMOTO_CONFIG_READ_SCALAR(entry)

            #define HUMOTO_CONFIG_ENUM_(entry)         HUMOTO_CONFIG_READ_ENUM_(entry)
            #define HUMOTO_CONFIG_ENUM(entry)          HUMOTO_CONFIG_READ_ENUM(entry)

            #define HUMOTO_CONFIG_PARENT_CLASS(entry)          HUMOTO_CONFIG_READ_PARENT_CLASS(entry)
            #define HUMOTO_CONFIG_MEMBER_CLASS(entry, name)    HUMOTO_CONFIG_READ_MEMBER_CLASS(entry, name)

            template <class t_Reader>
                void readConfigEntriesTemplate( t_Reader & reader,
                                        const bool crash_on_missing_entry = false)
            {
                HUMOTO_MACRO_SUBSTITUTE(HUMOTO_CONFIG_ENTRIES)
                finalize();
            }

            #undef HUMOTO_CONFIG_COMPOUND_
            #undef HUMOTO_CONFIG_COMPOUND

            #undef HUMOTO_CONFIG_SCALAR_
            #undef HUMOTO_CONFIG_SCALAR

            #undef HUMOTO_CONFIG_ENUM_
            #undef HUMOTO_CONFIG_ENUM

            #undef HUMOTO_CONFIG_PARENT_CLASS
            #undef HUMOTO_CONFIG_MEMBER_CLASS
        #endif


    protected:
        // Define node name
        #ifdef HUMOTO_CONFIG_SECTION_ID
            const std::string & getConfigSectionID() const
            {
                static const std::string name(HUMOTO_CONFIG_SECTION_ID);
                return (name);
            }
        #endif


        // Count number of entries and define a function, which returns it.
        #ifdef HUMOTO_CONFIG_ENTRIES
            #define HUMOTO_CONFIG_COMPOUND_(entry)       +1
            #define HUMOTO_CONFIG_COMPOUND(entry)        +1

            #define HUMOTO_CONFIG_SCALAR_(entry)       +1
            #define HUMOTO_CONFIG_SCALAR(entry)        +1

            #define HUMOTO_CONFIG_ENUM_(entry)         +1
            #define HUMOTO_CONFIG_ENUM(entry)          +1

            #define HUMOTO_CONFIG_PARENT_CLASS(entry)          +entry::getNumberOfEntries()
            #define HUMOTO_CONFIG_MEMBER_CLASS(entry, name)    +1

            std::size_t getNumberOfEntries() const
            {
                static const std::size_t    num_entries = (0 HUMOTO_MACRO_SUBSTITUTE(HUMOTO_CONFIG_ENTRIES) );
                return(num_entries);
            }

            #undef HUMOTO_CONFIG_COMPOUND_
            #undef HUMOTO_CONFIG_COMPOUND

            #undef HUMOTO_CONFIG_SCALAR_
            #undef HUMOTO_CONFIG_SCALAR

            #undef HUMOTO_CONFIG_ENUM_
            #undef HUMOTO_CONFIG_ENUM

            #undef HUMOTO_CONFIG_PARENT_CLASS
            #undef HUMOTO_CONFIG_MEMBER_CLASS
        #endif


    public:
        // Define constructors if requested
        #ifdef HUMOTO_CONFIG_CONSTRUCTOR
            /**
             * Define constructors for the given class.
             */
            template <class t_Reader>
                explicit HUMOTO_CONFIG_CONSTRUCTOR(
                        t_Reader &reader,
                        const std::string &node_name,
                        const bool crash_on_missing_entry = true)
            {
                readConfig(reader, node_name, crash_on_missing_entry);
            }

            template <class t_Reader>
                explicit HUMOTO_CONFIG_CONSTRUCTOR(
                        t_Reader &reader,
                        const bool crash_on_missing_entry = true)
            {
                readConfig(reader, crash_on_missing_entry);
            }
        #endif


            /**
             * @brief Read configuration (assuming the configuration node
             * to be in the root).
             *
             * @param[in] reader configuration reader
             * @param[in] crash_on_missing_entry
             */
            template <class t_Reader>
                void readConfig(t_Reader            & reader,
                                const bool          crash_on_missing_entry = true)
            {
                reader.readNestedConfig(*this, this->getConfigSectionID(), crash_on_missing_entry);
            }


            /**
             * @brief Read configuration (assuming the configuration node
             * to be in the root).
             *
             * @param[in] reader configuration reader
             * @param[in] crash_on_missing_entry
             * @param[in] node_name   node name, the default is used if empty
             */
            template <class t_Reader>
                void readConfig(t_Reader            & reader,
                                const std::string   & node_name,
                                const bool          crash_on_missing_entry = true)
            {
                reader.readNestedConfig(*this, node_name, crash_on_missing_entry);
            }


            /**
             * @brief Read configuration (assuming the configuration node
             * to be in the root).
             *
             * @param[in] reader configuration reader
             * @param[in] crash_on_missing_entry
             * @param[in] node_name   node name, the default is used if empty
             *
             * @note Intercept implicit conversion of a pointer to bool.
             */
            template <class t_Reader>
                void readConfig(t_Reader            & reader,
                                const char          * node_name,
                                const bool          crash_on_missing_entry = true)
            {
                reader.readNestedConfig(*this, node_name, crash_on_missing_entry);
            }


            /**
             * @brief Read configuration (assuming the configuration node
             * to be in the root).
             *
             * @param[in] file_name file name
             * @param[in] crash_on_missing_entry
             */
            template <class t_Reader>
                void readConfig(const std::string &file_name,
                                const bool        crash_on_missing_entry = true)
            {
                t_Reader reader(file_name);
                reader.readNestedConfig(*this, this->getConfigSectionID(), crash_on_missing_entry);
            }


            /**
             * @brief Read configuration (assuming the configuration node
             * to be in the root).
             *
             * @param[in] file_name file name
             * @param[in] node_name   node name, the default is used if empty
             * @param[in] crash_on_missing_entry
             */
            template <class t_Reader>
                void readConfig(const std::string &file_name,
                                const std::string &node_name,
                                const bool        crash_on_missing_entry = true)
            {
                t_Reader reader(file_name);
                reader.readNestedConfig(*this, node_name, crash_on_missing_entry);
            }


            /**
             * @brief Read configuration (assuming the configuration node
             * to be in the root).
             *
             * @param[in] file_name file name
             * @param[in] crash_on_missing_entry
             * @param[in] node_name   node name, the default is used if empty
             *
             * @note Intercept implicit conversion of a pointer to bool.
             */
            template <class t_Reader>
                void readConfig(const std::string &file_name,
                                const char        *node_name,
                                const bool        crash_on_missing_entry = true)
            {
                t_Reader reader(file_name);
                reader.readNestedConfig(*this, node_name, crash_on_missing_entry);
            }



// Format-specific stuff
    #define HUMOTO_CONFIG_NAMESPACE_WRAPPER(config_namespace) \
        public: \
            virtual void writeConfigEntries(humoto::config::config_namespace::Writer & writer) const \
            { \
                writeConfigEntriesTemplate(writer); \
            } \
            virtual void readConfigEntries( humoto::config::ReaderMixin<humoto::config::config_namespace::ReaderBase> & reader, \
                                            const bool crash_flag)\
            {\
                readConfigEntriesTemplate(reader, crash_flag);\
            }

    HUMOTO_MACRO_SUBSTITUTE(HUMOTO_CONFIG_NAMESPACE_LIST)

    #undef HUMOTO_CONFIG_NAMESPACE_WRAPPER

#endif //HUMOTO_USE_CONFIG

#undef HUMOTO_CONFIG_SECTION_ID
#undef HUMOTO_CONFIG_CONSTRUCTOR
#undef HUMOTO_CONFIG_ENTRIES
#endif
