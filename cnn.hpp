#pragma once
#include <iostream>
#define DEBUG_MODE

typedef enum _mattype {UNSIGNED_CHAR, SHORT, INT, FLOAT, DOUBLE} mattype;

template <typename T> class Mat_;   //forward declaration (let Mat class knows the existance of Mat_ class)

class Mat
{
    public:
    size_t rows;
    size_t cols;
    size_t channels;
    mattype type;
    int * _data;    //used to point to data from derived classes

    Mat(size_t rows = 0, size_t cols = 0, size_t channels = 0, mattype type = DOUBLE)
        : rows(rows), cols(cols), channels(channels), type(type)
    {
        #ifdef DEBUG_MODE
        std::cout << "Mat constructor called." << std::endl;
        #endif
    }

    Mat(const Mat & rhs)
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

    virtual Mat & operator= (const Mat & rhs)
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

    virtual bool operator== (const Mat & rhs)
    {
        #ifdef DEBUG_MODE
        std::cout << "Mat equality operator called." << std::endl;
        #endif
        return (rows == rhs.rows && cols == rhs.cols && channels == rhs.channels);
    }

    virtual bool operator!= (const Mat & rhs){ return !(*this == rhs);}
    // virtual Mat & operator+ (const Mat & rhs)=0;
    // virtual Mat & operator- (const Mat & rhs)=0;
    // virtual Mat & operator* (const Mat & rhs)=0;
    // virtual Mat & operator/ (const Mat & rhs)=0;
    // virtual Mat & operator+= (const Mat & rhs)=0;
    // virtual Mat & operator-= (const Mat & rhs)=0;
    // virtual Mat & operator*= (const Mat & rhs)=0;
    // virtual Mat & operator/= (const Mat & rhs)=0;
    //for type conversions
    template <typename T>
    operator Mat_<T>&();
};

template <typename T>
class Mat_:public Mat
{
    public:
    size_t * refcount;
    T * data;

    Mat_ (size_t rows = 0, size_t cols = 0, size_t channels = 0, T * data = NULL);
    Mat_(const Mat_ &rhs);
    Mat_ & operator= (const Mat_ & rhs);

    ~Mat_()
    {
        #ifdef DEBUG_MODE
        std::cout << "Mat_ destructor called." << std::endl;
        #endif
        if(--*refcount == 0)
        {
            delete [] data;
            delete refcount;
        }
    }

    bool operator== (const Mat &rhs);
    // Mat_ & operator+ (const Mat & rhs);
    // Mat_ & operator- (const Mat & rhs);
    // Mat_ & operator* (const Mat & rhs);
    // Mat_ & operator/ (const Mat & rhs);
};

// declare the template classes here
typedef Mat_<unsigned char> Mat_uchar;
typedef Mat_<short> Mat_short;
typedef Mat_<int> Mat_int;
typedef Mat_<float> Mat_float;
typedef Mat_<double> Mat_double;


