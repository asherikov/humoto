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

    enum SomeEnum
    {
        UNDEFINED = 0,
        SOME_VALUE = 1,
        ANOTHER_VALUE = 2
    };


    class Configurable : public humoto::config::ConfigurableBase
    {
        #define HUMOTO_CONFIG_SECTION_ID "Configurable"
        #define HUMOTO_CONFIG_CONSTRUCTOR Configurable
        #define HUMOTO_CONFIG_ENTRIES \
            HUMOTO_CONFIG_ENTRY_(integer) \
            HUMOTO_CONFIG_ENTRY_(real) \
            HUMOTO_CONFIG_ENTRY_(vector) \
            HUMOTO_CONFIG_ENTRY_(matrix) \
            HUMOTO_CONFIG_ENTRY_(matrix_x) \
            HUMOTO_CONFIG_ENTRY_(std_vector) \
            HUMOTO_CONFIG_ENTRY_(std_nested_vector) \
            HUMOTO_CONFIG_ENTRY_(string) \
            HUMOTO_CONFIG_ENTRY_(std_vector_evector) \
            HUMOTO_CONFIG_ENTRY_(std_nested_vector_evector) \
            HUMOTO_CONFIG_ENTRY_(enum)
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

            SomeEnum enum_;


        public:
            Configurable()
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

                enum_ = ANOTHER_VALUE;
            }
    };


    class Configurable2 : public humoto::config::ConfigurableBase
    {
        #define HUMOTO_CONFIG_SECTION_ID "Configurable2"
        #define HUMOTO_CONFIG_CONSTRUCTOR Configurable2
        #define HUMOTO_CONFIG_ENTRIES \
            HUMOTO_CONFIG_TYPED_ENTRY_(integer,     int) \
            HUMOTO_CONFIG_TYPED_ENTRY_(real,        double) \
            HUMOTO_CONFIG_TYPED_ENTRY_(string,      std::string) \
            HUMOTO_CONFIG_TYPED_ENTRY_(vector,      Eigen::Vector3d) \
            HUMOTO_CONFIG_TYPED_ENTRY_(matrix,      Eigen::Matrix3d) \
            HUMOTO_CONFIG_TYPED_ENTRY_(matrix_x,    Eigen::MatrixXd) \
            HUMOTO_CONFIG_TYPED_ENTRY_(std_vector,          std::vector<double>) \
            HUMOTO_CONFIG_TYPED_ENTRY_(std_nested_vector,   std::vector< std::vector<double> >) \
            HUMOTO_CONFIG_TYPED_ENTRY_(std_vector_evector,  std::vector<Eigen::Vector3d>) \
            HUMOTO_CONFIG_TYPED_ENTRY_(std_nested_vector_evector, std::vector< std::vector<Eigen::Vector3d> >)\
            HUMOTO_CONFIG_TYPED_ENTRY_(enum, SomeEnum)
        #include HUMOTO_CONFIG_DEFINE_ACCESSORS


        public:
            Configurable2()
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

                enum_ = ANOTHER_VALUE;
            }
    };


    class ConfigurableVector : public humoto::config::ConfigurableBase
    {
        #define HUMOTO_CONFIG_SECTION_ID "ConfigurableVector"
        #define HUMOTO_CONFIG_CONSTRUCTOR ConfigurableVector
        #define HUMOTO_CONFIG_ENTRIES \
            HUMOTO_CONFIG_ENTRY_(vector)
        #include HUMOTO_CONFIG_DEFINE_ACCESSORS

        public:
            std::vector<Configurable> vector_;


        public:
            ConfigurableVector()
            {
                setDefaults();
            }


            void setDefaults()
            {
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
        template<class t_Configurable, class t_Reader, class t_Writer>
            void test()
        {
            {
                t_Configurable configurable;

                t_Writer writer("configurable.cfg");
                configurable.writeConfig(writer);
            }

            {
                t_Configurable configurable;

                t_Reader reader("configurable.cfg");
                configurable.readConfig(reader);
            }

            // --------------------------------

            {
                t_Configurable configurable;
                configurable.template writeConfig<t_Writer>("configurable2.cfg");
            }

            {
                t_Configurable configurable;
                configurable.template readConfig<t_Reader>("configurable2.cfg");
            }
        }
};


class ConfigMatchTest : public ::testing::Test
{
    protected:
        template<class t_Configurable_out, class t_Configurable_in>
        void    compare(const t_Configurable_out    &configurable_out,
                        const t_Configurable_in     &configurable_in)
        {
            EXPECT_EQ(configurable_out.integer_,          configurable_in.integer_);
            EXPECT_EQ(configurable_out.real_,             configurable_in.real_);
            EXPECT_EQ(configurable_out.vector_,           configurable_in.vector_);
            EXPECT_EQ(configurable_out.matrix_,           configurable_in.matrix_);
            EXPECT_EQ(configurable_out.matrix_x_,         configurable_in.matrix_x_);
            EXPECT_EQ(configurable_out.string_,           configurable_in.string_);

            EXPECT_EQ(configurable_out.std_vector_.size(),                configurable_in.std_vector_.size());
            EXPECT_EQ(configurable_out.std_nested_vector_.size(),         configurable_in.std_nested_vector_.size());
            EXPECT_EQ(configurable_out.std_vector_evector_.size(),        configurable_in.std_vector_evector_.size());
            EXPECT_EQ(configurable_out.std_nested_vector_evector_.size(), configurable_in.std_nested_vector_evector_.size());

            for (std::size_t i = 0; i < configurable_out.std_vector_.size(); ++i)
            {
                EXPECT_NEAR(configurable_out.std_vector_[i],
                            configurable_in.std_vector_[i],
                            g_tolerance);
            }

            for (std::size_t i = 0; i < configurable_out.std_vector_evector_.size(); ++i)
            {
                EXPECT_EQ(  configurable_out.std_vector_evector_[i],
                            configurable_in.std_vector_evector_[i]);
            }


            for (std::size_t i = 0; i < configurable_out.std_nested_vector_.size(); ++i)
            {
                EXPECT_EQ(configurable_out.std_nested_vector_[i].size(),  configurable_in.std_nested_vector_[i].size());
                for (std::size_t j = 0; j < configurable_out.std_nested_vector_[i].size(); ++j)
                {
                    EXPECT_NEAR(configurable_out.std_nested_vector_[i][j],
                                configurable_in.std_nested_vector_[i][j],
                                g_tolerance);
                }
            }

            for (std::size_t i = 0; i < configurable_out.std_nested_vector_evector_.size(); ++i)
            {
                EXPECT_EQ(configurable_out.std_nested_vector_evector_[i].size(),  configurable_in.std_nested_vector_evector_[i].size());
                for (std::size_t j = 0; j < configurable_out.std_nested_vector_evector_[i].size(); ++j)
                {
                    EXPECT_EQ(  configurable_out.std_nested_vector_evector_[i][j],
                                configurable_in.std_nested_vector_evector_[i][j]);
                }
            }
        }


        template<class t_Reader, class t_Writer>
            void testSimple()
        {
            Configurable configurable_out;
            configurable_out.writeConfig<t_Writer>("configurable_match_simple.cfg");

            // -------

            Configurable configurable_in;
            configurable_in.readConfig<t_Reader>("configurable_match_simple.cfg");

            // -------

            compare(configurable_out, configurable_in);
        }


        template<class t_Reader, class t_Writer>
            void testMulti()
        {
            Configurable configurable_out1;
            Configurable configurable_out2;
            {
                t_Writer writer("configurable_match_multi.cfg");
                configurable_out1.writeConfig(writer, "node1");
                configurable_out2.writeConfig(writer, "node2");
            }

            // -------

            Configurable configurable_in1;
            Configurable configurable_in2;
            t_Reader reader("configurable_match_multi.cfg");
            configurable_in1.readConfig(reader, "node1");
            configurable_in2.readConfig(reader, "node2");

            // -------

            compare(configurable_out1, configurable_in1);
            compare(configurable_out2, configurable_in2);
        }


        template<class t_Reader, class t_Writer>
            void testVector()
        {
            ConfigurableVector configurable_vector_out;
            configurable_vector_out.writeConfig<t_Writer>("configurable_match_vector.cfg");

            // -------

            ConfigurableVector configurable_vector_in;
            configurable_vector_in.readConfig<t_Reader>("configurable_match_vector.cfg");

            // -------

            EXPECT_EQ(configurable_vector_out.vector_.size(), configurable_vector_in.vector_.size());
            for(std::size_t i = 0; i < configurable_vector_out.vector_.size(); ++i)
            {
                compare(configurable_vector_out.vector_[i], configurable_vector_in.vector_[i]);
            }
        }


        template<class t_Reader, class t_Writer>
            void testMemberDefinitions()
        {
            Configurable configurable_out;
            configurable_out.writeConfig<t_Writer>("configurable_match_member_definitions.cfg");

            // -------

            Configurable2 configurable_in;
            configurable_in.readConfig<t_Reader>("configurable_match_member_definitions.cfg", "Configurable");

            // -------

            compare(configurable_out, configurable_in);
        }
};



TEST_F(ConfigInterfaceTest, ConfigInterfaceYAML)
{
    test<Configurable, humoto::config::yaml::Reader, humoto::config::yaml::Writer>();
}


TEST_F(ConfigInterfaceTest, ConfigInterfaceMSGPACK)
{
    test<Configurable, humoto::config::msgpack::Reader, humoto::config::msgpack::Writer>();
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


TEST_F(ConfigMatchTest, ConfigMatchMemberDefinitionsYAML)
{
    testMemberDefinitions<humoto::config::yaml::Reader, humoto::config::yaml::Writer>();
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
