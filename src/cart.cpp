#include <iostream>
#include <fstream>
#include <iomanip>
#include "cart.h"

using namespace std;

Cart::Cart( const char* path ) {
    ifstream file( path, ios::in|ios::binary|ios::ate );
    ifstream::pos_type size;

    if( file.is_open() ) {
        size = file.tellg();
        rom = new u8[size];
        file.seekg( 0, ios::beg );
        file.read( ( char* ) rom, size );
        file.close();

        //read the cart header
        hdr = new Header();
        hdr->readHeader( rom );
    } else {
        cout << "ERROR: CANNOT OPEN FILE!" << endl;
    }
}


Cart::~Cart() {
    delete[] rom;
    delete hdr;
}

u8& Cart::operator[]( const int index ) {
    return rom[index];
}
