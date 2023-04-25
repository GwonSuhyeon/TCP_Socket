

#include "Main.h"


int main( void )
{
	CServer		*Server;

	bool		serverRun;


	// Server 객체 생성
	Server = new CServer();
	if( Server == nullptr )
	{
		// Server 객체 생성 실패
		return -1;
	}


	// Server 실행
	serverRun = Server->CS_Server_Run();
	if( serverRun == false )
	{
		// Server 실행 실패
		return -1;
	}


	// Server 종료
	delete Server;

	Server = nullptr;


	getchar();

	
	return 0;
}