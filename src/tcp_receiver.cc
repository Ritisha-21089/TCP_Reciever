#include "tcp_receiver.hh"
#include <bits/stdc++.h>
#include <algorithm>


using namespace std;

// void TCPReceiver::segment_received(const TCPSegment &seg) {
//     const TCPHeader head = seg.header();
//     // ...

//     // --- Hint ------
//         // convert the seqno into absolute seqno
//     uint64_t checkpoint = _reassembler.ack_index();
//     uint64_t abs_seqno = unwrap(head.seqno, _isn, checkpoint);
//     uint64_t stream_idx = abs_seqno - _synReceived;
//     // --- Hint ------  

//     // ... 
// }

// optional<WrappingInt32> TCPReceiver::ackno() const {
//     return {};
// }

// size_t TCPReceiver::window_size() const { return 0; }




void TCPReceiver::segment_received(const TCPSegment &seg) {

    const TCPHeader head = seg.header();
    string pay = seg.payload().copy();

    uint64_t checkpoint = _reassembler.ack_index();
    uint64_t abs_seqno = unwrap(head.seqno, _isn, checkpoint);
    //uint64_t stream_idx = abs_seqno - _synReceived;

    bool is_syn = head.syn;
    bool is_fin = head.fin;


    if (!_synReceived) {
        if (!is_syn)
            return;
        _synReceived = true;
        _isn = head.seqno;
    }
    

    if (pay.size() > 0) {
        
        if (is_syn || head.seqno != _isn) {
            
            size_t index = unwrap(head.seqno - (!is_syn), _isn, checkpoint);
            _reassembler.push_substring(pay, index, is_fin);
        }
    }

    if (is_fin || _finReceived) {
        _finReceived = true;
        if (_reassembler.unassembled_bytes() == 0)
            _reassembler.stream_out().end_input();
    }
}



optional<WrappingInt32> TCPReceiver::ackno() const {

    if (!_synReceived)
        return nullopt;

    uint64_t abs_seqno = _synReceived + _reassembler.ack_index();

    if (_reassembler.stream_out().input_ended())
        abs_seqno++;

    return wrap(abs_seqno, _isn);
}


size_t TCPReceiver::window_size() const {
    return (_capacity - _reassembler.stream_out().buffer_size());
}
