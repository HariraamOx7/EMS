#include "User.h"

// Initialize static member
int User::nextUserId = 1;

User::User(QWidget *parent) : QWidget(parent), userId(nextUserId++) {
    // Initialize with default values
    username = "";
    password = "";
    email = "";
}

