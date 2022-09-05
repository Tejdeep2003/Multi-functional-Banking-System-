#include "ds.h"
#include <stdbool.h>

#ifndef _DATABASE_H
#define  _DATABASE_H

bool deposit(int at,int ID,float amt);
bool withdraw(int at,int ID,float amt);
float getBalance(int at,int ID);
bool changePassword(int at,int ID,char newPwd[10]);
bool addNormalUser(normal record);
bool addJointUser(joint record);
bool deleteNormalUser(int ID);
bool deleteJointUser(int ID);
bool modifyNormalUser(normal modUser);
bool modifyJointUser(joint modUser);
normal getNormalUser(int ID);
joint getJointUser(int ID);


#endif