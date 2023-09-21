#ifndef SMTP_SMTP_H
#define SMTP_SMTP_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/errno.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <string>
#include "base64.h"

#define MAX_RECV_LEN 500
using namespace std;
typedef struct {
    string email;
    string name;
    string subject;
    string body;
}Email;


class SMTP {
public:
    SMTP();
    SMTP(string name,string user, string passwd, string domain, int port);
    int init();
    int sendEmail(Email &email);
    int quit();
    int sock;
    int port;
    string domain;
    string user;
    string name;
    string passwd;
    //string 

private:
    char recvBuff[MAX_RECV_LEN];
    int error;
    string errorMsg;
    bool isConnect = false;

private:
    void errorSet(int error, string errorMsg);

    bool judge(int statusCode);

    //receive
    //send
    int connection();

    // helo
    int sendHELO();

    // auth
    int sendAUTH();

    // email
    int sendUser();

    // passwd
    int sendPasswd();

    // mail from
    int sendMailFrom();
    // rcpt to
    int sendRcptTo(Email &email);
    //Data
    int sendData();
    //body
    int sendBody(Email &email);
};


#endif //SMTP_SMTP_H

