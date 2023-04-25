

#pragma comment( lib, "ws2_32" )


#include <iostream>
//#include <WinSock2.h>
#include <WS2tcpip.h>


#define		TCP_PORT		2374	// TCP 소켓 연결을 위한 포트 번호
#define		PACKET_SIZE		1024	// 송수신 할 수 있는 패킷의 최대 크기

// 소켓 연결 종료를 의미하는 입력 값
#define		QUIT_MESSAGE_1	"q"
#define		QUIT_MESSAGE_2	"Q"


using namespace std;


// 송수신 결과, Half-close 수행 여부를 확인하기 위한 enum 값
enum E_IN_OUT_Stream_Return
{
	E_Success		= 0,
	E_Fail			= 1,
	E_Shutdown		= 2
};


class CTCPServer
{
public :

	CTCPServer();
	~CTCPServer();

	
	bool						CTCPS_WSA_Init();					// WinSock 초기화 ( WSA : Windows Socket API )
	bool						CTCPS_WSA_Close();					// WinSock 제거
	
	bool						CTCPS_TCP_Socket_Init();			// TCP 소켓 초기화
	bool						CTCPS_TCP_Socket_Connect();			// TCP 소켓 연결
	bool						CTCPS_TCP_Socket_Communication();	// TCP 소켓 통신 시작
	E_IN_OUT_Stream_Return		CTCPS_TCP_Socket_Receive();			// TCP 소켓 수신
	E_IN_OUT_Stream_Return		CTCPS_TCP_Socket_Send();			// TCP 소켓 송신
	bool						CTCPS_TCP_Socket_Close();			// TCP 소켓 완전 연결 종료


private :

	WSADATA			m_WSAData;						// WinSock
	
	SOCKET			m_S_SocketHandle;				// 서버 측 소켓 디스크립터
	SOCKADDR_IN		m_S_SocketAddress;				// 서버 측 소켓 주소 구조체

	SOCKET			m_C_SocketHandle;				// 클라이언트 측 소켓 디스크립터
	SOCKADDR_IN		m_C_SocketAddress;				// 클라이언트 측 소켓 주소 구조체


	char			m_RecvBuffer[ PACKET_SIZE ];	// 수신 버퍼
	char			m_SendBuffer[ PACKET_SIZE ];	// 송신 버퍼

	char			clientIP[ 128 ];				// Client의 IP 주소


};