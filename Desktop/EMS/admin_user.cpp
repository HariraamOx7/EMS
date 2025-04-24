#include "admin_user.h"
#include <QCryptographicHash>

AdminUser::AdminUser(QWidget *parent) : User(parent), accessLevel(0) {
    adminRole = "Administrator";
}

bool AdminUser::authenticate(const QString& username, const QString& password) {
    try {
        if (username.isEmpty() || password.isEmpty()) {
            throw AuthenticationException("Username and password cannot be empty");
        }

        // In a real application, you would hash the password and compare with stored hash
        // This is just a simple example
        if (this->username == username && this->password == password) {
            return true;
        }

        throw AuthenticationException("Invalid username or password");
    }
    catch (const AuthenticationException& e) {
        // Log the error or handle it appropriately
        return false;
    }
}

QString AdminUser::getUserInfo() const {
    return QString("Admin User: %1\nRole: %2\nAccess Level: %3\nEmail: %4")
        .arg(username)
        .arg(adminRole)
        .arg(accessLevel)
        .arg(email);
}

void AdminUser::setAdminRole(const QString& role) {
    if (role.isEmpty()) {
        throw std::invalid_argument("Admin role cannot be empty");
    }
    adminRole = role;
}

void AdminUser::setAccessLevel(int level) {
    if (level < 0 || level > 10) {
        throw std::invalid_argument("Access level must be between 0 and 10");
    }
    accessLevel = level;
}

bool AdminUser::canManageUsers() const {
    return accessLevel >= 5;
}

bool AdminUser::canManageSystem() const {
    return accessLevel >= 8;
} 