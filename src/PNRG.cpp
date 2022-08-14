#include "PRNG.h"

using namespace Cowbot;

PRNG::PRNG(uint64_t seed) : s(seed)
{}

uint64_t PRNG::rand()
{
    s ^= s >> 12;
    s ^= s << 25;
    s ^= s >> 27;
    return s * uint64_t(2685821657736338717);
}