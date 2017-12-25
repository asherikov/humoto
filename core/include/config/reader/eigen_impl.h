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
        namespace reader
        {
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
            template <  class t_Reader,
                        typename t_Scalar,
                        int t_rows,
                        int t_flags>
                void readBody(  t_Reader &reader,
                                Eigen::Matrix<t_Scalar, t_rows, 1, t_flags> &entry,
                                const bool crash_on_missing_entry)
            {
                HUMOTO_ASSERT(reader.isArray(), "Entry is not an array.");


                std::size_t size = reader.startArray();

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
                    readBody(reader,entry[i]);
                }
                reader.endArray();
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
            template <  class t_Reader,
                        typename t_Scalar,
                        int t_rows,
                        int t_cols,
                        int t_flags>
                void readBody(  t_Reader & reader,
                                Eigen::Matrix<t_Scalar, t_rows, t_cols, t_flags> &entry,
                                const bool crash_on_missing_entry)
            {
                humoto::EigenIndex num_rows;
                humoto::EigenIndex num_cols;

                readEntry(reader, num_rows, "rows", true);
                readEntry(reader, num_cols, "cols", true);


                Eigen::VectorXd v;
                readEntry(reader, v, "data", true);

                HUMOTO_ASSERT(v.rows() == num_rows*num_cols, "Wrong entry size.");

                Eigen::Map< Eigen::Matrix<  double,
                                            Eigen::Dynamic,
                                            Eigen::Dynamic,
                                            Eigen::RowMajor> >  map(v.data(),
                                                                    num_rows,
                                                                    num_cols);
                entry = map;
            }
        }
    }
}

