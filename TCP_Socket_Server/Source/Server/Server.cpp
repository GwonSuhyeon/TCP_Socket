

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


	// WinSock �ʱ�ȭ
	wsaInit = TCP_Server.CTCPS_WSA_Init();
	if( wsaInit == false )
	{
		// WinSock �ʱ�ȭ ����
		return false;
	}

	// TCP ���� �ʱ�ȭ
	socketInit = TCP_Server.CTCPS_TCP_Socket_Init();
	if( socketInit == false )
	{
		// TCP ���� �ʱ�ȭ ����
		return false;
	}

	// TCP ���� ����
	socketConnect = TCP_Server.CTCPS_TCP_Socket_Connect();
	if( socketConnect == false )
	{
		// TCP ���� ���� ����
		return false;
	}

	// TCP ���
	socketCommunication = TCP_Server.CTCPS_TCP_Socket_Communication();
	if( socketCommunication == false )
	{
		// TCP ��� ����
		return false;
	}

	// TCP ���� ���� ���� ����
	socketDelete = TCP_Server.CTCPS_TCP_Socket_Close();
	if( socketDelete == false )
	{
		// TCP ���� ���� ���� ���� ����
		return false;
	}

	// WinSock ����
	wsaDelete = TCP_Server.CTCPS_WSA_Close();
	if( wsaDelete == false )
	{
		// WinSock ���� ����
		return false;
	}


	return true;
}