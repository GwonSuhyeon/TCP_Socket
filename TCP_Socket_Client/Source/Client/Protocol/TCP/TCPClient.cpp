

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


	// 소켓 주소 구조체 초기화
	memset( &this->m_SocketAddress, 0, sizeof( this->m_SocketAddress ) );


	// WinSock( Windows Socket API )을 사용하기 위해서 초기화 함.
	// WinSock : 윈도우에서 네트워크 기능( ex. TCP )에 접근하기 위한 API임.
	startupResult = WSAStartup( MAKEWORD( 2, 2 ), &this->m_WSAData );
	if( startupResult == SOCKET_ERROR )
	{
		// WSAStartup 성공 시 0 리턴, 실패 시 0이 아닌 Error Code 리턴함.
		cout << "SOCKET_ERROR Error code - WSAStartup : " << WSAGetLastError() << endl;

		return false;
	}


	return true;
}


bool CTCPClient::CTCPC_WSA_Cleanup()
{
	int		cleanupResult;


	// 소켓 통신을 종료하기 위해서 WinSock의 사용을 종료시킴.
	cleanupResult = WSACleanup();
	if( cleanupResult == SOCKET_ERROR )
	{
		// WSACleanup 성공 시 0 리턴, 실패 시 0이 아닌 Error Code 리턴함.
		cout << "SOCKET_ERROR Error code - WSACleanup : " << WSAGetLastError() << endl;

		return false;
	}


	return true;
}


bool CTCPClient::CTCPC_TCP_Socket_Init()
{
	int		ipConvertResult;


	// 서버와 연결하기 위한 소켓을 생성함.
	// PF_INET( af )			: IPv4 주소를 사용하기 위한 설정
	// SOCK_STREAM( type )		: 소켓 통신 유형을 설정하기 위한 것으로,
	//							  SOCK_STREAM은 양방향 연결지향( TCP ) 바이트 스트림을 제공하는 소켓 유형임.
	// IPPROTO_TCP( protocol )	: 통신에 사용할 프로토콜을 지정하는 것으로, IPPROTO_TCP는 TCP를 사용한다는 것을 의미함.
	//							  IP_PROTO_TCP는 af를 PF_INET 또는 AF_INET6으로 설정하고, type을 SOCK_STREAM으로 설정했을때 사용 가능함.
	this->m_SocketHandle = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( this->m_SocketHandle == INVALID_SOCKET )
	{
		// 소켓 생성 성공 시 소켓 디스크립터를 리턴하고, 생성 실패 시 INVALID_SOCKET을 리턴함.
		cout << "INVALID_SOCKET Error code - create client's socket : " << WSAGetLastError() << endl;

		return false;
	}

	// TCP/IP 네트워크 바이트( 빅엔디안 방식 ) 포트 번호를 unsigned short형으로 변환하여 소켓 주소 구조체에 저장함.
	this->m_SocketAddress.sin_port = htons( TCP_PORT );
	if( this->m_SocketAddress.sin_port != htons( TCP_PORT ) )
	{
		// Address 구조체 입력 값 예외처리
		return false;
	}

	// 사용할 IP 주소 형식을 소켓 주소 구조체에 저장함.
	this->m_SocketAddress.sin_family = AF_INET;
	if( this->m_SocketAddress.sin_family != ( unsigned short )AF_INET )
	{
		// Address 구조체 입력 값 예외처리
		return false;
	}

	// 텍스트 형식의 서버IP 주소를 binary 형식으로 변환하여 소켓 주소 구조체에 저장함.
	ipConvertResult = inet_pton( this->m_SocketAddress.sin_family, SERVER_IP, &( this->m_SocketAddress.sin_addr.S_un.S_addr ) );
	if( ipConvertResult == 0 )
	{
		// Address 구조체 입력 값 예외처리
		// inet_pton은 IP 주소가 유요한 주소 형태가 아닐 경우 0을 리턴함.
		cout << "inet_pton invalid IP address" << endl;

		return false;
	}
	else if( ipConvertResult == -1 )
	{
		// 유효 IP 주소 에러( 리턴값 0 ) 외에 발생하는 에러는 -1을 리턴함.
		cout << "inet_pton IP address convert error : " << WSAGetLastError() << endl;

		return false;
	}


	return true;
}


bool CTCPClient::CTCPC_TCP_Socket_Connect()
{
	int		clientSocketSize;

	int		connectResult;


	// connect는 Client의 소켓 디스크립터를 소켓 주소 구조체에 저장된 서버와 연결 시도를 함.
	connectResult = connect( this->m_SocketHandle, ( SOCKADDR * )&this->m_SocketAddress, sizeof( this->m_SocketAddress ) );
	if( connectResult == SOCKET_ERROR )
	{
		// connect 성공 시 0 리턴, 실패 시 0이 아닌 Error Code 리턴함.
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
		// 수신, 송신 입출력 버퍼 변수 초기화
		memset( &this->m_RecvBuffer, 0, sizeof( this->m_RecvBuffer ) );
		memset( &this->m_SendBuffer, 0, sizeof( this->m_SendBuffer ) );


		// TCP 기반으로 패킷 송신
		tcpSend = CTCPC_TCP_Socket_Send();
		if( tcpSend == E_Fail )
		{
			return false;
		}

		// TCP 기반으로 패킷 수신
		tcpReceive = CTCPC_TCP_Socket_Receive();
		if( tcpReceive == E_Fail )
		{
			return false;
		}


		// 소켓 Half-close 여부 확인
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
		// Client에서 Server로 송신할 Message를 입력 받음.
		cout << "Send << ";
		getline( cin, inputStream );

		// 입력된 Message의 크기가 송수신 할 수 있는 패킷의 최대 크기인 1024byte를 초과하는지
		// 또는, 패킷이 비어있는지 검사하는 과정임.
		messageLength = inputStream.length();
		if( messageLength < 1 )
		{
			// 입력된 Message가 없을 경우의 예외처리
			cout << endl << "Empty send message buffer" << endl << endl;
		}
		else if( messageLength > RECV_PACKET_SIZE )
		{
			// 송수신 가능한 최대 패킷 크기를 초과하는 Message가 입력되었을 경우의 예외처리
			cout << endl << "Exceed send message buffer" << endl << endl;
		}
		else
		{
			// 정상적인 Message가 입력되었을 경우에는 패킷 송신을 위해 입력스트림에 있는 Message를 송신 버퍼에 저장함.
			strcpy_s( this->m_SendBuffer, inputStream.c_str() );

			break;
		}
	}


	// Client에서 입력된 Message가 서버와의 소켓 연결 종료를 의미하는지 확인
	if( ( strcmp( this->m_SendBuffer, QUIT_MESSAGE_1 ) == 0 ) || ( strcmp( this->m_SendBuffer, QUIT_MESSAGE_2 ) == 0 ) )
	{
		// TCP 연결 종료 시에 4-way handshake 방식으로 종료하기 위해서 Half-close 방식을 사용함.
		// Half-close 방식으로 소켓 연결을 종료하기 위해서 Client 소켓의 출력 스트림을 먼저 종료시킴.
		// Half-close 방식을 사용하였기 때문에 Client는 출력 스트림 종료 이후에도 Server에서 보내오는 패킷은 수신할 수 있음.
		shutdownResult = shutdown( this->m_SocketHandle, SD_SEND );
		if( shutdownResult == SOCKET_ERROR )
		{
			// shutdown 성공 시 0을 리턴하고, 실패 시 Error Code 리턴함.
			cout << "SOCKET_ERROR Error code - shutdown client's socket : " << WSAGetLastError() << endl;
			
			return E_Fail;
		}

		this->m_SendMessageSize = shutdownResult;


		return E_Shutdown;
	}


	// 소켓 연결 종료가 아닌 정상적인 Message가 입력되면 Client의 송신 버퍼에 있는 데이터를 서버에게 전송함.
	sendResult = send( this->m_SocketHandle, this->m_SendBuffer, messageLength, 0 );
	if( sendResult == SOCKET_ERROR )
	{
		// send 성공 시 송신한 패킷의 바이트 수를 리턴하고, 실패 시 Error Code 리턴함.
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


	// 반복문으로 recv를 반복 호출하여 Client가 Server에게 수신 받아야 되는 패킷의 크기만큼 패킷을 모두 수신 받을 수 있도록 하였음.
	recvResult	= 0;
	recvBytes	= 0;
	packetSize	= RECV_PACKET_SIZE;
	while( recvBytes < this->m_SendMessageSize )
	{
		// Server와 연결된 Client의 소켓을 통해 패킷을 수신하여 수신 버퍼에 저장함.
		// recv는 동기 방식으로 실행되기 때문에 수신 받을 패킷이 없으면 수신 받을 패킷이 들어올 때까지 기다림.
		recvResult = recv( this->m_SocketHandle, &this->m_RecvBuffer[ recvBytes ], packetSize, 0 );
		if( recvResult == SOCKET_ERROR )
		{
			// recv 성공 시 수신된 패킷의 바이트 수를 리턴하고, 실패 시 Error Code 리턴함.
			cout << "SOCKET_ERROR Error code - recv client's socket : " << WSAGetLastError() << endl;
			
			return E_Fail;
		}

		// 현재까지 수신 받은 패킷의 크기와 앞으로 더 받아야 되는 패킷의 크기를 업데이트 함.
		recvBytes	+= recvResult;
		packetSize	-= recvResult;
	}
	
	
	if( recvResult == 0 )
	{
		// Client가 Server에게 수신 받은 패킷의 크기가 0 이라는 것은 Server가 Half-close를 수행하여 Server의 출력 스트림을 종료시켰다는 의미임.
		cout << "Recv >> Server is shutdown server's output( send ) stream"  << endl << endl;
	}
	else
	{
		// 수신 받은 Message를 콘솔에 출력함.
		cout << "Recv >> " << this->m_RecvBuffer << endl << endl;
	}


	return E_Success;
}


bool CTCPClient::CTCPC_TCP_Socket_Close()
{
	int		closeResult;

	
	// 소켓을 Close 하기 전에 소켓 디스크립터가 정상적으로 존재하는지 확인함.
	if( this->m_SocketHandle == 0 )
	{
		return false;
	}


	// CTCPC_TCP_Socket_Close 함수 호출 이전에 Half-close로 Client의 출력 스트림만 종료되어 있는 상태에서 Client의 소켓 연결을 완전히 종료시킴.
	closeResult = closesocket( this->m_SocketHandle );
	if( closeResult == SOCKET_ERROR )
	{
		// closesocket 성공 시 0 리턴, 실패 시 0이 아닌 Error Code 리턴함.
		cout << "SOCKET_ERROR Error code - close client's socket : " << WSAGetLastError() << endl;

		return false;
	}


	return true;
}