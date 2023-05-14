#include <iostream>
#include "cnn.hpp"

// member functions for Mat class
template <typename T>
Mat::operator Mat_<T>&()
{
#ifdef DEBUG_MODE
    std::cout << "Mat type conversion operator called." << std::endl;
#endif
    Mat_<T> *result = new Mat_<T>(rows, cols, channels);
    switch (type)
    {
    case UNSIGNED_CHAR:
    {
        unsigned char * temp = (unsigned char *)_data;
        for (size_t i = 0; i < rows * cols * channels; i++)
        {
            result->data[i] = (T)temp[i];
        }
        break;
    }
    case SHORT:
    {
        short * temp = (short *)_data;
        for (size_t i = 0; i < rows * cols * channels; i++)
        {
            result->data[i] = (T)temp[i];
        }
        break;
    }
    case INT:
    {
        int * temp = (int *)_data;
        for (size_t i = 0; i < rows * cols * channels; i++)
        {
            result->data[i] = (T)temp[i];
        }
        break;
    }
    case FLOAT:
    {
        float * temp = (float *)_data;
        for (size_t i = 0; i < rows * cols * channels; i++)
        {
            result->data[i] = (T)temp[i];
        }
        break;
    }
    case DOUBLE:
    {
        double * temp = (double *)_data;
        for (size_t i = 0; i < rows * cols * channels; i++)
        {
            result->data[i] = (T)temp[i];
        }
        break;
    }
    default:
        break;
    }
    return *result;
}



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
    this->_data = (int *)this->data; // let the base class has a pointer to the data of the derived class
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
    this->_data = (int *)this->data; // let the base class has a pointer to the data of the derived class
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
        _data = (int *)data;
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

// template <typename T>
// Mat_<T> &Mat_<T>::operator+(const Mat &rhs)
// {
// #ifdef DEBUG_MODE
//     std::cout << "Mat_ addition operator called." << std::endl;
// #endif
//     if (Mat::operator==(rhs)) // check the dimensions first
//     {
//         Mat_ &rhs_ = (Mat_ &)rhs;
//         Mat_ result(rows, cols, channels);
//         for (size_t i = 0; i < rows * cols * channels; i++)
//         {
//             result.data[i] = data[i] + rhs_.data[i];
//         }
//         return result;
//     }
//     else
//         throw std::invalid_argument("Mat_ addition operator: dimensions do not match.");
// }

// template <typename T>
// Mat_<T> &Mat_<T>::operator-(const Mat &rhs)
// {
//     return NULL;
// }

// template <typename T>
// Mat_<T> &Mat_<T>::operator*(const Mat &rhs)
// {
//     return NULL;
// }

// template <typename T>
// Mat_<T> &Mat_<T>::operator/(const Mat &rhs)
// {
//     return NULL;
// }
