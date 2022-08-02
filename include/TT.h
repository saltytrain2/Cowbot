#pragma once

#include <vector>
#include <cstdint>

#include "Move.h"
#include "types.h"

struct TTEntry
{
public:
    TTEntry(ZobristHash hash, Move move, int16_t score, uint8_t depth, NodeType node);
    TTEntry() = default;
    
    bool operator==(const TTEntry& rhs) const;
    bool operator!=(const TTEntry& rhs) const;
    operator bool() const;
    uint32_t key;
    Move bestMove;
    int16_t score;
    uint8_t depth;
    NodeType node;
};

class TT
{
public:
    TT() = default;

    void setSize(uint32_t mb);
    void clear();
    void setEntry(ZobristHash hash, TTEntry entry);
    bool keyExists(ZobristHash hash) const;
    const TTEntry& getEntry(ZobristHash hash) const;

private:
    std::vector<TTEntry> mTT;
    uint64_t mask;
};