

#include "Main.h"


int main( void )
{
	CServer		*Server;

	bool		serverRun;


	// Server ��ü ����
	Server = new CServer();
	if( Server == nullptr )
	{
		// Server ��ü ���� ����
		return -1;
	}


	// Server ����
	serverRun = Server->CS_Server_Run();
	if( serverRun == false )
	{
		// Server ���� ����
		return -1;
	}


	// Server ����
	delete Server;

	Server = nullptr;


	getchar();

	
	return 0;
}