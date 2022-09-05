#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include "db.h"
#define PNO 9000


bool checkNormalUser(normal user){
	int i=user.userID-1;
	int fd=open("database/normal_user_db.txt", O_RDONLY, 0744);
	bool success;
	normal t;
	
	struct flock l;
	l.l_type = F_RDLCK;
	l.l_whence=SEEK_SET;
	l.l_start=(i)*sizeof(normal);    	    
	l.l_len=sizeof(normal);	             
	l.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&l);	
	getchar();
	lseek(fd,(i)*sizeof(normal),SEEK_SET);  
	read(fd,&t,sizeof(normal));
	if(strcmp(user.password,t.password)!=0 && strcmp(t.status,"TRUE")!=0)	
	{
		success=false;
	}
	else
	{
		success=true;
	}

	l.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&l);

	close(fd);
	return success;
}

bool checkJointUser(joint user){
	int i=user.userID-1;
	int fd=open("database/joint_user_db.txt", O_RDONLY, 0744);
	bool success;
	joint t;
	
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(joint);    	     
	lock.l_len=sizeof(joint);	          
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	
	getchar();

	lseek(fd,(i)*sizeof(joint),SEEK_SET);  
	read(fd,&t,sizeof(joint));
	if(strcmp(user.password,t.password)!=0 && strcmp(t.status,"TRUE")!=0)	
	{
		success=false;
	}
	else
	{
		success=true;
	}

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return success;
}

bool checkAdmin(admin user){
	int i=user.userID-1;
	int fd=open("database/admin_db.txt", O_RDONLY, 0744);
	bool success;
	admin t;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin);    	     
	lock.l_len=sizeof(admin);	             
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);
	getchar();

	lseek(fd,(i)*sizeof(admin),SEEK_SET); 
	read(fd,&t,sizeof(admin));
	if(strcmp(user.password,t.password)!=0)	
	{
		success=false;
	}
	else
	{
		success=true;
	}

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return success;
}

void serverhandler(int nsd){
	int select,type,chosenopt,atype,uid;
	bool con;
	while(true){
		read(nsd,&chosenopt,sizeof(chosenopt));

		if(chosenopt==1){
			printf("Logging in as normal user\n");
			normal nu;
			atype=1;
			read(nsd,&nu,sizeof(normal));
			uid=nu.userID;
			con=checkNormalUser(nu);
			write(nsd,&con,sizeof(con));
		}
		else if(chosenopt==2){
			printf("Logging in as joint user\n");
			joint ju;
			atype=2;
			read(nsd,&ju,sizeof(joint));
			uid=ju.userID;
			con=checkJointUser(ju);
			write(nsd,&con,sizeof(con));
		}
		else if(chosenopt==3){
			printf("Logging in as admin\n");
			admin au3;
			atype=3;
			read(nsd,&au3,sizeof(admin));
			uid=au3.userID;
			con=checkAdmin(au3);
			write(nsd,&con,sizeof(con));
		}
		else{
			con=false;
			write(nsd,&con,sizeof(con));
		}
		if(con)	
		{
			printf("logged in successfully\n");
			break;
		}		
	}
	bool succ;
	int subopt;
	while(1){
		read(nsd,&subopt,sizeof(int));
		if(chosenopt==1 || chosenopt==2){
			if(subopt==1){
				float amnt;
				read(nsd,&amnt,sizeof(float));
				succ=deposit(atype,uid,amnt);
				write(nsd,&succ,sizeof(succ));
			}
			else if(subopt==2){
				float amnt;
				read(nsd,&amnt,sizeof(float));
				succ=withdraw(atype,uid,amnt);
				write(nsd,&succ,sizeof(succ));
			}
			else if(subopt==3){
				float amnt;
				amnt=getBalance(atype,uid);
				write(nsd,&amnt,sizeof(float));
			}
			else if(subopt==4){
				char pwd[10];
				read(nsd,pwd,sizeof(pwd));
				succ=changePassword(atype,uid,pwd);
				write(nsd,&succ,sizeof(succ));
			}
			else if(subopt==5){
				if(chosenopt==1){
					normal nut=getNormalUser(uid);
					write(nsd,&nut,sizeof(normal));
				}
				else if(chosenopt==2){
					joint jut=getJointUser(uid);
					write(nsd,&jut,sizeof(joint));
				}
			}
			else if(subopt==6)	break;
		}
		else if(chosenopt==3){
			read(nsd,&type,sizeof(int));
			if(subopt==1){
				if(type==1){
					normal nut1;
					read(nsd,&nut1,sizeof(normal));
					succ=addNormalUser(nut1);
					
				}
				else if(type==2){
					joint jut1;
					read(nsd,&jut1,sizeof(joint));
					succ=addJointUser(jut1);
				}
				write(nsd,&succ,sizeof(succ));
			}
			else if(subopt==2){
				if(type==1){
					int dnuid;
					read(nsd,&dnuid,sizeof(int));
					succ=deleteNormalUser(dnuid);					
				}
				else if(type==2){
					int djuid;
					read(nsd,&djuid,sizeof(int));
					succ=deleteJointUser(djuid);
				}
				write(nsd,&succ,sizeof(succ));
			}
			else if(subopt==3){
				if(type==1){
					normal mnu;
					read(nsd,&mnu,sizeof(normal));
					succ=modifyNormalUser(mnu);
				}
				else if(type==2){
					joint mju;
					read(nsd,&mju,sizeof(joint));
					succ=modifyJointUser(mju);
				}
				write(nsd,&succ,sizeof(succ));
			}
			else if(subopt==4){
				if(type==1){
					normal snu;
					int snuid;
					read(nsd,&snuid,sizeof(int));
					snu=getNormalUser(snuid);
					write(nsd,&snu,sizeof(normal));
				}
				else if(type==2){
					joint sju;
					int sjuid;
					read(nsd,&sjuid,sizeof(int));
					sju=getJointUser(sjuid);
					write(nsd,&sju,sizeof(joint));
				}
			}
			else if(select==5)	break;
		}
	}
	close(nsd);
	printf("Ended the session\n");
	return;
}

void *connection_handler(void *nsd) {
	int nsfd = *(int*)nsd;
	serverhandler(nsfd);
}

int main(){
	struct sockaddr_in server,client;
	int sd,nsd,len;

	pthread_t threads;
	sd=socket(AF_INET,SOCK_STREAM,0);

	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(PNO);

	bind(sd,(struct sockaddr *)&server,sizeof(server));
	listen(sd,5);	
	
	printf("Waiting for the client\n");
	while(1){
		len=sizeof(client);
		nsd=accept(sd,(struct sockaddr *)&client,&len);

		printf("connected to client\n");
		if(pthread_create(&threads,NULL,connection_handler,(void*) &nsd)<0){
			perror("could not create thread");
			return 1;
		}
	}
	pthread_exit(NULL);
	close(sd);
	return 0;
}
