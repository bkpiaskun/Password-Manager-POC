#include "pass.h"

Pass::Pass(QObject *parent) : QObject(parent)
{

}
Pass::Pass(int ID, QString Dest, QString Dest_User, QString Pass)
{
    this->ID = ID;
    this->Dest = Dest;
    this->Dest_User = Dest_User;
    this->Pass = Pass;
}

Pass::SetID(int ID)
{
    this->ID = ID;
}

Pass::SetDest(QString Dest)
{
    this->Dest = Dest;
}

Pass::SetDestUser(QString Dest_User)
{
    this->Dest_User = Dest_User;
}

Pass::SetPass(QString Pass)
{
    this->Pass = Pass;
}
