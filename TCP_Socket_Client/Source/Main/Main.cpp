

#include "Main.h"


int main( void )
{
	CClient		*Client;

	bool		clientRun;


	// Client ��ü ����
	Client = new CClient();
	if( Client == nullptr )
	{
		// Client ��ü ���� ����
		return -1;
	}


	// Client ����
	clientRun = Client->CC_Client_Run();
	if( clientRun == false )
	{
		// Client ���� ����
		return -1;
	}

	
	// Client ����
	delete Client;
	
	Client = nullptr;


	getchar();

	
	return 0;
}