#ifndef MATRIXOBJ_HPP
#define MATRIXOBJ_HPP

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <vector>

template<typename TObj> class VectorObj;

template<typename TObj>
class MatrixObj {
    private:
    int _n, _m;
    TObj *arr=nullptr;
    
    public:
    MatrixObj(){}
    MatrixObj(int n, int m) : _n(n), _m(m), arr(new TObj[n * m]()) {}
    MatrixObj(const TObj *other, int n, int m) : _n(n), _m(m){
        arr = new TObj[n * m]();
        std::copy(other, other + n * m, arr);
    }
    MatrixObj(const MatrixObj &other) : _n(other._n), _m(other._m) {
        arr = new TObj[_n * _m];
        std::copy(other.arr, other.arr + _n * _m, arr);
    }

    MatrixObj(MatrixObj&& other) noexcept : _n(other._n), _m(other._m), arr(other.arr) {
        other._n = 0;
        other._m = 0;
        other.arr = nullptr;
    }

    explicit MatrixObj(const VectorObj<TObj>& vector) : _n(vector.get_row()), _m(vector.get_col()), arr(new TObj[_m]()) {
        std::copy(vector.data(), vector.data() + _m, arr);
    }
    explicit MatrixObj(const std::vector<VectorObj<TObj>>& vectors) : _n(vectors.empty() ? 0 : vectors[0].get_row()), _m(vectors.size()) {
        arr = new TObj[_n * _m]();
        for (int i = 0; i < _m; ++i) {
            std::copy(vectors[i].arr, vectors[i].arr + _n, arr + i * _n);
        }
    }

    ~MatrixObj() {
        if (arr != nullptr) {
            delete[] arr;
        }
    }

    int get_row() const { return _n; }
    int get_col() const { return _m; }

    void setDim(int n, int m) {
        if (n != _n || m != _m) {
            throw std::invalid_argument("Cannot change the dimensions of a MatrixObj instance.");
        }
        _n = n;
        _m = m;
    }

    void Slice(TObj* slice, int n, int m) const {
        if (n < 0 || m < 0 || n + m > _n * _m) {
            throw std::out_of_range("Index out of range in Slice function.");
        }
        std::copy(arr + n, arr + n + m, slice);
    }

    void swap(MatrixObj<TObj>& other) {
        std::swap(this->arr, other.arr);
        std::swap(this->_n, other._n);
        std::swap(this->_m, other._m);
    }

    MatrixObj& operator=(const MatrixObj& other) {
        if (this != &other) {
            if (_n != other._n || _m != other._m) {
                delete[] arr;
                _n = other._n;
                _m = other._m;
                arr = new TObj[_n * _m];
            }
            std::copy(other.arr, other.arr + _n * _m, arr);
        }
        return *this;
    }

    MatrixObj& operator=(MatrixObj&& other) noexcept {
        if (this != &other) {
            delete[] arr;
            _n = other._n;
            _m = other._m;
            arr = std::move(other.arr);
            other._n = 0;
            other._m = 0;
            other.arr = nullptr;
        }
        return *this;
    }
    
    MatrixObj operator+(const MatrixObj &other) {
        if (_n != other.get_row() || _m != other.get_col()) {
            throw std::invalid_argument("Matrix dimensions do not match for addition.");
        }
        MatrixObj result(_n, _m);
        for (int i = 0; i < _n * _m; ++i) {
            result.arr[i] = arr[i] + other.arr[i];
        }
        return result;
    }

    MatrixObj operator-(const MatrixObj &other) {
        if (_n != other.get_row() || _m != other.get_col()) {
            throw std::invalid_argument("Matrix dimensions do not match for subtraction.");
        }
        MatrixObj result(_n, _m);
        for (int i = 0; i < _n * _m; ++i) {
            result.arr[i] = arr[i] - other.arr[i];
        }
        return result;
    }

    inline void scalarMultiple(TObj *arrObj, TObj scalar){
        for (int i = 0; i < _n * _m; ++i) {
            arrObj[i] *= scalar;
        }
    }

    MatrixObj &operator*=(double scalar) {
        scalarMultiple(&(arr[0]), scalar);
        return *this;
    }

    MatrixObj &operator*(double factor) {
        scalarMultiple(&(arr[0]), factor);
        return *this;
    }

    MatrixObj operator*(const MatrixObj &other) {
        if (_m != other.get_row()) {
            throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
        }
        MatrixObj result(_n, other.get_col());
        for (int i = 0; i < _n; ++i) {
            for (int j = 0; j < other.get_col(); ++j) {
                for (int k = 0; k < _m; ++k) {
                    result.arr[i * other.get_col() + j] += arr[i * _n + k] * other.arr[k * other.get_row() + j];
                }
            }
        }
        return result;
    }

    MatrixObj Transpose() {
        MatrixObj result(_m, _n);
        for (int i = 0; i < _n; ++i) {
            for (int j = 0; j < _m; ++j) {
                result.arr[j * _n + i] = arr[i * _m + j];
            }
        }
        return result;
    }

    const TObj &operator[](int index) const {
        if (index < 0 || index >= _n * _m) {
            throw std::out_of_range("Index out of range for matrix element access.");
        }
        return arr[index];
    }

    TObj &operator[](int index) {
        if (index < 0 || index >= _n * _m) {
            throw std::out_of_range("Index out of range for matrix element access.");
        }
        return arr[index];
    }

    VectorObj<TObj> get_Col(int index) const {
        if (index < 0 || index >= _m) {
            throw std::out_of_range("Index out of range for column access.");
        }
        return VectorObj<TObj>(arr+index * _n, _n);
    }

    TObj* data() { return arr; }
    const TObj* data() const { return arr; }

};

template <typename TObj>
class VectorObj : public MatrixObj<TObj> {
    public:
    VectorObj(){}
    VectorObj(int n) : MatrixObj<TObj>(n, 1) {}
    VectorObj(const TObj *other, int n) : MatrixObj<TObj>(other, n, 1) {
        if (other == nullptr) {
            throw std::invalid_argument("Null pointer provided to VectorObj constructor.");
        }
    }
    
    ~VectorObj() {}
    
    double L2norm() {
        double sum = 1e-8;
        for (int i = 0; i < MatrixObj<TObj>::get_row() * MatrixObj<TObj>::get_col(); i++) {
            sum += std::pow(this->data()[i], 2);
        }
        return std::sqrt(sum);
    }
    
    void normalized() {
        double l2norm = L2norm();
        if (l2norm == 0) {
            throw std::runtime_error("Cannot normalize a zero vector.");
        }
        for (int i = 0; i < MatrixObj<TObj>::get_row() * MatrixObj<TObj>::get_col(); i++) {
            this->data()[i] = this->data()[i] / l2norm;
        }
    }
    
    VectorObj operator+(const VectorObj<TObj> &other) {
        int n = other.get_row(), m = other.get_col();
        if (this->get_row() != n || this->get_col() != m) {
            throw std::invalid_argument("Vector dimensions do not match for addition.");
        }
        VectorObj<TObj> result(n);
        for (int i = 0; i < n * m ; ++i) {
            result.data()[i] = this->data()[i] + other.data()[i];
        }
        return result;
    }
    
    VectorObj &operator*=(double scalar) {
        int n = MatrixObj<TObj>::get_col();
        for(int i=0; i<n; i++){
            this->data()[i] = static_cast<TObj>(scalar) * this->data()[i]; 
        }
        return *this;
    }
    
    VectorObj &operator=(VectorObj other) {
        this->swap(other);
        return *this;
    }
    
    TObj &operator[](int index) {
        if (index < 0 || index >= this->get_row() * this->get_col()) {
            throw std::out_of_range("Index out of range for vector element access.");
        }
        return MatrixObj<TObj>::data()[index];
    }
    
    TObj operator*(const VectorObj<TObj>& other) const {
        if (this->get_row() != other.get_row()) {
            throw std::invalid_argument("Vector dimensions do not match for dot product.");
        }
        TObj sum = static_cast<TObj>(0);
        for (int i = 0; i < this->get_row(); ++i) {
            sum += this->data()[i] * other.data()[i];
        }
        return sum;
    }
    
    int get_col() const {
        return MatrixObj<TObj>::get_col();
    }
    
    int get_row() const {
        return MatrixObj<TObj>::get_row();
    }
};

#endif // MATRIXOBJ_HPP