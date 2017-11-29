/**
    @file
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
        namespace msgpack
        {
            /**
             * @brief Configuration reader class
             */
            class HUMOTO_LOCAL ReaderBase
            {
                protected:
                    enum Status
                    {
                        UNDEFINED = 0,
                        IN_ARRAY = 1
                    };


                protected:
                    std::string     buffer_;

                    std::vector< boost::shared_ptr< ::msgpack::object_handle >  >           handles_;

                    /// Stack of nodes.
                    std::stack< const ::msgpack::object * >     node_stack_;

                    std::vector< std::size_t >             array_counters_;
                    Status      status_;


                protected:
                    ReaderBase() {};
                    ~ReaderBase() {};


                    std::size_t startArray()
                    {
                        status_ = IN_ARRAY;
                        array_counters_.push_back(0);

                        return(getArraySize(*getCurrentNode(), 0));
                    }

                    void endArray()
                    {
                        array_counters_.pop_back();
                        if (0 == array_counters_.size())
                        {
                            status_ = UNDEFINED;
                        }
                        else
                        {
                            ++array_counters_.back();
                        }
                    }


                    /**
                     * @brief open configuration file
                     *
                     * @param[in] file_name
                     */
                    void openFile(const std::string& file_name)
                    {
                        std::ifstream config_ifs_;

                        config_ifs_.open(file_name.c_str());
                        if (!config_ifs_.good())
                        {
                            std::string file_name_default = std::string(HUMOTO_DEFAULT_CONFIG_PREFIX) + file_name;
                            config_ifs_.open(file_name_default.c_str());
                        }
                        if (!config_ifs_.good())
                        {
                            HUMOTO_THROW_MSG(std::string("Could not open configuration file: ") +  file_name.c_str());
                        }


                        std::stringstream   str_stream;
                        str_stream << config_ifs_.rdbuf();
                        buffer_ = str_stream.str();


                        handles_.clear();
                        try
                        {
                            std::size_t     buffer_offset = 0;

                            while (buffer_offset != buffer_.size())
                            {
                                handles_.push_back(boost::shared_ptr< ::msgpack::object_handle >(new ::msgpack::object_handle));

                                unpack(*handles_[handles_.size()-1], buffer_.data(), buffer_.size(), buffer_offset);
                            }
                        }
                        catch(const std::exception &e)
                        {
                            HUMOTO_THROW_MSG(std::string("Failed to parse the configuration file: ") + e.what());
                        }
                    }


                    /**
                     * @brief Get current node
                     *
                     * @return pointer to the current node
                     */
                    const ::msgpack::object * getCurrentNode() const
                    {
                        return(node_stack_.top());
                    }


                    bool isArray() const
                    {
                        return(::msgpack::type::ARRAY == getCurrentNode()->type);
                    }


                    template<class t_Node>
                    std::size_t getArraySize(t_Node & node, const std::size_t depth)
                    {
                        if ( depth == array_counters_.size()-1 )
                        {
                            return(node.via.array.size);
                        }
                        else
                        {
                            return(getArraySize(node.via.array.ptr[ array_counters_[depth] ], depth + 1));
                        }
                    }


                    template<   class t_ElementType,
                                class t_NodeType>
                        void readArrayElement(  t_ElementType &element,
                                                t_NodeType &node,
                                                const std::size_t depth)
                    {
                        if ( depth == array_counters_.size()-1 )
                        {
                            node.via.array.ptr[ array_counters_[depth] ] >> element;
                            ++array_counters_[depth];
                        }
                        else
                        {
                            readArrayElement(element, node.via.array.ptr[ array_counters_[depth] ], depth + 1);
                        }
                    }


                    template<class t_ElementType>
                        void readElement(t_ElementType &element)
                    {
                        if (IN_ARRAY == status_)
                        {
                            readArrayElement(element, *getCurrentNode(), 0);
                            //node[ array_counters_[depth] ] >> element;
                            //getCurrentNode()->via.array.ptr[array_counters_.top()] >> element;
                            //++array_counters_.top();
                        }
                        else
                        {
                            *getCurrentNode() >> element;
                        }
                    }


                public:
                    /**
                     * @brief Descend to the entry with the given name
                     *
                     * @param[in] child_name child node name
                     *
                     * @return true if successful.
                     */
                    bool descend(const std::string & child_name)
                    {
                        if (node_stack_.size() == 0)
                        {
                            for (std::size_t i = 0; i < handles_.size(); ++i)
                            {
                                if (::msgpack::type::MAP == handles_[i]->get().type)
                                {
                                    if (child_name == handles_[i]->get().via.map.ptr[0].key.as<std::string>())
                                    {
                                        if (::msgpack::type::MAP == handles_[i]->get().via.map.ptr[0].val.type)
                                        {
                                            node_stack_.push( &(handles_[i]->get().via.map.ptr[0].val) );
                                            return(true);
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (::msgpack::type::MAP == getCurrentNode()->type)
                            {
                                for (std::size_t i = 0; i < getCurrentNode()->via.map.size; ++i)
                                {
                                    if (child_name == getCurrentNode()->via.map.ptr[i].key.as<std::string>())
                                    {
                                        node_stack_.push( &(getCurrentNode()->via.map.ptr[i].val) );
                                        return(true);
                                    }
                                }
                            }
                        }

                        return (false);
                    }


                    /**
                     * @brief Ascend from the current entry to its parent.
                     */
                    void ascend()
                    {
                        node_stack_.pop();
                    }
            };


            class HUMOTO_LOCAL Reader : public ReaderMixin<ReaderBase>
            {
                public:
                    /**
                     * @brief Constructor
                     *
                     * @param[in] file_name
                     */
                    explicit Reader(const std::string& file_name)
                    {
                        openFile(file_name);
                    }


                    /**
                     * @brief Default constructor
                     */
                    Reader()
                    {
                    }
            };
        }
    }
}
