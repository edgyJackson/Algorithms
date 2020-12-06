#include "tpf_matrix.h"

int main()
{
    const char* nl = "\n\n";
    auto& cout = std::cout;

    // define element type of a matrix
    using element_t = int;

    // we define a matrix with element type element_t
    using matrix_t = tpf::matrix<element_t>;

    // we define a vector of matrices
    // a vector of matrices of element type element_t
    // we are defining a vector of int matrices
    using vector_of_matrices_t = tpf::vector_of_matrices_t<element_t>;

    // define random number generator
    using random_t = tpf::random_t<element_t>;

    // cm for Create Matrix
    auto& cm = tpf::make_matrix<element_t, size_t, size_t>;

    // vector_of_matrices_t ma = { cm(5, 4), cm(4, 6), cm(6, 2), cm(2, 7) };
    vector_of_matrices_t ma{ cm(10, 4), cm(4, 6), cm(6, 2), cm(2, 7),  cm(7, 5), cm(5, 10) };

    // we define a random number generator
    // that generates from 0 to 10 inclusive
    random_t random(0, 10);

    for(auto& A: ma)
    {
        A.generate(random);

        cout << "A("<<  A.dim(0) <<", " << A.dim(1) << ")=\n";
        cout << A << nl;
    }

    // matrix chain multiplication
    auto mp = tpf::matrix_chain_product(ma, true);
    
    // simple matrix product
    auto smp = ma[0] * ma[1] * ma[2] * ma[3] * ma [4] * ma[5];

    cout <<"mp = \n" << mp << nl;
    cout << "smp = \n" << smp << nl;

}