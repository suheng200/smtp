#include <iostream>
#include "smtp.h"


int main() {
    SMTP smtp("you name","you email @126.com", "authorization code", "server domain", port);
    Email email;
    email.email = "receiver email";
    email.name = "name";
    email.subject = "theme";
    email.body = "content";
    smtp.init();//
    smtp.sendEmail(email);
    smtp.sendEmail(email);
    smtp.quit();
    return 0;
}

