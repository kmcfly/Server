#include "NetServer.h"


CNetServer::CNetServer()
{

}

CNetServer::~CNetServer()
{

}

CNetServer * CNetServer::Instance()
{
	static CNetServer netServer;
	return &netServer;
}

void CNetServer::Initial()
{
	
}

void CNetServer::Start()
{
	
}
void CNetServer::Stop()
{
	
}

