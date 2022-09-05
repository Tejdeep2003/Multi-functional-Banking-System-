#include "db.h"
#include <sys/types.h>          /* See NOTES */
#include <stdio.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    mkdir("database", 0744);
    printf("populating normal userdb\n");
    int fd1 = open("database/normal_user_db.txt", O_RDWR | O_CREAT, 0744);
    normal u1 = {
        .userID = 1,
        .name = "Tejdeep",
        .password = "102",
        .account_no = 10,
        .balance = 5600.0,
        .status = "TRUE"
    };
    write(fd1, &u1, sizeof(u1));
    close(fd1);

    printf("populating joint userdb\n");
    int fd2 = open("database/joint_user_db.txt", O_RDWR | O_CREAT, 0744);
    joint u2 = {
        .userID = 1,
        .name1 = "Kellan",
        .name2 = "Mark",
        .password = "12345",
        .account_no = 200,
        .balance = 1000.0,
        .status = "TRUE"
    };
    write(fd2, &u2, sizeof(u2));
    close(fd2);

    printf("populating admin userdb\n");
    int fd3 = open("database/admin_db.txt", O_RDWR | O_CREAT, 0744);
    admin u3 = {
        .userID = 1,
        .username = "admin",
        .password = "1234"
    };
    write(fd3, &u3, sizeof(u3));
    close(fd3);
    return 0;
}