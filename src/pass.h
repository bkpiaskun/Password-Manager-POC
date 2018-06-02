#ifndef PASS_H
#define PASS_H

#include <QObject>

class Pass : public QObject
{
    Q_OBJECT
public:
    explicit Pass(QObject *parent = nullptr);
    Pass();
    Pass(int ID,QString Dest,QString Dest_User,QString Pass);
    SetID(int ID);
    SetDest(QString Dest);
    SetDestUser(QString Dest_User);
    SetPass(QString Pass);
signals:

public slots:

private:
    int ID;
    QString Dest;
    QString Dest_User;
    QString Pass;
};

#endif // PASS_H
