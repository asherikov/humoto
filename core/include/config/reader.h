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
                using t_ReaderBase::startArray;
                using t_ReaderBase::endArray;


            protected:
                /**
                 * @brief Read configuration entry (vector)
                 *
                 * @tparam t_Scalar Eigen template parameter
                 * @tparam t_rows   Eigen template parameter
                 * @tparam t_flags  Eigen template parameter
                 *
                 * @param[out] entry     configuration parameter
                 * @param[in] crash_on_missing_entry
                 */
                template <  typename t_Scalar,
                            int t_rows,
                            int t_flags>
                    void readBody(  Eigen::Matrix<t_Scalar, t_rows, 1, t_flags> &entry,
                                    const bool crash_on_missing_entry = false)
                {
                    HUMOTO_ASSERT(isArray(), "Entry is not an array.");


                    std::size_t size = startArray();

                    if (Eigen::Dynamic == t_rows)
                    {
                        entry.resize(size);
                    }
                    else
                    {
                        HUMOTO_ASSERT(  (static_cast<int>(size) == t_rows),
                                        "Wrong entry size.");
                    }

                    for(humoto::EigenIndex i = 0; i < (Eigen::Dynamic == t_rows ? entry.rows() : t_rows); ++i)
                    {
                        readBody(entry[i]);
                    }
                    endArray();
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
                 * @param[in] crash_on_missing_entry
                 */
                template <  typename t_Scalar,
                            int t_rows,
                            int t_cols,
                            int t_flags>
                    void readBody(  Eigen::Matrix<t_Scalar, t_rows, t_cols, t_flags> &entry,
                                    const bool crash_on_missing_entry = false)
                {
                    humoto::EigenIndex num_rows;
                    humoto::EigenIndex num_cols;

                    readEntry(num_rows, "rows", true);
                    readEntry(num_cols, "cols", true);


                    Eigen::VectorXd v;
                    readEntry(v, "data", true);

                    HUMOTO_ASSERT(v.rows() == num_rows*num_cols, "Wrong entry size.");

                    Eigen::Map< Eigen::Matrix<  double,
                                                Eigen::Dynamic,
                                                Eigen::Dynamic,
                                                Eigen::RowMajor> >  map(v.data(),
                                                                        num_rows,
                                                                        num_cols);
                    entry = map;
                }


                /**
                 * @brief Read configuration entry (std::vector)
                 *
                 * @tparam t_VectorEntryType type of the entry of std::vector
                 *
                 * @param[out] entry      configuration parameter
                 * @param[in]  crash_on_missing_entry
                 */
                template <typename t_VectorEntryType>
                    void readBody(  std::vector<t_VectorEntryType> & entry,
                                    const bool crash_on_missing_entry = false)
                {
                    HUMOTO_ASSERT(isArray(), "Entry is not an array.");

                    entry.resize(startArray());
                    for(std::size_t i = 0; i < entry.size(); ++i)
                    {
                        std::cout << "i " << i << std::endl;
                        readBody(entry[i], crash_on_missing_entry);
                    }
                    endArray();
                }



                /**
                 * @brief Read configuration entry (scalar template)
                 *
                 * @tparam t_EntryType type of the entry
                 *
                 * @param[out] entry     configuration parameter
                 * @param[in] crash_on_missing_entry
                 */
                template <typename t_EntryType>
                    void readBody(t_EntryType        & entry,
                                  const bool crash_on_missing_entry = false)
                {
                    t_ReaderBase::readElement(entry);
                }


            public:
                using t_ReaderBase::descend;
                using t_ReaderBase::ascend;


            public:
                /**
                 * @brief Read configuration entry
                 *
                 * @tparam t_Entry type of the entry
                 *
                 * @param[out] entry      configuration parameter
                 * @param[in]  entry_name name of the configuration parameter
                 * @param[in]  crash_on_missing_entry
                 */
                template <class t_Entry>
                    void readEntry( t_Entry &entry,
                                    const std::string & entry_name,
                                    const bool crash_on_missing_entry = false)
                {
                    if (descend(entry_name))
                    {
                        try
                        {
                            readBody(entry, crash_on_missing_entry);
                        }
                        catch(const std::exception &e)
                        {
                            HUMOTO_THROW_MSG(   std::string("Failed to parse entry <")
                                                + entry_name
                                                + "> in the configuration file: "
                                                + e.what());
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

                    readEntry(tmp_value, entry_name, crash_on_missing_entry);

                    entry = static_cast<t_EnumerationType> (tmp_value);
                }
        };
    }
}
