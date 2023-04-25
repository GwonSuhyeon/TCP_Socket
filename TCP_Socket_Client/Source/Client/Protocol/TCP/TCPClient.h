

#pragma comment( lib, "ws2_32" )


#include <iostream>
#include <istream>
#include <string>
//#include <WinSock2.h>
#include <WS2tcpip.h>


#define		TCP_PORT			2374			// TCP 소켓 연결을 위한 포트 번호

#define		RECV_PACKET_SIZE	1024			// 수신 받을 수 있는 패킷의 최대 크기
#define		SEND_PACKET_SIZE	1028			// 송신할 수 있는 패킷의 최대 크기를 제한하기 위한 1024byte + 4byte

#define		SERVER_IP			"10.21.20.24"	// Client가 접속하고자 하는 Server의 IP 주소

// 소켓 연결 종료를 의미하는 입력 값
#define		QUIT_MESSAGE_1		"q"
#define		QUIT_MESSAGE_2		"Q"


using namespace std;


// 송수신 결과, Half-close 수행 여부를 확인하기 위한 enum 값
enum E_IN_OUT_Stream_Return
{
	E_Success		= 0,
	E_Fail			= 1,
	E_Shutdown		= 2
};


class CTCPClient
{
public :

	CTCPClient();
	~CTCPClient();

	
	bool						CTCPC_WSA_Init();					// WinSock 초기화 ( WSA : Windows Socket API )
	bool						CTCPC_WSA_Cleanup();				// WinSock 제거
	
	bool						CTCPC_TCP_Socket_Init();			// TCP 소켓 초기화
	bool						CTCPC_TCP_Socket_Connect();			// TCP 소켓 연결
	bool						CTCPC_TCP_Socket_Communication();	// TCP 소켓 통신 시작
	E_IN_OUT_Stream_Return		CTCPC_TCP_Socket_Send();			// TCP 소켓 패킷 송신
	E_IN_OUT_Stream_Return		CTCPC_TCP_Socket_Receive();			// TCP 소켓 패킷 수신
	bool						CTCPC_TCP_Socket_Close();			// TCP 소켓 완전 연결 종료


private :
	
	WSADATA			m_WSAData;							// WinSock

	SOCKET			m_SocketHandle;						// 클라이언트 소켓 디스크립터
	SOCKADDR_IN		m_SocketAddress;					// 클라이언트 소켓 주소 구조체

	
	char			m_RecvBuffer[ RECV_PACKET_SIZE ];	// 수신 버퍼
	char			m_SendBuffer[ SEND_PACKET_SIZE ];	// 송신 버퍼

	int				m_SendMessageSize;					// 송신한 패킷의 전체 크기


};