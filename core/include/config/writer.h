/**
    @file
    @author  Alexander Sherikov

    @copyright 2014-2017 Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once

namespace humoto
{
    namespace config
    {
        template <class t_WriterBase>
            class HUMOTO_LOCAL WriterMixin : public t_WriterBase
        {
            protected:
                using t_WriterBase::startArray;
                using t_WriterBase::endArray;
                using t_WriterBase::writeArrayElement;

            public:
                using t_WriterBase::descend;
                using t_WriterBase::ascend;
                using t_WriterBase::writeScalar;


            public:
                /**
                 * @brief Write a configuration entry (vector)
                 *
                 * @tparam t_Derived Eigen template parameter
                 *
                 * @param[in] entry      data
                 * @param[in] entry_name name
                 */
                template <  typename t_Scalar,
                            int t_rows,
                            int t_flags>
                    void writeCompound( const Eigen::Matrix<t_Scalar, t_rows, 1, t_flags> &entry,
                                        const std::string & entry_name)
                {
                    startArray(entry_name, entry.rows());
                    for (humoto::EigenIndex i = 0; i < entry.rows(); ++i)
                    {
                        writeArrayElement(entry[i]);
                    }
                    endArray();
                }



                /**
                 * @brief Write a configuration entry (matrix)
                 *
                 * @tparam t_Scalar Eigen template parameter
                 * @tparam t_rows   Eigen template parameter
                 * @tparam t_cols   Eigen template parameter
                 * @tparam t_flags  Eigen template parameter
                 *
                 * @param[in] entry      data
                 * @param[in]  entry_name name
                 */
                template <  typename t_Scalar,
                            int t_rows,
                            int t_cols,
                            int t_flags>
                    void writeCompound( const Eigen::Matrix<t_Scalar, t_rows, t_cols, t_flags> &entry,
                                        const std::string& entry_name)
                {
                    descend(entry_name, 3);

                    writeScalar(entry.cols(), "cols");
                    writeScalar(entry.rows(), "rows");


                    startArray("data", entry.size());
                    for (humoto::EigenIndex i = 0; i < entry.size(); ++i)
                    {
                        writeArrayElement(entry.data()[i]);
                    }
                    endArray();

                    ascend();
                }



                /**
                 * @brief Read configuration entry (std::vector)
                 *
                 * @tparam t_VectorEntryType type of the entry of std::vector
                 *
                 * @param[in] entry      data
                 * @param[in] entry_name name
                 */
                template <typename t_VectorEntryType>
                    void writeCompound( const std::vector<t_VectorEntryType> & entry,
                                        const std::string                    & entry_name)
                {
                    startArray(entry_name, entry.size());
                    for (std::size_t i = 0; i < entry.size(); ++i)
                    {
                        writeArrayElement(entry[i]);
                    }
                    endArray();
                }


                /**
                 * @brief Write configuration entry (std::vector<std::vector<std::string>>)
                 *
                 * @tparam t_VectorEntryType type of the entry of std::vector
                 *
                 * @param[in] entry      configuration parameter
                 * @param[in] entry_name name of the configuration parameter
                 */
                template <typename t_VectorEntryType>
                    void writeCompound( const std::vector< std::vector<t_VectorEntryType> > & entry,
                                        const std::string                                   & entry_name) const
                {
                    startArray(entry_name, entry.size());
                    for(std::size_t i = 0; i < entry.size(); ++i)
                    {
                        writeArrayElement(entry[i]);
                        /*
                        for(std::size_t i = 0; i < entry.size(); ++i)
                        {
                            for(std::size_t j = 0; j < entry[i].size(); ++j)
                            {
                                *emitter_ << entry[i][j];
                            }
                        }*/
                    }
                    endArray();
                }


                /**
                 * @brief Write a configuration entry (enum)
                 *
                 * @tparam t_EnumType type of the enum
                 *
                 * @param[in] entry      data
                 * @param[in] entry_name name
                 */
                template <typename t_EnumType>
                    void writeEnum( const t_EnumType  entry,
                                    const std::string  & entry_name)
                {
                    int tmp_value = entry;
                    writeScalar(tmp_value, entry_name);
                }
        };
    }
}
