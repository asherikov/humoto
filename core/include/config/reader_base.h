/**
    @file
    @author  Alexander Sherikov

    @brief
*/

#pragma once


namespace humoto
{
    namespace config
    {
        template <class t_RawNode>
            class Node
        {
            public:
                const t_RawNode     *node_;
                std::size_t         index_;
                const std::size_t   size_;

            public:
                Node(const t_RawNode *node) : node_(node), size_(0)
                {
                    index_ = 0;
                }

                Node(const std::size_t index, std::size_t size) : index_(index), size_(size)
                {
                    node_ = NULL;
                }

                bool isArray() const
                {
                    return(NULL == node_);
                }
        };


        class ReaderBase
        {
            protected:
                /**
                 * @brief open configuration file
                 *
                 * @param[out] config_ifs_
                 * @param[in] file_name
                 */
                void openFile(std::ifstream &config_ifs_, const std::string& file_name)
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
                }
        };
    }
}
