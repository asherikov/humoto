/**
    @file
    @author  Alexander Sherikov
    @copyright 2014-2017 INRIA. Licensed under the Apache License, Version 2.0.
    (see @ref LICENSE or http://www.apache.org/licenses/LICENSE-2.0)

    @brief
*/

#pragma once

namespace etools
{
#define EIGENTOOLS_PARENT_CLASS_SHORTHAND BlockMatrixBase<  const typename TypeWithoutConst<t_MatrixType>::Type, \
                                                            t_block_rows_num, t_block_cols_num, t_sparsity_type>
    /**
     * @brief Represents block kronecker product "Identity(size) [X] Matrix".
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     * @tparam t_sparsity_type  sparsity type
     */
    template<   class t_MatrixType,
                int t_block_rows_num,
                int t_block_cols_num,
                MatrixSparsityType::Type t_sparsity_type>
    class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockKroneckerProductBase : protected EIGENTOOLS_PARENT_CLASS_SHORTHAND
    {
        private:
            std::ptrdiff_t     identity_size_;


        protected:
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::operator();
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::matrix_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_hor_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_vert_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::multiplyRight;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::column;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_rows_num_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_cols_num_;


            /**
             * @brief Conversion to a matrix
             *
             * @tparam t_Derived    Eigen template parameter
             *
             * @param[out]  output  matrix
             */
            template<class t_Derived>
                void evaluateWithoutInitialization(const Eigen::MatrixBase<t_Derived> & output) const
            {
                Eigen::MatrixBase<t_Derived> & out = const_cast< Eigen::MatrixBase<t_Derived> & > (output);

                for (std::ptrdiff_t j = 0; j < num_blocks_hor_; ++j)
                {
                    std::ptrdiff_t primary_block_col = j * identity_size_;

                    for (std::ptrdiff_t i = 0; i < num_blocks_vert_; ++i)
                    {
                        std::ptrdiff_t primary_block_row = i * identity_size_;

                        for(std::ptrdiff_t k = 0; k < identity_size_; ++k)
                        {
                            out.block(  (primary_block_row + k)*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                        (primary_block_col + k)*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM,
                                        EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                        EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM) = (*this)(i, j);
                        }
                    }
                }
            }


        public:
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::DecayedRawMatrix    DecayedRawMatrix;


            /**
             * @brief this * BlockMatrix<DIAGONAL>
             *
             * @tparam t_Derived            Eigen parameter
             * @tparam t_DBMMatrixType      raw diagonal block matrix type
             * @tparam t_dbm_block_rows_num number of rows in a block of the diagonal matrix
             * @tparam t_dbm_block_cols_num number of columns in a block of the diagonal matrix
             *
             * @param[out] result result of multiplication
             * @param[in] dbm diagonal block matrix
             */
            template<   class t_Derived,
                        typename t_DBMMatrixType,
                        int t_dbm_block_rows_num,
                        int t_dbm_block_cols_num>
                void multiplyRight( Eigen::PlainObjectBase<t_Derived>   &result,
                                    const BlockMatrixBase<  t_DBMMatrixType,
                                                            t_dbm_block_rows_num,
                                                            t_dbm_block_cols_num,
                                                            MatrixSparsityType::DIAGONAL> &dbm) const
            {
                EIGENTOOLS_ASSERT(dbm.getBlockRowsNum() == EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM*identity_size_,
                                    "Block sizes do not match.");
                EIGENTOOLS_ASSERT(num_blocks_hor_ == dbm.getNumberOfBlocksVertical(),
                                    "Numbers of blocks do not match.");
                EIGENTOOLS_ASSERT(num_blocks_hor_*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM*identity_size_ == dbm.getNumberOfRows(),
                                    "Sizes of matrices do not match.");


                result.resize(identity_size_ * matrix_.rows(), dbm.getNumberOfColumns());
                if (num_blocks_hor_ != 0)
                {
                    Eigen::VectorXi indices;

                    indices.resize(identity_size_ * matrix_.rows());

                    for (std::ptrdiff_t i = 0; i < identity_size_; ++i)
                    {
                        for (std::ptrdiff_t j = 0; j < num_blocks_vert_; ++j)
                        {
                            indices.segment(j*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM*identity_size_ + i*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM) =
                                Eigen::VectorXi::LinSpaced( EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                                            i*matrix_.rows() + j*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                                            i*matrix_.rows() + (j+1)*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM - 1);
                        }
                    }


                    std::ptrdiff_t dbm_block_cols = dbm.getNumberOfColumns() / num_blocks_hor_;

                    for (std::ptrdiff_t i = 0; i < identity_size_; ++i)
                    {
                        for (std::ptrdiff_t j = 0; j < num_blocks_hor_; ++j)
                        {
                            result.block(i*matrix_.rows(), dbm_block_cols*j, matrix_.rows(), dbm_block_cols).noalias() =
                                column(j)
                                *
                                dbm(j).block(i*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM, 0, EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM, dbm_block_cols);
                        }
                    }

                    result = (Eigen::PermutationWrapper<Eigen::VectorXi> (indices)).transpose() * result;
                }
            }


            /*
            // this is slower
            template<   typename t_DBMMatrixType,
                        int t_dbm_block_rows_num,
                        int t_dbm_block_cols_num>
            void multiplyRight( Eigen::MatrixBaseXd &result,
                                const BlockMatrixBase<  t_DBMMatrixType,
                                                        t_dbm_block_rows_num,
                                                        t_dbm_block_cols_num,
                                                        MatrixSparsityType::DIAGONAL> &dbm) const
            {
                EIGENTOOLS_ASSERT(dbm.getBlockRowsNum() == EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM*identity_size_,
                                    "Block sizes do not match.");
                EIGENTOOLS_ASSERT(num_blocks_hor_ == dbm.getNumberOfBlocksVertical(),
                                    "Numbers of blocks do not match.");
                EIGENTOOLS_ASSERT(num_blocks_hor_*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM*identity_size_ == dbm.getNumberOfRows(),
                                    "Sizes of matrices do not match.");


                result.resize(identity_size_ * matrix_.rows(), dbm.getNumberOfColumns());

                BlockMatrixInterface<   t_block_rows_num,
                                                t_dbm_block_cols_num,
                                                MatrixSparsityType::NONE >
                    result_bmi( result,
                                t_block_rows_num == MatrixBlockSizeType::DYNAMIC ? EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM : MatrixBlockSizeType::UNDEFINED,
                                t_dbm_block_cols_num == MatrixBlockSizeType::DYNAMIC ? dbm.getBlockRowsNum() : MatrixBlockSizeType::UNDEFINED);


                std::ptrdiff_t dbm_block_cols = dbm.getBlockColsNum();

                for (std::ptrdiff_t j = 0; j < num_blocks_hor_; ++j)
                {
                    for (std::ptrdiff_t k = 0; k < identity_size_; ++k)
                    {
                        for (std::ptrdiff_t i = 0; i < num_blocks_vert_; ++i)
                        {
                            result_bmi(i*identity_size_ + k, j).noalias() =
                                (*this)(i,j)
                                *
                                dbm(j).block(   k*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM,
                                                0,
                                                EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM,
                                                dbm_block_cols);
                        }
                    }
                }
            }
            */



            /**
             * @brief this * Vector
             *
             * @tparam  t_DerivedOutput     Eigen template parameter
             * @tparam  t_Scalar            Eigen template parameter
             * @tparam  t_vector_size       Eigen template parameter
             * @tparam  t_vector_options    Eigen template parameter
             *
             * @param[out] result result of multiplication
             * @param[in] vector
             */
            template<   class t_DerivedOutput,
                        typename t_Scalar,
                        int t_vector_size,
                        int t_vector_options>
                void multiplyRight( Eigen::PlainObjectBase<t_DerivedOutput>      &result,
                                    const Eigen::Matrix<t_Scalar, t_vector_size, 1, t_vector_options> & vector) const
            {
                EIGENTOOLS_DYNAMIC_VECTOR(typename Eigen::PlainObjectBase<t_DerivedOutput>::Scalar)  result_part;
                EIGENTOOLS_DYNAMIC_VECTOR(typename Eigen::PlainObjectBase<t_DerivedOutput>::Scalar)  vector_part;


                result.resize(identity_size_ * matrix_.rows());

                vector_part.resize(matrix_.cols());

                for (std::ptrdiff_t i = 0; i < identity_size_; ++i)
                {
                    for (std::ptrdiff_t j = 0; j < num_blocks_hor_; ++j)
                    {
                        vector_part.segment(j*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM, EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM) =
                            vector.segment(j*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM*identity_size_ + i*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM, EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM);
                    }

                    result_part.noalias() = matrix_ * vector_part;

                    for (std::ptrdiff_t j = 0; j < num_blocks_vert_; ++j)
                    {
                        result.segment(j*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM*identity_size_ + i*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM) =
                            result_part.segment(j*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM);
                    }
                }
            }



            /**
             * @brief Conversion to Matrix
             *
             * @return Matrix
             */
            DecayedRawMatrix   evaluate() const
            {
                DecayedRawMatrix output;
                evaluate(output);
                return(output);
            }



            /**
             * @brief Conversion to a matrix
             *
             * @tparam t_Derived    Eigen template parameter
             *
             * @param[out]  output  matrix
             */
            template<class t_Derived>
                void evaluate(Eigen::PlainObjectBase<t_Derived> & output) const
            {
                output.setZero(identity_size_ * matrix_.rows(),
                               identity_size_ * matrix_.cols());
                evaluateWithoutInitialization(output);
            }


            /**
             * @brief Conversion to a matrix
             *
             * @tparam t_Derived    Eigen template parameter
             * @tparam t_rows       Eigen template parameter
             * @tparam t_cols       Eigen template parameter
             * @tparam t_flag       Eigen template parameter
             *
             * @param[out] output matrix block of appropriate size
             */
            template<class t_Derived, int t_rows, int t_cols, bool t_flag>
                void evaluate(Eigen::Block<t_Derived, t_rows, t_cols, t_flag> output) const
            {
                output.setZero();
                evaluateWithoutInitialization(output);
            }



            /**
             * @brief Constructor
             *
             * @param[in] matrix
             * @param[in] identity_size
             * @param[in] block_rows_num    number of rows in a block if    t_block_rows_num = MatrixBlockSizeType::DYNAMIC
             * @param[in] block_cols_num    number of cols in a block if    t_block_cols_num = MatrixBlockSizeType::DYNAMIC
             */
            BlockKroneckerProductBase(  const DecayedRawMatrix & matrix,
                                        const std::ptrdiff_t identity_size = 1,
                                        const std::ptrdiff_t block_rows_num = MatrixBlockSizeType::UNDEFINED,
                                        const std::ptrdiff_t block_cols_num = MatrixBlockSizeType::UNDEFINED) :
                EIGENTOOLS_PARENT_CLASS_SHORTHAND(matrix, block_rows_num, block_cols_num)
            {
                EIGENTOOLS_ASSERT(  identity_size > 1,
                                    "Identity size cannot be less than 2.");
                identity_size_ = identity_size;
            }
    };
#undef EIGENTOOLS_PARENT_CLASS_SHORTHAND



#define EIGENTOOLS_PARENT_CLASS_SHORTHAND BlockMatrixBase<  const typename TypeWithoutConst<t_MatrixType>::Type, \
                                                            t_block_rows_num, t_block_cols_num, MatrixSparsityType::LEFT_LOWER_TRIANGULAR>
    /**
     * @brief Represents block kronecker product "Identity(size) [X] Matrix",
     * without computing it explicitly.
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     */
    template<   class t_MatrixType,
                int t_block_rows_num,
                int t_block_cols_num>
    class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockKroneckerProductBase<t_MatrixType,
                                                                    t_block_rows_num,
                                                                    t_block_cols_num,
                                                                    MatrixSparsityType::LEFT_LOWER_TRIANGULAR>
        : protected EIGENTOOLS_PARENT_CLASS_SHORTHAND
    {
        private:
            std::ptrdiff_t     identity_size_;


        protected:
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::operator();
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::matrix_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_hor_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_vert_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::multiplyRight;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::column;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_rows_num_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_cols_num_;


            /**
             * @brief Conversion to a matrix
             *
             * @tparam t_Derived    Eigen template parameter
             *
             * @param[out]  output  matrix
             */
            template<class t_Derived>
                void evaluateWithoutInitialization(const Eigen::MatrixBase<t_Derived> & output) const
            {
                Eigen::MatrixBase<t_Derived> & out = const_cast< Eigen::MatrixBase<t_Derived> & > (output);

                for (std::ptrdiff_t j = 0; j < num_blocks_hor_; ++j)
                {
                    std::ptrdiff_t primary_block_col = j * identity_size_;

                    for (std::ptrdiff_t i = j; i < num_blocks_vert_; ++i)
                    {
                        std::ptrdiff_t primary_block_row = i * identity_size_;

                        for(std::ptrdiff_t k = 0; k < identity_size_; ++k)
                        {
                            out.block(  (primary_block_row + k)*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                        (primary_block_col + k)*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM,
                                        EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                        EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM) = (*this)(i, j);
                        }
                    }
                }
            }


        public:
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::DecayedRawMatrix    DecayedRawMatrix;


            /**
             * @brief this<LEFT_LOWER_TRIANGULAR> * BlockMatrix<DIAGONAL>
             *
             * @tparam t_Derived            Eigen parameter
             * @tparam t_DBMMatrixType      raw diagonal block matrix type
             * @tparam t_dbm_block_rows_num number of rows in a block of the diagonal matrix
             * @tparam t_dbm_block_cols_num number of columns in a block of the diagonal matrix
             *
             * @param[out] result result of multiplication
             * @param[in] dbm diagonal block matrix
             */
            template<   class t_Derived,
                        typename t_DBMMatrixType,
                        int t_dbm_block_rows_num,
                        int t_dbm_block_cols_num>
            void multiplyRight( Eigen::PlainObjectBase<t_Derived> &result,
                                const BlockMatrixBase<   t_DBMMatrixType,
                                                         t_dbm_block_rows_num,
                                                         t_dbm_block_cols_num,
                                                         MatrixSparsityType::DIAGONAL> &dbm) const
            {
                EIGENTOOLS_ASSERT(dbm.getBlockRowsNum() == EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM*identity_size_,
                                    "Block sizes do not match.");
                EIGENTOOLS_ASSERT(num_blocks_hor_ == dbm.getNumberOfBlocksVertical(),
                                    "Numbers of blocks do not match.");
                EIGENTOOLS_ASSERT(num_blocks_hor_*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM*identity_size_ == dbm.getNumberOfRows(),
                                    "Sizes of matrices do not match.");

                result.setZero(identity_size_ * matrix_.rows(), dbm.getNumberOfColumns());
                if (num_blocks_hor_ != 0)
                {
                    Eigen::VectorXi indices;

                    indices.resize(identity_size_ * matrix_.rows());

                    for (std::ptrdiff_t i = 0; i < identity_size_; ++i)
                    {
                        for (std::ptrdiff_t j = 0; j < num_blocks_vert_; ++j)
                        {
                            indices.segment(j*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM*identity_size_ + i*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM) =
                                Eigen::VectorXi::LinSpaced( EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                                            i*matrix_.rows() + j*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM,
                                                            i*matrix_.rows() + (j+1)*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM - 1);
                        }
                    }


                    std::ptrdiff_t dbm_block_cols = dbm.getNumberOfColumns() / num_blocks_hor_;

                    for (std::ptrdiff_t i = 0; i < identity_size_; ++i)
                    {
                        for (std::ptrdiff_t j = 0; j < num_blocks_hor_; ++j)
                        {
                            result.block(i*matrix_.rows() + j*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, dbm_block_cols*j, (num_blocks_vert_ - j) * EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, dbm_block_cols).noalias() =
                                column(j, j)
                                *
                                dbm(j).block(i*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM, 0, EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM, dbm_block_cols);
                        }
                    }

                    result = (Eigen::PermutationWrapper<Eigen::VectorXi> (indices)).transpose() * result;
                }
            }


            /**
             * @brief this * Vector
             *
             * @tparam  t_DerivedOutput     Eigen template parameter
             * @tparam  t_Scalar            Eigen template parameter
             * @tparam  t_vector_size       Eigen template parameter
             * @tparam  t_vector_options    Eigen template parameter
             *
             * @param[out] result result of multiplication
             * @param[in] vector
             */
            template<   class t_DerivedOutput,
                        typename t_Scalar,
                        int t_vector_size,
                        int t_vector_options>
                void multiplyRight( Eigen::PlainObjectBase<t_DerivedOutput>      &result,
                                    const Eigen::Matrix<t_Scalar, t_vector_size, 1, t_vector_options> & vector) const
            {
                EIGENTOOLS_DYNAMIC_VECTOR(typename Eigen::PlainObjectBase<t_DerivedOutput>::Scalar)  result_part;
                EIGENTOOLS_DYNAMIC_VECTOR(typename Eigen::PlainObjectBase<t_DerivedOutput>::Scalar)  vector_part;


                result.resize(identity_size_ * matrix_.rows());

                vector_part.resize(matrix_.cols());

                for (std::ptrdiff_t i = 0; i < identity_size_; ++i)
                {
                    for (std::ptrdiff_t j = 0; j < num_blocks_hor_; ++j)
                    {
                        vector_part.segment(j*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM, EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM) =
                            vector.segment(j*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM*identity_size_ + i*EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM, EIGENTOOLS_BLOCKMATRIX_BLOCK_COLS_NUM);
                    }

                    result_part.noalias() = matrix_ * vector_part;

                    for (std::ptrdiff_t j = 0; j < num_blocks_vert_; ++j)
                    {
                        result.segment(j*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM*identity_size_ + i*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM) =
                            result_part.segment(j*EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM, EIGENTOOLS_BLOCKMATRIX_BLOCK_ROWS_NUM);
                    }
                }
            }



            /**
             * @brief Conversion to Matrix
             *
             * @return Matrix
             */
            DecayedRawMatrix   evaluate() const
            {
                DecayedRawMatrix output;
                evaluate(output);
                return(output);
            }



            /**
             * @brief Conversion to a matrix
             *
             * @tparam t_Derived    Eigen template parameter
             *
             * @param[out]  output  matrix
             */
            template<class t_Derived>
                void evaluate(Eigen::PlainObjectBase<t_Derived> & output) const
            {
                output.setZero(identity_size_ * matrix_.rows(),
                               identity_size_ * matrix_.cols());
                evaluateWithoutInitialization(output);
            }


            /**
             * @brief Conversion to a matrix
             *
             * @tparam t_Derived    Eigen template parameter
             * @tparam t_rows       Eigen template parameter
             * @tparam t_cols       Eigen template parameter
             * @tparam t_flag       Eigen template parameter
             *
             * @param[out] output matrix block of appropriate size
             */
            template<class t_Derived, int t_rows, int t_cols, bool t_flag>
                void evaluate(Eigen::Block<t_Derived, t_rows, t_cols, t_flag> output) const
            {
                output.setZero();
                evaluateWithoutInitialization(output);
            }


            /**
             * @brief Constructor
             *
             * @param[in] matrix
             * @param[in] identity_size
             * @param[in] block_rows_num    number of rows in a block if    t_block_rows_num = MatrixBlockSizeType::DYNAMIC
             * @param[in] block_cols_num    number of cols in a block if    t_block_cols_num = MatrixBlockSizeType::DYNAMIC
             */
            BlockKroneckerProductBase(  const DecayedRawMatrix & matrix,
                                        const std::ptrdiff_t identity_size = 1,
                                        const std::ptrdiff_t block_rows_num = MatrixBlockSizeType::UNDEFINED,
                                        const std::ptrdiff_t block_cols_num = MatrixBlockSizeType::UNDEFINED) :
                EIGENTOOLS_PARENT_CLASS_SHORTHAND(matrix, block_rows_num, block_cols_num)
            {
                EIGENTOOLS_ASSERT(  identity_size > 1,
                                    "Identity size cannot be less than 2.");
                identity_size_ = identity_size;
            }
    };
#undef EIGENTOOLS_PARENT_CLASS_SHORTHAND



#define EIGENTOOLS_PARENT_CLASS_SHORTHAND BlockMatrixBase<  const typename TypeWithoutConst<t_MatrixType>::Type, \
                                                            1, 1, t_sparsity_type>
    /**
     * @brief Represents block kronecker product "Identity(size) [X] Matrix",
     * without computing it explicitly.
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_sparsity_type  sparsity type
     */
    template<   class t_MatrixType,
                MatrixSparsityType::Type t_sparsity_type>
        class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockKroneckerProductBase<t_MatrixType, 1, 1, t_sparsity_type>
            : protected EIGENTOOLS_PARENT_CLASS_SHORTHAND
    {
        private:
            std::ptrdiff_t     identity_size_;


        protected:
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::operator();
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::matrix_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_hor_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_vert_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::multiplyRight;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::column;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_rows_num_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_cols_num_;


            /**
             * @brief Conversion to a matrix
             *
             * @tparam t_Derived    Eigen template parameter
             *
             * @param[out]  output  matrix
             */
            template<class t_Derived>
                void evaluateWithoutInitialization(const Eigen::MatrixBase<t_Derived> & output) const
            {
                Eigen::MatrixBase<t_Derived> & out = const_cast< Eigen::MatrixBase<t_Derived> & > (output);

                for (std::ptrdiff_t j = 0; j < num_blocks_hor_; ++j)
                {
                    std::ptrdiff_t primary_block_col = j * identity_size_;

                    for (std::ptrdiff_t i = 0; i < num_blocks_vert_; ++i)
                    {
                        std::ptrdiff_t primary_block_row = i * identity_size_;

                        for(std::ptrdiff_t k = 0; k < identity_size_; ++k)
                        {
                            out(primary_block_row + k,
                                primary_block_col + k) = (*this)(i, j);
                        }
                    }
                }
            }


        public:
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::DecayedRawMatrix    DecayedRawMatrix;


            /**
             * @brief this * BlockMatrix<DIAGONAL>
             *
             * @tparam t_Derived            Eigen parameter
             * @tparam t_DBMMatrixType      raw diagonal block matrix type
             * @tparam t_dbm_block_rows_num number of rows in a block of the diagonal matrix
             * @tparam t_dbm_block_cols_num number of columns in a block of the diagonal matrix
             *
             * @param[out] result result of multiplication
             * @param[in] dbm diagonal block matrix
             */
            template<   class t_Derived,
                        typename t_DBMMatrixType,
                        int t_dbm_block_rows_num,
                        int t_dbm_block_cols_num>
                void multiplyRight( Eigen::PlainObjectBase<t_Derived>   &result,
                                    const BlockMatrixBase<  t_DBMMatrixType,
                                                            t_dbm_block_rows_num,
                                                            t_dbm_block_cols_num,
                                                            MatrixSparsityType::DIAGONAL> &dbm) const
            {
                EIGENTOOLS_ASSERT(dbm.getBlockRowsNum() == identity_size_,
                                    "Block sizes do not match.");
                EIGENTOOLS_ASSERT(num_blocks_hor_ == dbm.getNumberOfBlocksVertical(),
                                    "Numbers of blocks do not match.");
                EIGENTOOLS_ASSERT(num_blocks_hor_*identity_size_ == dbm.getNumberOfRows(),
                                    "Sizes of matrices do not match.");


                BlockMatrixBase<Eigen::PlainObjectBase<t_Derived> &,
                                t_dbm_block_rows_num,
                                t_dbm_block_cols_num,
                                MatrixSparsityType::NONE >   result_bmi(result);

                result_bmi.resize(num_blocks_vert_, dbm.getNumberOfBlocksHorizontal());

                if (num_blocks_hor_ != 0)
                {
                    for (std::ptrdiff_t j = 0; j < dbm.getNumberOfBlocksHorizontal(); ++j)
                    {
                        for (std::ptrdiff_t i = 0; i < num_blocks_vert_; ++i)
                        {
                            // dbm.getNumberOfBlocksHorizontal() = num_blocks_hor_
                            result_bmi(i,j) = EIGENTOOLS_PARENT_CLASS_SHORTHAND::operator()(i,j) * dbm(j);
                        }
                    }
                }
            }



            /**
             * @brief this * Vector
             *
             * @tparam  t_DerivedOutput     Eigen template parameter
             * @tparam  t_Scalar            Eigen template parameter
             * @tparam  t_vector_size       Eigen template parameter
             * @tparam  t_vector_options    Eigen template parameter
             *
             * @param[out] result result of multiplication
             * @param[in] vector
             */
            template<   class t_DerivedOutput,
                        typename t_Scalar,
                        int t_vector_size,
                        int t_vector_options>
                void multiplyRight( Eigen::PlainObjectBase<t_DerivedOutput>      &result,
                                    const Eigen::Matrix<t_Scalar, t_vector_size, 1, t_vector_options> & vector) const
            {
                result.setZero(identity_size_ * matrix_.rows());

                for (std::ptrdiff_t j = 0; j < num_blocks_hor_; ++j)
                {
                    for (std::ptrdiff_t i = 0; i < num_blocks_vert_; ++i)
                    {
                        result.segment(i*identity_size_, identity_size_)
                            +=  EIGENTOOLS_PARENT_CLASS_SHORTHAND::operator()(i,j)
                                *
                                vector.segment(j*identity_size_, identity_size_);
                    }
                }
            }



            /**
             * @brief Conversion to Matrix
             *
             * @return Matrix
             */
            DecayedRawMatrix   evaluate() const
            {
                DecayedRawMatrix output;
                evaluate(output);
                return(output);
            }



            /**
             * @brief Conversion to a matrix
             *
             * @tparam t_Derived    Eigen template parameter
             *
             * @param[out]  output  matrix
             */
            template<class t_Derived>
                void evaluate(Eigen::PlainObjectBase<t_Derived> & output) const
            {
                output.setZero(identity_size_ * matrix_.rows(),
                               identity_size_ * matrix_.cols());
                evaluateWithoutInitialization(output);
            }


            /**
             * @brief Conversion to a matrix
             *
             * @tparam t_Derived    Eigen template parameter
             * @tparam t_rows       Eigen template parameter
             * @tparam t_cols       Eigen template parameter
             * @tparam t_flag       Eigen template parameter
             *
             * @param[out] output matrix block of appropriate size
             */
            template<class t_Derived, int t_rows, int t_cols, bool t_flag>
                void evaluate(Eigen::Block<t_Derived, t_rows, t_cols, t_flag> output) const
            {
                output.setZero();
                evaluateWithoutInitialization(output);
            }



            /**
             * @brief Constructor
             *
             * @param[in] matrix
             * @param[in] identity_size
             * @param[in] block_rows_num    number of rows in a block if    t_block_rows_num = MatrixBlockSizeType::DYNAMIC
             * @param[in] block_cols_num    number of cols in a block if    t_block_cols_num = MatrixBlockSizeType::DYNAMIC
             */
            BlockKroneckerProductBase(  const DecayedRawMatrix & matrix,
                                        const std::ptrdiff_t identity_size = 1,
                                        const std::ptrdiff_t block_rows_num = MatrixBlockSizeType::UNDEFINED,
                                        const std::ptrdiff_t block_cols_num = MatrixBlockSizeType::UNDEFINED) :
                EIGENTOOLS_PARENT_CLASS_SHORTHAND(matrix, block_rows_num, block_cols_num)
            {
                EIGENTOOLS_ASSERT(  identity_size > 1,
                                    "Identity size cannot be less than 2.");
                identity_size_ = identity_size;
            }
    };
#undef EIGENTOOLS_PARENT_CLASS_SHORTHAND


#define EIGENTOOLS_PARENT_CLASS_SHORTHAND BlockMatrixBase<  const typename TypeWithoutConst<t_MatrixType>::Type, \
                                                            1, 1, MatrixSparsityType::LEFT_LOWER_TRIANGULAR>
    /**
     * @brief Represents block kronecker product "Identity(size) [X] Matrix",
     * without computing it explicitly.
     *
     * @tparam t_MatrixType     type of raw matrix
     */
    template<class t_MatrixType>
        class EIGENTOOLS_VISIBILITY_ATTRIBUTE BlockKroneckerProductBase<t_MatrixType, 1, 1, MatrixSparsityType::LEFT_LOWER_TRIANGULAR>
            : protected EIGENTOOLS_PARENT_CLASS_SHORTHAND
    {
        private:
            std::ptrdiff_t     identity_size_;


        protected:
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::operator();
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::matrix_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_hor_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::num_blocks_vert_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::multiplyRight;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::column;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_rows_num_;
            using EIGENTOOLS_PARENT_CLASS_SHORTHAND::block_cols_num_;


            /**
             * @brief Conversion to a matrix
             *
             * @tparam t_Derived    Eigen template parameter
             *
             * @param[out]  output  matrix
             */
            template<class t_Derived>
                void evaluateWithoutInitialization(const Eigen::MatrixBase<t_Derived> & output) const
            {
                Eigen::MatrixBase<t_Derived> & out = const_cast< Eigen::MatrixBase<t_Derived> & > (output);

                for (std::ptrdiff_t j = 0; j < num_blocks_hor_; ++j)
                {
                    std::ptrdiff_t primary_block_col = j * identity_size_;

                    for (std::ptrdiff_t i = j; i < num_blocks_vert_; ++i)
                    {
                        std::ptrdiff_t primary_block_row = i * identity_size_;

                        for(std::ptrdiff_t k = 0; k < identity_size_; ++k)
                        {
                            out(primary_block_row + k,
                                primary_block_col + k) = (*this)(i, j);
                        }
                    }
                }
            }


        public:
            typedef typename EIGENTOOLS_PARENT_CLASS_SHORTHAND::DecayedRawMatrix    DecayedRawMatrix;


            /**
             * @brief this<LEFT_LOWER_TRIANGULAR> * BlockMatrix<DIAGONAL>
             *
             * @tparam t_Derived            Eigen parameter
             * @tparam t_DBMMatrixType      raw diagonal block matrix type
             * @tparam t_dbm_block_rows_num number of rows in a block of the diagonal matrix
             * @tparam t_dbm_block_cols_num number of columns in a block of the diagonal matrix
             *
             * @param[out] result result of multiplication
             * @param[in] dbm diagonal block matrix
             */
            template<   class t_Derived,
                        typename t_DBMMatrixType,
                        int t_dbm_block_rows_num,
                        int t_dbm_block_cols_num>
                void multiplyRight( Eigen::PlainObjectBase<t_Derived>   &result,
                                    const BlockMatrixBase<   t_DBMMatrixType,
                                                             t_dbm_block_rows_num,
                                                             t_dbm_block_cols_num,
                                                             MatrixSparsityType::DIAGONAL> &dbm) const
            {
                EIGENTOOLS_ASSERT(dbm.getBlockRowsNum() == identity_size_,
                                    "Block sizes do not match.");
                EIGENTOOLS_ASSERT(num_blocks_hor_ == dbm.getNumberOfBlocksVertical(),
                                    "Numbers of blocks do not match.");
                EIGENTOOLS_ASSERT(num_blocks_hor_*identity_size_ == dbm.getNumberOfRows(),
                                    "Sizes of matrices do not match.");



                BlockMatrixBase<Eigen::PlainObjectBase<t_Derived>   &,
                                t_dbm_block_rows_num,
                                t_dbm_block_cols_num,
                                MatrixSparsityType::NONE >   result_bmi(result);

                result_bmi.setZero(num_blocks_vert_, dbm.getNumberOfBlocksHorizontal());

                if (num_blocks_hor_ != 0)
                {
                    for (std::ptrdiff_t j = 0; j < dbm.getNumberOfBlocksHorizontal(); ++j)
                    {
                        for (std::ptrdiff_t i = j; i < num_blocks_vert_; ++i)
                        {
                            // dbm.getNumberOfBlocksHorizontal() = num_blocks_hor_
                            result_bmi(i,j) = EIGENTOOLS_PARENT_CLASS_SHORTHAND::operator()(i,j) * dbm(j);
                        }
                    }
                }
            }



            /**
             * @brief BlockMatrix<DIAGONAL> * this<LEFT_LOWER_TRIANGULAR>
             *
             * @tparam t_Derived            Eigen template parameter
             * @tparam t_DBMMatrixType      raw diagonal block matrix type
             * @tparam t_dbm_block_rows_num number of rows in a block of the diagonal matrix
             * @tparam t_dbm_block_cols_num number of columns in a block of the diagonal matrix
             *
             * @param[out] result result of multiplication
             * @param[in] dbm diagonal block matrix
             */
            template<   class t_Derived,
                        typename t_DBMMatrixType,
                        int t_dbm_block_rows_num,
                        int t_dbm_block_cols_num>
                void multiplyLeft(  Eigen::PlainObjectBase<t_Derived>    &result,
                                    const BlockMatrixBase<   t_DBMMatrixType,
                                                             t_dbm_block_rows_num,
                                                             t_dbm_block_cols_num,
                                                             MatrixSparsityType::DIAGONAL> &dbm) const
            {
                EIGENTOOLS_ASSERT(dbm.getBlockRowsNum() == identity_size_,
                                    "Block sizes do not match.");
                EIGENTOOLS_ASSERT(num_blocks_hor_ == dbm.getNumberOfBlocksVertical(),
                                    "Numbers of blocks do not match.");
                EIGENTOOLS_ASSERT(num_blocks_hor_*identity_size_ == dbm.getNumberOfRows(),
                                    "Sizes of matrices do not match.");


                BlockMatrixBase<    Eigen::PlainObjectBase<t_Derived> &,
                                    t_dbm_block_rows_num,
                                    t_dbm_block_cols_num,
                                    MatrixSparsityType::NONE >   result_bmi(result);

                result_bmi.setZero(num_blocks_vert_, dbm.getNumberOfBlocksHorizontal());

                if (num_blocks_hor_ != 0)
                {
                    for (std::ptrdiff_t j = 0; j < num_blocks_hor_; ++j)
                    {
                        for (std::ptrdiff_t i = j; i < dbm.getNumberOfBlocksVertical(); ++i)
                        {
                            result_bmi(i,j) = dbm(i) * EIGENTOOLS_PARENT_CLASS_SHORTHAND::operator()(i,j);
                        }
                    }
                }
            }



            /**
             * @brief Conversion to Matrix
             *
             * @return Matrix
             */
            DecayedRawMatrix   evaluate() const
            {
                DecayedRawMatrix output;
                evaluate(output);
                return(output);
            }



            /**
             * @brief Conversion to a matrix
             *
             * @tparam t_Derived    Eigen template parameter
             *
             * @param[out]  output  matrix
             */
            template<class t_Derived>
                void evaluate(Eigen::PlainObjectBase<t_Derived> & output) const
            {
                output.setZero(identity_size_ * matrix_.rows(),
                               identity_size_ * matrix_.cols());
                evaluateWithoutInitialization(output);
            }


            /**
             * @brief Conversion to a matrix
             *
             * @tparam t_Derived    Eigen template parameter
             * @tparam t_rows       Eigen template parameter
             * @tparam t_cols       Eigen template parameter
             * @tparam t_flag       Eigen template parameter
             *
             * @param[out] output matrix block of appropriate size
             */
            template<class t_Derived, int t_rows, int t_cols, bool t_flag>
                void evaluate(Eigen::Block<t_Derived, t_rows, t_cols, t_flag> output) const
            {
                output.setZero();
                evaluateWithoutInitialization(output);
            }


            /**
             * @brief Constructor
             *
             * @param[in] matrix
             * @param[in] identity_size
             * @param[in] block_rows_num    number of rows in a block if    t_block_rows_num = MatrixBlockSizeType::DYNAMIC
             * @param[in] block_cols_num    number of cols in a block if    t_block_cols_num = MatrixBlockSizeType::DYNAMIC
             */
            BlockKroneckerProductBase(  const DecayedRawMatrix & matrix,
                                        const std::ptrdiff_t identity_size = 1,
                                        const std::ptrdiff_t block_rows_num = MatrixBlockSizeType::UNDEFINED,
                                        const std::ptrdiff_t block_cols_num = MatrixBlockSizeType::UNDEFINED) :
                EIGENTOOLS_PARENT_CLASS_SHORTHAND(matrix, block_rows_num, block_cols_num)
            {
                EIGENTOOLS_ASSERT(  identity_size > 1,
                                    "Identity size cannot be less than 2.");
                identity_size_ = identity_size;
            }
    };
#undef EIGENTOOLS_PARENT_CLASS_SHORTHAND


    // ===========================================================================
    // ===========================================================================
    // ===========================================================================


    /**
     * @addtogroup BlockMatrixOperators
     * @{
     */


    /**
     * @brief BlockKroneckerProduct * Vector
     *
     * @tparam t_MatrixType     type of raw matrix
     * @tparam t_block_rows_num number of rows in one block
     * @tparam t_block_cols_num number of columns in one block
     * @tparam t_sparsity_type  sparsity type
     * @tparam t_Scalar         vector type
     * @tparam t_vector_size    vector size
     * @tparam t_vector_options vector options
     *
     * @param[in] bm     block matrix
     * @param[in] vector vector
     *
     * @return result of multiplication
     */
    template<   class                       t_MatrixType,
                int              t_block_rows_num,
                int              t_block_cols_num,
                MatrixSparsityType::Type    t_sparsity_type,
                typename                    t_Scalar,
                int                         t_vector_size,
                int                         t_vector_options >
        EIGENTOOLS_DYNAMIC_VECTOR(typename TypeDecayed<t_MatrixType>::Type::Scalar)
        EIGENTOOLS_VISIBILITY_ATTRIBUTE
            operator* ( const BlockKroneckerProductBase<t_MatrixType,
                                                        t_block_rows_num,
                                                        t_block_cols_num,
                                                        t_sparsity_type> & bm,
                        const Eigen::Matrix<t_Scalar,
                                            t_vector_size,
                                            1,
                                            t_vector_options> & vector)
    {
        EIGENTOOLS_DYNAMIC_VECTOR(typename TypeDecayed<t_MatrixType>::Type::Scalar) result;
        bm.multiplyRight(result, vector);
        return (result);
    }


    /**
     * @brief BlockKroneckerProduct * BlockMatrix
     *
     * @tparam t_right_MatrixType       type of raw matrix
     * @tparam t_left_block_rows_num    number of rows in one block
     * @tparam t_left_block_cols_num    number of columns in one block
     * @tparam t_left_sparsity_type     sparsity type
     * @tparam t_right_MatrixType       type of raw matrix
     * @tparam t_right_block_rows_num   number of rows in one block
     * @tparam t_right_block_cols_num   number of columns in one block
     * @tparam t_right_sparsity_type    sparsity type
     *
     * @param[in] left BlockKroneckerProduct
     * @param[in] right BlockMatrix
     *
     * @return result of multiplication
     */
    template<   typename                    t_left_MatrixType,
                int              t_left_block_rows_num,
                int              t_left_block_cols_num,
                MatrixSparsityType::Type    t_left_sparsity_type,
                typename                    t_right_MatrixType,
                int              t_right_block_rows_num,
                int              t_right_block_cols_num,
                MatrixSparsityType::Type    t_right_sparsity_type>
        EIGENTOOLS_DYNAMIC_MATRIX(typename TypeDecayed<t_left_MatrixType>::Type::Scalar)
        EIGENTOOLS_VISIBILITY_ATTRIBUTE
            operator* ( const BlockKroneckerProductBase<t_left_MatrixType,
                                                        t_left_block_rows_num,
                                                        t_left_block_cols_num,
                                                        t_left_sparsity_type> & left,
                        const BlockMatrixBase<  t_right_MatrixType,
                                                t_right_block_rows_num,
                                                t_right_block_cols_num,
                                                t_right_sparsity_type> & right)
    {
        EIGENTOOLS_DYNAMIC_MATRIX(typename TypeDecayed<t_left_MatrixType>::Type::Scalar)  result;
        left.multiplyRight(result, right);
        return (result);
    }



    /**
     * @anchor eigentools_bkp_by_bm
     * @brief BlockKroneckerProduct * BlockMatrix
     *
     * @tparam t_left_MatrixType        type of raw matrix
     * @tparam t_left_block_rows_num    number of rows in one block
     * @tparam t_left_block_cols_num    number of columns in one block
     * @tparam t_left_sparsity_type     sparsity type
     * @tparam t_right_MatrixType       type of raw matrix
     * @tparam t_right_block_rows_num   number of rows in one block
     * @tparam t_right_block_cols_num   number of columns in one block
     * @tparam t_right_sparsity_type    sparsity type
     *
     * @param[in] left BlockKroneckerProduct
     * @param[in] right BlockMatrix
     *
     * @return result of multiplication
     */
    template<   typename                    t_left_MatrixType,
                int              t_left_block_rows_num,
                int              t_left_block_cols_num,
                MatrixSparsityType::Type    t_left_sparsity_type,
                typename                    t_right_MatrixType,
                int              t_right_block_rows_num,
                int              t_right_block_cols_num,
                MatrixSparsityType::Type    t_right_sparsity_type>
        EIGENTOOLS_DYNAMIC_MATRIX( typename TypeDecayed<t_left_MatrixType>::Type::Scalar )
        EIGENTOOLS_VISIBILITY_ATTRIBUTE
            operator* ( const BlockMatrixBase<  t_left_MatrixType,
                                                t_left_block_rows_num,
                                                t_left_block_cols_num,
                                                t_left_sparsity_type> & left,
                        const BlockKroneckerProductBase<t_right_MatrixType,
                                                        t_right_block_rows_num,
                                                        t_right_block_cols_num,
                                                        t_right_sparsity_type> & right)
    {
        EIGENTOOLS_DYNAMIC_MATRIX( typename TypeDecayed<t_left_MatrixType>::Type::Scalar ) result;
        right.multiplyLeft(result, left);
        return (result);
    }


    // BlockMatrixOperators
    /**
     * @}
     */
} // etools
