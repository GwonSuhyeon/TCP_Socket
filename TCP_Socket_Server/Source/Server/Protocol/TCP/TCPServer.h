

#pragma comment( lib, "ws2_32" )


#include <iostream>
//#include <WinSock2.h>
#include <WS2tcpip.h>


#define		TCP_PORT		2374	// TCP ���� ������ ���� ��Ʈ ��ȣ
#define		PACKET_SIZE		1024	// �ۼ��� �� �� �ִ� ��Ŷ�� �ִ� ũ��

// ���� ���� ���Ḧ �ǹ��ϴ� �Է� ��
#define		QUIT_MESSAGE_1	"q"
#define		QUIT_MESSAGE_2	"Q"


using namespace std;


// �ۼ��� ���, Half-close ���� ���θ� Ȯ���ϱ� ���� enum ��
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

	
	bool						CTCPS_WSA_Init();					// WinSock �ʱ�ȭ ( WSA : Windows Socket API )
	bool						CTCPS_WSA_Close();					// WinSock ����
	
	bool						CTCPS_TCP_Socket_Init();			// TCP ���� �ʱ�ȭ
	bool						CTCPS_TCP_Socket_Connect();			// TCP ���� ����
	bool						CTCPS_TCP_Socket_Communication();	// TCP ���� ��� ����
	E_IN_OUT_Stream_Return		CTCPS_TCP_Socket_Receive();			// TCP ���� ����
	E_IN_OUT_Stream_Return		CTCPS_TCP_Socket_Send();			// TCP ���� �۽�
	bool						CTCPS_TCP_Socket_Close();			// TCP ���� ���� ���� ����


private :

	WSADATA			m_WSAData;						// WinSock
	
	SOCKET			m_S_SocketHandle;				// ���� �� ���� ��ũ����
	SOCKADDR_IN		m_S_SocketAddress;				// ���� �� ���� �ּ� ����ü

	SOCKET			m_C_SocketHandle;				// Ŭ���̾�Ʈ �� ���� ��ũ����
	SOCKADDR_IN		m_C_SocketAddress;				// Ŭ���̾�Ʈ �� ���� �ּ� ����ü


	char			m_RecvBuffer[ PACKET_SIZE ];	// ���� ����
	char			m_SendBuffer[ PACKET_SIZE ];	// �۽� ����

	char			clientIP[ 128 ];				// Client�� IP �ּ�


};