

#include "TCPServer.h"


CTCPServer::CTCPServer()
{
	cout << endl << "[ TCP_Server ]" << endl << endl << endl;
}


CTCPServer::~CTCPServer()
{
	cout << endl << "Disconnect" << endl << endl;
}


bool CTCPServer::CTCPS_WSA_Init()
{
	int		startupResult;


	// ���� �ּ� ����ü �ʱ�ȭ
	memset( &this->m_S_SocketAddress, 0, sizeof( this->m_S_SocketAddress ) );
	memset( &this->m_C_SocketAddress, 0, sizeof( this->m_C_SocketAddress ) );


	// WinSock( Windows Socket API )�� ����ϱ� ���ؼ� �ʱ�ȭ ��.
	// WinSock : �����쿡�� ��Ʈ��ũ ���( ex. TCP )�� �����ϱ� ���� API��.
	startupResult = WSAStartup( MAKEWORD( 2, 2 ), &this->m_WSAData );
	if( startupResult == SOCKET_ERROR )
	{
		// WSAStartup ���� �� 0 ����, ���� �� 0�� �ƴ� Error Code ������.
		cout << "SOCKET_ERROR Error code - WSAStartup : " << WSAGetLastError() << endl;

		return false;
	}


	return true;
}


bool CTCPServer::CTCPS_WSA_Close()
{
	int		cleanupResult;


	// ���� ����� �����ϱ� ���ؼ� WinSock�� ����� �����Ŵ.
	cleanupResult = WSACleanup();
	if( cleanupResult == SOCKET_ERROR )
	{
		// WSACleanup ���� �� 0 ����, ���� �� 0�� �ƴ� Error Code ������.
		cout << "SOCKET_ERROR Error code - WSACleanup : " << WSAGetLastError() << endl;

		return false;
	}


	return true;
}


bool CTCPServer::CTCPS_TCP_Socket_Init()
{
	// Client�� �����ϱ� ���� ������ ������.
	// PF_INET( af )			: IPv4 �ּҸ� ����ϱ� ���� ����
	// SOCK_STREAM( type )		: ���� ��� ������ �����ϱ� ���� ������,
	//							  SOCK_STREAM�� ����� ��������( TCP ) ����Ʈ ��Ʈ���� �����ϴ� ���� ������.
	// IPPROTO_TCP( protocol )	: ��ſ� ����� ���������� �����ϴ� ������, IPPROTO_TCP�� TCP�� ����Ѵٴ� ���� �ǹ���.
	//							  IP_PROTO_TCP�� af�� PF_INET �Ǵ� AF_INET6���� �����ϰ�, type�� SOCK_STREAM���� ���������� ��� ������.
	this->m_S_SocketHandle = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( this->m_S_SocketHandle == INVALID_SOCKET )
	{
		// ���� ���� ���� �� ���� ��ũ���͸� �����ϰ�, ���� ���� �� INVALID_SOCKET�� ������.
		cout << "INVALID_SOCKET Error code - create server's socket : " << WSAGetLastError() << endl;

		return false;
	}

	// TCP/IP ��Ʈ��ũ ����Ʈ( �򿣵�� ��� ) ��Ʈ ��ȣ�� unsigned short������ ��ȯ�Ͽ� ���� �ּ� ����ü�� ������.
	this->m_S_SocketAddress.sin_port = htons( TCP_PORT );
	if( this->m_S_SocketAddress.sin_port != htons( TCP_PORT ) )
	{
		// Address ����ü �Է� �� ����ó��
		return false;
	}

	// ����� IP �ּ� ������ ���� �ּ� ����ü�� ������.
	this->m_S_SocketAddress.sin_family = AF_INET;
	if( this->m_S_SocketAddress.sin_family != ( unsigned short )AF_INET )
	{
		// Address ����ü �Է� �� ����ó��
		return false;
	}

	// INADDR_ANY : �ش� ���� ��ǻ���� ��ī�� �߿��� ���� ��� ������ IP �ּҸ� �����ͼ� �ش� IP �ּҷ� ������ ���ڴٴ� �ǹ���.
	// TCP/IP ��Ʈ��ũ ����Ʈ( �򿣵�� ��� ) IP �ּҸ� unsigned long������ ��ȯ�Ͽ� ���� �ּ� ����ü�� ������.
	this->m_S_SocketAddress.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	if( this->m_S_SocketAddress.sin_addr.S_un.S_addr != INADDR_ANY )
	{
		// Address ����ü �Է� �� ����ó��
		return false;
	}


	return true;
}


bool CTCPServer::CTCPS_TCP_Socket_Connect()
{
	int		clientSocketSize;

	PCSTR	getIP;

	int		bindResult;
	int		listenResult;


	// bind�� ������ ���� ��ũ���Ϳ� Client�� ���� ��û�� ���� ������ 
	// bind�� ������ ���� ��ũ���Ϳ� ���� �ּ� ����ü ������ �Ҵ���.
	bindResult = bind( this->m_S_SocketHandle, ( SOCKADDR * )&this->m_S_SocketAddress, sizeof( this->m_S_SocketAddress ) );
	if( bindResult == SOCKET_ERROR )
	{
		// bind ���� �� 0 ����, ���� �� 0�� �ƴ� Error Code ������.
		cout << "SOCKET_ERROR Error code - bind server's socket : " << WSAGetLastError() << endl;

		return false;
	}


	// Server�� ���� ���¸� Client�� ���� ��û�� ���� ����ϴ� ���·� ������.
	// listen�� �ι�° ������ backlog�� Server�� accept �������� ����ϸ鼭 ������ ���� �� �ִ� ���� ��û�� �ִ� ���� �ǹ���.
	// ���� ��û�� ���� ������ accept �ϸ� backlog���� �ش� ��û�� ���ŵ�.
	// backlog�� SOMAXCONN���� �����ϸ� �ý��ۿ��� �ո����� �ִ밪�� �ڵ����� �������ְ�,
	// SOMAXCONN_HINT(N)���� �����ϸ� �Է����� N�� ����ŭ �ִ� ������ ���� ������.
	// SOMAXCONN_HINT�� N�� ������ 200 ~ 65535 ��.
	// �ش� TCP ��� ���������� �ϴ��� ���� ����̱� ������ 1�� �ʰ��ϴ� backlog�� �ǹ̰� ����.
	listenResult = listen( this->m_S_SocketHandle, 1 );
	if( listenResult == SOCKET_ERROR )
	{
		// bind ���� �� 0 ����, ���� �� 0�� �ƴ� Error Code ������.
		cout << "SOCKET_ERROR Error code - listen server's socket : " << WSAGetLastError() << endl;

		return false;
	}


	cout << "Await client connect..." << endl << endl;

	clientSocketSize = sizeof( this->m_C_SocketAddress );
	// Server�� �������� ���� ��û�� ������ Client�� ���� ��Ŵ.
	// accept�� ���� ������� ����Ǳ� ������ ���� ��û�� ������ ������ ��� ���� ��û�� ��ٸ�.
	this->m_C_SocketHandle = accept( this->m_S_SocketHandle, ( SOCKADDR * )&this->m_C_SocketAddress, &clientSocketSize );
	if( this->m_C_SocketHandle == INVALID_SOCKET )
	{
		// Ŭ���̾�Ʈ ���� ���� �� ���� ��ũ���͸� �����ϰ�, ���� ���� �� INVALID_SOCKET�� ������.
		cout << "INVALID_SOCKET Error code - accept server's socket : " << WSAGetLastError() << endl;

		return false;
	}

	// ����� Client�� ���� �ּ� ����ü �ȿ� ����� IP �ּҸ� �����ͼ� ���ڿ� �������� ��ȯ��.
	getIP = inet_ntop( this->m_C_SocketAddress.sin_family, ( PSOCKADDR_IN )&this->m_C_SocketAddress.sin_addr, this->clientIP, sizeof( this->clientIP ) );
	if( getIP == NULL )
	{
		cout << "inet_ntop IP address convert error : " << WSAGetLastError() << endl;

		return false;
	}

	cout << "Success connected to client( IP : " << this->clientIP << " )" << endl << endl;


	return true;
}


bool CTCPServer::CTCPS_TCP_Socket_Communication()
{
	E_IN_OUT_Stream_Return	tcpReceive;
	E_IN_OUT_Stream_Return	tcpSend;


	while( 1 )
	{
		// ����, �۽� ����� ���� ���� �ʱ�ȭ
		memset( &this->m_RecvBuffer, 0, sizeof( this->m_RecvBuffer ) );
		memset( &this->m_SendBuffer, 0, sizeof( this->m_SendBuffer ) );


		// TCP ������� ��Ŷ ����
		tcpReceive = CTCPS_TCP_Socket_Receive();
		if( tcpReceive == E_Fail )
		{
			return false;
		}
		// ���� Half-close ���� Ȯ��
		else if( tcpReceive == E_Shutdown )
		{
			break;
		}


		// TCP ������� ��Ŷ �۽�
		tcpSend = CTCPS_TCP_Socket_Send();
		if( tcpSend == E_Fail )
		{
			return false;
		}
	}


	return true;
}


E_IN_OUT_Stream_Return CTCPServer::CTCPS_TCP_Socket_Receive()
{
	int		recvResult;
	int		shutdownResult;


	// ����� Client�� ���� ��Ŷ�� �����Ͽ� ���� ���ۿ� ������.
	recvResult = recv( this->m_C_SocketHandle, this->m_RecvBuffer, PACKET_SIZE, 0 );
	if( recvResult == SOCKET_ERROR )
	{
		// recv ���� �� ���ŵ� ��Ŷ�� ����Ʈ ���� �����ϰ�, ���� �� Error Code ������.
		cout << "SOCKET_ERROR Error code - recv server's socket : " << WSAGetLastError() << endl;

		return E_Fail;
	}
	else if( recvResult == 0 )
	{
		cout << this->clientIP << "> Received" << endl;
		cout << this->clientIP << "> Client is shutdown client's output( send ) stream"  << endl;

		// TCP ���� ���� �ÿ� 4-way handshake ������� �����ϱ� ���ؼ� Half-close ����� �����.
		// Half-close ������� ���� ������ �����ϱ� ���ؼ� Server ������ ��� ��Ʈ���� ���� �����Ŵ.
		// Half-close ����� ����Ͽ��� ������ Server�� ��� ��Ʈ�� ���� ���Ŀ��� Client���� �������� ��Ŷ�� ������ �� ����.
		shutdownResult = shutdown( this->m_C_SocketHandle, SD_SEND );
		if( shutdownResult == SOCKET_ERROR )
		{
			// shutdown ���� �� 0�� �����ϰ�, ���� �� Error Code ������.
			cout << "SOCKET_ERROR Error code - shutdown server's client socket : " << WSAGetLastError() << endl;
			
			return E_Fail;
		}


		return E_Shutdown;
	}
	else
	{
		// ���� ���� Message�� �ֿܼ� �����.
		cout << this->clientIP << "> Received" << endl;
		cout << this->clientIP << "> " << this->m_RecvBuffer << endl;
	}


	return E_Success;
}


E_IN_OUT_Stream_Return CTCPServer::CTCPS_TCP_Socket_Send()
{
	size_t		messageLength;

	errno_t		bufferCopyResult;

	int			sendResult;


	// Client�� Server�� ���� Message�� Server�� Client���� �ٽ� �������ֱ� ���ؼ� ���� ������ �����͸� �۽� ���ۿ� ������.
	bufferCopyResult = strcpy_s( this->m_SendBuffer, this->m_RecvBuffer );
	if( bufferCopyResult != 0 )
	{
		cout << "Receive buffer copy to Send buffer error code : " << bufferCopyResult << endl;

		return E_Fail;
	}

	// �۽��� ��Ŷ�� ����ִ��� �˻��ϱ� ���ؼ� �۽� ������ ũ�⸦ Ȯ����.
	messageLength = strlen( this->m_SendBuffer );
	if( messageLength < 1 )
	{
		cout << "Empty send message buffer" << endl;

		return E_Fail;
	}

	// �۽� ���ۿ� �ִ� �����͸� Client���� ������.
	sendResult = send( this->m_C_SocketHandle, this->m_SendBuffer, messageLength, 0 );
	if( sendResult == SOCKET_ERROR )
	{
		// send ���� �� �۽��� ��Ŷ�� ����Ʈ ���� �����ϰ�, ���� �� Error Code ������.
		cout << "SOCKET_ERROR Error code - send server's client socket : " << WSAGetLastError() << endl;

		return E_Fail;
	}

	cout << this->clientIP << "> Sent"<< endl << endl;


	return E_Success;
}


bool CTCPServer::CTCPS_TCP_Socket_Close()
{
	int		closeResult;


	// ������ Close �ϱ� ���� ���� ��ũ���Ͱ� ���������� �����ϴ��� Ȯ����.
	if( this->m_S_SocketHandle == 0 )
	{
		return false;
	}
	
	if( this->m_C_SocketHandle == 0 )
	{
		return false;
	}


	// CTCPS_TCP_Socket_Close �Լ� ȣ�� ������ Half-close�� Server�� ��� ��Ʈ���� ����Ǿ� �ִ� ���¿��� Server�� ���� ������ ������ �����Ŵ.
	closeResult = closesocket( this->m_S_SocketHandle );
	if( closeResult == SOCKET_ERROR )
	{
		// closesocket ���� �� 0 ����, ���� �� 0�� �ƴ� Error Code ������.
		cout << "SOCKET_ERROR Error code - close server's server socket : " << WSAGetLastError() << endl;

		return false;
	}
	
	closeResult = closesocket( this->m_C_SocketHandle );
	if( closeResult == SOCKET_ERROR )
	{
		cout << "SOCKET_ERROR Error code - close server's client socket : " << WSAGetLastError() << endl;

		return false;
	}


	return true;
}