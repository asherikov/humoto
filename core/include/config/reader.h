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
        template <class t_ReaderBase>
            class HUMOTO_LOCAL ReaderMixin : public t_ReaderBase
        {
            protected:
                using t_ReaderBase::isArray;
                using t_ReaderBase::getArraySize;
                using t_ReaderBase::readArrayElement;
                using t_ReaderBase::readElement;


            public:
                using t_ReaderBase::descend;
                using t_ReaderBase::ascend;


            public:
                /**
                 * @brief Read configuration entry (vector)
                 *
                 * @tparam t_Scalar Eigen template parameter
                 * @tparam t_rows   Eigen template parameter
                 * @tparam t_flags  Eigen template parameter
                 *
                 * @param[out] entry     configuration parameter
                 * @param[in] entry_name name of the configuration parameter
                 * @param[in] crash_on_missing_entry
                 */
                template <  typename t_Scalar,
                            int t_rows,
                            int t_flags>
                    void readCompound(Eigen::Matrix<t_Scalar, t_rows, 1, t_flags> &entry,
                                      const std::string & entry_name,
                                      const bool crash_on_missing_entry = false)
                {
                    if (descend(entry_name))
                    {
                        HUMOTO_ASSERT(isArray(), "[Config] Entry is not an array.");

                        if (Eigen::Dynamic == t_rows)
                        {
                            entry.resize(getArraySize());
                        }
                        else
                        {
                            HUMOTO_ASSERT(  (static_cast<int>(getArraySize()) == t_rows),
                                            "[Config] Wrong entry sequence size.");
                        }

                        for(humoto::EigenIndex i = 0; i < (Eigen::Dynamic == t_rows ? entry.rows() : t_rows); ++i)
                        {
                            readArrayElement(entry[i], i);
                        }

                        ascend();
                    }
                    else
                    {
                        if (crash_on_missing_entry)
                        {
                            HUMOTO_THROW_MSG(std::string("Configuration file does not contain entry '") + entry_name + "'.");
                        }
                    }
                }



                /**
                 * @brief Read a configuration entry (matrix)
                 *
                 * @tparam t_Scalar Eigen template parameter
                 * @tparam t_rows   Eigen template parameter
                 * @tparam t_cols   Eigen template parameter
                 * @tparam t_flags  Eigen template parameter
                 *
                 * @param[out] entry      data
                 * @param[in]  entry_name name
                 * @param[in] crash_on_missing_entry
                 */
                template <  typename t_Scalar,
                            int t_rows,
                            int t_cols,
                            int t_flags>
                    void readCompound(  Eigen::Matrix<t_Scalar, t_rows, t_cols, t_flags> &entry,
                                        const std::string& entry_name,
                                        const bool crash_on_missing_entry = false)
                {
                    if (descend(entry_name))
                    {
                        humoto::EigenIndex num_rows;
                        humoto::EigenIndex num_cols;

                        readScalar(num_rows, "rows", true);
                        readScalar(num_cols, "cols", true);


                        Eigen::VectorXd v;
                        readCompound(v, "data", true);

                        HUMOTO_ASSERT(  v.rows() == num_rows*num_cols,
                                        std::string("Inconsistent configuration file entry: ") + entry_name);

                        Eigen::Map< Eigen::Matrix<  double,
                                                    Eigen::Dynamic,
                                                    Eigen::Dynamic,
                                                    Eigen::RowMajor> >  map(v.data(),
                                                                            num_rows,
                                                                            num_cols);
                        entry = map;

                        ascend();
                    }
                    else
                    {
                        if (crash_on_missing_entry)
                        {
                            HUMOTO_THROW_MSG(std::string("Configuration file does not contain entry '") + entry_name + "'.");
                        }
                    }
                }


                /**
                 * @brief Read configuration entry (std::vector)
                 *
                 * @tparam t_VectorEntryType type of the entry of std::vector
                 *
                 * @param[out] entry      configuration parameter
                 * @param[in]  entry_name name of the configuration parameter
                 * @param[in]  crash_on_missing_entry
                 */
                template <typename t_VectorEntryType>
                    void readCompound(  std::vector<t_VectorEntryType> & entry,
                                        const std::string              & entry_name,
                                        const bool crash_on_missing_entry = false)
                {
                    if (descend(entry_name))
                    {
                        HUMOTO_ASSERT(isArray(), "[Config] Entry is not an array.");

                        entry.resize(getArraySize());

                        for(std::size_t i = 0; i < entry.size(); ++i)
                        {
                            readArrayElement(entry[i], i);
                        }

                        ascend();
                    }
                    else
                    {
                        if (crash_on_missing_entry)
                        {
                            HUMOTO_THROW_MSG(std::string("Configuration file does not contain entry '") + entry_name + "'.");
                        }
                    }
                }



                /**
                 * @brief Read configuration entry (scalar template)
                 *
                 * @tparam t_EntryType type of the entry
                 *
                 * @param[out] entry     configuration parameter
                 * @param[in] entry_name name of the configuration parameter
                 * @param[in] crash_on_missing_entry
                 */
                template <typename t_EntryType>
                    void readScalar(t_EntryType        & entry,
                                    const std::string  & entry_name,
                                    const bool crash_on_missing_entry = false)
                {
                    if (descend(entry_name))
                    {
                        readElement(entry);
                        ascend();
                    }
                    else
                    {
                        if (crash_on_missing_entry)
                        {
                            HUMOTO_THROW_MSG(std::string("Configuration file does not contain entry '") + entry_name + "'.");
                        }
                    }
                }


                /**
                 * @brief Read configuration entry (an enum). This method
                 * is added since an explicit casting to integer is needed.
                 *
                 * @tparam t_EnumerationType enumeration type
                 *
                 * @param[out] entry     configuration parameter
                 * @param[in] entry_name name of the configuration parameter
                 * @param[in] crash_on_missing_entry
                 */
                template <typename t_EnumerationType>
                    void readEnum(  t_EnumerationType  & entry,
                                    const std::string  & entry_name,
                                    const bool crash_on_missing_entry = false)
                {
                    int tmp_value = 0;

                    readScalar(tmp_value, entry_name, crash_on_missing_entry);

                    entry = static_cast<t_EnumerationType> (tmp_value);
                }
        };
    }
}
