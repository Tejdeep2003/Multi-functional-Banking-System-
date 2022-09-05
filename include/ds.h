typedef struct normal{
	int userID;
	char name[40];
	char password[10];
	int account_no;
	float balance;
	char status[20];
}normal;

typedef struct joint{
	int userID;
	char name1[40];
	char name2[40];
	char password[10];
	int account_no;
	float balance;
	char status[20];
}joint;

typedef struct admin{
	int userID;
	char username[40];
	char password[10];
}admin;
