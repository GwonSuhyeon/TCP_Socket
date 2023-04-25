

#include "TCPClient.h"


CTCPClient::CTCPClient()
{
	cout << endl << "[ TCP_Client ]" << endl << endl << endl;
}


CTCPClient::~CTCPClient()
{
	cout << endl << "Disconnect" << endl << endl;
}


bool CTCPClient::CTCPC_WSA_Init()
{
	int		startupResult;


	// ���� �ּ� ����ü �ʱ�ȭ
	memset( &this->m_SocketAddress, 0, sizeof( this->m_SocketAddress ) );


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


bool CTCPClient::CTCPC_WSA_Cleanup()
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


bool CTCPClient::CTCPC_TCP_Socket_Init()
{
	int		ipConvertResult;


	// ������ �����ϱ� ���� ������ ������.
	// PF_INET( af )			: IPv4 �ּҸ� ����ϱ� ���� ����
	// SOCK_STREAM( type )		: ���� ��� ������ �����ϱ� ���� ������,
	//							  SOCK_STREAM�� ����� ��������( TCP ) ����Ʈ ��Ʈ���� �����ϴ� ���� ������.
	// IPPROTO_TCP( protocol )	: ��ſ� ����� ���������� �����ϴ� ������, IPPROTO_TCP�� TCP�� ����Ѵٴ� ���� �ǹ���.
	//							  IP_PROTO_TCP�� af�� PF_INET �Ǵ� AF_INET6���� �����ϰ�, type�� SOCK_STREAM���� ���������� ��� ������.
	this->m_SocketHandle = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( this->m_SocketHandle == INVALID_SOCKET )
	{
		// ���� ���� ���� �� ���� ��ũ���͸� �����ϰ�, ���� ���� �� INVALID_SOCKET�� ������.
		cout << "INVALID_SOCKET Error code - create client's socket : " << WSAGetLastError() << endl;

		return false;
	}

	// TCP/IP ��Ʈ��ũ ����Ʈ( �򿣵�� ��� ) ��Ʈ ��ȣ�� unsigned short������ ��ȯ�Ͽ� ���� �ּ� ����ü�� ������.
	this->m_SocketAddress.sin_port = htons( TCP_PORT );
	if( this->m_SocketAddress.sin_port != htons( TCP_PORT ) )
	{
		// Address ����ü �Է� �� ����ó��
		return false;
	}

	// ����� IP �ּ� ������ ���� �ּ� ����ü�� ������.
	this->m_SocketAddress.sin_family = AF_INET;
	if( this->m_SocketAddress.sin_family != ( unsigned short )AF_INET )
	{
		// Address ����ü �Է� �� ����ó��
		return false;
	}

	// �ؽ�Ʈ ������ ����IP �ּҸ� binary �������� ��ȯ�Ͽ� ���� �ּ� ����ü�� ������.
	ipConvertResult = inet_pton( this->m_SocketAddress.sin_family, SERVER_IP, &( this->m_SocketAddress.sin_addr.S_un.S_addr ) );
	if( ipConvertResult == 0 )
	{
		// Address ����ü �Է� �� ����ó��
		// inet_pton�� IP �ּҰ� ������ �ּ� ���°� �ƴ� ��� 0�� ������.
		cout << "inet_pton invalid IP address" << endl;

		return false;
	}
	else if( ipConvertResult == -1 )
	{
		// ��ȿ IP �ּ� ����( ���ϰ� 0 ) �ܿ� �߻��ϴ� ������ -1�� ������.
		cout << "inet_pton IP address convert error : " << WSAGetLastError() << endl;

		return false;
	}


	return true;
}


bool CTCPClient::CTCPC_TCP_Socket_Connect()
{
	int		clientSocketSize;

	int		connectResult;


	// connect�� Client�� ���� ��ũ���͸� ���� �ּ� ����ü�� ����� ������ ���� �õ��� ��.
	connectResult = connect( this->m_SocketHandle, ( SOCKADDR * )&this->m_SocketAddress, sizeof( this->m_SocketAddress ) );
	if( connectResult == SOCKET_ERROR )
	{
		// connect ���� �� 0 ����, ���� �� 0�� �ƴ� Error Code ������.
		cout << "SOCKET_ERROR Error code - connect client's socket : " << WSAGetLastError() << endl;

		return false;
	}


	cout << "Success connected to server" << endl << endl;


	return true;
}


bool CTCPClient::CTCPC_TCP_Socket_Communication()
{
	E_IN_OUT_Stream_Return	tcpSend;
	E_IN_OUT_Stream_Return	tcpReceive;


	while( 1 )
	{
		// ����, �۽� ����� ���� ���� �ʱ�ȭ
		memset( &this->m_RecvBuffer, 0, sizeof( this->m_RecvBuffer ) );
		memset( &this->m_SendBuffer, 0, sizeof( this->m_SendBuffer ) );


		// TCP ������� ��Ŷ �۽�
		tcpSend = CTCPC_TCP_Socket_Send();
		if( tcpSend == E_Fail )
		{
			return false;
		}

		// TCP ������� ��Ŷ ����
		tcpReceive = CTCPC_TCP_Socket_Receive();
		if( tcpReceive == E_Fail )
		{
			return false;
		}


		// ���� Half-close ���� Ȯ��
		if( tcpSend == E_Shutdown )
		{
			break;
		}
	}


	return true;
}


E_IN_OUT_Stream_Return CTCPClient::CTCPC_TCP_Socket_Send()
{
	size_t		messageLength;

	string		inputStream;

	int			sendResult;
	int			shutdownResult;

	
	while( 1 )
	{
		// Client���� Server�� �۽��� Message�� �Է� ����.
		cout << "Send << ";
		getline( cin, inputStream );

		// �Էµ� Message�� ũ�Ⱑ �ۼ��� �� �� �ִ� ��Ŷ�� �ִ� ũ���� 1024byte�� �ʰ��ϴ���
		// �Ǵ�, ��Ŷ�� ����ִ��� �˻��ϴ� ������.
		messageLength = inputStream.length();
		if( messageLength < 1 )
		{
			// �Էµ� Message�� ���� ����� ����ó��
			cout << endl << "Empty send message buffer" << endl << endl;
		}
		else if( messageLength > RECV_PACKET_SIZE )
		{
			// �ۼ��� ������ �ִ� ��Ŷ ũ�⸦ �ʰ��ϴ� Message�� �ԷµǾ��� ����� ����ó��
			cout << endl << "Exceed send message buffer" << endl << endl;
		}
		else
		{
			// �������� Message�� �ԷµǾ��� ��쿡�� ��Ŷ �۽��� ���� �Է½�Ʈ���� �ִ� Message�� �۽� ���ۿ� ������.
			strcpy_s( this->m_SendBuffer, inputStream.c_str() );

			break;
		}
	}


	// Client���� �Էµ� Message�� �������� ���� ���� ���Ḧ �ǹ��ϴ��� Ȯ��
	if( ( strcmp( this->m_SendBuffer, QUIT_MESSAGE_1 ) == 0 ) || ( strcmp( this->m_SendBuffer, QUIT_MESSAGE_2 ) == 0 ) )
	{
		// TCP ���� ���� �ÿ� 4-way handshake ������� �����ϱ� ���ؼ� Half-close ����� �����.
		// Half-close ������� ���� ������ �����ϱ� ���ؼ� Client ������ ��� ��Ʈ���� ���� �����Ŵ.
		// Half-close ����� ����Ͽ��� ������ Client�� ��� ��Ʈ�� ���� ���Ŀ��� Server���� �������� ��Ŷ�� ������ �� ����.
		shutdownResult = shutdown( this->m_SocketHandle, SD_SEND );
		if( shutdownResult == SOCKET_ERROR )
		{
			// shutdown ���� �� 0�� �����ϰ�, ���� �� Error Code ������.
			cout << "SOCKET_ERROR Error code - shutdown client's socket : " << WSAGetLastError() << endl;
			
			return E_Fail;
		}

		this->m_SendMessageSize = shutdownResult;


		return E_Shutdown;
	}


	// ���� ���� ���ᰡ �ƴ� �������� Message�� �ԷµǸ� Client�� �۽� ���ۿ� �ִ� �����͸� �������� ������.
	sendResult = send( this->m_SocketHandle, this->m_SendBuffer, messageLength, 0 );
	if( sendResult == SOCKET_ERROR )
	{
		// send ���� �� �۽��� ��Ŷ�� ����Ʈ ���� �����ϰ�, ���� �� Error Code ������.
		cout << "SOCKET_ERROR Error code - send client's socket : " << WSAGetLastError() << endl;

		return E_Fail;
	}

	this->m_SendMessageSize = sendResult;


	return E_Success;
}


E_IN_OUT_Stream_Return CTCPClient::CTCPC_TCP_Socket_Receive()
{
	int		recvResult;
	int		recvBytes;

	int		packetSize;


	// �ݺ������� recv�� �ݺ� ȣ���Ͽ� Client�� Server���� ���� �޾ƾ� �Ǵ� ��Ŷ�� ũ�⸸ŭ ��Ŷ�� ��� ���� ���� �� �ֵ��� �Ͽ���.
	recvResult	= 0;
	recvBytes	= 0;
	packetSize	= RECV_PACKET_SIZE;
	while( recvBytes < this->m_SendMessageSize )
	{
		// Server�� ����� Client�� ������ ���� ��Ŷ�� �����Ͽ� ���� ���ۿ� ������.
		// recv�� ���� ������� ����Ǳ� ������ ���� ���� ��Ŷ�� ������ ���� ���� ��Ŷ�� ���� ������ ��ٸ�.
		recvResult = recv( this->m_SocketHandle, &this->m_RecvBuffer[ recvBytes ], packetSize, 0 );
		if( recvResult == SOCKET_ERROR )
		{
			// recv ���� �� ���ŵ� ��Ŷ�� ����Ʈ ���� �����ϰ�, ���� �� Error Code ������.
			cout << "SOCKET_ERROR Error code - recv client's socket : " << WSAGetLastError() << endl;
			
			return E_Fail;
		}

		// ������� ���� ���� ��Ŷ�� ũ��� ������ �� �޾ƾ� �Ǵ� ��Ŷ�� ũ�⸦ ������Ʈ ��.
		recvBytes	+= recvResult;
		packetSize	-= recvResult;
	}
	
	
	if( recvResult == 0 )
	{
		// Client�� Server���� ���� ���� ��Ŷ�� ũ�Ⱑ 0 �̶�� ���� Server�� Half-close�� �����Ͽ� Server�� ��� ��Ʈ���� ������״ٴ� �ǹ���.
		cout << "Recv >> Server is shutdown server's output( send ) stream"  << endl << endl;
	}
	else
	{
		// ���� ���� Message�� �ֿܼ� �����.
		cout << "Recv >> " << this->m_RecvBuffer << endl << endl;
	}


	return E_Success;
}


bool CTCPClient::CTCPC_TCP_Socket_Close()
{
	int		closeResult;

	
	// ������ Close �ϱ� ���� ���� ��ũ���Ͱ� ���������� �����ϴ��� Ȯ����.
	if( this->m_SocketHandle == 0 )
	{
		return false;
	}


	// CTCPC_TCP_Socket_Close �Լ� ȣ�� ������ Half-close�� Client�� ��� ��Ʈ���� ����Ǿ� �ִ� ���¿��� Client�� ���� ������ ������ �����Ŵ.
	closeResult = closesocket( this->m_SocketHandle );
	if( closeResult == SOCKET_ERROR )
	{
		// closesocket ���� �� 0 ����, ���� �� 0�� �ƴ� Error Code ������.
		cout << "SOCKET_ERROR Error code - close client's socket : " << WSAGetLastError() << endl;

		return false;
	}


	return true;
}