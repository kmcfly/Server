#ifndef _NET_SERVER_H
#define _NET_SERVER_H

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
		void Start();	
		void Stop();

	private:
		
		
};
#endif
