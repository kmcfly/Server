#ifndef _NET_SERVER_H
#define _NET_SERVER_H

class CNetServer
{
	public:
		static CNetServer * Instance();
	private:
		CNetServer();
		virtual ~CNetServer();
			//��ֹʹ��Ĭ�ϵĿ������캯����=�����
		CNetServer(const CNetServer&);
		CNetServer& operator=(const CNetServer&);
	public:
		void Initial();
		void Start();	
		void Stop();

	private:
		
		
};
#endif
