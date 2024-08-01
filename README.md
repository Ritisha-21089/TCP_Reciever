# TCP Receiver Project

## Background on TCP

TCP (Transmission Control Protocol) is used to ensure reliable byte streams between applications. The Internet, however, only provides a "best effort" service for delivering short pieces of data, called Internet datagrams. TCP provides reliability by ensuring that the bytes received at the receiver side are exactly the same and in the same order as sent by the sender.

## Part I: Building ByteStream

A ByteStream class stores a collection of bytes from which bytes can be read or written to. The ByteStream class should have the following properties:

1. Bytes are written on the input side and read out from the output side.
2. The byte stream is finite. The writer can end the input and no more bytes can be written.
3. When the reader has read to the end of the stream, it will reach EOF.
4. The stream has a particular capacity which limits the total amount of bytes that can be held in memory at once.
5. The writer cannot write into the byte stream if it exceeds the storage capacity.
6. As the reader reads bytes from the stream, the writer is allowed to write more.

**Implementation Steps:**
- Implemented the ByteStream class in `src/byte_stream.cc` based on the interface in `src/byte_stream.hh`.
- Run `make` inside the build directory to build the project.
- Run `ctest -R '^byte_stream'` to test the implementation.

## Part II: Building a Reassembler

A Reassembler class reassembles the segments received from the sender into a contiguous stream of bytes. The Reassembler receives substrings from the sender along with their position within the larger stream.

![image](https://github.com/user-attachments/assets/a41b7897-af7c-41f3-99da-7555a55070aa)

**Implementation Steps:**
- Implemented the Reassembler class in `src/reassembler.cc` based on the interface in `src/reassembler.hh`.
- The Reassembler handles three categories of knowledge:
  1. Bytes that are the next in the stream.
  2. Bytes that fit within the stream’s capacity but can't yet be written.
  3. Bytes that lie beyond the stream’s capacity and should be discarded.
- Run `make` inside the build directory to build the project.
- Run `ctest -R '^reassembler'` to test the implementation.

## Part III: Building the TCP Receiver

The TCP Receiver accepts TCPSegment from the sender and uses the Reassembler to correctly write the bytes into the ByteStream. The TCP Receiver also generates messages back to the sender indicating the acknowledgment number (ackno) and the available capacity in the output ByteStream (window size).

**Implementation Steps:**
- Implemented the TCP Receiver class in `src/tcp_receiver.cc` based on the interface in `src/tcp_receiver.hh`.
- Implemented the conversion routines for WrappingInt32 in `src/wrapping_integers.cc`.
- Run `make` inside the build directory to build the project.
- Run `ctest -R '^tcp_receiver'` to test the implementation.

## Running Tests

To test the entire implementation of the TCP receiver, run the following command inside the build directory:
```sh
ctest
```
