// #include "wrapping_integers.hh"

// #include <iostream>

// using namespace std;

// //! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
// //! \param n The input absolute 64-bit sequence number
// //! \param isn The initial sequence number
// WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) { return isn + uint32_t(n); }

// //! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
// //! \param n The relative sequence number
// //! \param isn The initial sequence number
// //! \param checkpoint A recent absolute 64-bit sequence number
// //! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
// //!
// //! \note Each of the two streams of the TCP connection has its own ISN. One stream
// //! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
// //! and the other stream runs from the remote TCPSender to the local TCPReceiver and
// //! has a different ISN.
// uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
//  return 0;
// }

#include "wrapping_integers.hh"

#include <bits/stdc++.h>

using namespace std;

const uint64_t val = 1ULL << 32;

WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    // Calculate the relative sequence number by subtracting ISN from the absolute sequence number,
    // and then apply modulo 2^32 to ensure it fits within a 32-bit integer range.
    return(WrappingInt32(static_cast<uint32_t>((n + static_cast<uint64_t>(isn.raw_value())) % val)));
}

uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    uint64_t ch_val = checkpoint % val;

    if (checkpoint - ch_val >= val) {
        ch_val += (ch_val == 0)? val : 0;
    }

    uint64_t diff = static_cast<uint64_t>(n.raw_value() - isn.raw_value());
    uint64_t change = checkpoint - ch_val;
    uint64_t res = change + diff;

    if (diff > ch_val) {
        if ((diff - ch_val) >= (val - diff + ch_val))
            res -= (change >= val) ? val : 0;
    }
    
    else if ((ch_val - diff) > (diff + val - ch_val))
        res += val;

    return res;
}






