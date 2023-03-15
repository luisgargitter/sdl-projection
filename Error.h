#ifndef ERROR_H
#define ERROR_H

typedef enum
{
    ERR_OK = 0U,    //<! Everything fine, no error
    ERR_NULLPTR,    //<! Null pointer 
    ERR_EXCEEDS_BOUNDS //<! Index is out of bounds
} Error_t;

#endif
