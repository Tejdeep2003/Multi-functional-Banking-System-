#include "ds.h"

#ifndef _SERVER_H
#define _SERVER_H

bool checkNormalUser(normal currUser);
bool checkJointUser(joint currUser);
bool checkAdmin(admin currUser);
void serverhandler(int nsd);

#endif // !SERVER_H_