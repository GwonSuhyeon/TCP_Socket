

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


	// 소켓 주소 구조체 초기화
	memset( &this->m_S_SocketAddress, 0, sizeof( this->m_S_SocketAddress ) );
	memset( &this->m_C_SocketAddress, 0, sizeof( this->m_C_SocketAddress ) );


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


bool CTCPServer::CTCPS_WSA_Close()
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


bool CTCPServer::CTCPS_TCP_Socket_Init()
{
	// Client와 연결하기 위한 소켓을 생성함.
	// PF_INET( af )			: IPv4 주소를 사용하기 위한 설정
	// SOCK_STREAM( type )		: 소켓 통신 유형을 설정하기 위한 것으로,
	//							  SOCK_STREAM은 양방향 연결지향( TCP ) 바이트 스트림을 제공하는 소켓 유형임.
	// IPPROTO_TCP( protocol )	: 통신에 사용할 프로토콜을 지정하는 것으로, IPPROTO_TCP는 TCP를 사용한다는 것을 의미함.
	//							  IP_PROTO_TCP는 af를 PF_INET 또는 AF_INET6으로 설정하고, type을 SOCK_STREAM으로 설정했을때 사용 가능함.
	this->m_S_SocketHandle = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( this->m_S_SocketHandle == INVALID_SOCKET )
	{
		// 소켓 생성 성공 시 소켓 디스크립터를 리턴하고, 생성 실패 시 INVALID_SOCKET을 리턴함.
		cout << "INVALID_SOCKET Error code - create server's socket : " << WSAGetLastError() << endl;

		return false;
	}

	// TCP/IP 네트워크 바이트( 빅엔디안 방식 ) 포트 번호를 unsigned short형으로 변환하여 소켓 주소 구조체에 저장함.
	this->m_S_SocketAddress.sin_port = htons( TCP_PORT );
	if( this->m_S_SocketAddress.sin_port != htons( TCP_PORT ) )
	{
		// Address 구조체 입력 값 예외처리
		return false;
	}

	// 사용할 IP 주소 형식을 소켓 주소 구조체에 저장함.
	this->m_S_SocketAddress.sin_family = AF_INET;
	if( this->m_S_SocketAddress.sin_family != ( unsigned short )AF_INET )
	{
		// Address 구조체 입력 값 예외처리
		return false;
	}

	// INADDR_ANY : 해당 서버 컴퓨터의 랜카드 중에서 현재 사용 가능한 IP 주소를 가져와서 해당 IP 주소로 서버를 열겠다는 의미임.
	// TCP/IP 네트워크 바이트( 빅엔디안 방식 ) IP 주소를 unsigned long형으로 변환하여 소켓 주소 구조체에 저장함.
	this->m_S_SocketAddress.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	if( this->m_S_SocketAddress.sin_addr.S_un.S_addr != INADDR_ANY )
	{
		// Address 구조체 입력 값 예외처리
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


	// bind는 서버의 소켓 디스크립터에 Client의 연결 요청이 들어올 때까지 
	// bind는 서버의 소켓 디스크립터에 소켓 주소 구조체 정보를 할당함.
	bindResult = bind( this->m_S_SocketHandle, ( SOCKADDR * )&this->m_S_SocketAddress, sizeof( this->m_S_SocketAddress ) );
	if( bindResult == SOCKET_ERROR )
	{
		// bind 성공 시 0 리턴, 실패 시 0이 아닌 Error Code 리턴함.
		cout << "SOCKET_ERROR Error code - bind server's socket : " << WSAGetLastError() << endl;

		return false;
	}


	// Server의 소켓 상태를 Client의 연결 요청을 수신 대기하는 상태로 변경함.
	// listen의 두번째 인자인 backlog는 Server가 accept 이전까지 대기하면서 가지고 있을 수 있는 연결 요청의 최대 수를 의미함.
	// 연결 요청에 대해 서버가 accept 하면 backlog에서 해당 요청이 제거됨.
	// backlog를 SOMAXCONN으로 설정하면 시스템에서 합리적인 최대값을 자동으로 설정해주고,
	// SOMAXCONN_HINT(N)으로 설정하면 입력해준 N의 값만큼 최대 접속자 수를 설정함.
	// SOMAXCONN_HINT의 N의 범위는 200 ~ 65535 임.
	// 해당 TCP 통신 과제에서는 일대일 연결 통신이기 때문에 1을 초과하는 backlog는 의미가 없음.
	listenResult = listen( this->m_S_SocketHandle, 1 );
	if( listenResult == SOCKET_ERROR )
	{
		// bind 성공 시 0 리턴, 실패 시 0이 아닌 Error Code 리턴함.
		cout << "SOCKET_ERROR Error code - listen server's socket : " << WSAGetLastError() << endl;

		return false;
	}


	cout << "Await client connect..." << endl << endl;

	clientSocketSize = sizeof( this->m_C_SocketAddress );
	// Server의 소켓으로 연결 요청이 들어오면 Client와 연결 시킴.
	// accept는 동기 방식으로 실행되기 때문에 연결 요청이 들어오지 않으면 계속 연결 요청을 기다림.
	this->m_C_SocketHandle = accept( this->m_S_SocketHandle, ( SOCKADDR * )&this->m_C_SocketAddress, &clientSocketSize );
	if( this->m_C_SocketHandle == INVALID_SOCKET )
	{
		// 클라이언트 소켓 연결 시 소켓 디스크립터를 리턴하고, 연결 실패 시 INVALID_SOCKET을 리턴함.
		cout << "INVALID_SOCKET Error code - accept server's socket : " << WSAGetLastError() << endl;

		return false;
	}

	// 연결된 Client의 소켓 주소 구조체 안에 저장된 IP 주소를 가져와서 문자열 형식으로 변환함.
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
		// 수신, 송신 입출력 버퍼 변수 초기화
		memset( &this->m_RecvBuffer, 0, sizeof( this->m_RecvBuffer ) );
		memset( &this->m_SendBuffer, 0, sizeof( this->m_SendBuffer ) );


		// TCP 기반으로 패킷 수신
		tcpReceive = CTCPS_TCP_Socket_Receive();
		if( tcpReceive == E_Fail )
		{
			return false;
		}
		// 소켓 Half-close 여부 확인
		else if( tcpReceive == E_Shutdown )
		{
			break;
		}


		// TCP 기반으로 패킷 송신
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


	// 연결된 Client가 보낸 패킷을 수신하여 수신 버퍼에 저장함.
	recvResult = recv( this->m_C_SocketHandle, this->m_RecvBuffer, PACKET_SIZE, 0 );
	if( recvResult == SOCKET_ERROR )
	{
		// recv 성공 시 수신된 패킷의 바이트 수를 리턴하고, 실패 시 Error Code 리턴함.
		cout << "SOCKET_ERROR Error code - recv server's socket : " << WSAGetLastError() << endl;

		return E_Fail;
	}
	else if( recvResult == 0 )
	{
		cout << this->clientIP << "> Received" << endl;
		cout << this->clientIP << "> Client is shutdown client's output( send ) stream"  << endl;

		// TCP 연결 종료 시에 4-way handshake 방식으로 종료하기 위해서 Half-close 방식을 사용함.
		// Half-close 방식으로 소켓 연결을 종료하기 위해서 Server 소켓의 출력 스트림을 먼저 종료시킴.
		// Half-close 방식을 사용하였기 때문에 Server는 출력 스트림 종료 이후에도 Client에서 보내오는 패킷은 수신할 수 있음.
		shutdownResult = shutdown( this->m_C_SocketHandle, SD_SEND );
		if( shutdownResult == SOCKET_ERROR )
		{
			// shutdown 성공 시 0을 리턴하고, 실패 시 Error Code 리턴함.
			cout << "SOCKET_ERROR Error code - shutdown server's client socket : " << WSAGetLastError() << endl;
			
			return E_Fail;
		}


		return E_Shutdown;
	}
	else
	{
		// 수신 받은 Message를 콘솔에 출력함.
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


	// Client가 Server로 보낸 Message를 Server가 Client에게 다시 전송해주기 위해서 수신 버퍼의 데이터를 송신 버퍼에 저장함.
	bufferCopyResult = strcpy_s( this->m_SendBuffer, this->m_RecvBuffer );
	if( bufferCopyResult != 0 )
	{
		cout << "Receive buffer copy to Send buffer error code : " << bufferCopyResult << endl;

		return E_Fail;
	}

	// 송신할 패킷이 비어있는지 검사하기 위해서 송신 버퍼의 크기를 확인함.
	messageLength = strlen( this->m_SendBuffer );
	if( messageLength < 1 )
	{
		cout << "Empty send message buffer" << endl;

		return E_Fail;
	}

	// 송신 버퍼에 있는 데이터를 Client에게 전송함.
	sendResult = send( this->m_C_SocketHandle, this->m_SendBuffer, messageLength, 0 );
	if( sendResult == SOCKET_ERROR )
	{
		// send 성공 시 송신한 패킷의 바이트 수를 리턴하고, 실패 시 Error Code 리턴함.
		cout << "SOCKET_ERROR Error code - send server's client socket : " << WSAGetLastError() << endl;

		return E_Fail;
	}

	cout << this->clientIP << "> Sent"<< endl << endl;


	return E_Success;
}


bool CTCPServer::CTCPS_TCP_Socket_Close()
{
	int		closeResult;


	// 소켓을 Close 하기 전에 소켓 디스크립터가 정상적으로 존재하는지 확인함.
	if( this->m_S_SocketHandle == 0 )
	{
		return false;
	}
	
	if( this->m_C_SocketHandle == 0 )
	{
		return false;
	}


	// CTCPS_TCP_Socket_Close 함수 호출 이전에 Half-close로 Server의 출력 스트림만 종료되어 있는 상태에서 Server의 소켓 연결을 완전히 종료시킴.
	closeResult = closesocket( this->m_S_SocketHandle );
	if( closeResult == SOCKET_ERROR )
	{
		// closesocket 성공 시 0 리턴, 실패 시 0이 아닌 Error Code 리턴함.
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