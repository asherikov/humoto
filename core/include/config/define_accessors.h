/**
    @file
    @author  Alexander Sherikov
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief Inclusion of this file results in generation of functions which
    read and write entries 'HUMOTO_CONFIG_ENTRIES' defined in the including
    header from / to a configuration file.
*/


#ifdef HUMOTO_USE_CONFIG

    #ifndef HUMOTO_DOXYGEN_PROCESSING
    protected:
        // Define read and write methods
        #ifdef HUMOTO_CONFIG_ENTRIES

            #define HUMOTO_CONFIG_ENTRY_(entry)     HUMOTO_CONFIG_WRITE_ENTRY_(entry)
            #define HUMOTO_CONFIG_ENTRY(entry)      HUMOTO_CONFIG_WRITE_ENTRY(entry)

            #define HUMOTO_CONFIG_PARENT_CLASS(entry)           HUMOTO_CONFIG_WRITE_PARENT_CLASS(entry)
            #define HUMOTO_CONFIG_MEMBER_CLASS(entry, name)     HUMOTO_CONFIG_WRITE_MEMBER_CLASS(entry, name)

            template <class t_Writer>
                void writeConfigEntriesTemplate(t_Writer & writer) const
            {
                HUMOTO_MACRO_SUBSTITUTE(HUMOTO_CONFIG_ENTRIES)
            }

            #undef HUMOTO_CONFIG_ENTRY_
            #undef HUMOTO_CONFIG_ENTRY

            #undef HUMOTO_CONFIG_PARENT_CLASS
            #undef HUMOTO_CONFIG_MEMBER_CLASS


            #define HUMOTO_CONFIG_ENTRY_(entry)     HUMOTO_CONFIG_READ_ENTRY_(entry)
            #define HUMOTO_CONFIG_ENTRY(entry)      HUMOTO_CONFIG_READ_ENTRY(entry)

            #define HUMOTO_CONFIG_PARENT_CLASS(entry)           HUMOTO_CONFIG_READ_PARENT_CLASS(entry)
            #define HUMOTO_CONFIG_MEMBER_CLASS(entry, name)     HUMOTO_CONFIG_READ_MEMBER_CLASS(entry, name)

            template <class t_Reader>
                void readConfigEntriesTemplate( t_Reader & reader,
                                                const bool crash_on_missing_entry = false)
            {
                HUMOTO_MACRO_SUBSTITUTE(HUMOTO_CONFIG_ENTRIES)
                finalize();
            }

            #undef HUMOTO_CONFIG_ENTRY_
            #undef HUMOTO_CONFIG_ENTRY

            #undef HUMOTO_CONFIG_PARENT_CLASS
            #undef HUMOTO_CONFIG_MEMBER_CLASS

        #endif


        // Count number of entries and define a function, which returns it.
        #ifdef HUMOTO_CONFIG_ENTRIES

            #define HUMOTO_CONFIG_ENTRY_(entry)         +1
            #define HUMOTO_CONFIG_ENTRY(entry)          +1

            #define HUMOTO_CONFIG_PARENT_CLASS(entry)          +entry::getNumberOfEntries()
            #define HUMOTO_CONFIG_MEMBER_CLASS(entry, name)    +1

            std::size_t getNumberOfEntries() const
            {
                static const std::size_t    num_entries = (0 HUMOTO_MACRO_SUBSTITUTE(HUMOTO_CONFIG_ENTRIES) );
                return(num_entries);
            }

            #undef HUMOTO_CONFIG_ENTRY_
            #undef HUMOTO_CONFIG_ENTRY

            #undef HUMOTO_CONFIG_PARENT_CLASS
            #undef HUMOTO_CONFIG_MEMBER_CLASS

        #endif
    #endif


    public:
        // Define constructors if requested
        #ifdef HUMOTO_CONFIG_CONSTRUCTOR
            /**
             * Define constructors for the given class.
             */
            template <class t_Reader>
                HUMOTO_CONFIG_CONSTRUCTOR(
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


        // Define node name
        #ifdef HUMOTO_CONFIG_SECTION_ID
            const std::string & getConfigSectionID() const
            {
                static const std::string name(HUMOTO_CONFIG_SECTION_ID);
                return (name);
            }
        #endif


        // Format-specific stuff
        #define HUMOTO_CONFIG_NAMESPACE(config_namespace) \
            virtual void writeConfigEntries(humoto::config::config_namespace::Writer & writer) const \
            { \
                writeConfigEntriesTemplate(writer); \
            } \
            virtual void readConfigEntries( humoto::config::config_namespace::Reader & reader, \
                                            const bool crash_flag) \
            {\
                readConfigEntriesTemplate(reader, crash_flag);\
            }

            HUMOTO_MACRO_SUBSTITUTE(HUMOTO_CONFIG_NAMESPACE_LIST)

        #undef HUMOTO_CONFIG_NAMESPACE


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
            humoto::config::reader::readEntry(reader, *this, this->getConfigSectionID(), crash_on_missing_entry);
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
            humoto::config::reader::readEntry(reader, *this, node_name, crash_on_missing_entry);
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
            humoto::config::reader::readEntry(reader, *this, node_name, crash_on_missing_entry);
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
            humoto::config::reader::readEntry(reader, *this, this->getConfigSectionID(), crash_on_missing_entry);
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
            humoto::config::reader::readEntry(reader, *this, node_name, crash_on_missing_entry);
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
            humoto::config::reader::readEntry(reader, *this, node_name, crash_on_missing_entry);
        }


        // ============================================


        /**
         * @brief Write configuration
         *
         * @param[in,out] writer configuration writer
         */
        template <class t_Writer>
            void writeConfig(t_Writer& writer) const
        {
            writeConfig(writer, this->getConfigSectionID());
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
            humoto::config::writer::writeEntry(writer, *this, node_name);
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

#endif //HUMOTO_USE_CONFIG

#undef HUMOTO_CONFIG_SECTION_ID
#undef HUMOTO_CONFIG_CONSTRUCTOR
#undef HUMOTO_CONFIG_ENTRIES
