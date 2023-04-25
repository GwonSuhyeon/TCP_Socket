

#pragma comment( lib, "ws2_32" )


#include <iostream>
#include <istream>
#include <string>
//#include <WinSock2.h>
#include <WS2tcpip.h>


#define		TCP_PORT			2374			// TCP ���� ������ ���� ��Ʈ ��ȣ

#define		RECV_PACKET_SIZE	1024			// ���� ���� �� �ִ� ��Ŷ�� �ִ� ũ��
#define		SEND_PACKET_SIZE	1028			// �۽��� �� �ִ� ��Ŷ�� �ִ� ũ�⸦ �����ϱ� ���� 1024byte + 4byte

#define		SERVER_IP			"10.21.20.24"	// Client�� �����ϰ��� �ϴ� Server�� IP �ּ�

// ���� ���� ���Ḧ �ǹ��ϴ� �Է� ��
#define		QUIT_MESSAGE_1		"q"
#define		QUIT_MESSAGE_2		"Q"


using namespace std;


// �ۼ��� ���, Half-close ���� ���θ� Ȯ���ϱ� ���� enum ��
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

	
	bool						CTCPC_WSA_Init();					// WinSock �ʱ�ȭ ( WSA : Windows Socket API )
	bool						CTCPC_WSA_Cleanup();				// WinSock ����
	
	bool						CTCPC_TCP_Socket_Init();			// TCP ���� �ʱ�ȭ
	bool						CTCPC_TCP_Socket_Connect();			// TCP ���� ����
	bool						CTCPC_TCP_Socket_Communication();	// TCP ���� ��� ����
	E_IN_OUT_Stream_Return		CTCPC_TCP_Socket_Send();			// TCP ���� ��Ŷ �۽�
	E_IN_OUT_Stream_Return		CTCPC_TCP_Socket_Receive();			// TCP ���� ��Ŷ ����
	bool						CTCPC_TCP_Socket_Close();			// TCP ���� ���� ���� ����


private :
	
	WSADATA			m_WSAData;							// WinSock

	SOCKET			m_SocketHandle;						// Ŭ���̾�Ʈ ���� ��ũ����
	SOCKADDR_IN		m_SocketAddress;					// Ŭ���̾�Ʈ ���� �ּ� ����ü

	
	char			m_RecvBuffer[ RECV_PACKET_SIZE ];	// ���� ����
	char			m_SendBuffer[ SEND_PACKET_SIZE ];	// �۽� ����

	int				m_SendMessageSize;					// �۽��� ��Ŷ�� ��ü ũ��


};