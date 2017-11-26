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
             * @brief Configuration reader class
             */
            class HUMOTO_LOCAL ReaderBase
            {
                protected:
                    /// input file stream
                    std::ifstream config_ifs_;

                    /// instance of YAML parser
                    YAML::Parser  parser_;

                    /// root node
                    YAML::Node    root_node_;

                    /// Stack of nodes.
                    std::stack<const YAML::Node *>    node_stack_;


                protected:
                    ReaderBase() {};
                    ~ReaderBase() {};


                    /**
                     * @brief open configuration file
                     *
                     * @param[in] file_name
                     */
                    void openFile(const std::string& file_name)
                    {
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

                        parser_.Load(config_ifs_),
                        parser_.GetNextDocument(root_node_);
                        node_stack_.push(&root_node_);
                    }


                    /**
                     * @brief Get current node
                     *
                     * @return pointer to the current node
                     */
                    const YAML::Node * getCurrentNode()
                    {
                        return(node_stack_.top());
                    }


                    bool isArray()
                    {
                        return(YAML::NodeType::Sequence == getCurrentNode()->Type());
                    }

                    std::size_t getArraySize()
                    {
                        return(getCurrentNode()->size());
                    }

                    template<class t_ElementType>
                        void readArrayElement(t_ElementType &element, const std::size_t index)
                    {
                        (*getCurrentNode())[index] >> element;
                    }


                    template<class t_ElementType>
                        void readElement(t_ElementType &element)
                    {
                        *getCurrentNode() >> element;
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
                        const YAML::Node * child = getCurrentNode()->FindValue(child_name);

                        if (child == NULL)
                        {
                            return(false);
                        }
                        else
                        {
                            node_stack_.push(child);
                            return(true);
                        }
                    }


                    /**
                     * @brief Ascend from the current entry to its parent.
                     */
                    void ascend()
                    {
                        node_stack_.pop();
                    }
            };


            class Reader : public ReaderMixin<ReaderBase>
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
