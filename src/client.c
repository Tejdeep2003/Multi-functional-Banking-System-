#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include "ds.h"
#include "client.h"
#define PNO 9000
int option,uid;

void LoginHandler(int sd)
{
	int valid = 1;
	while(valid) {
		printf("1 : login as Normal-User\n");
		printf("2 : login as Joint Account-User\n");
		printf("3 : login as Admin\n");
		
		printf("Choose an option\n");
		scanf("%d",&option);
		printf("Option : %d\n",option);

		switch(option){
			case 1 :
				NormalUserLogin(sd);
				valid = 0;
				break;
			case 2 :
				JointUserLogin(sd);
				valid = 0;
				break;
			case 3 :
				AdminLogin(sd);
				valid = 0;
				break;
			default :
				write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
				// LoginHandler(sd);
				break;
		}
	}
	return;
}

void Menu(int sd)
{
	int select; int valid = 1;
	while(valid) {
		if(option==1 || option==2){
			printf("1 : Deposit\n");
			printf("2 : Withdraw\n");
			printf("3 : Balance Enquiry\n");
			printf("4 : Password Change\n");
			printf("5 : View Details\n");
			printf("6 : Exit\n");
		
			printf("Choose an option : ");
			scanf("%d",&select);
			printf("Option : %d\n",select);
			
			switch(select){
			case 1 :
				deposit(sd);
				valid = 0;
				break;
			case 2 :
				withdraw(sd);
				valid = 0;
				break;
			case 3 :
				balanceEnquiry(sd);
				valid = 0;
				break;
			case 4 :
				changePassword(sd);
				valid = 0;
				break;
			case 5 :
				viewDetails(sd);
				valid = 0;
				break;
			case 6 :
				write(sd,&select,sizeof(int));
				write(1,"Thank you\n",sizeof("Thank you\n"));
				exit(0);
			default :
				write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
				// Menu(sd);
				break;
			}
		}
		else if(option==3){
			write(1,"1 : Add Account\n",sizeof("1 : Add Account\n"));
			write(1,"2 : Delete Account\n",sizeof("2 : Delete Account\n"));
			write(1,"3 : Modify Account\n",sizeof("3 : Modify Account\n"));
			write(1,"4 : Search Account\n",sizeof("4 : Search Account\n"));
			write(1,"5 : Exit\n",sizeof("6 : Exit\n"));

			write(1,"Choose an option : ",sizeof("Choose an option : "));
			scanf("%d",&select);
			printf("Option : %d\n",select);
			
			switch(select){
			case 1 :
				addAccount(sd);
				valid = 0;
				break;
			case 2 :
				deleteAccount(sd);
				valid = 0;
				break;
			case 3 :
				modifyAccount(sd);
				valid = 0;
				break;
			case 4 :
				searchAccount(sd);
				valid = 0;
				break;
			case 5 :
				write(sd,&select,sizeof(int));
				printf("Thank you\n");
				exit(0);
			default :
				printf("Re-enter a valid option\n");
				break;
			}
		}
	}
}

void NormalUserLogin(int sd)
{
	bool verified;
	normal user;
	printf("Enter User ID: ");
	scanf("%d",&user.userID);
	uid=user.userID;
	printf("Enter Password: ");
	scanf("%s",user.password);

	write(sd,&option,sizeof(int));
	write(sd,&user,sizeof(normal));

	read(sd,&verified,sizeof(verified)); 

	if(!verified){
		printf("Invalid Login\n");
		printf("Try Again\n");
		LoginHandler(sd);
	}
	else{
		printf("Loggedin successfully\n");
	}
	return;
}

void JointUserLogin(int sd)
{
	bool verified;
	normal user;
	printf("Enter User ID: ");
	scanf("%d",&user.userID);
	uid=user.userID;
	printf("Enter Password: ");
	scanf("%s",user.password);

	write(sd,&option,sizeof(int));
	write(sd,&user,sizeof(normal));

	read(sd,&verified,sizeof(verified)); 


	if(!verified){
		printf("Invalid Login\n");
		printf("Try Again\n");
		LoginHandler(sd);
	}
	else{
		printf("Loggedin successfully\n");
	}
	return;
}

void AdminLogin(int sd)
{
	bool verified;
	normal user;
	printf("Enter User ID: ");
	scanf("%d",&user.userID);
	uid=user.userID;
	printf("Enter Password: ");
	scanf("%s",user.password);

	write(sd,&option,sizeof(int));
	write(sd,&user,sizeof(normal));
	read(sd,&verified,sizeof(verified)); 

	if(!verified){
		printf("Invalid Login\n");
		printf("Try Again\n");
		LoginHandler(sd);
	}
	else{
		printf("Loggedin successfully\n");
	}
	return;
}

void deposit(int sd){
	float amnt;
	int optchosen=1;
	bool success;

	scanf("Enter the amount to be deposited: %f",&amnt);

	while(amnt<=0){
		printf("Enter a valid amount!!\n");
		scanf("Enter the amount to be deposited: %f",&amnt);
	}
	write(sd,&optchosen,sizeof(int));
	write(sd,&amnt,sizeof(float));
	read(sd,&success,sizeof(success)); 

	if(!success){
		printf("Money not deposited\n");
	}
	else{
		printf("Money deposited successfully\n");
	}
	Menu(sd);
	return;
}

void withdraw(int sd){
	float amnt;
	int optchosen=2;
	bool result;

	scanf("Enter the amount to be withdrawn: %f",&amnt);

	while(amnt<=0){
		printf("Enter a valid amount!!\n");
		scanf("Enter the amount to be withdrawn: %f",&amnt);
	}

	write(sd,&optchosen,sizeof(int));
	write(sd,&amnt,sizeof(float));

	read(sd,&result,sizeof(result)); 

	if(!result){
		printf("Unable to withdraw kindly check balance\n");
	}
	else{
		printf("Money withdrawn successfully\n");
	}
	Menu(sd);
	return;
}

void balanceEnquiry(int sd){
	float amnt;
	int optchosen=3;
	write(sd,&optchosen,sizeof(int));
	int c = read(sd,&amnt,sizeof(float));	
	if(c<0)
	{
		printf("Unable to get balance\n");
	}
	else{
		printf("Available Balance: %0.2f\n\n",amnt);
	}
	
	Menu(sd);
	return;
}

void changePassword(int sd){
	int optchosen=4;
	char pwd[10];
	bool success;
	
	scanf("Enter new password: %s",pwd);
	write(sd,&optchosen,sizeof(int));
	write(sd,pwd,sizeof(pwd));
	read(sd,&success,sizeof(success)); 

	if(!success){
		printf("Unnable to update to new password\n");
	}
	else{
		printf("Changed password successfully\n");
	}
	Menu(sd);
	return;
}

void viewDetails(int sd){
	int select=5;

	//to the server
	write(sd,&select,sizeof(int));

	if(option==1){
		normal currUser1;
		read(sd,&currUser1,sizeof(normal));
		
		printf("User ID : %d\n",currUser1.userID);
		printf("Name : %s\n",currUser1.name);
		printf("Account Number : %d\n",currUser1.account_no);
		printf("Available Balance : Rs.%0.2f\n",currUser1.balance);
		printf("Status : %s\n\n",currUser1.status);
	}
	else if(option==2){
		joint currUser2;
		read(sd,&currUser2,sizeof(joint));
		
		printf("User ID : %d\n",currUser2.userID);
		printf("Main Account Holder's Name : %s\n",currUser2.name1);
		printf("Other Account Holder's Name : %s\n",currUser2.name2);
		printf("Account Number : %d\n",currUser2.account_no);
		printf("Available Balance : Rs.%0.2f\n",currUser2.balance);
		printf("Status : %s\n\n",currUser2.status);
	}
	Menu(sd);
	return;
}

void addAccount(int sd){
	int optchosen=1;
	int type;
	bool success;

	printf("select the type of account\n");
	printf("Enter 1 for normal user\n");
	printf("Enter 2 for Joint user\n");

	scanf("%d",&type);
	write(sd,&optchosen,sizeof(int));
	write(sd,&type,sizeof(int));

	if(type==1){
		normal nu;
		printf("Enter the name of the account holder: ");
		scanf(" %[^\n]",nu.name);
		printf("Enter the password: ");
		scanf("%s",nu.password);
		printf("Enter initial deposit: ");
		scanf("%f",&nu.balance);
		write(sd,&nu,sizeof(normal));
	}

	if(type==2){
		joint ju;
		printf("Enter the name of the first account holder: ");
		scanf(" %[^\n]",ju.name1);
		printf("Enter the name of the second account holder: ");
		scanf(" %[^\n]",ju.name2);
		printf("Enter the password: ");
		scanf("%s",ju.password);
		printf("Enter initial deposit: ");
		scanf("%f",&ju.balance);
		write(sd,&ju,sizeof(joint));
	}
	
	read(sd,&success,sizeof(success)); //from the server

	if(!success){
		printf("Account not created due to error\n");
	}
	else{
		printf("Account created successfully\n");
	}
	Menu(sd);
	return;
}

void deleteAccount(int sd){
	int optchosen=2;
	int type,userID;
	bool success;

	printf("select the type of account\n");
	printf("Enter 1 for normal user\n");
	printf("Enter 2 for Joint user\n");
	scanf("%d",&type);
	write(sd,&optchosen,sizeof(int));
	write(sd,&type,sizeof(int));

	scanf("Enter User id to delete: %d",&userID);
	write(sd,&userID,sizeof(int));
	
	read(sd,&success,sizeof(success)); 

	if(!success){
		printf("Error occured during deletion\n");
	}
	else{
		printf("Account deleted successfully\n");
	}
	Menu(sd);
	return;
}

void modifyAccount(int sd){
	int optchosen=3;
	int type;
	bool success;

	printf("select the type of account\n");
	printf("Enter 1 for normal user\n");
	printf("Enter 2 for Joint user\n");
	scanf("%d",&type);
	write(sd,&optchosen,sizeof(int));
	write(sd,&type,sizeof(int));

	if(type==1){
		normal nu;
		scanf("Enter user id: %d",&nu.userID);
		scanf("Enter Account no: %d",&nu.account_no);
		scanf("Enter new name: %[^\n]",nu.name);
		scanf("Enter new password: %s",nu.password);
		scanf("Enter new balance: %f",&nu.balance);
		write(sd,&nu,sizeof(normal));
	}

	if(type==2){
		joint ju;
		
		scanf("Enter user id: %d",&ju.userID);
		scanf("Enter Account no: %d",&ju.account_no);
		scanf("Enter new primary name:  %[^\n]",ju.name1);
		scanf("Enter new secondary name:  %[^\n]",ju.name2);
		scanf("Enter new password: %s",ju.password);
		scanf("Enter new balance: %f",&ju.balance);
		write(sd,&ju,sizeof(joint));
	}
	
	read(sd,&success,sizeof(success)); //from the server

	if(!success){
		printf("Unable to modify account\n");
	}
	else{
		printf("Account modified successfully\n");
	}
	Menu(sd);
	return;
}

void searchAccount(int sd){
	int optchosen=4;
	int type,len;
	
	printf("select the type of account\n");
	printf("Enter 1 for normal user\n");
	printf("Enter 2 for Joint user\n");
	scanf("%d",&type);
	write(sd,&optchosen,sizeof(int));
	write(sd,&type,sizeof(int));

	if(type==1){
		normal nu;
		int nuid;
		scanf("Enter user id: %d",&nuid);
		write(sd,&nuid,sizeof(int));
		len=read(sd,&nu,sizeof(normal));
		if(len==0){
			printf("Unable to retrieve data\n");
		}
		else{
			printf("User ID : %d\n",nu.userID);
			printf("Name : %s\n",nu.name);
			printf("Account Number : %d\n",nu.account_no);
			printf("Available Balance : Rs.%0.2f\n",nu.balance);
			printf("Status : %s\n\n",nu.status);
		}
	}

	else if(type==2){
		joint ju;
		int juid1;
		scanf("Enter user id: %d",&juid1);
		write(sd,&juid1,sizeof(int));
		len=read(sd,&ju,sizeof(joint));
		if(len==0){
			printf("Unable to retrieve data\n");
		}
		else{
			printf("User ID : %d\n",ju.userID);
			printf("Main Account Holder's Name : %s\n",ju.name1);
			printf("Other Account Holder's Name : %s\n",ju.name2);
			printf("Account Number : %d\n",ju.account_no);
			printf("Available Balance : Rs.%0.2f\n",ju.balance);
			printf("Status : %s\n\n",ju.status);
		}
	}
	Menu(sd);
	return;
}

int main(){
	struct sockaddr_in server;
	int sd;
	char buff[50];
	char result;

	sd=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(PNO);
	connect(sd,(struct sockaddr *)&server,sizeof(server));

	LoginHandler(sd);
	Menu(sd);	

	close(sd);

	return 0;
}