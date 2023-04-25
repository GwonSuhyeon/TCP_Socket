

#include "Client.h"


CClient::CClient()
{

}


CClient::~CClient()
{
	
}


bool CClient::CC_Client_Run()
{
	bool	wsaInit;
	bool	wsaDelete;
	bool	socketInit;
	bool	socketConnect;
	bool	socketCommunication;
	bool	socketDelete;


	// WinSock �ʱ�ȭ
	wsaInit = TCP_Client.CTCPC_WSA_Init();
	if( wsaInit == false )
	{
		// WinSock �ʱ�ȭ ����
		return false;
	}

	// TCP ���� �ʱ�ȭ
	socketInit = TCP_Client.CTCPC_TCP_Socket_Init();
	if( socketInit == false )
	{
		// TCP ���� �ʱ�ȭ ����
		return false;
	}

	// TCP ���� ����
	socketConnect = TCP_Client.CTCPC_TCP_Socket_Connect();
	if( socketConnect == false )
	{
		// TCP ���� ���� ����
		return false;
	}

	// TCP ���
	socketCommunication = TCP_Client.CTCPC_TCP_Socket_Communication();
	if( socketCommunication == false )
	{
		// TCP ��� ����
		return false;
	}

	// TCP ���� ���� ���� ����
	socketDelete = TCP_Client.CTCPC_TCP_Socket_Close();
	if( socketDelete == false )
	{
		// TCP ���� ���� ���� ���� ����
		return false;
	}

	// WinSock ����
	wsaDelete = TCP_Client.CTCPC_WSA_Cleanup();
	if( wsaDelete == false )
	{
		// WinSock ���� ����
		return false;
	}


	return true;
}