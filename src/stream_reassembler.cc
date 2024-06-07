#include "stream_reassembler.hh"
#include <bits/stdc++.h>

// You will need to add private members to the class declaration in stream_reassembler.hh

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

// StreamReassembler::StreamReassembler(const size_t capacity)
//     :_output(capacity)
// {}


// //! \details This function accepts a substring (aka a segment) of bytes,
// //! possibly out-of-order, from the logical stream, and assembles any newly
// //! contiguous substrings and writes them into the output stream in order.
// void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
// }

// size_t StreamReassembler::unassembled_bytes() const { return 0; }

// bool StreamReassembler::empty() const { return 0; }

// size_t StreamReassembler::ack_index() const { return 0; }

//_______________________________________________
#include <map>

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), ack_index_(0), unass_base_(0), _buffer(), capac(capacity), eof_(-1) {
}

void StreamReassembler::slide_window() {
    //if there is data in _buffer for the next expected value
    string next_data = "";

    while (_buffer.count(unass_base_) > 0) {
        next_data = _buffer.at(unass_base_);

        // if(_output.remaining_capacity() == 0){
        //     return;
        // }
        //int update = min(data2.size(), _output.remaining_capacity());

        _output.write(next_data);

        _buffer.erase(unass_base_);


        unass_base_ +=1;
        if (eof_ == unass_base_ - 1){
            _output.end_input();
        }
    }
    return ;
}


void StreamReassembler::push_substring(const string &data, const uint64_t index, const bool eof) {
    
    uint64_t index2 = index;
    string data2 = data;

    if (eof == true){
        eof_ = index2;
        eof_ += (data2.size()==0) ? 0 : data2.size() - 1; //empty string   
    }

    //outside 
    if (index2 >= unass_base_ + capac) //--
        return;

    //overlap
    if (index2 < unass_base_ && index2 + data2.size() > unass_base_){
        data2 = data2.substr(unass_base_- index2); //bc
        index2 = unass_base_;
    }

    //behind
    else if (index2 < unass_base_ && index2 + data2.size() <= unass_base_ )
        return;


    //trim
    if( index2 + data2.size() > unass_base_ + capac){
        data2 = data2.substr(0, unass_base_+ capac);
    }


    if ( data2.size() > _output.remaining_capacity()){
        data2 = data2.substr(0, _output.remaining_capacity());
    }
    
    
    // Output stream
    if(unass_base_ == index2 && _output.remaining_capacity()>0){
        
        //int update = min(data2.size(), _output.remaining_capacity());

        _output.write(data2);

        // if(_output.remaining_capacity()==0){
        //     //pass
        // }
        // else 
        unass_base_ += (data2.size()==0)? 1 : data2.size(); //&& _output.remaining_capacity()>0){ // Deadlock situation empty string

        //unass_base_ = ack_index_; 

        if (eof_ == unass_base_ - 1){// || capac == unass_base_){
            _output.end_input();
        }

    }
    //Buffered
    else {
            
        for (int i = 0; i < data2.size(); i++) {
            //cout << index2 <<" and " << data2[i] << endl;
            if (_buffer.count(index2) == 0 && index2 < capac) {
                //cout << index2<<" : "<<data2[i]<<endl;
                _buffer[index2] = data2[i];
            }
            index2 +=1;
            }
    }
    
    //cleaning buffer 
    auto it = _buffer.lower_bound(unass_base_);
    _buffer.erase(_buffer.begin(), it);

    slide_window();
    ack_index_ = unass_base_;


}

size_t StreamReassembler::unassembled_bytes() const {
    return _buffer.size();
}

bool StreamReassembler::empty() const {
    return _output.buffer_empty() && unassembled_bytes() == 0;
}

size_t StreamReassembler::ack_index() const {
    return ack_index_;
}