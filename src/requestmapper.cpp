/**
  @file
  @author Stefan Frings
*/

#include <QCoreApplication>
#include "requestmapper.h"
#include "filelogger.h"
#include "staticfilecontroller.h"
#include "controller/dumpcontroller.h"
#include "controller/templatecontroller.h"
#include "controller/formcontroller.h"
#include "controller/fileuploadcontroller.h"
#include "controller/sessioncontroller.h"

#include "mariadb.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

/** Redirects log messages to a file */
extern FileLogger* logger;

/** Controller for static files */
extern StaticFileController* staticFileController;

MariaDB db;

RequestMapper::RequestMapper(QObject* parent)
    :HttpRequestHandler(parent)
{
    qDebug("RequestMapper: created");
    MariaDB * db = new MariaDB("QMYSQL","kluchens.pl","PWSZTAR","popwsz","projekt",3306);
}


RequestMapper::~RequestMapper()
{
    qDebug("RequestMapper: deleted");

}


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());

    // For the following pathes, each request gets its own new instance of the related controller.
/*
    if (path.endsWith("/dump"))
    {
        DumpController().service(request, response);
    }
    else if (path.endsWith("/template"))
    {
        TemplateController().service(request, response);
    }
    else if (path.endsWith("/file"))
    {
        FileUploadController().service(request, response);
    }
    if (path.endsWith("/form"))
    {
        FormController().service(request, response);
    }
  */

    if (path.endsWith("/login"))
    {
        QString login,password;
        response.setHeader("Content-Type", "text/html; charset=utf-8");
        //qDebug() << "################ user_id: "<< request.getCookie("User_ID").toInt();
        //qDebug() << "addpass: "<< db.AddPassword(request.getCookie("User_ID").toInt(),"axsd","zsgxc","Qdfgdfgdfwe");
        if(request.getMethod() == "POST")
        {
            login = request.getParameter("login");
            password = request.getParameter("password");
        } else {

        }
        if(login != NULL && password != NULL)
        {
            int userid = db.Login(login,password);
            if(userid > 0)
            {
                HttpCookie cookie;
                cookie.setName("User_ID");
                cookie.setValue(QByteArray::number(userid));
                cookie.setMaxAge(600);
                //response.setStatus(200,QByteArray::fromStdString("OK"));
                response.setCookie(HttpCookie(cookie.toByteArray()));
                response.write("<html><body><h1>Password Storage Application</h1><ul><li><a href='/register'>Rejestracja</a></li><li><a href='/login'>Logowanie</a></li><li><a href='/passwords'>Moje Hasła</a></li></ul>");
                response.write("<p>Login = ");
                response.write(request.getParameter("login")+"</p>");
                response.write("<p>Hasło = ");
                response.write(request.getParameter("password")+"</p>");
                //const QByteArray name, const QByteArray value, const int maxAge, const QByteArray path, const QByteArray comment, const QByteArray domain, const bool secure, const bool httpOnly
                qDebug() << "Login Succeed on passes: "<< login << " / " << password;
                response.write("<p>Pomyślnie zalogowano</p>");
                //void HttpResponse::setStatus(int statusCode, QByteArray description)
            }
            else
            {
                qDebug() << "Login Failed on passes: "<< login << " / " << password;
                response.write("<html><body><h1>Password Storage Application</h1><ul><li><a href='/register'>Rejestracja</a></li><li><a href='/login'>Logowanie</a></li><li><a href='/passwords'>Moje Hasła</a></li></ul>");
                response.write("<p>Login = ");
                response.write(request.getParameter("login")+"</p>");
                response.write("<p>Hasło = ");
                response.write(request.getParameter("password")+"</p>");
                response.write("<p>Błąd logowania</p>");
            }
        } else {

                response.write("<html><body><h1>Password Storage Application</h1><ul><li><a href='/register'>Rejestracja</a></li><li><a href='/login'>Logowanie</a></li><li><a href='/passwords'>Moje Hasła</a></li></ul>");
                response.write("<p>Login = ");
                response.write(request.getParameter("login")+"</p>");
                response.write("<p>Hasło = ");
                response.write(request.getParameter("password")+"</p>");
                response.write("<form method=\"post\">");
                response.write("  <input type=\"hidden\" name=\"action\" value=\"show\">");
                response.write("  Login: <input type=\"text\" name=\"login\"><br>");
                response.write("  Hasło: <input type=\"password\" name=\"password\"><br>");
                response.write("  <input type=\"submit\">");
                response.write("</form>");
        }

        response.write("</body></html>",true);
    }
    else if (path.endsWith("/register"))
    {
        QString login,password;
        response.setHeader("Content-Type", "text/html; charset=utf-8");
        login = request.getParameter("login");
        password = request.getParameter("password");
        if(login != NULL && password != NULL)
        {
            if(request.getMethod() == "POST")
            {

                bool effect = db.RegisterUser( login, password);
                if(effect)
                {
                    response.write("<html><body>"
                                   "<h1>Password Storage Application</h1>"
                                   "<ul>"
                                   "<li><a href='/register'>Rejestracja</a></li>"
                                   "<li><a href='/login'>Logowanie</a></li>"
                                   "<li><a href='/passwords'>Moje Hasła</a></li>"
                                   "</ul>");
                    response.write("Pomyślnie zarejestrowano");
                } else {
                    response.write("<html><body>"
                                   "<h1>Password Storage Application</h1>"
                                   "<ul>"
                                   "<li><a href='/register'>Rejestracja</a></li>"
                                   "<li><a href='/login'>Logowanie</a></li>"
                                   "<li><a href='/passwords'>Moje Hasła</a></li>"
                                   "</ul>");
                    response.write("Błąd rejestracji!");
                }
            }
        } else {
            response.write("<html><body>"
                           "<h1>Password Storage Application</h1>"
                           "<ul>"
                           "<li><a href='/register'>Rejestracja</a></li>"
                           "<li><a href='/login'>Logowanie</a></li>"
                           "<li><a href='/passwords'>Moje Hasła</a></li>"
                           "</ul>");
            response.write("<form method=\"post\">");
            response.write("  <input type=\"hidden\" name=\"action\" value=\"show\">");
            response.write("  Login: <input type=\"text\" name=\"login\"><br>");
            response.write("  Hasło: <input type=\"password\" name=\"password\"><br>");
            response.write("  <input type=\"submit\">");
            response.write("</form>");

        }

        response.write("</body></html>",true);
        //check
    }
    else if (path.endsWith("/passwords")) //#######################################################################################################################
    {
        response.setHeader("Content-Type", "text/html; charset=utf-8");
        response.write("<html><body>"
                       "<h1>Password Storage Application</h1>"
                       "<ul>"
                       "<li><a href='/register'>Rejestracja</a></li>"
                       "<li><a href='/login'>Logowanie</a></li>"
                       "<li><a href='/passwords'>Moje Hasła</a></li>"
                       "</ul>");

        response.write("<h3>Hasła</h3>"
                       "<ul>");

        // TU NALEŻY POBRAĆ WSZYSTKIE HASŁA


        //        QList<QString> temp;

        /*
         for(hasla)
         {
            response.write("<li>"+haslo+costam+costam2+costamxd+"</li>");
         }
        */
        response.write("</ul></body></html>",true);
        //check
    }
    else if (path.endsWith("/app_getpasswords")) // ###############################################################################################################
    {
        response.setHeader("Content-Type", "application/json; charset=utf-8");

        HttpCookie user_Id = request.getCookie("User_ID");

        if(user_Id.getValue() <= 0)
        {
            response.setStatus(401,QByteArray::fromStdString("Unauthorized"));
            response.write(QByteArray::fromStdString("Unauthorized"),1);
        }
        else
        {
            QJsonDocument json = QJsonDocument::fromJson(request.getBody());
            //QJsonArray jsonArray = json.array();
            QJsonObject temp = json.object();

            QString login = temp.value("login").toString();
            QString password = temp.value("password").toString();

            //QString login = request.getParameter("login");
            //QString password = request.getParameter("password");

            qDebug() << login << password << request.getBody() << request.getMethod();
            QList<QString>* list = db.listPasswords( login, password);
            QJsonArray plot_array;
            QJsonObject item_data;
            //qDebug() << login << password << request.getBody() << request.getMethod();
            //temp.find("password");
            //QJsonArray jsonArray = jsonObject["user"].toArray();
            qDebug() << list->size();

            for(int i = 0;i<list->size();)
            {
                qDebug() <<"Lista at"<<i<<":"<< list->at(i);
                item_data.insert("Pass_ID", QJsonValue(list->at(i++)));
                item_data.insert("Destination", QJsonValue(list->at(i++)));
                item_data.insert("Destination_User", QJsonValue(list->at(i++)));
                item_data.insert("Hashed_Password", QJsonValue(list->at(i++)));
                qDebug() << QJsonValue(item_data).toString();

                plot_array.push_back(QJsonValue(item_data));
            }
            qDebug() << "array"<< plot_array;
            QJsonDocument doc = QJsonDocument(plot_array);
            qDebug() << "doc" <<doc;
            QByteArray toWrite = doc.toJson();
            //QByteArray toWrite = doc.toBinaryData();
            response.write(toWrite,1);
            response.setStatus(200,QByteArray::fromStdString("OK"));
        }
    }
    else if (path.endsWith("/app_login"))
    {
        response.setHeader("Content-Type", "text/html; charset=utf-8");


        //QJsonDocument doc = request.getBody();
        //QJsonDocument jsonResponse = QJsonDocument::fromJson(request.getBody().toStdString());
        QJsonDocument json = QJsonDocument::fromJson(request.getBody());
        qDebug() << json;
        //QJsonArray jsonArray = json.array();


        QJsonObject temp = json.object();

        QString login = temp.value("login").toString();
        QString password = temp.value("password").toString();
        //temp.find("password");
        //QString login = request.getParameter("login");
        //QString password = request.getParameter("password");
        //QJsonArray jsonArray = jsonObject["user"].toArray();

        qDebug() << login << password << request.getBody() << request.getMethod();
        if(login != NULL && password != NULL)
        {
            int userid = db.Login(login,password);
            if(userid > 0)
            {
                HttpCookie cookie;
                cookie.setName("User_ID");
                cookie.setValue(QByteArray::number(userid));
                cookie.setMaxAge(600);
                response.setStatus(200,QByteArray::fromStdString("OK"));
                response.setCookie(HttpCookie(cookie.toByteArray()));
                qDebug() << "Login Succeed on passes: "<< login << " / " << password;
            }
            else
            {
                response.setStatus(400,QByteArray::fromStdString("Bad Request"));
                //response.write("",true);
                qDebug() << "Login Failed on passes: "<< login << " / " << password;
            }
        }
        response.write("",true);
    }
    else if (path.endsWith("/app_rem_pass"))
    {
        response.setHeader("Content-Type", "text/html; charset=utf-8");
        if(request.getMethod() == "POST")
        {
            QString login = request.getParameter("login");
            QString password = request.getParameter("password");

            int pass_id = request.getParameter("pass_id").toInt();
            int user_Id = db.Login(login,password);

            if(user_Id <= 0)
            {
                response.setStatus(401,QByteArray::fromStdString("Unauthorized"));
                response.write(QByteArray::fromStdString("Unauthorized"),1);
            }
            else
            {
                bool effect = db.RemovePassword(user_Id,pass_id);
                if(effect)
                {
                    response.setStatus(200,QByteArray::fromStdString("OK"));
                    response.write("",true);
                } else {
                    response.setStatus(400,QByteArray::fromStdString("Bad Request"));
                    response.write("",true);
                }
            }
        }
    }
    else if (path.endsWith("/app_mod_pass")) // ###################################################################################################################
    {
        response.setStatus(400,QByteArray::fromStdString("Bad Request"));
        response.write("",true);
    }
    else if (path.endsWith("/app_register"))
    {

       response.setHeader("Content-Type", "text/html; charset=utf-8");


        QJsonDocument json = QJsonDocument::fromJson(request.getBody());
        qDebug() << json;
        //QJsonArray jsonArray = json.array();


        QJsonObject temp = json.object();

        QString login = temp.value("login").toString();
        QString password = temp.value("password").toString();


        qDebug() << "register user WYWOŁANA";
        qDebug() << "login ->"<<login<<"password ->"<<password;
        if(login != NULL && password != NULL)
        {
            if(request.getMethod() == "POST")
            {
                if(!db.CheckLogin(login))
                {
                    bool effect = db.RegisterUser( login, password);
                    qDebug() << "register user: "<<effect;
                    if(effect)
                    {
                        response.setStatus(200,QByteArray::fromStdString("OK"));
                        response.write("",true);
                    } else {
                        response.setStatus(400,QByteArray::fromStdString("Bad Request"));
                        response.write("",true);
                    }
                } else {

                    response.setStatus(400,QByteArray::fromStdString("Bad Request"));
                    response.write("",true);
                }
            }
        } else {
            qDebug() << "login lub hasło równe null";
        }
        //check
    }
    else if (path.endsWith("/app_add_pass"))
    {
        //AddPassword(int Owner_ID,QString Hashed_Password,QString Destination,QString Destination_User)

        QString login,password;
        response.setHeader("Content-Type", "text/html; charset=utf-8");


        QString Hashed_Password,Destination,Destination_User;

        Hashed_Password = request.getParameter("Hashed_Password");
        Destination = request.getParameter("Destination");
        Destination_User = request.getParameter("Destination_User");

        login = request.getParameter("login");
        password = request.getParameter("password");
        if(login != NULL && password != NULL)
        {
            if(request.getMethod() == "POST")
            {
                if(Hashed_Password.size() > 3 && Destination.size() > 3 && Destination_User.size() > 3)
                {
                    int user_id = db.Login( login, password);
                    bool effect = db.AddPassword(user_id,Hashed_Password,Destination,Destination_User);
                    if(effect)
                    {
                        response.setStatus(200,QByteArray::fromStdString("OK"));
                        response.write("",true);
                    } else {
                        response.setStatus(400,QByteArray::fromStdString("Bad Request"));
                        response.write("",true);
                    }
                }

            }
        }
    }



    // All other pathes are mapped to the static file controller.
    // In this case, a single instance is used for multiple requests.
    else
    {
        staticFileController->service(request, response);
    }

    qDebug("RequestMapper: finished request");

    // Clear the log buffer
    if (logger)
    {
       logger->clear();
    }
}
