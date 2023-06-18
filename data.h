
#ifndef DATA_H
#define DATA_H

#include <QtGlobal>

#define UDP_HEADER_SERVER 0xABCD
#define UDP_HEADER_CLIENT 0x1234

// Server
#pragma pack(push, 1)
struct Message1
{
    quint16 header = UDP_HEADER_SERVER;
    quint16 height;
};
#pragma pack(pop)

// Client
#pragma pack(push, 1)
struct Message2
{
    quint16 header = UDP_HEADER_CLIENT;
};
#pragma pack(pop)

#define UDP_TOKEN_ADDRESS "address"
#define UDP_TOKEN_PORT    "port"
#define UDP_TOKEN_SERVER  "server"
#define UDP_TOKEN_CLIENT  "client"

#endif // DATA_H
