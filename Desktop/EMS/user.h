#ifndef USER_H
#define USER_H

#include <QWidget>

namespace Ui {
class User;
}

class User: public QWidget
{

    Q_OBJECT

public:
    explicit User(QWidget *parent = nullptr);

    virtual bool authenticate(const QString& username, const QString& password) = 0;




};

#endif // USER_H
