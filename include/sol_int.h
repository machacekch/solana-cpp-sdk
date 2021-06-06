#pragma once
/**
 * Integral types with an explicit width, as well as types with specific semantics
 *
 * Epoch, Lamports and Slot are currently thin wrappers around uint64_t.
 * This might change as they would benefit from operators with implicit wrap-around detection.
 */

#ifndef __LP64__
#error LP64 data model required
#endif

namespace sol {

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long int int64_t;
typedef unsigned long int uint64_t;
typedef int64_t ssize_t;
typedef uint64_t size_t;

static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(uint8_t) == 1);
static_assert(sizeof(int16_t) == 2);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(int32_t) == 4);
static_assert(sizeof(uint32_t) == 4);
static_assert(sizeof(int64_t) == 8);
static_assert(sizeof(uint64_t) == 8);

class Epoch {
    uint64_t epoch;

public:
    Epoch() : epoch(0) {}
    explicit Epoch(uint64_t epoch) : epoch(epoch) {}

    operator uint64_t() const {return epoch;}
};

class Lamports {
    uint64_t lamports;

public:
    Lamports() : lamports(0) {}
    explicit Lamports(uint64_t lamports) : lamports(lamports) {}

    operator uint64_t() const {return lamports;}
};

class Slot {
    uint64_t slot;

public:
    Slot() : slot(0) {}
    explicit Slot(uint64_t slot) : slot(slot) {}

    operator uint64_t() const {return slot;}
};

} // namespace sol
