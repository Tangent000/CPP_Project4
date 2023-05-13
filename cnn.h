#pragma once
#include <iostream>
#define DEBUG_MODE

typedef enum _mattype {UNSIGNED_CHAR, SHORT, INT, FLOAT, DOUBLE} mattype;

class Mat
{
    public:
    size_t rows;
    size_t cols;
    size_t channels;
    mattype type;

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
    virtual Mat & operator+ (const Mat & rhs)=0;
    virtual Mat & operator- (const Mat & rhs)=0;
    virtual Mat & operator* (const Mat & rhs)=0;
    virtual Mat & operator/ (const Mat & rhs)=0;
    virtual Mat & operator+= (const Mat & rhs)=0;
    virtual Mat & operator-= (const Mat & rhs)=0;
    virtual Mat & operator*= (const Mat & rhs)=0;
    virtual Mat & operator/= (const Mat & rhs)=0;
};


template <typename T>
class Mat_:public Mat
{
    public:
    size_t * refcount;
    T * data;

    Mat_ (size_t rows = 0, size_t cols = 0, size_t channels = 0, T * data = NULL)
        : Mat(rows, cols, channels, type), data(data), refcount(new size_t(1))
    {
        #ifdef DEBUG_MODE
        std::cout << "Mat_ constructor called." << std::endl;
        #endif
        //check the dimensions
        if(rows == 0 || cols == 0 || channels == 0)
            throw std::invalid_argument("Mat_ constructor: rows, cols, channels must be positive.");
        //check if the data and dimensions match
        if(rows * cols * channels != sizeof(data) / sizeof(T))
            throw std::invalid_argument("Mat_ constructor: data size does not match.");
        if(data == NULL)
        {
            this->data = new T[rows * cols * channels];
        }
        //set the type according to the typeid of T
        if(typeid(T) == typeid(unsigned char))
            type = UNSIGNED_CHAR;
        else if(typeid(T) == typeid(short))
            type = SHORT;
        else if(typeid(T) == typeid(int))
            type = INT;
        else if(typeid(T) == typeid(float))
            type = FLOAT;
        else if(typeid(T) == typeid(double))
            type = DOUBLE;
        else
            throw std::invalid_argument("Mat_ constructor: type not supported.");
    }
    
    Mat_(const Mat_ &rhs) :Mat(rhs), data(rhs.data), refcount(rhs.refcount) 
    { 
        #ifdef DEBUG_MODE
        std::cout << "Mat_ copy constructor called." << std::endl;
        #endif
        refcount++;
    }
    
    Mat_ & operator= (const Mat_ & rhs)
    {
        #ifdef DEBUG_MODE
        std::cout << "Mat_ assignment operator called." << std::endl;
        #endif
        if (this != &rhs)
        {
            Mat::operator=(rhs);
            if(--*refcount == 0)
            {
                delete [] data;
                delete refcount;
            }
            data = rhs.data;
            refcount = rhs.refcount;
            refcount++;
        }
        return *this;
    }

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

    bool operator== (const Mat &rhs)
    {
        #ifdef DEBUG_MODE
        std::cout << "Mat_ equality operator called." << std::endl;
        #endif
        if(Mat::operator==(rhs))    //do not care the type, e.g. for Matrix of short type or Matrix of int type, if their data are of the same value, they are "equal".
        {
            Mat_ & rhs_ = (Mat_ &)rhs;  //cast (Mat)rhs to Mat_ type
            for(size_t i = 0; i < rows * cols * channels; i++)
            {
                if(data[i] != rhs_.data[i])
                    return false;
            }
            return true;
        }
        return false;
    }
};