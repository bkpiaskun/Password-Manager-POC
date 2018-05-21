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

/** Redirects log messages to a file */
extern FileLogger* logger;

/** Controller for static files */
extern StaticFileController* staticFileController;

RequestMapper::RequestMapper(QObject* parent)
    :HttpRequestHandler(parent)
{
    qDebug("RequestMapper: created");
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
*/
/*
    else if (path.endsWith("/template"))
    {
        TemplateController().service(request, response);
    }
*/
/*
    else if (path.endsWith("/file"))
    {
        FileUploadController().service(request, response);
    }
*/
/*
    if (path.endsWith("/form"))
    {
        FormController().service(request, response);
    }
*/
    if (path.endsWith("/login")) //#################################################################################################################################################
    {
        //path.startsWith("/logon")
        //path.endsWith("/logon")
        //SessionController().service(request, response);



        //check
        response.setHeader("Content-Type", "text/html; charset=ISO-8859-1");


        response.write("<html><body>");
        response.write("<form method=\"post\">");
        response.write("  <input type=\"hidden\" name=\"action\" value=\"show\">");
        response.write("  Name: <input type=\"text\" name=\"name\"><br>");
        response.write("  City: <input type=\"text\" name=\"city\"><br>");
        response.write("  <input type=\"submit\">");
        response.write("</form>");

        response.write("Kapkapedau = ");
        response.write(request.getParameter("name"));
        response.write("<br>segz = ");
        response.write(request.getParameter("city"));
        response.write("</body></html>",true);



    }
    else if (path.endsWith("/register")) //##########################################################################################################################################
    {
        //check
    }
    else if (path.endsWith("/password")) //##########################################################################################################################################
    {
        //check
    }
    else if (path.endsWith("/getpasswords")) //######################################################################################################################################
    {
        //check
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
