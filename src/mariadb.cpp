#include "mariadb.h"
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

//QSqlDatabase db;

MariaDB::MariaDB()
{
    //QSqlDatabase db = QSqlDatabase::addDatabase(dbtype);
}
MariaDB::MariaDB(QString dbtype, QString hostname,QString dbname, QString username, QString pass,int port)
{
    qDebug() << QSqlDatabase::drivers();
    QSqlDatabase db = QSqlDatabase::addDatabase(dbtype);
    db.setHostName(hostname);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPassword(pass);
    db.setPort(port);

    if (db.open()) {
        qDebug() << "It's opened!";
    } else {
        qDebug() << "Problem" << db.lastError().text();
    }
}

int MariaDB::Login(QString login,QString pass)
{

    QSqlQuery query;
       QString asd = "select User_ID from PWSZTAR.PSA_Users where Login = (:log) and Passhash = (:pass);";
       query.prepare(asd);
       //query.addBindValue(login);
       //query.addBindValue(pass);
       query.bindValue(":log",login);
       query.bindValue(":pass",pass);
       query.exec();
    qDebug()<<query.executedQuery();
       query.first();
    if(query.size() == 1)
    {
    return query.value(0).toInt();
    } else {
    return 0;
    }
    return query.value(0).toInt();
}
bool MariaDB::AddPassword(int Owner_ID,QString Hashed_Password,QString Destination,QString Destination_User)
{
    QSqlQuery query;
    query.prepare("INSERT INTO PWSZTAR.PSA_Passwords (Owner_ID,Hashed_Password,Destination,Destination_User) VALUES (?, ?, ?, ?);");
    query.addBindValue(Owner_ID);
    query.addBindValue(Hashed_Password);
    query.addBindValue(Destination);
    query.addBindValue( Destination_User);
    return query.exec();
}
