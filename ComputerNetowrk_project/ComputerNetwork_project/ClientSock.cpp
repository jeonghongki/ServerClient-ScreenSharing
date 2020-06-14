// ClientSock.cpp: 구현 파일
//

#include "stdafx.h"
#include "ComputerNetwork_project.h"
#include "ClientSock.h"


// ClientSock

ClientSock::ClientSock()
{
}

ClientSock::~ClientSock()
{
}


// ClientSock 멤버 함수


void ClientSock::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	((CComputerNetworkprojectApp*)AfxGetApp())->CloseChild();
	CAsyncSocket::OnClose(nErrorCode);
}


void ClientSock::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	((CComputerNetworkprojectApp*)AfxGetApp())->ReceiveData();
	CAsyncSocket::OnReceive(nErrorCode);
}
