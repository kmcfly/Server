#include "NetServer.h"
#include <pthread.h>
#include <unistd.h> // 头文件

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
	    //生成用于处理accept的epoll专用的文件描述符

	    epfd=epoll_create(MAX_EPOLL_DEAL_FD);
	    struct sockaddr_in clientaddr;
	    struct sockaddr_in serveraddr;
	    listenfd = socket(AF_INET, SOCK_STREAM, 0);
	    //把socket设置为非阻塞方式
	    ev.data.fd=listenfd;
	    //设置要处理的事件类型

	    ev.events=EPOLLIN|EPOLLET;
	    //ev.events=EPOLLIN;

	    //注册epoll事件

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
       			 //处理所发生的所有事件

       			 for(i=0;i<nfds;++i)
		       	 {
		            if(events[i].data.fd==listenfd)//如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接。

		            {
		                connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
		                if(connfd<0){
		                    perror("connfd<0");
		                    exit(1);
		                }
		                //setnonblocking(connfd);

		                char *str = inet_ntoa(clientaddr.sin_addr);
		                cout << "accapt a connection from " << str << endl;
		                //设置用于读操作的文件描述符

		                ev.data.fd=connfd;
		                //设置用于注测的读操作事件

		                ev.events=EPOLLIN|EPOLLET;
		                //ev.events=EPOLLIN;

		                //注册ev

		                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
		            }
		            else if(events[i].events&EPOLLIN)//如果是已经连接的用户，并且收到数据，那么进行读入。

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
		                //设置用于写操作的文件描述符

		                ev.data.fd=sockfd;
		                //设置用于注测的写操作事件

		                ev.events=EPOLLOUT|EPOLLET;
		                //修改sockfd上要处理的事件为EPOLLOUT

		                //epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);

		            }
		            else if(events[i].events&EPOLLOUT) // 如果有数据发送

		            {
		                sockfd = events[i].data.fd;
		                write(sockfd, line, n);
		                //设置用于读操作的文件描述符

		                ev.data.fd=sockfd;
		                //设置用于注测的读操作事件

		                ev.events=EPOLLIN|EPOLLET;
		                //修改sockfd上要处理的事件为EPOLIN

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
