#include "password.h"

Pass::Password()
{

}

Password::Password(int ID, QString Dest, QString Dest_User, QString Pass)
{
    this->ID = ID;
    this->Dest = Dest;
    this->Dest_User = Dest_User;
    this->Pass = Pass;
}

Password::SetID(int ID)
{
    this->ID = ID;
}

Password::SetDest(QString Dest)
{
    this->Dest = Dest;
}

Password::SetDestUser(QString Dest_User)
{
    this->Dest_User = Dest_User;
}

Password::SetPass(QString Pass)
{
    this->Pass = Pass;
}
