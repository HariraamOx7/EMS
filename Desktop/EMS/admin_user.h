#ifndef ADMIN_USER_H
#define ADMIN_USER_H

#include "user.h"

class AdminUser : public User {
    Q_OBJECT

private:
    QString adminRole;
    int accessLevel;

public:
    explicit AdminUser(QWidget *parent = nullptr);
    ~AdminUser() override = default;

    // Implementation of pure virtual functions
    bool authenticate(const QString& username, const QString& password) override;
    QString getUserInfo() const override;

    // Admin-specific methods
    void setAdminRole(const QString& role);
    void setAccessLevel(int level);
    QString getAdminRole() const { return adminRole; }
    int getAccessLevel() const { return accessLevel; }

    // Admin-specific functionality
    bool canManageUsers() const;
    bool canManageSystem() const;
};

#endif // ADMIN_USER_H 