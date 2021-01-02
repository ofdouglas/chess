#include "CppUTest/SimpleString.h"
#include "bitboard.h"

inline SimpleString StringFrom(Bitboard b) {
    return StringFromFormat("%016lx", b.GetBits());
}