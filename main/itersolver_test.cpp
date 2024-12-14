#include "../src/LinearAlgebra/Solver/IterSolver.hpp"
#include "gtest/gtest.h"

// Helper function to create an identity matrix of a given size.
template<typename TNum>
MatrixObj<TNum> CreateIdentityMatrix(int size) {
    MatrixObj<TNum> identity(size, size);
    for (int i = 0; i < size; ++i) {
        identity[i * size + i] = 1; // Set the diagonal elements to 1.
    }
    return identity;
}

// Helper function to create an identity vector of a given size.
template<typename TNum>
VectorObj<TNum> CreateIdentityVector(int size, TNum value) {
    VectorObj<TNum> identity(size);
    for (int i = 0; i < size; ++i) {
        identity[i] = value;
    }
    return identity;
}

class GradientDesentTest : public ::testing::Test {
protected:
    virtual void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    int size_ = 100;
    MatrixObj<double> P = CreateIdentityMatrix<double>(size_);
    MatrixObj<double> A = CreateIdentityMatrix<double>(size_);
    VectorObj<double> b = CreateIdentityVector<double>(size_, 1.0);
    VectorObj<double> x = CreateIdentityVector<double>(size_, 0.0);
    std::unique_ptr<GradientDesent<double>> gradientDesent;

    virtual void SetUp() override {
        gradientDesent = std::make_unique<GradientDesent<double>>(P, A, b, 100);
    }
};

// Test the constructor of GradientDesent.
TEST_F(GradientDesentTest, Constructor) {
    EXPECT_EQ(gradientDesent->getIter(), 100);
}

TEST_F(GradientDesentTest, CalGrad) {
    VectorObj<double> expectedGradient = b - (A * x);
    VectorObj<double> actualGradient = gradientDesent->calGrad(x);
    VectorObj<double> vec_output = actualGradient - expectedGradient;
    double output = vec_output.L2norm();
    ASSERT_NEAR(output, 0.0, 1e-8);
}

template<typename TNum>
void printscan(VectorObj<TNum> x){
	for(int i=0; i<x.get_row(); i++){
		std::cout << x[i] << " ";
	}
	std::cout << std::endl;
}

TEST_F(GradientDesentTest, Update) {
    VectorObj<double> expectedX = CreateIdentityVector<double>(size_, 1.0);
	x = CreateIdentityVector<double>(size_, 0.0);
	gradientDesent->callUpdate(x);
    VectorObj<double> vec_output = x - expectedX;
    double output = vec_output.L2norm();
    ASSERT_NEAR(output, 0.0, 1e-8);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}