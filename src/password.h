#ifndef PASS_H
#define PASS_H
#include <QString>

class Pass
{
public:
    Pass();
    Pass(int ID,QString Dest,QString Dest_User,QString Pass);
    SetID(int ID);
    SetDest(QString Dest);
    SetDestUser(QString Dest_User);
    SetPass(QString Pass);
private:
    int ID;
    QString Dest;
    QString Dest_User;
    QString Pass;
};

#endif // PASS_H
