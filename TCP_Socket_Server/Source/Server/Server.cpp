

#include "Server.h"


CServer::CServer()
{

}


CServer::~CServer()
{
	
}


bool CServer::CS_Server_Run()
{
	bool	wsaInit;
	bool	wsaDelete;
	bool	socketInit;
	bool	socketConnect;
	bool	socketCommunication;
	bool	socketDelete;


	// WinSock 초기화
	wsaInit = TCP_Server.CTCPS_WSA_Init();
	if( wsaInit == false )
	{
		// WinSock 초기화 실패
		return false;
	}

	// TCP 소켓 초기화
	socketInit = TCP_Server.CTCPS_TCP_Socket_Init();
	if( socketInit == false )
	{
		// TCP 소켓 초기화 실패
		return false;
	}

	// TCP 소켓 연결
	socketConnect = TCP_Server.CTCPS_TCP_Socket_Connect();
	if( socketConnect == false )
	{
		// TCP 소켓 연결 실패
		return false;
	}

	// TCP 통신
	socketCommunication = TCP_Server.CTCPS_TCP_Socket_Communication();
	if( socketCommunication == false )
	{
		// TCP 통신 실패
		return false;
	}

	// TCP 소켓 완전 연결 종료
	socketDelete = TCP_Server.CTCPS_TCP_Socket_Close();
	if( socketDelete == false )
	{
		// TCP 소켓 완전 연결 종료 실패
		return false;
	}

	// WinSock 제거
	wsaDelete = TCP_Server.CTCPS_WSA_Close();
	if( wsaDelete == false )
	{
		// WinSock 제거 실패
		return false;
	}


	return true;
}