

#include "Protocol/TCP/TCPServer.h"


class CServer
{
public :

	CServer();
	~CServer();


	bool CS_Server_Run();	// Server 실행


private :

	CTCPServer	TCP_Server;		// Server TCP 통신 객체


};