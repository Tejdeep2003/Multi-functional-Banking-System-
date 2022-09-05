#include "db.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>


bool deposit(int at,int ID,float amt){
	int loc=ID-1;
	bool success;
	if(at==1){
		int fd=open("database/normal_user_db.txt",O_RDWR,0744);
		struct flock l;
		l.l_type = F_WRLCK;
		l.l_whence=SEEK_SET;
		l.l_start=(loc)*sizeof(normal);    
		l.l_len=sizeof(normal);	            
		l.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&l);	

		normal usr;
		lseek(fd,(loc)*sizeof(normal),SEEK_SET);  
		read(fd,&usr,sizeof(normal));
		
		if(!strcmp(usr.status,"TRUE")){
			usr.balance+=amt;
			lseek(fd,(loc)*sizeof(normal),SEEK_SET);
			write(fd,&usr,sizeof(normal));
			success=true;
		}
		else
		{
			success=false;
		}
		l.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&l);

		close(fd);
		return success;		
	}
	else if(at==2){
		int fd=open("database/joint_user_db.txt",O_RDWR,0744);
		struct flock l;
		l.l_type = F_WRLCK;
		l.l_whence=SEEK_SET;
		l.l_start=(loc)*sizeof(joint);   
		l.l_len=sizeof(joint);	             
		l.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&l);	


		joint usr;
		lseek(fd,(loc)*sizeof(joint),SEEK_SET);  
		read(fd,&usr,sizeof(joint));
		
		if(!strcmp(usr.status,"TRUE")){
			usr.balance+=amt;
			lseek(fd,(loc)*sizeof(joint),SEEK_SET);
			write(fd,&usr,sizeof(joint));
			success=true;
		}
		else	
		{
			success=false;
		}
		l.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&l);

		close(fd);
		return success;	
	}
	else{
		return false;
	}
}

bool withdraw(int at,int ID,float amnt){
	int i=ID-1;
	bool success;
	if(at==1){
		int fd=open("database/normal_user_db.txt",O_RDWR,0744);
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normal);    
		lock.l_len=sizeof(normal);	            
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);	

		normal usr;
		lseek(fd,(i)*sizeof(normal),SEEK_SET);  
		read(fd,&usr,sizeof(normal));
		
		if(!strcmp(usr.status,"TRUE") && usr.balance-amnt>=0){
			usr.balance-=amnt;
			lseek(fd,(i)*sizeof(normal),SEEK_SET);  
			write(fd,&usr,sizeof(normal));
			success=true;
		}
		else{
			success=false;
		}	
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return success;	
	}
	else if(at==2){
		int fd=open("database/joint_user_db.txt",O_RDWR,0744);
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(joint);   
		lock.l_len=sizeof(joint);	             
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);	


		joint usr;
		lseek(fd,(i)*sizeof(joint),SEEK_SET);  
		read(fd,&usr,sizeof(joint));
		
		if(!strcmp(usr.status,"TRUE") && usr.balance-amnt>=0){
			usr.balance-=amnt;
			lseek(fd,(i)*sizeof(joint),SEEK_SET);  
			write(fd,&usr,sizeof(joint));
			success=true;
		}
		else	
		{
			success=false;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return success;
	}
	else{
		return false;
	}
}

float getBalance(int at,int ID){
	int i=ID-1;
	float bal;
	if(at==1){
		int fd=open( "database/normal_user_db.txt",O_RDONLY,0744);
		normal usr;
	
		struct flock l;
		l.l_type = F_WRLCK;
		l.l_whence=SEEK_SET;
		l.l_start=(i)*sizeof(normal);   
		l.l_len=sizeof(normal);	             
		l.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&l);	

		lseek(fd,(i)*sizeof(normal),SEEK_SET);  
		read(fd,&usr,sizeof(normal));
		if(!strcmp(usr.status,"TRUE"))	
		{
			bal=usr.balance;
		}
		else					
		{
			bal=-1;
		}
		l.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&l);

		close(fd);
		return bal;
	}
	else if(at==2){
		int fd=open("database/joint_user_db.txt",O_RDONLY,0744);
		joint usr;
	
		struct flock l;
		l.l_type = F_WRLCK;
		l.l_whence=SEEK_SET;
		l.l_start=(i)*sizeof(joint);   
		l.l_len=sizeof(joint);	             
		l.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&l);	

		lseek(fd,(i)*sizeof(joint),SEEK_SET);  
		read(fd,&usr,sizeof(joint));
		if(!strcmp(usr.status,"TRUE"))	
		{
			bal=usr.balance;
		}
		else					
		{
			bal=-1;
		}

		l.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&l);

		close(fd);
		return bal;
	}
	else
	{
		return -1;
	}
}

bool changePassword(int at,int ID,char newPwd[10]){
	int i=ID-1;
	bool success;
	if(at==1){
		int fd=open( "database/normal_user_db.txt",O_RDWR,0744);
		normal usr;
		struct flock l;
		l.l_type = F_WRLCK;
		l.l_whence=SEEK_SET;
		l.l_start=(i)*sizeof(normal);   
		l.l_len=sizeof(normal);	             
		l.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&l);

		
		lseek(fd,(i)*sizeof(normal),SEEK_SET); 
		read(fd,&usr,sizeof(normal));
		
		if(!strcmp(usr.status,"TRUE"))
		{
			strcpy(usr.password,newPwd);
			lseek(fd,(i)*sizeof(normal),SEEK_SET);
			write(fd,&usr,sizeof(normal));
			success=true;
		}
		else	
		{
			success=false;
		}
		l.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&l);

		close(fd);
		return success;
	}
	else if(at==2){
		int fd=open("database/joint_user_db.txt",O_RDWR,0744);
		joint usr;
		
		struct flock l;
		l.l_type = F_WRLCK;
		l.l_whence=SEEK_SET;
		l.l_start=(i)*sizeof(joint);   
		l.l_len=sizeof(joint);	             
		l.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&l);	

		lseek(fd,(i)*sizeof(joint),SEEK_SET);  
		read(fd,&usr,sizeof(joint));
		
		if(!strcmp(usr.status,"TRUE")){
			strcpy(usr.password,newPwd);
			lseek(fd,(i)*sizeof(joint),SEEK_SET);  
			write(fd,&usr,sizeof(joint));
			success=true;
		}
		else	
		{
			success=false;
		}
		l.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&l);

		close(fd);
		return success;
	}
	else
	{
		return false;
	}
}

bool addNormalUser(normal record){
	int fd=open( "database/normal_user_db.txt",O_RDWR,0744);
	bool success;
	
	normal usr;
	struct flock l;
	l.l_type = F_WRLCK;
	l.l_whence=SEEK_SET;
	l.l_start=(-1)*sizeof(normal);   
	l.l_len=sizeof(normal);	             
	l.l_pid=getpid();

	fcntl(fd,F_SETLKW,&l);

	lseek(fd,(-1)*sizeof(normal),SEEK_END);  
	read(fd,&usr,sizeof(normal));
		
	record.userID=usr.userID+1;
	record.account_no=usr.account_no+1;
	strcpy(record.status,"TRUE");
	
	int j=write(fd,&record,sizeof(normal));
	if(j!=0)	
	{
		success=true;
	}
	else	
	{
		success=false;
	}
	l.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&l);
	
	close(fd);
	return success;	
}

bool addJointUser(joint record){
	int fd=open("database/joint_user_db.txt",O_RDWR,0744);
	bool success;
	
	joint usr;
		
	struct flock l;
	l.l_type = F_WRLCK;
	l.l_whence=SEEK_SET;
	l.l_start=(-1)*sizeof(joint);   
	l.l_len=sizeof(joint);	             
	l.l_pid=getpid();

	fcntl(fd,F_SETLKW,&l);	
	lseek(fd,(-1)*sizeof(joint),SEEK_END); 
	read(fd,&usr,sizeof(joint));
		
	record.userID=usr.userID+1;
	record.account_no=usr.account_no+1;
	strcpy(record.status,"TRUE");
	
	int j=write(fd,&record,sizeof(joint));
	if(j!=0)	
	{
		success=true;
	}
	else	
	{
		success=false;
	}
	l.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&l);
	
	close(fd);
	return success;	
}

bool deleteNormalUser(int ID){
	int i=ID-1;
	int fd=open( "database/normal_user_db.txt",O_RDWR,0744);
	bool success;
	
	normal usr;
		
	struct flock l;
	l.l_type = F_WRLCK;
	l.l_whence=SEEK_SET;
	l.l_start=(i)*sizeof(normal);   
	l.l_len=sizeof(normal);	             
	l.l_pid=getpid();

	fcntl(fd,F_SETLKW,&l);	

	lseek(fd,(i)*sizeof(normal),SEEK_SET);  
	read(fd,&usr,sizeof(normal));
	
	if(!strcmp(usr.status,"TRUE"))
	{	
		strcpy(usr.status,"CLOSED");
		usr.balance=0;
		
		lseek(fd,(i)*sizeof(normal),SEEK_SET); 
		int c=write(fd,&usr,sizeof(normal));
		if(c!=0)	
		{
			success=true;
		}
		else		
		{
			success=false;
		}
	}
	else
	{
		success = false;
	}
	
	l.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&l);
	
	close(fd);
	return success;	
}

bool deleteJointUser(int ID){
	int i=ID-1;
	int fd=open("database/joint_user_db.txt",O_RDWR,0744);
	bool success;
	
	joint usr;
		
	struct flock l;
	l.l_type = F_WRLCK;
	l.l_whence=SEEK_SET;
	l.l_start=(-1)*sizeof(joint);   
	l.l_len=sizeof(joint);	             
	l.l_pid=getpid();

	fcntl(fd,F_SETLKW,&l);

	lseek(fd,(i)*sizeof(joint),SEEK_SET);  
	
	if(!strcmp(usr.status,"TRUE")){	
		strcpy(usr.status,"CLOSED");
		usr.balance=0;
		
		lseek(fd,(-1)*sizeof(joint),SEEK_CUR); 
		int c=write(fd,&usr,sizeof(joint));
		if(c!=0)	
		{
			success=true;
		}
		else		
		{
			success=false;
		}
	}
	else{
		success = false;
	}
	
	l.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&l);
	
	close(fd);
	return success;	
}

bool modifyNormalUser(normal modUser){
    printf("id mod user = %d\n", modUser.userID);
	int i=modUser.userID-1;
	int fd=open( "database/normal_user_db.txt",O_RDWR,0744);
	bool success;
	
	normal usr;
		
	struct flock l;
	l.l_type = F_WRLCK;
	l.l_whence=SEEK_SET;
	l.l_start=(i)*sizeof(normal);   
	l.l_len=sizeof(normal);	             
	l.l_pid=getpid();

	fcntl(fd,F_SETLKW,&l);

	lseek(fd,(i)*sizeof(normal),SEEK_SET);  
	read(fd,&usr,sizeof(normal));
	
	if(!strcmp(usr.status,"TRUE") && (modUser.account_no==usr.account_no)){	
		strcpy(modUser.status,"TRUE");
		lseek(fd,(-1)*sizeof(normal),SEEK_CUR); 
		int c=write(fd,&modUser,sizeof(normal));
		if(c!=0)	
		{
			success=true;
		}
		else		
		{
			success=false;
		}
	}
	else
	{
		success = false;
	}
	
	l.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&l);
	
	close(fd);
	return success;	
}

bool modifyJointUser(joint modUser){
	int i=modUser.userID-1;
	int fd=open("database/joint_user_db.txt",O_RDWR,0744);
	bool success;
	
	joint usr;
		
	struct flock l;
	l.l_type = F_WRLCK;
	l.l_whence=SEEK_SET;
	l.l_start=(i)*sizeof(joint);   
	l.l_len=sizeof(joint);	             
	l.l_pid=getpid();

	fcntl(fd,F_SETLKW,&l);

	lseek(fd,(i)*sizeof(joint),SEEK_SET);  
	read(fd,&usr,sizeof(joint));
	
	if(!strcmp(usr.status,"TRUE")  && (modUser.account_no==usr.account_no)){	
		strcpy(modUser.status,"TRUE");
		lseek(fd,(-1)*sizeof(joint),SEEK_CUR); 
		int c=write(fd,&modUser,sizeof(joint));
		if(c!=0)	
		{
			success=true;
		}
		else		
		{
			success=false;
		}
	}
	else
	{
		success = false;
	}
	
	l.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&l);
	
	close(fd);
	return success;	
}

normal getNormalUser(int ID){
	int i=ID-1;
	int fd=open( "database/normal_user_db.txt",O_RDONLY,0744);
	normal usr;
		
	struct flock l;
	l.l_type = F_WRLCK;
	l.l_whence=SEEK_SET;
	l.l_start=(i)*sizeof(normal);   
	l.l_len=sizeof(normal);	             
	l.l_pid=getpid();

	fcntl(fd,F_SETLKW,&l);

	lseek(fd,(i)*sizeof(normal),SEEK_SET);  
	int c = read(fd,&usr,sizeof(normal));

	l.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&l);

	close(fd);
	return usr;
}

joint getJointUser(int ID){
	int i=ID-1;
	int fd=open("database/joint_user_db.txt",O_RDONLY,0744);
	
	joint usr;
		
	struct flock l;
	l.l_type = F_WRLCK;
	l.l_whence=SEEK_SET;
	l.l_start=(i)*sizeof(joint);   
	l.l_len=sizeof(joint);	             
	l.l_pid=getpid();

	fcntl(fd,F_SETLKW,&l);

	lseek(fd,(i)*sizeof(joint),SEEK_SET);  
	int c = read(fd,&usr,sizeof(joint));

	l.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&l);

	close(fd);
	return usr;
}