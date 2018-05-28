#ifndef MARIADB_H
#define MARIADB_H
#include <QString>
#include <QSqlDatabase>


class MariaDB
{
private:
    QSqlDatabase db;
public:
    MariaDB();
    MariaDB(QString dbtype, QString hostname,QString dbname, QString username, QString pass,int port);
    int Login(QString login,QString pass);
    bool AddPassword(int Owner_ID,QString Hashed_Password,QString Destination,QString Destination_User);
};

#endif // MARIADB_H
