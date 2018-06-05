#include "mariadbconnection.h"
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlResult>


MariaDBConnection::MariaDBConnection()
{
}
/**
 * Default Constructor
 *
 * \param[in] dbtype DBMS type
 * \param[in] hostname Hostname to the DataBase
 * \param[in] dbname name of database
 * \param[in] username username used to connect to database
 * \param[in] pass password used to connect to database
 * \param[in] port DBMS's port
 */
MariaDBConnection::MariaDBConnection(QString dbtype, QString hostname,QString dbname, QString username, QString pass,int port)
{
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

/**
 * Check's if provided password and login is correct
 *
 * \param[in] login username used to connect to server
 * \param[in] pass password used to connect to server
 * \return User_ID if there is such user, 0 if there's no such user
 */
int MariaDBConnection::Login(QString login,QString pass)
{

    QSqlQuery query;
       QString asd = "select User_ID from PWSZTAR.PSA_Users where Login = (:log) and Passhash = (:pass);";
       query.prepare(asd);
       query.bindValue(":log",login);
       query.bindValue(":pass",pass);
       query.exec();
       query.first();
    if(query.size() == 1)
    {
    return query.value(0).toInt();
    } else {
    return 0;
    }
    return query.value(0).toInt();
}
/**
 * Add's passwords to database
 *
 * \param[in] login username used to connect to server
 * \param[in] pass password used to connect to server
 * \param[in] Hashed_Password user's fragile data
 * \param[in] Destination user's fragile data
 * \param[in] Destination_User user's fragile data
 * \return true if query succeed, 0 if there's an error
 */
bool MariaDBConnection::AddPassword(int Owner_ID,QString Hashed_Password,QString Destination,QString Destination_User)
{
    QSqlQuery query;
    query.prepare("INSERT INTO PWSZTAR.PSA_Passwords (Owner_ID,Hashed_Password,Destination,Destination_User) VALUES (?, ?, ?, ?);");
    query.addBindValue(Owner_ID);
    query.addBindValue(Hashed_Password);
    query.addBindValue(Destination);
    query.addBindValue( Destination_User);
    return query.exec();
}
/**
 * Check's if provided login is in the database
 *
 * \param[in] login username used to connect to server
 * \return true if there is such login, 0 if there's no login
 */
bool MariaDBConnection::CheckLogin(QString login)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM PWSZTAR.PSA_Users where Login =(?);" );
    query.addBindValue(login);
    query.exec();
    if(query.size() == 1)
    {
    return true;
    } else {
    return false;
    }
    return true;
}
/**
 * Add's an user to database
 *
 * \param[in] login username used to connect to server
 * \param[in] Hashed_Password user password
 * \return true if query succeed, 0 if there's an error or there is already an user with that name
 */
bool MariaDBConnection::RegisterUser(QString login, QString HashedPassword)
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
/**
 * Remove's passwords from the database
 *
 * \param[in] Owner_ID id of the owner
 * \param[in] ID id of the password
 * \return true if query succeed, 0 if there's an error
 */
bool MariaDBConnection::RemovePassword(int Owner_ID, int ID)
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
/**
 * Return's a list of the data of the user
 *
 * \param[in] Login username used to connect to server
 * \param[in] Password password used to connect to server
 * \return list of data
 */
QList<QString>* MariaDBConnection::listPasswords(QString Login, QString Password)
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
    return temp;
}
/**
 * Modify's provided password with new data
 *
 * \param[in] login username used to connect to server
 * \param[in] pass password used to connect to server
 * \param[in] Hashed_Password user's fragile data
 * \param[in] Destination user's fragile data
 * \param[in] Destination_User user's fragile data
 * \param[in] Pass_ID id of the password
 * \return true if query succeed, 0 if there's an error
 */
bool MariaDBConnection::ModifyPassword(QString login,QString pass, QString Hashed_Password, QString Destination, QString Destination_User, int Pass_ID)
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
