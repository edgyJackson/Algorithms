#ifndef _TPF_MATRIX_H
#define _TPF_MATRIX_H

#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <algorithm>
#include <cassert>
#include <utility>
#include <type_traits>

#include "tpf_type_name.h"
#include "include\tpf_random.h"

namespace tpf
{
    template<typename ElementType>
    class matrix
    {
        public:

        using element_t = ElementType;
        using dimension_t = std::vector<size_t>;
        using array_t = std::vector<element_t>;

    #ifdef VERBOSE
        
        inline static int CopyConstructor = 0;
        inline static int CopyAssignment = 0;
        inline static int MoveConstructor = 0;
        inline static int MoveAssignment = 0;

        static void copy_constructor()
        {
            ++CopyConstructor;
            std::cout << "Copy Constructor: "<< CopyConstructor<<std::endl;
        }

        static void copy_assignment()
        {
            ++CopyAssignment;
            std::cout << "Copy Assignment: "<< CopyAssignment<<std::endl;
        }

        static void move_constructor()
        {
            ++MoveConstructor;
            std::cout << "Move Constructor: "<< MoveConstructor << std::endl;
        }

        static void move_assignment()
        {
            ++MoveAssignment;
            std::cout  << "Move Assignment: "<< MoveAssignment << std::endl;
        }

        #define CopyConstructorCalled() copy_constructor()
        #define CopyAssignmentCalled() copy_assignment()
        #define MoveConstructorCalled() move_constructor()
        #define MoveAssignmentCalled() move_assignment()

    #else

        #define CopyConstructorCalled()
        #define CopyAssignmentCalled()
        #define MoveConstructorCalled()
        #define MoveAssignmentCalled()

    #endif // end of VERBOSE

        private:
        dimension_t m_d; // dimensions of the array or matrix
        dimension_t m_m; // multipliers for indexing
        array_t m_e; // the elements of the matrix

        void allocate_array()
        {
            // m[0], m[1], m[2] ... these multipliers

            size_t rank = this->m_d.size();
            this->m_m.resize(rank);
            this->m_m[0] = 1;
            for(size_t n = rank-1; n >=1; --n)
            {
                this->m_m[n] = this->m_m[0];  //     m[n] = m[0];
                this->m_m[0] *= this->m_d[n]; //     m[0] *= d[n];
            }

            // total number of elements = d0 * m0 = d0 * (d1*d2*d3)
            // total = d0 * m0
            size_t total = this->m_d[0] * this->m_m[0];
            assert(total >= 1);

            this->m_e.resize(total);
            // initialize();
        }
        
        void invariant()
        {
            this->m_d.resize(1);
            this->m_d[0]=1;
            this->m_m.resize(1);
            this->m_m[0]=1;
            this->m_e.resize(1);
            this->m_e[0]=element_t{};
        }

        public:

        template<typename T = element_t>
        void initialize(T v = T{})
        {
            size_t total = this->m_e.size();
            for(size_t n = 0; n < total; ++n)
                this->m_e[n] = v;
        }

        matrix& generate(random_t<ElementType>& rnd)
        {
            for(auto& e: this->m_e) e = rnd();
            return *this;
        }

        matrix()
        {
            // more settings here
            invariant();
        }

        template<typename T, typename... Types>
        static decltype(auto) get_head(T&& first, Types&&... tails)
        {
            return std::move(first);
        }    

        std::pair<size_t, size_t> get_dims()
        {
            return { this->m_d[0], this->m_d[1] };
        }
        
        template<typename DimType, typename... DimTypes>
        matrix(const DimType& dim1, const DimTypes&... dims)
        {
            using ncnr_t = tpf::remove_const_reference_t<DimType>;
            using ele_t = tpf::element_t<ncnr_t>;
           
            if constexpr(tpf::is_scalar_v<ncnr_t>)
            {
                // d0, d1, d2, ...
                this->m_d = { static_cast<size_t>(dim1), static_cast<size_t>(dims)...};   
                
                allocate_array();      
            }
            else if constexpr(tpf::is_iterator_v<ncnr_t>)
            {
                using ele_t = tpf::iterator_value_type<ncnr_t>;

                if constexpr(std::is_same_v<ele_t, element_t>)
                {
                    this->m_e = {dim1, dims...};
                    this->m_d={this->m_e.size()};
                    this->m_m={(size_t)1};
                }
                else
                {
                    // optimize later
                    std::vector<ele_t> v = {dim1, dims...};
                    size_t size = v.size();

                    this->m_e.resize(size);
                    
                    for(size_t i=0; i < size; ++i)
                       this->m_e[i] = (element_t)v[i];
                    
                    this->m_d = {size};
                    this->m_m = {(size_t)1};
                }
            }
            else
            {
                if(dim1.empty())
                    this->invariant();
                else
                {
                    if constexpr(std::is_same_v<ele_t, element_t>)
                    {
                        this->m_e = { dim1.cbegin(), dim1.cend() };
                        this->m_d = {this->m_e.size()};
                        this->m_m = {(size_t)1};
                    }
                    else if constexpr(std::is_assignable_v<element_t, ele_t>)
                    {
                        size_t size = dim1.size();
                        this->m_e.resize(size);

                        auto itr = this->m_e.begin();
                        for(const auto& e: dim1) 
                        {
                            *itr = e; ++itr;
                        }
                    
                        this->m_d = { size };
                        this->m_m = {(size_t)1};
                    }
                    else if constexpr(std::is_constructible_v<element_t, ele_t>)
                    {
                        size_t size = dim1.size();
                        this->m_e.resize(size);

                        auto itr = this->m_e.begin();

                        for(const auto& e: dim1) 
                        {
                            *itr = { static_cast<element_t>(e) }; 
                            ++itr;
                        }
                        
                        this->m_d = { size };
                        this->m_m = {(size_t)1};
                    }
                    else
                    {
                        //  static_assert(false, "Failed to assign...");
                    }
                }
            }
        }
        
        matrix(const dimension_t& dims)
        {
            this->m_d = dims;
            allocate_array();
        }

        // copy constructor
        matrix(const matrix& right): m_d{ right.m_d },
        m_m{ right.m_m}, m_e{ right.m_e }
        {
            CopyConstructorCalled();
        }

        // copy assignment operator
        matrix& operator=(const matrix& right)
        {
            CopyAssignmentCalled();

            if(this != std::addressof(right))
            {
                this->m_d = right.m_d;
                this->m_m = right.m_m;
                this->m_e = right.m_e;
            }

            return *this;
        }

        template<typename Type,
            typename ncnr_t = tpf::remove_const_reference_t<Type>,
            typename ele_t = tpf::element_t<ncnr_t>>
        std::enable_if_t<tpf::is_not_scalar_v<ncnr_t> &&
            !std::is_same_v<ncnr_t, matrix<ElementType>>, matrix&>
            operator=(Type&& arg)
        {
            MoveAssignmentCalled();

            if(arg.empty())
                this->invariant();
            else
            {
                if constexpr(std::is_rvalue_reference_v<decltype(arg)>
                    && std::is_same_v<ncnr_t, array_t>)
                {
                    this->m_e = std::move(arg);
                    this->m_d = { this->m_e.size() };
                    this->m_m = { (size_t)1};
                }
                else
                {
                    if constexpr(std::is_same_v<ele_t, element_t>
                        && std::is_same_v<ncnr_t, array_t>)
                    {
                        this->m_e = arg;
                        this->m_d = { this->m_e.size() };
                        this->m_m = {(size_t)1};
                    }
                    else if constexpr(std::is_assignable_v<element_t, ele_t>)
                    {
                        size_t size = arg.size();
                        this->m_e.resize(size);

                        auto itr = this->m_e.begin();

                        for(const auto& e: arg) 
                        {
                            *itr = e; ++itr;
                        }

                        this->m_d = { size };
                        this->m_m = {(size_t)1}; 
                    }
                    else if constexpr(std::is_constructible_v<element_t, ele_t>)
                    {
                        size_t size = arg.size();
                        this->m_e.resize(size);

                        auto itr = this->m_e.begin();

                        for(const auto& e: arg) 
                        {
                            *itr = { static_cast<element_t>(e) }; 
                            ++itr;
                        }
                    
                        this->m_d = { size };
                        this->m_m = {(size_t)1}; 
                    }
                    else
                    {
                        // static_assert(false, "Failed to assign...");
                    }
                }
            }

            return *this;
        }

        // move constructor
        matrix(matrix&& right): m_d{std::move(right.m_d)},
        m_m{std::move(right.m_m)}, m_e{std::move(right.m_e)}
        {
            right.invariant();
            MoveConstructorCalled();
        } 

        // move assignment
        matrix& operator=(matrix&& right)
        {
            MoveAssignmentCalled();

            if(this != std::addressof(right))
            {
                this->m_d = std::move(right.m_d);
                this->m_m = std::move(right.m_m);
                this->m_e = std::move(right.m_e);
                right.invariant();
            }

            return *this;
        } 
        
        array_t& elements() { return this->m_e; }

        matrix& swap(matrix& m) noexcept
        {
            this->m_d.swap(m.m_d);
            this->m_m.swap(m.m_m);
            this->m_e.swap(m.m_e);

            return *this;
        }

        matrix transpose() const
        {
            assert(this->m_d.size() == 2);

            matrix m{this->m_d[1], this->m_d[0]};

            for(size_t i = 0; i < this->m_d[0]; ++i)
                for(size_t j = 0; j < this->m_d[1]; ++j)
                    m(j, i) = this->operator()(i, j);

            return m;
        }
        
        template<typename DimType, typename... DimTypes>
        void resize(DimType dim1, DimTypes... dims)
        {
            this->m_d = { static_cast<size_t>(dim1), static_cast<size_t>(dims)...};  
            allocate_array();  
        }

        // returns total number of elements
        // of the matrix
        size_t total() const
        {
            return this->m_e.size();
        }

        size_t size() const
        {
            return this->m_e.size();
        }

        // returns the rank of the matrix
        size_t rank() const
        {
            return this->m_d.size();
        }

        // returns dimension of the matrix
        template<typename DimIndexType>
        const size_t& dim(DimIndexType dim_idx) const
        {
            return this->m_d[(size_t)dim_idx];
        }

        template<typename IndexType, typename... IndexTypes>
        size_t calculate_address(IndexType idx1, IndexTypes... idxs) const
        {
            dimension_t i = {static_cast<size_t>(idx1), static_cast<size_t>(idxs)...};
            assert(i.size() == this->m_d.size());

            size_t address = 0, rank = this->m_d.size();

            for(size_t n = 0; n<rank; ++n)
            {
                // e(i0, i1, i2, i3) = m0 * i0 + m1 * i1 + m2 * i2 + m3*i3
                address += this->m_m[n] * i[n];
            }

            return address;
        }

        template<typename IndexType, typename... IndexTypes>
        element_t& operator() (IndexType idx1, IndexTypes... idxs)
        {
            return this->m_e[calculate_address(idx1, idxs...)];
        }

        template<typename IndexType, typename... IndexTypes>
        const element_t& operator() (IndexType idx1, IndexTypes... idxs) const
        {
        return this->m_e[calculate_address(idx1, idxs...)];
        }

        template<typename IndexType>
        element_t& operator[](IndexType idx)
        {
            return this->m_e[(size_t)idx];
        }
        
        template<typename IndexType>
        const element_t& operator[](IndexType idx) const
        {
            return this->m_e[(size_t)idx];
        }

        friend std::ostream& operator<<(std::ostream& os, const matrix& m)
        {
            size_t total = m.m_e.size()-1;
            size_t rank_1 = m.m_d.size()-1;

            os<<m.m_e[0]; // display first element

            for(size_t i = 1; i < total; ++i)
            {
                bool bNoNewLine = true;
                for(size_t j=0; j<rank_1; ++j)
                {
                    if(i % m.m_m[j] == 0)
                    {
                        os<<"\n";
                        bNoNewLine = false;   
                    }
                }

                if(bNoNewLine) os<<", ";

                os<<m.m_e[i];
            }

            if(total>0)
            {
                os<<", ";
                os<<m.m_e[total];
            }

            return os;
        }

    private:
        template<typename T>
        struct is_matrix_st
        {
            static constexpr bool value = false;
        };

        template<typename T>
        struct is_matrix_st<matrix<T>>
        {
            static constexpr bool value = true;
        };

    public:

        void mul(const matrix& left, const matrix& right)
        {
            // matrix matrix multiplication
            assert(left.m_d.size() == 2 && right.m_d.size() == 2 
                && left.m_d[1] == right.m_d[0]);

            size_t dim0 = left.m_d[0];
            size_t dim1 = right.m_d[1];
            size_t N = left.m_d[1];

            this->resize(dim0, dim1);

            for(size_t i = 0; i < dim0; ++i)
                for(size_t j = 0; j < dim1; ++j)
                {
                    this->operator()(i, j) = element_t{};

                    for(size_t k = 0; k < N; ++k)
                        this->operator()(i, j) += left(i, k) * right(k, j);
                }
        }

        // this is to pass any type of 
        // parameter values, rvalue, xvalue, lvalue
        // forwarding reference or universal reference
        template<typename RType>
        matrix& operator*=(RType&& right)
        {
            using rtype_t = tpf::remove_const_reference_t<RType>;

            if constexpr(is_matrix_st<rtype_t>::value)
            {
                // matrix matrix multiplication
                assert(this->m_d.size() == 2 && right.m_d.size() == 2 
                    && this->m_d[1] == right.m_d[0]);

                size_t dim0 = this->m_d[0];
                size_t dim1 = right.m_d[1];
                size_t N = this->m_d[1];

                matrix m{dim0, dim1};
                
                for(size_t i = 0; i < dim0; ++i)
                    for(size_t j = 0; j < dim1; ++j)
                    {
                        m(i, j) = element_t{};

                        for(size_t k = 0; k < N; ++k)
                            m(i, j) += this->operator()(i, k) * right(k, j);
                    }

                *this = std::move(m);
                return *this;
            }
            else
            {
                // scalar matrix multiplication
                size_t total = this->m_e.size();
        
                for(size_t n = 0; n < total; ++n)
                    this->m_e[n] *= (element_t) right;

                return *this;
            }
        }

        // this is to pass any type of 
        // parameter values, rvalue, xvalue, lvalue
        // forwarding reference or universal reference
        template<typename RType>
        matrix operator*=(RType&& right) const
        {
            using rtype_t = tpf::remove_const_reference_t<RType>;

            if constexpr(is_matrix_st<rtype_t>::value)
            {
                // matrix matrix multiplication
                assert(this->m_d.size() == 2
                && right.m_d.size() == 2
                && this->m_d[1] == right.m_d[0]);

                size_t dim0 = this->m_d[0];
                size_t dim1 = right.m_d[1];
                size_t N = this->m_d[1];

                matrix m{dim0, dim1};
               
                for(size_t i = 0; i < dim0; ++i)
                    for(size_t j = 0; j < dim1; ++j)
                    {
                        m(i, j) = element_t{};

                        for(size_t k = 0; k < N; ++k)
                            m(i, j) += this->operator()(i, k) * right(k, j);
                    }

               return m;
            }
            else
            {
                // scalar matrix multiplication
                size_t total = this->m_e.size();
                auto m = *this;
        
                for(size_t n = 0; n < total; ++n)
                    m.m_e[n] *= (element_t) right;

                return m;
            }
        }

        // this is to pass any type of 
        // parameter values, rvalue, xvalue, lvalue
        // forwarding reference or universal reference
        template<typename RType>
        matrix& operator/=(RType&& right)
        {
            size_t total = this->m_e.size();
        
            for(size_t n = 0; n < total; ++n)
                this->m_e[n] /= (element_t) right;

            return *this;
        }

        // this is to pass any type of 
        // parameter values, rvalue, xvalue, lvalue
        // forwarding reference or universal reference
        template<typename RType>
        matrix& operator+=(RType&& right)
        {
            size_t total = this->m_e.size();
            assert(total == right.m_e.size());

            for(size_t n = 0; n < total; ++n)
                this->m_e[n] += right.m_e[n];

            return *this;
        }

        // this is to pass any type of 
        // parameter values, rvalue, xvalue, lvalue
        // forwarding reference or universal reference
        template<typename RType>
        matrix& operator-=(RType&& right)
        {
            size_t total = this->m_e.size();
            assert(total == right.m_e.size());

            for(size_t n = 0; n < total; ++n)
                this->m_e[n] -= right.m_e[n];

            return *this;
        }
        
        template<typename T1, typename T2>
        struct matrix_opr_st;

        // matrix matrix operation
        template<typename T1, typename T2>
        struct matrix_opr_st<matrix<T1>, matrix<T2>>
        {
            template<typename LType, typename RType>
            static decltype(auto) mul(LType&& m1, RType&& m2)
            {

#ifdef VERBOSE

                if(!(m1.m_d.size() == 2 && m2.m_d.size() == 2 && m1.m_d[1] == m2.m_d[0]))
                {
                    if(m1.m_d.size() == 2 && m2.m_d.size() == 2)
                    {
                        std::cout<<"\nError: m1["<<m1.dim(0)<<", "<<m1.dim(1)<<"] x m2["
                            <<m2.dim(0)<<", "<<m2.dim(1)<<"] is not possible."<<std::endl<<std::endl;
                    }
                    else 
                    {
                        std::cout<<"\nError: either m1 or m2 is not 2-dimensional matrix."<<std::endl<<std::endl;
                    }

                    std::cout << "m1 = \n" << m1<< "\n\n";
                    std::cout << "m2 = \n" << m2<< "\n\n";
                }
#endif   
                // matrix matrix multiplication
                assert(m1.m_d.size() == 2 && m2.m_d.size() == 2 && m1.m_d[1] == m2.m_d[0]);


                size_t dim0 = m1.m_d[0];
                size_t dim1 = m2.m_d[1];
                size_t N = m1.m_d[1];

                matrix m{dim0, dim1};
              
                for(size_t i = 0; i < dim0; ++i)
                    for(size_t j = 0; j < dim1; ++j)
                    {
                        m(i, j) = element_t{};

                        for(size_t k = 0; k < N; ++k)
                            m(i, j) += m1(i, k) * m2(k, j);
                    }

                return m;
            }

            template<typename LType, typename RType>
            static decltype(auto) add(LType&& m1, RType&& m2)
            {
                // if constexpr is introduced to C++17 Standard
                if constexpr(std::is_rvalue_reference_v<decltype(m1)>)
                {
                    m1 += m2;
                    return std::forward<LType>(m1);
                }
                else if constexpr(std::is_rvalue_reference_v<decltype(m2)>)
                {
                    m2 += m1;
                    return std::forward<RType>(m2);
                }
                else
                {
                    auto m = m1; // copy constructor
                    m += m2;
                    return m;
                }
            }

            template<typename LType, typename RType>
            static decltype(auto) sub(LType&& m1, RType&& m2)
            {
                // if constexpr is introduced to C++17 Standard
                if constexpr(std::is_rvalue_reference_v<decltype(m1)>)
                {
                    m1 -= m2;
                    return std::forward<LType>(m1);
                }
                else if constexpr(std::is_rvalue_reference_v<decltype(m2)>)
                {
                    size_t total = m1.total();
                    assert(total == m2.total());

                    using ele_t = tpf::element_t<decltype(m2)>;

                    for(size_t i= 0; i <total; ++i)
                        m2[i] = (ele_t)m1[i] - m2[i];

                    return std::forward<RType>(m2);
                }
                else
                {
                    auto m = m1; // copy constructor
                    m -= m2;
                    return m;
                }
            }
         };

        // scalar matrix operation
        template<typename ScalarType, typename METype>
        struct matrix_opr_st<ScalarType, matrix<METype>>
        {
            template<typename ScalarT, typename MatrixT>
            static decltype(auto) mul(ScalarT&& s, MatrixT&& m)
            {
                if constexpr(std::is_rvalue_reference_v<decltype(m)>)
                {
                    m *= std::forward<ScalarT>(s);

                    return std::forward<MatrixT>(m); 
                }
                else
                {
                    auto r = m;
                    r *= std::forward<ScalarT>(s);

                    return r;
                }
            }

            template<typename ScalarT, typename MatrixT>
            static decltype(auto) div(ScalarT&& s, MatrixT&& m)
            {
                if constexpr(std::is_rvalue_reference_v<decltype(m)>)
                {
                    size_t total = m.total();
                    for(size_t i=0; i < total; ++i)
                        m[i] = s / m[i];

                    return std::forward<MatrixT>(m); 
                }
                else
                {
                    auto r = m;
                    size_t total = m.total();

                    for(size_t i=0; i < total; ++i)
                        r[i] = s / m[i];

                    return r;
                }
            }
        };

        // matrix scalar operation
        template<typename METype, typename ScalarType>
        struct matrix_opr_st<matrix<METype>, ScalarType>
        {
            template<typename MatrixT, typename ScalarT>
            static decltype(auto) mul(MatrixT&& m, ScalarT&& s)
            {
                using mtype_t = tpf::remove_const_reference_t<MatrixT>;
                using stype_t = tpf::remove_const_reference_t<ScalarT>;

                return matrix_opr_st<stype_t, mtype_t>:: template
                    mul(std::forward<ScalarT>(s), std::forward<MatrixT>(m));
            }

            template<typename MatrixT, typename ScalarT>
            static decltype(auto) div(MatrixT&& m, ScalarT&& s)
            {
                if constexpr(std::is_rvalue_reference_v<decltype(m)>)
                {
                    m /= s;

                    return std::forward<MatrixT>(m); 
                }
                else
                {
                    auto r = m;
                    r /= s;

                    return r;
                }
            }
        };

        template<typename LType, typename RType>
        friend decltype(auto) operator+(LType&& left, RType&& right);

        template<typename LType, typename RType>
        friend decltype(auto) operator-(LType&& left, RType&& right);

        template<typename LType, typename RType>
        friend decltype(auto) operator*(LType&& left, RType&& right);

        template<typename LType, typename RType>
        friend decltype(auto) operator/(LType&& left, RType&& right);
    
    }; // end of class matrix

    namespace hide
    {
        using index_pair_t = std::pair<size_t, size_t>;
        using index_pair_vctr_t = std::vector<index_pair_t>;
        using index_vctr_t = std::vector<size_t>;
    }

    template<typename ElementType>
    using vector_of_matrices_t = std::vector<tpf::matrix<ElementType>>;

    using ms_table_t = std::pair<matrix<size_t>, matrix<size_t>>;
    
    namespace hide
    {
        template<typename T>
        struct is_matrix_st
        {
            static constexpr bool value = false;
            using type = T;
        };

        template<typename T>
        struct is_matrix_st<matrix<T>>
        {
            static constexpr bool value = true;
            using type = T;
        };
    }

    template<typename T>
    constexpr bool is_matrix_v = hide::is_matrix_st<tpf::remove_const_reference_t<T>>::value;

    template<typename T>
    constexpr bool matrix_element_t = hide::is_matrix_st<tpf::remove_const_reference_t<T>>::type;

    template<typename IndexPairType, typename... IndexPairTypes>
    hide::index_vctr_t make_index_vctr(IndexPairType&& m1, IndexPairTypes&&... ms)
    {
        using namespace hide;

        using index_t = tpf::remove_const_reference_t<IndexPairType>;
                
        if constexpr(std::is_same_v<index_t, index_pair_t>)
        {
            index_pair_vctr_t mm = 
                { std::forward<IndexPairType>(m1), std::forward<IndexPairTypes>(ms)... };
            
            index_vctr_t index; index.reserve(mm.size()+1);

            for(const auto& e: mm)
                index.emplace_back(e.first);

            index.emplace_back(mm.back().second);

            return index;
        }
        else if constexpr(std::is_same_v<index_t, index_pair_vctr_t>)
        {
            index_vctr_t index; index.reserve(m1.size() + 1);

            for(const auto& e: m1)
                index.emplace_back(e.first);

            index.emplace_back(m1.back().second);

            return index;
        }
        else if constexpr (is_matrix_v<index_t>)
        {
            index_pair_vctr_t mm{ m1.get_dims(), ms.get_dims()...};

            index_vctr_t index; index.reserve(mm.size()+1);

            for(const auto& e: mm)
                index.emplace_back(e.first);

            index.emplace_back(mm.back().second);

            return index;
        }
        else
        {
            std::cout << "Here 4 "<<std::endl;
            
            return {};
        }
        
    }

    template<typename LType, typename RType>
    decltype(auto) operator+(LType&& left, RType&& right)
    {
        using ltype_t = tpf::remove_const_reference_t<LType>;
        using rtype_t = tpf::remove_const_reference_t<RType>;

        return ltype_t::template matrix_opr_st<ltype_t, rtype_t>:: template
            add(std::forward<LType>(left), std::forward<RType>(right));
    }

    template<typename LType, typename RType>
    decltype(auto) operator-(LType&& left, RType&& right)
    {
        using ltype_t = tpf::remove_const_reference_t<LType>;
        using rtype_t = tpf::remove_const_reference_t<RType>;

        return ltype_t::template matrix_opr_st<ltype_t, rtype_t>:: template
            sub(std::forward<LType>(left), std::forward<RType>(right));
    }

    template<typename LType, typename RType>
    decltype(auto) operator*(LType&& left, RType&& right)
    {
        using ltype_t = tpf::remove_const_reference_t<LType>;
        using rtype_t = tpf::remove_const_reference_t<RType>;

        if constexpr (!tpf::is_scalar_v<ltype_t> && !tpf::is_scalar_v<rtype_t>)
        {
            // matrix and matrix multiplication
            return ltype_t::template matrix_opr_st<ltype_t, rtype_t>:: template
                mul(std::forward<LType>(left), std::forward<RType>(right));
        }
        else if constexpr(tpf::is_scalar_v<ltype_t>)
        {
            // scalar matrix multiplication
            return rtype_t::template matrix_opr_st<ltype_t, rtype_t>:: template
                mul(std::forward<LType>(left), std::forward<RType>(right));
        }
        else if constexpr(tpf::is_scalar_v<rtype_t>)
        {
            // matrix scalar multiplication
             return ltype_t::template matrix_opr_st<ltype_t, rtype_t>:: template
                mul(std::forward<LType>(left), std::forward<RType>(right));
        }
    }

    template<typename LType, typename RType>
    decltype(auto) operator/(LType&& left, RType&& right)
    {
        using ltype_t = tpf::remove_const_reference_t<LType>;
        using rtype_t = tpf::remove_const_reference_t<RType>;

        if constexpr(tpf::is_scalar_v<ltype_t>)
        {
            return rtype_t::template matrix_opr_st<ltype_t, rtype_t>:: template
                div(std::forward<LType>(left), std::forward<RType>(right));
        }
        else
        {
             return ltype_t::template matrix_opr_st<ltype_t, rtype_t>:: template
                div(std::forward<LType>(left), std::forward<RType>(right));
        }
    }

    //////////////////////////////////////////////////////////////
    template<typename T, typename DimType, typename... DimTypes>
    /* matrix<T> */
    decltype(auto) make_matrix(DimType&& first, DimTypes&&... args)
    {
        if constexpr(tpf::is_scalar_v<DimType>)
        {
            return matrix<T>{(size_t)first, static_cast<size_t>(args)...};
        }
        else
        {
            return matrix<T>{ std::forward<first>(first) };
        }
    }

    //extern template tpf::matrix<int> tpf::make_matrix<int>(const size_t&, const size_t&);

    template<typename ElementType, typename... ElementTypes>
    matrix<ElementType> fill_matrix(ElementType&& ele, ElementTypes&&... eles)
    {
        size_t count = sizeof...(eles) + 1;
        matrix<ElementType> m{count};
        
        m.elements() = 
            {   
                std::forward<ElementType>(ele), 
                static_cast<ElementType>(std::forward<ElementTypes>(eles))... 
            };
        
        return m;
    }

    template<typename T, typename DimType>
    matrix<T> make_unit_matrix(DimType first)
    {
        matrix<T> m{(size_t)first, (size_t)first};

        for(size_t i = 0; i < first; ++ i)
            for(size_t j = 0; j < first; ++j)
            {
                m(i,j) = (i==j) ? 1 : 0;
            }

        return m;
    }

    template<typename SeqType>
    SeqType longest_common_subsequence(const SeqType& seq1, const SeqType& seq2, bool bShowTable=false)
    {
        const auto& short_seq = seq1.size() > seq2.size() ? seq2 : seq1;
        const auto& long_seq = seq1.size() > seq2.size() ? seq1 : seq2;

        size_t short_rows = short_seq.size();
        size_t long_columns = long_seq.size();

        auto table = tpf::make_matrix<int>(short_rows + 1, long_columns + 1);
        table.initialize();

        for(size_t short_i = 0; short_i < short_rows; ++short_i)
            for(size_t long_j = 0; long_j < long_columns; ++long_j)
            {
                if(short_seq[short_i] == long_seq[long_j])
                {
                    table(short_i+1, long_j+1) = 1 + table(short_i, long_j);
                }
                else
                {
                    table(short_i+1, long_j+1) = 
                        std::max(table(short_i, long_j+1), table(short_i+1, long_j));
                }
            }
        
        if(bShowTable)
        {
            // std::cout << "Table: \n"<< table <<std::endl;
            for(size_t i = 0; i <= short_rows; ++i)
                for(size_t j = 0; j <= long_columns; ++j)
                {
                    if(i==0 && j>0)
                        std::cout << long_seq[j-1];
                    else if(j==0 && i>0)
                        std::cout << short_seq[i-1];
                    else
                    {
                        std::cout << table(i, j);
                    }

                    if(j == long_columns)
                        std::cout <<"\n";
                    else
                        std::cout <<", "; 
                }
        }

        using ele_t = tpf::element_t<SeqType>;

        std::deque<ele_t> stack;

        size_t short_i = short_rows;
        size_t long_j = long_columns;

        while(short_i != 0 && long_j != 0 &&
            table(short_i, long_j) != 0)
        {
            if(table(short_i, long_j-1) == table(short_i-1, long_j))
            {
                if(table(short_i, long_j-1) != table(short_i, long_j))
                    stack.emplace_front(long_seq[long_j-1]);

                --short_i; --long_j;
            }
            else
            {
                if(table(short_i, long_j-1) > table(short_i-1, long_j))
                    --long_j;
                else
                    --short_i;
            }       
        }

        return {stack.cbegin(), stack.cend()};
    }

    template<typename MatrixType, typename... MatrixTypes> 
    ms_table_t matrix_chain_build_table(MatrixType&& mat, MatrixTypes&&... mats)
    {   
        hide::index_vctr_t p;

        if constexpr(is_matrix_v<MatrixType>)
        {
            p = make_index_vctr(mat, mats...);
        }
        else
        {
            hide::index_pair_vctr_t pv;

            for(const auto& e: mat)
                pv.push_back({e.dim(0), e.dim(1)});

            p = make_index_vctr(pv);
        }
        
        ms_table_t m_and_s { make_matrix<size_t>(p.size(), p.size()),
                                make_matrix<size_t>(p.size(), p.size())};
        
        auto& [m, s] =  m_and_s;

        m.initialize(); s.initialize(); 
                
        size_t n = p.size(), j, q;

        for (size_t d=1; d<n-1; d++)
        {
            for (size_t i=1; i<n-d; i++)
            {
                j = i+d;
                
                m(i, j) = size_t(-1);  
    
                for (size_t k=i; k<=j-1; k++)
                {
                    q = m(i, k) + m(k+1, j) + p[i-1]*p[k]*p[j];
                    if (q < m(i, j))
                    {
                        m(i, j) = q;
                        s(i, j) = k;   
                    }
                }
            }
        }
        
        return m_and_s;
    }

    namespace hide
    {
        template<typename T>
        std::string matrix_chain_print_order(const matrix<T>& s, int l, int h)
        {
            if(s(l, h) != 0)
            {           
                std::ostringstream os;

                os << "(";
                os << matrix_chain_print_order(s, l, (int)s(l, h));               
                os << " x ";
                os << matrix_chain_print_order(s, (int)s(l, h)+1, h);
                os << ")";

                return os.str();
            }
            else // s(l, h) == 0 (or l==h)
            {
                std::ostringstream os;
                os << "A"<<h;
                return os.str();
            }
        }

        // naive approach
        template<typename ElementType, typename IndexType>
        matrix<ElementType> matrix_chain_product_naive(const vector_of_matrices_t<ElementType>& ma,
            const matrix<IndexType>& s, int l, int h)
        {
            if(s(l, h) != 0)
            {
                return matrix_chain_product_naive(ma, s, l, (int)s(l, h))
                        * matrix_chain_product_naive(ma, s, (int)s(l, h)+1, h);
            }
            else // s(l, h) == 0 (or l==h)
            {
                return ma[h-1];
            }
        }

        // optimized approach
        template<typename ElementType, typename IndexType>
            size_t matrix_chain_product(matrix<ElementType>& m, 
                const vector_of_matrices_t<ElementType>& ma,
                const matrix<IndexType>& s, int l, int h)
        {
            if(s(l, h) != 0)
            {
                matrix<ElementType> m1, m2;

                size_t left_idx = matrix_chain_product(m1, ma, s, l, (int)s(l, h));
                size_t right_idx = matrix_chain_product(m2, ma, s, (int)s(l, h)+1, h);

                size_t size = ma.size();

                auto&& left = left_idx < size ? ma[left_idx] : m1;
                auto&& right = right_idx < size? ma[right_idx] : m2;

                m.mul(left, right); return size;
            }
            else // s(l, h) == 0 (or l==h)
            {
                return (h-1);
            }
        }
    } // end of namespace hide

    template<typename T>
    std::string matrix_chain_print_order(const matrix<T>& s)
    {
                // s.dim(0)-1 because all zero' column
        auto order_string = hide::matrix_chain_print_order(s, 1, s.dim(0)-1);
        
        return order_string.substr(1, order_string.size()-2);
    }

    template<typename ElementType>
    matrix<ElementType> matrix_chain_product_naive(const vector_of_matrices_t<ElementType>& ma, bool bVerbose=false)
    {
        // STEP 1 - build table
         auto [table_m, table_s] = matrix_chain_build_table(ma);
        
        if(bVerbose)
        {
            std::cout << "Muliplication Count Table (m): \n" << table_m << "\n\n";
            std::cout << "Muliplication Order Table (s): \n" << table_s << "\n\n";
            std::cout << "Muliplication Order: " << matrix_chain_print_order(table_s) << "\n";
            std::cout << "Muliplication Count: " << table_m(1, table_m.dim(1)-1) << "\n\n";
        }

        // compute and return the result
        return hide::matrix_chain_product_naive(ma, table_s, 1, ma.size());
    }

    template<typename ElementType>
    matrix<ElementType> matrix_chain_product(const vector_of_matrices_t<ElementType>& ma, bool bVerbose=false)
    {
        // STEP 1 - build table
         auto [table_m, table_s] = matrix_chain_build_table(ma);
        
        if(bVerbose)
        {
            std::cout << "Muliplication Count Table (m): \n" << table_m << "\n\n";
            std::cout << "Muliplication Order Table (s): \n" << table_s << "\n\n";
            std::cout << "Muliplication Order: " << matrix_chain_print_order(table_s) << "\n";
            std::cout << "Muliplication Count: " << table_m(1, table_m.dim(1)-1) << "\n\n";
        }

        // compute and return the result
        matrix<ElementType> m;
        
        if(hide::matrix_chain_product(m, ma, table_s, 1, ma.size()) != ma.size())
        {
            std::cout << "Failed"<<std::endl;
        }

        return m;
    }

    template<typename ElementType>
    size_t matrix_chain_analyze(const vector_of_matrices_t<ElementType>& ma, bool bVerbose=false)
    {
        // STEP 1 - build table
        auto [table_m, table_s] = matrix_chain_build_table(ma);
        
        if(bVerbose)
        {
            std::cout<< "Muliplication Count Table (m): \n" << table_m << "\n\n";
            std::cout<< "Muliplication Order Table (s): \n" << table_s << "\n\n";
            std::cout <<"Muliplication Order: " << matrix_chain_print_order(table_s) << "\n";
            std::cout <<"Muliplication Count: " << table_m(1, table_m.dim(1)-1) << "\n\n";
        }

        // compute and return the result
        return table_m(1, table_m.dim(1)-1);
    }

    template<typename MatrixType, typename... MatrixTypes> 
    auto matrix_chain_product(const MatrixType& mat, const MatrixTypes&... mats)
    {
        using element_t = tpf::element_t<MatrixType>;
        vector_of_matrices_t<element_t> ma { mat, mats...};

        return matrix_chain_product(ma);
    }

    template<typename ElementType>
    matrix<ElementType> matrix_simple_product(const vector_of_matrices_t<ElementType>& ma)
    {
        if(ma.empty())
            return matrix<ElementType>{};
        else if(ma.size()==1)
            return ma[0];
        else
        {   // two or more matrices

            auto m = ma[0];
            size_t size = ma.size();

            for(size_t i = 1; i < size; ++i)
            {
                m *= ma[i];
            }

            return m;
        }
    }
}
#endif // end of _TPF_MATRIX_H