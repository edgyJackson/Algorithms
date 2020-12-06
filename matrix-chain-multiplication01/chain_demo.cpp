#include "tpf_matrix.h"

int main()
{
    const char* nl = "\n\n";

    using element_t = int;
    using matrix_t = tpf::matrix<element_t>;
    using vector_of_matrices_t = tpf::vector_of_matrices_t<element_t>;
    using random_t = tpf::random_t<element_t>;

    auto& mm = tpf::make_matrix<element_t, size_t, size_t>;

    vector_of_matrices_t ma = { mm(5, 4), mm(4, 6), mm(6, 2), mm(2, 7) };
    // vector_of_matrices_t ma{ mm(10, 4), mm(4, 6), mm(6, 2), mm(2, 7),  mm(7, 5), mm(5, 10) };
   
    random_t rand{0, 10};

    for(auto& A: ma)
    {
        A.generate(rand);

        std::cout <<"A(" <<A.dim(0) <<", "<<A.dim(1)<< ") = \n" << A << nl;
    }

    // auto chain_product = tpf::matrix_chain_product_naive(ma, true);
    auto chain_improved = tpf::matrix_chain_product(ma, true);
    
    // std::cout << "Chain  Product = \n" << chain_product  << nl;
    std::cout << "Imp  Product = \n" << chain_improved  << nl;
    
}