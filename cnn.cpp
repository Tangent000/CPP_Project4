#include <iostream>
#include "cnn.h"
// member functions for Mat class

// member functions for Mat_ class
template <typename T>
Mat_<T>::Mat_(size_t rows, size_t cols, size_t channels, T *data)
    : Mat(rows, cols, channels, type), data(data), refcount(new size_t(1))
{
#ifdef DEBUG_MODE
    std::cout << "Mat_ constructor called." << std::endl;
#endif
    // check the dimensions
    if (rows == 0 || cols == 0 || channels == 0)
        throw std::invalid_argument("Mat_ constructor: rows, cols, channels must be positive.");
    // check if the data and dimensions match
    if (rows * cols * channels != sizeof(*data) / sizeof(T))
        throw std::invalid_argument("Mat_ constructor: data size does not match.");
    if (data == NULL)
    {
        this->data = new T[rows * cols * channels];
    }
    // set the type according to the typeid of T
    if (typeid(T) == typeid(unsigned char))
        type = UNSIGNED_CHAR;
    else if (typeid(T) == typeid(short))
        type = SHORT;
    else if (typeid(T) == typeid(int))
        type = INT;
    else if (typeid(T) == typeid(float))
        type = FLOAT;
    else if (typeid(T) == typeid(double))
        type = DOUBLE;
    else
        throw std::invalid_argument("Mat_ constructor: type not supported.");
}

template <typename T>
Mat_<T>::Mat_(const Mat_ &rhs) : Mat(rhs), data(rhs.data), refcount(rhs.refcount)
{
#ifdef DEBUG_MODE
    std::cout << "Mat_ copy constructor called." << std::endl;
#endif
    refcount++;
}

template <typename T>
Mat_<T> &Mat_<T>::operator=(const Mat_ &rhs)
{
#ifdef DEBUG_MODE
    std::cout << "Mat_ assignment operator called." << std::endl;
#endif
    if (this != &rhs)
    {
        Mat::operator=(rhs);
        if (--*refcount == 0)
        {
            delete[] data;
            delete refcount;
        }
        data = rhs.data;
        refcount = rhs.refcount;
        refcount++;
    }
    return *this;
}

template <typename T>
bool Mat_<T>::operator==(const Mat &rhs)
{
#ifdef DEBUG_MODE
    std::cout << "Mat_ equality operator called." << std::endl;
#endif
    if (Mat::operator==(rhs)) // do not care the type, e.g. for Matrix of short type or Matrix of int type, if their data are of the same value, they are "equal".
    {
        Mat_ &rhs_ = (Mat_ &)rhs; // cast (Mat)rhs to Mat_ type
        for (size_t i = 0; i < rows * cols * channels; i++)
        {
            if (data[i] != rhs_.data[i])
                return false;
        }
        return true;
    }
    return false;
}

template <typename T>
Mat_<T> Mat_<T>::operator+(const Mat &rhs)
{
#ifdef DEBUG_MODE
    std::cout << "Mat_ addition operator called." << std::endl;
#endif
    if (Mat::operator==(rhs)) // check the dimensions first
    {
        Mat_ &rhs_ = (Mat_ &)rhs;
        Mat_ result(rows, cols, channels);
        for (size_t i = 0; i < rows * cols * channels; i++)
        {
            result.data[i] = data[i] + rhs_.data[i];
        }
        return result;
    }
    else
        throw std::invalid_argument("Mat_ addition operator: dimensions do not match.");
}

// declare the template classes here
typedef Mat_<unsigned char> Mat_uchar;
typedef Mat_<short> Mat_short;
typedef Mat_<int> Mat_int;
typedef Mat_<float> Mat_float;
typedef Mat_<double> Mat_double;
