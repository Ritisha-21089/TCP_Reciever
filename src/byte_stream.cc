#include "byte_stream.hh"
#include <queue> 

using namespace std;

ByteStream::ByteStream(const size_t capa) : _capacity(capa), input_ends(false), total_written(0), total_read(0), _error(false) {}

size_t ByteStream::write(const std::string &data) {

    if (input_ended() || error()) { 
        return 0;
    }

    size_t to_write = min(data.size(), remaining_capacity());
    total_written +=  to_write; 

    for (size_t i = 0; i < to_write; i++) {
        _buffer.push_back(data[i]);
    }
    
    return to_write;
}

size_t ByteStream::remaining_capacity() const {
    if(_capacity <= _buffer.size()){
        return 0;
    }
    return (_capacity - _buffer.size());
}

void ByteStream::end_input() {
    input_ends = true;
}

void ByteStream::set_error() { 
    _error = true; 
}

string ByteStream::peek_output(const size_t len) const {
    size_t to_peek = min(len, _buffer.size());
    return string(_buffer.begin(), _buffer.begin() + to_peek);
}

void ByteStream::pop_output(const size_t len) {

    if(len > _buffer.size()){
        set_error();
        return;
    }

    if (_buffer.empty()) {
        return;
    }

    //Altho i have returned the function, as soon as we get to have a case where len is greater than buffer size, 
    //but still here i am taking min out of len & buffer size (redundant code) 
    size_t to_pop = min(len, buffer_size());
    total_read += to_pop;

    for (size_t i = 0; i < to_pop; i++) {
        if (!_buffer.empty()) {
            _buffer.pop_front(); 
        }
    }
}

string ByteStream::read(const size_t len) {

    string output= "";

    if(len > _buffer.size()){
        set_error();
        return output;
    }

    //Altho i have returned the function, as soon as we get to have a case where len is greater than buffer size, 
    //but still here i am taking min out of len & buffer size (redundant code) 

    size_t to_read = min(len, buffer_size());
    total_read += to_read;
    
    for (size_t i = 0; i < to_read; i++) {
        if (!_buffer.empty()) {
            output += _buffer.front();
            _buffer.pop_front();
        }
        else{
            break;
        }
    }

    return output;
}

bool ByteStream::input_ended() const {
    return input_ends;
}

bool ByteStream::error() const { 
    return _error; 
}

size_t ByteStream::buffer_size() const {
    return _buffer.size();
}

bool ByteStream::buffer_empty() const {
    return _buffer.empty();
}

bool ByteStream::eof() const {
    return input_ended() && buffer_empty();
}

size_t ByteStream::bytes_written() const {
    return total_written;
}

size_t ByteStream::bytes_read() const {
    return total_read;
}
