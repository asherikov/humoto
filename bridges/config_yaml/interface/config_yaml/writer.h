/**
    @file
    @author Jan Michalczyk
    @author Alexander Sherikov
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once

namespace humoto
{
    namespace config
    {
        namespace yaml
        {
            /**
             * @brief Configuration writer class
             */
            class HUMOTO_LOCAL WriterBase
            {
                protected:
                    /// output file stream
                    std::ofstream   config_ofs_;

                    /// instance of YAML emitter, is destroyed and reinitialized by flush()
                    YAML::Emitter   *emitter_;


                protected:
                    WriterBase(){}
                    ~WriterBase(){}


                    /**
                     * @brief Initialize emitter
                     */
                    void initEmitter()
                    {
                        emitter_ = new YAML::Emitter;
                        emitter_->SetDoublePrecision(std::numeric_limits<double>::digits10);
                        if (config_ofs_.tellp() != 0)
                        {
                            *emitter_ << YAML::Newline;
                        }
                        *emitter_ << YAML::BeginMap;
                    }


                    /**
                     * @brief Destroy emitter
                     */
                    void destroyEmitter()
                    {
                        *emitter_ << YAML::EndMap;
                        config_ofs_ << emitter_->c_str();
                        delete emitter_;
                    }



                    void startArray(const std::string &name, const std::size_t size)
                    {
                        *emitter_ << YAML::Key << name;
                        *emitter_ << YAML::Value << YAML::Flow;
                        *emitter_ << YAML::BeginSeq;
                    }

                    void endArray()
                    {
                        *emitter_ << YAML::EndSeq;
                    }


                    template<class t_Element>
                        void writeArrayElement(t_Element & element)
                    {
                        *emitter_ << element;
                    }


                public:
                    /**
                     * @brief Starts a nested map in the configuration file
                     *
                     * @param[in] map_name name of the map
                     * @param[in] num_entries number of child entries
                     */
                    void descend(const std::string &map_name, const std::size_t num_entries)
                    {
                        *emitter_ << YAML::Key << map_name;
                        *emitter_ << YAML::Value;
                        *emitter_ << YAML::BeginMap;
                    }


                    /**
                     * @brief Starts a nested map in the configuration file
                     */
                    void initRoot()
                    {
                    }


                    /**
                     * @brief Ends a nested map in the configuration file
                     */
                    void ascend()
                    {
                        *emitter_ << YAML::EndMap;
                    }



                    /**
                     * @brief Write a configuration entry (scalar template)
                     *
                     * @tparam t_EntryType type of the entry
                     *
                     * @param[in] entry_name name
                     * @param[in] entry      data
                     */
                    template <typename t_EntryType>
                        void writeScalar(   const t_EntryType  entry,
                                            const std::string  & entry_name)
                    {
                        *emitter_ << YAML::Key << entry_name << YAML::Value << entry;
                    }



                    /**
                     * @brief Flush the configuration to the file
                     */
                    void flush()
                    {
                        destroyEmitter();
                        initEmitter();
                    }
            };



            class HUMOTO_LOCAL Writer : public WriterMixin<WriterBase>
            {
                public:
                    explicit Writer(const std::string& file_name)
                    {
                        config_ofs_.open(file_name.c_str());

                        if (!config_ofs_.good())
                        {
                            HUMOTO_THROW_MSG(std::string("Could not open configuration file for writing: ") +  file_name.c_str());
                        }

                        initEmitter();
                    }

                    ~Writer()
                    {
                        delete emitter_;
                    }
            };
        }
    }
}
