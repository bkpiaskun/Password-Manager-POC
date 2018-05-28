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

    if (path.endsWith("/login")) //################################################################################################################################
    {

        response.setHeader("Content-Type", "text/html; charset=utf-8");
        //qDebug() << "################ user_id: "<< request.getCookie("User_ID").toInt();
        //qDebug() << "addpass: "<< db.AddPassword(request.getCookie("User_ID").toInt(),"axsd","zsgxc","Qdfgdfgdfwe");

        QString login = request.getParameter("login");
        QString password = request.getParameter("password");
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
    else if (path.endsWith("/register")) //########################################################################################################################
    {
        response.setHeader("Content-Type", "text/html; charset=utf-8");
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

        QJsonArray destination;
        QJsonArray hashed_pass;
        QJsonArray destination_user;
        // TU NALEŻY POBRAĆ WSZYSTKIE HASŁA

        //destination.append(dest);
        //hashed_pass.append(dest);
        //destination_user.append(destuser);


        //display_all


    }
    else if (path.endsWith("/app_login")) // ######################################################################################################################
    {

    }
    else if (path.endsWith("/app_rem_pass")) // ###################################################################################################################
    {

    }
    else if (path.endsWith("/app_mod_pass")) // ###################################################################################################################
    {

    }
    else if (path.endsWith("/app_register")) // ###################################################################################################################
    {

    }
    else if (path.endsWith("/app_add_pass")) // ###################################################################################################################
    {

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
