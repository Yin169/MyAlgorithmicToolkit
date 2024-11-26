#include "basic.hpp"
#include "gtest/gtest.h"
#include <cmath>

namespace {

class BasicTest : public ::testing::Test {
protected:
    MatrixObj<double> A;
    VectorObj<double> b;

    void SetUp() override {
        A = MatrixObj<double>(3, 3);
        double setA[3*3] = {1,0,0,0,1,0,0,0,1};
        double setB[3] = {1, 0, 0};

        for (size_t i=0; i< 3*3; ++i){
            A[i] = setA[i];
        }
        b = VectorObj<double>(3);
        for (size_t i=0; i<3; ++i){
            b[i] = setB[i];
        }
    }
};

TEST_F(BasicTest, PowerIterTest) {
    VectorObj<double> b_init = b; 
    basic::powerIter(A, b, 10);
    for (int i = 0; i < b.get_row(); ++i) {
        EXPECT_NEAR(b_init[i], b[i], 1e-2);
    }
}

TEST_F(BasicTest, RayleighQuotientTest) {
    double quotient = basic::rayleighQuotient(A, b);
    EXPECT_GT(quotient, -100);
    EXPECT_LT(quotient, 100);
}

TEST_F(BasicTest, GramSmithTest) {
    MatrixObj<double> orthMatrix = basic::gramSmith(A);
    for (size_t i = 0; i < orthMatrix.get_row(); ++i) {
        for (size_t j = i + 1; j < orthMatrix.get_col(); ++j) {
            VectorObj<double> u = orthMatrix.get_Col(i);
            VectorObj<double> v = orthMatrix.get_Col(j);
            double dot_product = u * v;
            EXPECT_NEAR(dot_product, 0, 1e-6);
        }
    }
}

}