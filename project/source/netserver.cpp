#include "NetServer.h"
#include <pthread.h>
#include <unistd.h> // ͷ�ļ�

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
	bRecvThreadRun = true;
}

bool CNetServer::Start()
{

	pthread_t threadRecv;
	if(pthread_create(&threadRecv,NULL,recvThread,this))
	{
		printf("####error: create thread error   [line:%d] [file:%s] \n",__LINE__,__FILE__);
	}
}
void CNetServer::Stop()
{
	
}


void *CNetServer::recvThread(void *pParm)
{
	CNetServer *pThis = reinterpret_cast<CNetServer *>(pParam);
	pThis->recvDataRun();
	
	return NULL;
}
void CNetServer::recvDataRun()
{
	int i, maxi, listenfd, connfd, sockfd,epfd,nfds, portnumber;
    ssize_t n;
    {
		struct epoll_event ev,events[20];
	    //�������ڴ���accept��epollר�õ��ļ�������

	    epfd=epoll_create(MAX_EPOLL_DEAL_FD);
	    struct sockaddr_in clientaddr;
	    struct sockaddr_in serveraddr;
	    listenfd = socket(AF_INET, SOCK_STREAM, 0);
	    //��socket����Ϊ��������ʽ
	    ev.data.fd=listenfd;
	    //����Ҫ������¼�����

	    ev.events=EPOLLIN|EPOLLET;
	    //ev.events=EPOLLIN;

	    //ע��epoll�¼�

	    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
	    bzero(&serveraddr, sizeof(serveraddr));
	    serveraddr.sin_family = AF_INET;
	    char *local_addr="127.0.0.1";
	    inet_aton(local_addr,&(serveraddr.sin_addr));//htons(portnumber);

	    serveraddr.sin_port=htons(portnumber);
	    bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr));
	    listen(listenfd, LISTENQ);
	}
	while(1)
	{
		if(bRecvThreadRun)
		{
			{
				 nfds=epoll_wait(epfd,events,MAX_EPOLL_DEAL_FD,0);
       			 //�����������������¼�

       			 for(i=0;i<nfds;++i)
		       	 {
		            if(events[i].data.fd==listenfd)//����¼�⵽һ��SOCKET�û����ӵ��˰󶨵�SOCKET�˿ڣ������µ����ӡ�

		            {
		                connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
		                if(connfd<0){
		                    perror("connfd<0");
		                    exit(1);
		                }
		                //setnonblocking(connfd);

		                char *str = inet_ntoa(clientaddr.sin_addr);
		                cout << "accapt a connection from " << str << endl;
		                //�������ڶ��������ļ�������

		                ev.data.fd=connfd;
		                //��������ע��Ķ������¼�

		                ev.events=EPOLLIN|EPOLLET;
		                //ev.events=EPOLLIN;

		                //ע��ev

		                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
		            }
		            else if(events[i].events&EPOLLIN)//������Ѿ����ӵ��û��������յ����ݣ���ô���ж��롣

		            {
		                cout << "EPOLLIN" << endl;
		                if ( (sockfd = events[i].data.fd) < 0)
		                    continue;
		                if ( (n = read(sockfd, line, MAXLINE)) < 0) {
		                    if (errno == ECONNRESET) {
		                        close(sockfd);
		                        events[i].data.fd = -1;
		                    } else
		                        std::cout<<"readline error"<<std::endl;
		                } else if (n == 0) {
		                    close(sockfd);
		                    events[i].data.fd = -1;
		                }
		                line[n] = '/0';
		                cout << "read " << line << endl;
		                //��������д�������ļ�������

		                ev.data.fd=sockfd;
		                //��������ע���д�����¼�

		                ev.events=EPOLLOUT|EPOLLET;
		                //�޸�sockfd��Ҫ������¼�ΪEPOLLOUT

		                //epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);

		            }
		            else if(events[i].events&EPOLLOUT) // ��������ݷ���

		            {
		                sockfd = events[i].data.fd;
		                write(sockfd, line, n);
		                //�������ڶ��������ļ�������

		                ev.data.fd=sockfd;
		                //��������ע��Ķ������¼�

		                ev.events=EPOLLIN|EPOLLET;
		                //�޸�sockfd��Ҫ������¼�ΪEPOLIN

		                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
		            }
					}
			usleep(10);
			continue
		}

		usleep(100);
	}
}
void CNetServer::sendDataRun()
{
	
}
