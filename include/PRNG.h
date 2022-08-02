#pragma once
#include <cstdint>


// Xorshift Random Number Generator
// Choice of shift amounts comes from Stockfish
class PRNG 
{
public:
    PRNG(uint64_t seed);
    uint64_t rand();

private:
    uint64_t s;
};