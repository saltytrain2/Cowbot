#include "TT.h"

#include "Move.h"
#include "types.h"

using namespace Cowbot;

TTEntry::TTEntry(ZobristHash hash, Move move, int16_t score, uint8_t depth, NodeType node)
    : key(hash),
      bestMove(move), 
      score(score),
      depth(depth),
      node(node)
{}

bool TTEntry::operator==(const TTEntry& rhs) const
{
    return node == rhs.node && depth == rhs.depth && score == rhs.score && bestMove == rhs.bestMove && key == rhs.key;
}

bool TTEntry::operator!=(const TTEntry& rhs) const
{
    return !(*this == rhs);
}

TTEntry::operator bool() const
{
    return *this != TTEntry();
}

void TT::clear()
{
    mTT.clear();
}

void TT::setEntry(ZobristHash hash, TTEntry entry)
{
    mTT[hash & mask] = entry;
}

bool TT::keyExists(ZobristHash hash) const
{
    return mTT[hash & mask] != TTEntry() && hash == mTT[hash & mask].key;
}

const TTEntry& TT::getEntry(ZobristHash hash) const
{
    return mTT[hash & mask];
}

void TT::setSize(uint32_t mb)
{
    uint32_t numElements = mb * 1024 * 1024 / sizeof(TTEntry);

    uint64_t size = 1;
    while (numElements > size) {
        size <<= 1;
    }
    
    std::fill(mTT.begin(), mTT.end(), TTEntry());
    mTT.resize(size);
    mask = size - 1;
}