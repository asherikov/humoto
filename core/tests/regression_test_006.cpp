/**
    @file
    @author  Alexander Sherikov
    @copyright 2017 Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/


// Enable YAML configuration files (must be first)
#include "humoto/config_yaml.h"
#include "humoto/config_msgpack.h"
// common & abstract classes
#include "humoto/config.h"

//testing
#include "gtest/gtest.h"


namespace
{
    const double g_tolerance = 1e-12;

    class TypeContainer : public humoto::config::ConfigurableBase
    {
        #define HUMOTO_CONFIG_SECTION_ID "TypeContainer"
        #define HUMOTO_CONFIG_CONSTRUCTOR TypeContainer
        #define HUMOTO_CONFIG_ENTRIES \
            HUMOTO_CONFIG_ENTRY_(integer) \
            HUMOTO_CONFIG_ENTRY_(real) \
            HUMOTO_CONFIG_ENTRY_(vector) \
            HUMOTO_CONFIG_ENTRY_(matrix) \
            HUMOTO_CONFIG_ENTRY_(std_vector) \
            HUMOTO_CONFIG_ENTRY_(std_nested_vector) \
            HUMOTO_CONFIG_ENTRY_(string) \
            HUMOTO_CONFIG_ENTRY_(std_vector_evector) \
            HUMOTO_CONFIG_ENTRY_(std_nested_vector_evector)
        #include HUMOTO_CONFIG_DEFINE_ACCESSORS


        public:
            int                     integer_;
            double                  real_;

            Eigen::Vector3d         vector_;
            Eigen::Matrix3d         matrix_;
            Eigen::MatrixXd         matrix_x_;

            std::vector<double>                 std_vector_;
            std::vector< std::vector<double> >  std_nested_vector_;

            std::string             string_;

            std::vector<Eigen::Vector3d>                std_vector_evector_;
            std::vector< std::vector<Eigen::Vector3d> > std_nested_vector_evector_;


        public:
            TypeContainer()
            {
                setDefaults();
            }


            void setDefaults()
            {
                integer_ = 10;
                real_ = 1.33;
                vector_.setConstant(3);
                matrix_ << 1, 2, 3, 4, 5, 6, 7, 8, 9;
                string_ = "blahblah";

                matrix_x_.resize(2, 3);
                matrix_x_ << 8, 7, 6, 3, 2, 1;

                std_vector_.resize(5);
                for(std::size_t i = 0; i < std_vector_.size(); ++i)
                {
                    std_vector_[i] = i * 5.22 + 2.3;
                }

                std_nested_vector_.resize(3);
                for(std::size_t i = 0; i < std_nested_vector_.size(); ++i)
                {
                    std_nested_vector_[i].resize(7-i);

                    for(std::size_t j = 0; j < std_nested_vector_[i].size(); ++j)
                    {
                        std_nested_vector_[i][j] = 5.2 + i*0.1 + j*0.3;
                    }
                }

                std_vector_evector_.resize(4);
                for(std::size_t i = 0; i < std_vector_evector_.size(); ++i)
                {
                    std_vector_evector_[i].setConstant(i*9 + 0.43);
                }

                std_nested_vector_evector_.resize(2);
                for(std::size_t i = 0; i < std_nested_vector_evector_.size(); ++i)
                {
                    std_nested_vector_evector_[i].resize(1+i);

                    for(std::size_t j = 0; j < std_nested_vector_evector_[i].size(); ++j)
                    {
                        std_nested_vector_evector_[i][j].setConstant(5.2 + i*0.1 + j*0.3);
                    }
                }
            }
    };


    class TypeContainerVector : public humoto::config::ConfigurableBase
    {
        #define HUMOTO_CONFIG_SECTION_ID "TypeContainerVector"
        #define HUMOTO_CONFIG_CONSTRUCTOR TypeContainerVector
        #define HUMOTO_CONFIG_ENTRIES \
            HUMOTO_CONFIG_ENTRY_(vector)
        #include HUMOTO_CONFIG_DEFINE_ACCESSORS

        public:
            std::vector<TypeContainer> vector_;


        public:
            TypeContainerVector()
            {
                setDefaults();
            }


            void setDefaults()
            {
                vector_.resize(3);

                vector_.resize(4);
                for(std::size_t i = 0; i < vector_.size(); ++i)
                {
                    vector_[i].integer_ = i;
                }
            }
    };
}



class ConfigInterfaceTest : public ::testing::Test
{
    protected:
        template<class t_Reader, class t_Writer>
            void test()
        {
            {
                TypeContainer type_container;

                t_Writer writer("type_container.cfg");
                type_container.writeConfig(writer);
            }

            {
                TypeContainer type_container;

                t_Reader reader("type_container.cfg");
                type_container.readConfig(reader);
            }

            // --------------------------------

            {
                TypeContainer type_container;
                type_container.writeConfig<t_Writer>("type_container2.cfg");
            }

            {
                TypeContainer type_container;
                type_container.readConfig<t_Reader>("type_container2.cfg");
            }
        }
};


class ConfigMatchTest : public ::testing::Test
{
    protected:
        void    compare(const TypeContainer &type_container_out,
                        const TypeContainer &type_container_in)
        {
            EXPECT_EQ(type_container_out.integer_,          type_container_in.integer_);
            EXPECT_EQ(type_container_out.real_,             type_container_in.real_);
            EXPECT_EQ(type_container_out.vector_,           type_container_in.vector_);
            EXPECT_EQ(type_container_out.matrix_,           type_container_in.matrix_);
            EXPECT_EQ(type_container_out.matrix_x_,         type_container_in.matrix_x_);
            EXPECT_EQ(type_container_out.string_,           type_container_in.string_);

            EXPECT_EQ(type_container_out.std_vector_.size(),                type_container_in.std_vector_.size());
            EXPECT_EQ(type_container_out.std_nested_vector_.size(),         type_container_in.std_nested_vector_.size());
            EXPECT_EQ(type_container_out.std_vector_evector_.size(),        type_container_in.std_vector_evector_.size());
            EXPECT_EQ(type_container_out.std_nested_vector_evector_.size(), type_container_in.std_nested_vector_evector_.size());

            for (std::size_t i = 0; i < type_container_out.std_vector_.size(); ++i)
            {
                EXPECT_NEAR(type_container_out.std_vector_[i],
                            type_container_in.std_vector_[i],
                            g_tolerance);
            }

            for (std::size_t i = 0; i < type_container_out.std_vector_evector_.size(); ++i)
            {
                EXPECT_EQ(  type_container_out.std_vector_evector_[i],
                            type_container_in.std_vector_evector_[i]);
            }


            for (std::size_t i = 0; i < type_container_out.std_nested_vector_.size(); ++i)
            {
                EXPECT_EQ(type_container_out.std_nested_vector_[i].size(),  type_container_in.std_nested_vector_[i].size());
                for (std::size_t j = 0; j < type_container_out.std_nested_vector_[i].size(); ++j)
                {
                    EXPECT_NEAR(type_container_out.std_nested_vector_[i][j],
                                type_container_in.std_nested_vector_[i][j],
                                g_tolerance);
                }
            }

            for (std::size_t i = 0; i < type_container_out.std_nested_vector_evector_.size(); ++i)
            {
                EXPECT_EQ(type_container_out.std_nested_vector_evector_[i].size(),  type_container_in.std_nested_vector_evector_[i].size());
                for (std::size_t j = 0; j < type_container_out.std_nested_vector_evector_[i].size(); ++j)
                {
                    EXPECT_EQ(  type_container_out.std_nested_vector_evector_[i][j],
                                type_container_in.std_nested_vector_evector_[i][j]);
                }
            }
        }


        template<class t_Reader, class t_Writer>
            void testSimple()
        {
            TypeContainer type_container_out;
            type_container_out.writeConfig<t_Writer>("type_container_config_match_simple.cfg");

            // -------

            TypeContainer type_container_in;
            type_container_in.readConfig<t_Reader>("type_container_config_match_simple.cfg");

            // -------

            compare(type_container_out, type_container_in);
        }


        template<class t_Reader, class t_Writer>
            void testMulti()
        {
            TypeContainer type_container_out1;
            TypeContainer type_container_out2;
            {
                t_Writer writer("type_container_config_match_multi.cfg");
                type_container_out1.writeConfig(writer, "node1");
                type_container_out2.writeConfig(writer, "node2");
            }

            // -------

            TypeContainer type_container_in1;
            TypeContainer type_container_in2;
            t_Reader reader("type_container_config_match_multi.cfg");
            type_container_in1.readConfig(reader, "node1");
            type_container_in2.readConfig(reader, "node2");

            // -------

            compare(type_container_out1, type_container_in1);
            compare(type_container_out2, type_container_in2);
        }


        template<class t_Reader, class t_Writer>
            void testVector()
        {
            TypeContainerVector type_container_vector_out;
            type_container_vector_out.writeConfig<t_Writer>("type_container_vector_config_match_simple.cfg");

            // -------

            TypeContainerVector type_container_vector_in;
            type_container_vector_in.readConfig<t_Reader>("type_container_vector_config_match_simple.cfg");

            // -------

            EXPECT_EQ(type_container_vector_out.vector_.size(), type_container_vector_in.vector_.size());
            for(std::size_t i = 0; i < type_container_vector_out.vector_.size(); ++i)
            {
                compare(type_container_vector_out.vector_[i], type_container_vector_in.vector_[i]);
            }
        }
};



TEST_F(ConfigInterfaceTest, ConfigInterfaceYAML)
{
    test<humoto::config::yaml::Reader, humoto::config::yaml::Writer>();
}


TEST_F(ConfigInterfaceTest, ConfigInterfaceMSGPACK)
{
    test<humoto::config::msgpack::Reader, humoto::config::msgpack::Writer>();
}


//---------------



TEST_F(ConfigMatchTest, ConfigMatchSimpleYAML)
{
    testSimple<humoto::config::yaml::Reader, humoto::config::yaml::Writer>();
}


TEST_F(ConfigMatchTest, ConfigMatchSimpleMSGPACK)
{
    testSimple<humoto::config::msgpack::Reader, humoto::config::msgpack::Writer>();
}



TEST_F(ConfigMatchTest, ConfigMatchMultiYAML)
{
    testMulti<humoto::config::yaml::Reader, humoto::config::yaml::Writer>();
}


TEST_F(ConfigMatchTest, ConfigMatchMultiMSGPACK)
{
    testMulti<humoto::config::msgpack::Reader, humoto::config::msgpack::Writer>();
}


TEST_F(ConfigMatchTest, ConfigMatchVectorYAML)
{
    testVector<humoto::config::yaml::Reader, humoto::config::yaml::Writer>();
}


TEST_F(ConfigMatchTest, ConfigMatchVectorMSGPACK)
{
    testVector<humoto::config::msgpack::Reader, humoto::config::msgpack::Writer>();
}


/**
 * @brief main
 *
 * @param[in] argc number of args
 * @param[in] argv args
 *
 * @return status
 */
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
