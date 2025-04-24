#ifndef USER_H
#define USER_H

#include <QWidget>
#include <QString>
#include <stdexcept>

// Custom exception class for authentication errors
class AuthenticationException : public std::runtime_error {
public:
    explicit AuthenticationException(const QString& message) 
        : std::runtime_error(message.toStdString()) {}
};

namespace Ui {
class User;
}

// Base class for all user types
class User : public QWidget {
    Q_OBJECT

protected:
    QString username;
    QString password;
    QString email;
    int userId;
    static int nextUserId;  // Static member for generating unique IDs

public:
    explicit User(QWidget *parent = nullptr);
    virtual ~User() = default;

    // Pure virtual function for authentication
    virtual bool authenticate(const QString& username, const QString& password) = 0;

    // Encapsulated getters and setters
    QString getUsername() const { return username; }
    QString getEmail() const { return email; }
    int getUserId() const { return userId; }

    void setUsername(const QString& newUsername) { username = newUsername; }
    void setEmail(const QString& newEmail) { email = newEmail; }

    // Virtual function for displaying user information
    virtual QString getUserInfo() const = 0;

protected:
    // Protected setter for password to ensure proper encapsulation
    void setPassword(const QString& newPassword) { password = newPassword; }
};

#endif // USER_H
