#ifndef _NET_SERVER_H
#define _NET_SERVER_H

#define MAX_EPOLL_DEAL_FD  5000
class CNetServer
{
	public:
		static CNetServer * Instance();
	private:
		CNetServer();
		virtual ~CNetServer();
			//禁止使用默认的拷贝构造函数和=运算符
		CNetServer(const CNetServer&);
		CNetServer& operator=(const CNetServer&);
	public:
		void Initial();
		bool Start();	
		void Stop();

	private:
		void *recvThread(void *pParm);
		void *sendThread();
		void recvDataRun();
		void sendDataRun();
	private:
		bool bRecvThreadRun;
		
		
};
#endif
