

#include "Protocol/TCP/TCPServer.h"


class CServer
{
public :

	CServer();
	~CServer();


	bool CS_Server_Run();	// Server ����


private :

	CTCPServer	TCP_Server;		// Server TCP ��� ��ü


};