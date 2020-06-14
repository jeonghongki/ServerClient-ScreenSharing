#pragma once
#include <afxsock.h>
// ServerSock 명령 대상

class ServerSock : public CAsyncSocket
{
public:
	ServerSock();
	virtual ~ServerSock();
	virtual void OnReceive(int nErrorCode);
};


