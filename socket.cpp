#include<iostream>                                                         

#include<sys/socket.h>//socket
#include<string.h>
#include<string>
#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<errno.h>
#include<fcntl.h> 
#include<assert.h>
 
#define MAX_EVENT_NUMBER 10000
 
using namespace std;

//设置文件描述符非阻塞 
int setnoblocking(int fd){
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
} 
//将相应文件描述符挂上树
void addfd(int epollfd,int fd,int one_shot,int trig_mode){
         struct epoll_event event;
         event.data.fd = fd;
         if(1==trig_mode){
         	event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
         }
         else{
         	event.events = EPOLLIN | EPOLLRDHUP;
         }
         if(oneshot == 1){
         	event.events |= EPOLLONESHOT;
         }
         epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
         setnoblocking(fd);
 
 }
 
 //重置epolloneshot
 void modfd(int epollfd,int fd,int trig_mode){
         
         struct epoll_event event;
         event.data.fd = fd;                                                
         if(trig_mode == 1)
         {
            event.events = EPOLLIN | EPOLLET |EPOLLONESHOT | EPOLLRDHUP;
         }
         else
			event.events = EPOLLIN | EPOLLONESHOT|EPOLLRDHUP;
         epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
 }
 void removefd(int epollfd,int fd){
         epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
         close(fd);
}

//分别设置监听连接和监听读写的文件描述符的ET或LT模式 
void trig_mode(){
         //LT+LT
         if(m_trig_mode == 0){
             m_listen_trig_mode = 0;
             m_connect_trig_mode =0;                                        
         }
         //LT+ET
         else if(m_trig_mode == 1){
             m_listen_trig_mode = 0;
             m_connect_trig_mode = 1;
         }
         //ET+LT
         else if(m_trig_mode == 2){
             m_listen_trig_mode = 1;
             m_connect_trig_mode = 0;
         }
         else if(m_trig_mode == 3){
             m_listen_trig_mode = 1;
             m_connect_trig_mode = 1;
         }
 
}


int main(){
		
		//初始化 
		 struct sockaddr_in server_addr;
         bzero(&server_addr,sizeof(server_addr));                           
         server_addr.sin_family = AF_INET;
         server_addr.sin_port = htons(port);
         inet_pton(AF_INET,ip,&server_addr.sin_addr);
         
         int m_listenfd;
         m_listenfd = socket(AF_INET,SOCK_STREAM,0);//创建socket 
         assert(m_listenfd>=0);//错误处理 
      
         int ret = bind(m_listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr));//给服务器绑定端口和地址 
         assert(ret>=0);
         int ret_2 = listen(m_listenfd,5);//同时能进行的tcp连接为3
         assert(ret_2>=0);
         //int flag = 1;
         //setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));//设置端口复用解决2MSL
         
         struct epoll_event events[MAX_EVENT_NUMBER];//最大监听上限为10000
         int m_epollfd = epoll_create(5);                                      
         assert(m_epollfd!=-1);
         addfd(m_epollfd,m_listenfd,0,m_listen_trig_mode);//将listenfd挂上树，addfd的实现可结合et模式和epolloneshot模式,第一个0代表非oneshot，挂上的是监听事件，不能用oneshot，第2个数字代表选择et或lt
		 
		 while(1)
		 {
		 	int ret_3 = epoll_wait(m_epollfd,events,MAX_EVENT_NUMBER,-1);
		 	assert(ret_3>=0);
		 	
			 
			 //for(;;)
		 	{
		 		//循环处理监听事件和读写事件。
				//可启用新线程处理读写事件 
				 
				 	
			}
			
		 	
		 }

}

