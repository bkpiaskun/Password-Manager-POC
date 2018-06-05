#include "mariadb.h"
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlResult>

//QSqlDatabase db;

MariaDB::MariaDB()
{
    //QSqlDatabase db = QSqlDatabase::addDatabase(dbtype);
}
MariaDB::MariaDB(QString dbtype, QString hostname,QString dbname, QString username, QString pass,int port)
{
    //qDebug() << QSqlDatabase::drivers();
    QSqlDatabase db = QSqlDatabase::addDatabase(dbtype);
    db.setHostName(hostname);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPassword(pass);
    db.setPort(port);

    if (db.open()) {
        qDebug() << "Logged in to the Database";
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
    //qDebug()<<query.executedQuery();
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
bool MariaDB::CheckLogin(QString login)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM PWSZTAR.PSA_Users where Login =(?);" );
    query.addBindValue(login);
    query.exec();

    qDebug()<<query.executedQuery();

    if(query.size() == 1)
    {
    qDebug() << query.size()<<endl;
    return true;
    } else {
    qDebug() << query.size()<<endl;
    return false;
    }
    return true;
}
bool MariaDB::RegisterUser(QString login, QString HashedPassword)
{
    QSqlQuery query;
    query.prepare("INSERT INTO PWSZTAR.PSA_Users (Login, PassHash) VALUES ((?), (?));");
    query.addBindValue(login);
    query.addBindValue(HashedPassword);
    if (CheckLogin(login)) {
        return false;
    }
    else
    {
        return query.exec();
    }
    return false;
}
bool MariaDB::RemovePassword(int Owner_ID, int ID)
{
    QSqlQuery query;
    query.prepare("DELETE PWSZTAR.PSA_Passwords FROM PWSZTAR.PSA_Passwords where PWSZTAR.PSA_Passwords.Pass_ID = (?) and PWSZTAR.PSA_Passwords.Owner_ID = (?);");

    query.addBindValue(ID);
    query.addBindValue(Owner_ID);

     query.exec();
     if (query.numRowsAffected() == 1){
         return true;
     } else {
        return false;
     }
}

QList<QString>* MariaDB::listPasswords(QString Login, QString Password)
{
    int User_ID = this->Login(Login,Password);
    QList<QString>* temp = new QList<QString>();
    if(User_ID != 0)
    {
        QSqlQuery query;
        query.prepare("SELECT Pass_ID,Destination,Destination_User,Hashed_Password FROM PWSZTAR.PSA_Passwords INNER JOIN PWSZTAR.PSA_Users ON PWSZTAR.PSA_Passwords.Owner_ID = PWSZTAR.PSA_Users.User_ID where PWSZTAR.PSA_Users.Login = (?) and PWSZTAR.PSA_Users.PassHash = (?);");
        query.addBindValue(Login);
        query.addBindValue(Password);
        qDebug() << "QueryExecuted?:" << query.exec();
        query.result();
        while (query.next())
        {
            temp->append( query.value(0).toString());
            temp->append( query.value(1).toString());
            temp->append( query.value(2).toString());
            temp->append( query.value(3).toString());
        }
    }
    qDebug() <<temp;
    return temp;
}

bool MariaDB::ModifyPassword(QString login,QString pass, QString Hashed_Password, QString Destination, QString Destination_User, int Pass_ID)
{
    int user_id = Login(login,pass);
    RemovePassword(user_id,Pass_ID);
    bool effect = AddPassword(user_id,Hashed_Password,Destination,Destination_User);
     if (effect){
         return true;
     } else {
        return false;
     }
}
