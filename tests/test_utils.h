#include <sstream>
#include <iomanip>
#include <iostream>
#include "CppUTest/SimpleString.h"
#include "bitboard.h"

inline SimpleString StringFrom(Bitboard b) {
    std::stringstream sstream;
    sstream << b;
    return StringFrom(sstream.str());
}