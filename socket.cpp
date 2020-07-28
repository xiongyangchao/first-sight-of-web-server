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

//�����ļ������������� 
int setnoblocking(int fd){
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
} 
//����Ӧ�ļ�������������
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
 
 //����epolloneshot
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

//�ֱ����ü������Ӻͼ�����д���ļ���������ET��LTģʽ 
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
		
		//��ʼ�� 
		 struct sockaddr_in server_addr;
         bzero(&server_addr,sizeof(server_addr));                           
         server_addr.sin_family = AF_INET;
         server_addr.sin_port = htons(port);
         inet_pton(AF_INET,ip,&server_addr.sin_addr);
         
         int m_listenfd;
         m_listenfd = socket(AF_INET,SOCK_STREAM,0);//����socket 
         assert(m_listenfd>=0);//������ 
      
         int ret = bind(m_listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr));//���������󶨶˿ں͵�ַ 
         assert(ret>=0);
         int ret_2 = listen(m_listenfd,5);//ͬʱ�ܽ��е�tcp����Ϊ3
         assert(ret_2>=0);
         //int flag = 1;
         //setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));//���ö˿ڸ��ý��2MSL
         
         struct epoll_event events[MAX_EVENT_NUMBER];//����������Ϊ10000
         int m_epollfd = epoll_create(5);                                      
         assert(m_epollfd!=-1);
         addfd(m_epollfd,m_listenfd,0,m_listen_trig_mode);//��listenfd��������addfd��ʵ�ֿɽ��etģʽ��epolloneshotģʽ,��һ��0�����oneshot�����ϵ��Ǽ����¼���������oneshot����2�����ִ���ѡ��et��lt
		 
		 while(1)
		 {
		 	int ret_3 = epoll_wait(m_epollfd,events,MAX_EVENT_NUMBER,-1);
		 	assert(ret_3>=0);
		 	
			 
			 //for(;;)
		 	{
		 		//ѭ����������¼��Ͷ�д�¼���
				//���������̴߳����д�¼� 
				 
				 	
			}
			
		 	
		 }

}

