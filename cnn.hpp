#pragma once
#include <iostream>
#define DEBUG_MODE

typedef enum _mattype
{
    UNSIGNED_CHAR,
    SHORT,
    INT,
    FLOAT,
    DOUBLE
} mattype;

template <typename T>
class Mat_; // forward declaration (let Mat class knows the existance of Mat_ class)

class Mat
{
public:
    size_t rows;
    size_t cols;
    size_t channels;
    mattype type;
    int *_data; // used to point to data from derived classes

    Mat(size_t rows = 0, size_t cols = 0, size_t channels = 0, mattype type = DOUBLE)
        : rows(rows), cols(cols), channels(channels), type(type)
    {
#ifdef DEBUG_MODE
        std::cout << "Mat constructor called." << std::endl;
#endif
    }

    Mat(const Mat &rhs)
        : rows(rhs.rows), cols(rhs.cols), channels(rhs.channels), type(rhs.type)
    {
#ifdef DEBUG_MODE
        std::cout << "Mat copy constructor called." << std::endl;
#endif
    }

    ~Mat()
    {
#ifdef DEBUG_MODE
        std::cout << "Mat destructor called." << std::endl;
#endif
    }

    virtual Mat &operator=(const Mat &rhs)
    {
#ifdef DEBUG_MODE
        std::cout << "Mat assignment operator called." << std::endl;
#endif
        if (this != &rhs)
        {
            rows = rhs.rows;
            cols = rhs.cols;
            channels = rhs.channels;
            type = rhs.type;
        }
        return *this;
    }

    virtual bool operator==(const Mat &rhs)
    {
#ifdef DEBUG_MODE
        std::cout << "Mat equality operator called." << std::endl;
#endif
        return (rows == rhs.rows && cols == rhs.cols && channels == rhs.channels);
    }

    virtual bool operator!=(const Mat &rhs) { return !(*this == rhs); }
    // virtual Mat & operator+ (const Mat & rhs)=0;
    // virtual Mat & operator- (const Mat & rhs)=0;
    // virtual Mat & operator* (const Mat & rhs)=0;
    // virtual Mat & operator/ (const Mat & rhs)=0;
    // virtual Mat & operator+= (const Mat & rhs)=0;
    // virtual Mat & operator-= (const Mat & rhs)=0;
    // virtual Mat & operator*= (const Mat & rhs)=0;
    // virtual Mat & operator/= (const Mat & rhs)=0;

    virtual void printMat() = 0;

    // for type conversions
    template <typename T>
    operator Mat_<T> &();
};

template <typename T>
class Mat_ : public Mat
{
public:
    size_t *refcount;
    T *data;

    Mat_(size_t rows = 0, size_t cols = 0, size_t channels = 0, T *data = NULL);
    Mat_(const Mat_ &rhs);
    Mat_ &operator=(const Mat_ &rhs);

    ~Mat_()
    {
#ifdef DEBUG_MODE
        std::cout << "Mat_ destructor called." << std::endl;
#endif
        if (--*refcount == 0)
        {
            delete[] data;
            delete refcount;
        }
    }

    bool operator==(const Mat &rhs);
    // Mat_ & operator+ (const Mat & rhs);
    // Mat_ & operator- (const Mat & rhs);
    // Mat_ & operator* (const Mat & rhs);
    // Mat_ & operator/ (const Mat & rhs);

    void printMat();
};

// declare the template classes here
template class Mat_<unsigned char>;
template class Mat_<short>;
template class Mat_<int>;
template class Mat_<float>;
template class Mat_<double>;

typedef Mat_<unsigned char> Mat_uchar;
typedef Mat_<short> Mat_short;
typedef Mat_<int> Mat_int;
typedef Mat_<float> Mat_float;
typedef Mat_<double> Mat_double;

// member functions for Mat class
template <typename T>
Mat::operator Mat_<T> &()
{
#ifdef DEBUG_MODE
    std::cout << "Mat type conversion operator called." << std::endl;
#endif
    Mat_<T> *result = new Mat_<T>(rows, cols, channels);
    switch (type)
    {
    case UNSIGNED_CHAR:
    {
        unsigned char *temp = (unsigned char *)_data;
        for (size_t i = 0; i < rows * cols * channels; i++)
        {
            result->data[i] = (T)temp[i];
        }
        break;
    }
    case SHORT:
    {
        short *temp = (short *)_data;
        for (size_t i = 0; i < rows * cols * channels; i++)
        {
            result->data[i] = (T)temp[i];
        }
        break;
    }
    case INT:
    {
        int *temp = (int *)_data;
        for (size_t i = 0; i < rows * cols * channels; i++)
        {
            result->data[i] = (T)temp[i];
        }
        break;
    }
    case FLOAT:
    {
        float *temp = (float *)_data;
        for (size_t i = 0; i < rows * cols * channels; i++)
        {
            result->data[i] = (T)temp[i];
        }
        break;
    }
    case DOUBLE:
    {
        double *temp = (double *)_data;
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

template <typename T>
void Mat_<T>::printMat()
{
    for (size_t i = 0; i < channels; i++)
    {
        std::cout << "Channel " << i + 1 << ":" << std::endl
                  << "[";
        for (size_t j = 0; j < rows * cols; j++)
        {
            std::cout << data[rows * cols * i + j];
            if ((j + 1) % cols == 0 && j != 0 && j != rows * cols - 1)
                std::cout << ";" << std::endl;
            else if (j != rows * cols - 1)
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
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
