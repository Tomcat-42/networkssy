## intro

1. **What is a Socket?**: In networking, a socket is an endpoint in a communication between two systems over a network. It provides a way for a process to communicate with another process, either on the same system or on another system.

2. **Types of Sockets**: There are two main types of sockets used in Linux:
    - **Stream Sockets (SOCK_STREAM)**: These use TCP (Transmission Control Protocol) for data transmission. They're reliable (data sent arrives intact and in order), connection-oriented (communication only begins once a connection is established), and they use byte streams, not messages.
    - **Datagram Sockets (SOCK_DGRAM)**: These use UDP (User Datagram Protocol). They're unreliable (data may not arrive at all, or it may be corrupted), connectionless (data can be sent without establishing a connection), and they use messages, not byte streams.

3. **Socket System Calls**: These are essential functions used when dealing with sockets:
    - `socket()`: creates a new socket of a certain socket type, and returns a file descriptor.
    - `bind()`: binds a socket to a specific IP address and port number.
    - `listen()`: enables the socket to accept connections.
    - `accept()`: accepts a new connection and returns a new socket file descriptor for that connection.
    - `connect()`: initiates a connection to a remote server.
    - `send()`, `recv()`, `read()`, `write()`: used to send and receive data.
    - `close()`: closes a socket (ends the connection).

4. **Byte Ordering**: Network protocols expect network byte order (Big Endian), but host systems can be either Big Endian or Little Endian. Functions like `htons()`, `htonl()`, `ntohs()`, and `ntohl()` are used to ensure that integer values are transmitted through the network correctly.

5. **Blocking vs Non-blocking**: By default, socket operations are blocking. This means that the execution of your program will stop or "block" until the operation completes. However, you can set a socket to non-blocking mode which means the call will not block and instead return an error if it would have to block (or, for connect(), continue working in the background and complete later).

6. **Error Handling**: All system calls return `-1` on error, and set the `errno` variable to the error number. You can get a human-readable version of the error with `perror()` or `strerror()`.

7. **Address Structures**: `sockaddr_in` for IPv4, `sockaddr_in6` for IPv6, and `sockaddr_un` for Unix domain sockets. These structures are used to specify addresses to bind to, connect to, etc.

This is a basic overview of Linux sockets. However, the specifics and nuances of each point can fill a book. You'll want to delve deeper into each of these topics as you get more comfortable with networking in Linux.

```text
╔═══════════╦══════════════════════════╗
║           ║       Socket Type        ║
║ Address   ╟────────────┬─────────────╢
║ Family    ║ SOCK_DGRAM │ SOCK_STREAM ║
╠═══════════╬════════════╪═════════════╣
║ IPX/SPX   ║ SPX        │ IPX         ║
║ NetBIOS   ║ NetBIOS    │ n/a         ║
║ IPv4      ║ UDP        │ TCP         ║
║ AppleTalk ║ DDP        │ ADSP        ║
║ IPv6      ║ UDP        │ TCP         ║
║ IrDA      ║ IrLMP      │ IrTTP       ║
║ Bluetooth ║ ?          │ RFCOMM      ║
╚═══════════╩════════════╧═════════════╝

╔═══════════╦══════════════════════════════════════════════════════╗
║           ║                      Socket Type                     ║
║ Address   ╟────────────┬─────────────┬──────────┬────────────────╢
║ Family    ║ SOCK_DGRAM │ SOCK_STREAM │ SOCK_RDM │ SOCK_SEQPACKET ║
╠═══════════╬════════════╪═════════════╪══════════╪════════════════╣
║ IPX/SPX   ║ SPX        │ IPX         │ ?        │ ?              ║
║ NetBIOS   ║ NetBIOS    │ n/a         │ ?        │ ?              ║
║ IPv4      ║ UDP        │ TCP         │ ?        │ SCTP           ║
║ AppleTalk ║ DDP        │ ADSP        │ ?        │ ?              ║
║ IPv6      ║ UDP        │ TCP         │ ?        │ SCTP           ║
║ IrDA      ║ IrLMP      │ IrTTP       │ ?        │ ?              ║
║ Bluetooth ║ ?          │ RFCOMM      │ ?        │ ?              ║
╚═══════════╩════════════╧═════════════╧══════════╧════════════════╝
```


## Resources

Here are some key manual (man) pages for you to explore further about Linux networking and sockets:

1. **Socket Programming**: 
   - `man 2 socket`: Information about the socket system call.
   - `man 2 bind`: Details on binding a socket to an address.
   - `man 2 listen`: Information on preparing a socket to accept connections.
   - `man 2 accept`: Details on accepting a new connection on a socket.
   - `man 2 connect`: Information about initiating a connection on a socket.
   - `man 2 send`: Information about sending data on a socket.
   - `man 2 recv`: Details on receiving data from a socket.
   - `man 2 close`: Information on closing a socket.

2. **Network Data Handling**:
   - `man 3 htons`: Describes the byte ordering functions.
   - `man 3 ntohs`: Describes the byte ordering functions.

3. **Addressing**:
   - `man 7 ip`: Describes the format of IPv4 socket addresses.
   - `man 7 ipv6`: Describes the format of IPv6 socket addresses.
   - `man 7 unix`: Describes the format of Unix socket addresses.

4. **General Information**:
   - `man 7 socket`: Overview of sockets.
   - `man 2 fcntl`: Information about how to control file descriptors, including setting non-blocking mode.
   - `man 3 perror`: Information about error reporting.

Remember that man pages are divided into sections. The number before the page name corresponds to a particular section. The sections cover different areas like system calls, library functions, special files, file formats, games, etc.

In a Linux terminal, you can access these manual pages by typing `man` followed by the section number and the topic. For example, `man 2 socket` would show you the manual page for the socket system call.
