

#include "Protocol/TCP/TCPClient.h"


class CClient
{
public :

	CClient();
	~CClient();


	bool CC_Client_Run();	// Client ����


private :

	CTCPClient	TCP_Client;		// Client TCP ��� ��ü


};