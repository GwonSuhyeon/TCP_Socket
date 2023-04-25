

#include "Protocol/TCP/TCPClient.h"


class CClient
{
public :

	CClient();
	~CClient();


	bool CC_Client_Run();	// Client 실행


private :

	CTCPClient	TCP_Client;		// Client TCP 통신 객체


};