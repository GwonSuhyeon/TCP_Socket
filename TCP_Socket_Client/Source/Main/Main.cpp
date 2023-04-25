

#include "Main.h"


int main( void )
{
	CClient		*Client;

	bool		clientRun;


	// Client 객체 생성
	Client = new CClient();
	if( Client == nullptr )
	{
		// Client 객체 생성 실패
		return -1;
	}


	// Client 실행
	clientRun = Client->CC_Client_Run();
	if( clientRun == false )
	{
		// Client 실행 실패
		return -1;
	}

	
	// Client 종료
	delete Client;
	
	Client = nullptr;


	getchar();

	
	return 0;
}