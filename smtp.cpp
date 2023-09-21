#include "smtp.h"


SMTP::SMTP() {}

SMTP::SMTP(string name,string user, string passwd, string domain, int port) {
    this->name = name;
    this->user = user;
    this->passwd = passwd;
    this->domain = domain;
    this->port = port;
}

int SMTP::init() {
    // 1.connection
    connection();
    // 2.send helo
    sendHELO();
    // 3.send auth login
    sendAUTH();
    // 4.send user
    sendUser();
    // 5.send passwd
    sendPasswd();

    return 0;
}
int SMTP::sendEmail(Email &email) {
    // 6.send mail from
    sendMailFrom();
    sendRcptTo(email);
    sendData();
    sendBody(email);
    return 0;
}
int SMTP::connection() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        errorSet(1, "Socket creation failed");
        return -1;
    }

    sockaddr_in serverAddr;
    hostent *hostent = gethostbyname(domain.c_str());
    serverAddr.sin_port = htons(port);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = *((unsigned long *) hostent->h_addr_list[0]);
    if (connect(sock, (sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        errorSet(3, "Connection failed");
        return -1;
    }
    memset(recvBuff, 0, MAX_RECV_LEN);
    if (read(sock, recvBuff, MAX_RECV_LEN) < 0) {
        perror("Read failed");
        errorSet(-1, "Read failed");
        return -1;
    }
    if (judge(220))
        cout << "connect success" << endl;
    return 0;
}

void SMTP::errorSet(int error, string errorMsg) {
    this->error = error;
    this->errorMsg = errorMsg;
}

bool SMTP::judge(int statusCode) {
    int temp = (recvBuff[0] - '0') * 100 + (recvBuff[1] - '0') * 10 + (recvBuff[2] - '0');
    if (temp == statusCode)
        return true;
    return false;
}


int SMTP::quit() {
    string sendBuff = "QUIT\r\n";
    write(sock, sendBuff.c_str(), sendBuff.length());
    memset(recvBuff, 0, MAX_RECV_LEN);
    read(sock, recvBuff, MAX_RECV_LEN);
    if (judge(221))
        cout << "QUIT success" << endl;
    return 0;
}



int SMTP::sendHELO() {
    string sendMsg = "HELO " + user + "\r\n";
    write(sock, sendMsg.c_str(), sendMsg.length());
    memset(recvBuff, 0, MAX_RECV_LEN);
    read(sock, recvBuff, MAX_RECV_LEN);
    if (judge(250))
        cout << "HELO success" << endl;
    //cout<<sendMsg;
    return 0;
}

int SMTP::sendAUTH() {
    string sendMsg = "AUTH LOGIN\r\n";
    write(sock, sendMsg.c_str(), sendMsg.length());
    memset(recvBuff, 0, MAX_RECV_LEN);
    read(sock, recvBuff, MAX_RECV_LEN);
    if (judge(334))
        cout << "AUTH success" << endl;
    return 0;
}

int SMTP::sendUser() {
    char src[user.length() + 1];
    char dst[user.length() * 2];
    strcpy(src, user.c_str());
    base64_encode_str(src, strlen(src), dst, user.length() * 2);
    string user_b64(dst);
    user_b64 = user_b64 + "\r\n";
    write(sock, user_b64.c_str(), user_b64.length());
    memset(recvBuff, 0, MAX_RECV_LEN);
    read(sock, recvBuff, MAX_RECV_LEN);
    if (judge(334))
        cout << "user success" << endl;
    return 0;
}

int SMTP::sendPasswd() {
    char src[passwd.length() + 1];
    char dst[passwd.length() * 2];
    strcpy(src, passwd.c_str());
    base64_encode_str(src,  strlen(src), dst,  passwd.length() * 2);
    string passwd_b64(dst);
    passwd_b64 = passwd_b64 + "\r\n";
    write(sock, passwd_b64.c_str(), passwd_b64.length());
    memset(recvBuff, 0, MAX_RECV_LEN);
    read(sock, recvBuff, MAX_RECV_LEN);
    if (judge(235))
        cout << "Authentication successful" << endl;
    return 0;
}

int SMTP::sendMailFrom() {
    string sendMsg = "MAIL FROM:<"+user+">\r\n";
    write(sock, sendMsg.c_str(), sendMsg.length());
    memset(recvBuff, 0, MAX_RECV_LEN);
    read(sock, recvBuff, MAX_RECV_LEN);
    if (judge(250))
        cout << "MAIL FROM success" << endl;
    return 0;
}

int SMTP::sendRcptTo(Email &email){
    string sendMsg = "RCPT TO:<"+email.email+">\r\n";
    write(sock, sendMsg.c_str(), sendMsg.length());
    memset(recvBuff, 0, MAX_RECV_LEN);
    read(sock, recvBuff, MAX_RECV_LEN);
    if (judge(250))
        cout << "RCPT TO success" << endl;
    return 0;


}



int SMTP::sendData() {
    string sendMsg = "DATA\r\n";
    write(sock, sendMsg.c_str(), sendMsg.length());
    memset(recvBuff, 0, MAX_RECV_LEN);
    read(sock, recvBuff, MAX_RECV_LEN);
    if (judge(354))
        cout << "DATA success" << endl;
    return 0;
}

int SMTP::sendBody(Email &email) {
    string sendMsg = "From:\""+name+"\""+"<"+user+">\r\n";//From:"alias"<xxxxx@126.com>
    sendMsg += "TO:\""+email.name+"\""+"<"+email.email+">\r\n";//TO:"tly"<xxxxxxxx@163.com>
    sendMsg += "Subject:"+email.subject+"\r\n\r\n";//Subject:Demo
    sendMsg += email.body + "\r\n.\r\n";
    write(sock, sendMsg.c_str(), sendMsg.length());
    memset(recvBuff, 0, MAX_RECV_LEN);
    read(sock, recvBuff, MAX_RECV_LEN);
    if (judge(250))
        cout << "Email send success" << endl;
    return 0;
}

