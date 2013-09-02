#ifndef CART_H
#define CART_H

#include <string>
#include "types.h"
#include "header.h"

class Cart {
    friend class MMU;
public:
    Header* hdr;

    Cart( const char* path );
    ~Cart();

    u8& operator[]( const int index );
private:
    u8* rom;
};

#endif // CART_H
