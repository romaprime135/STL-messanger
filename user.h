#pragma once
#include <string>
#include <vector>
#include <memory>

class User {
    std::string username;
    std::string password;
    std::string name;

public:
    User ( const std::string &UN, const std::string &PW, const std::string &Name );
    std::string getUsername ( ) const;
    std::string getPassword ( ) const;
    std::string getName ( ) const;
};

void saveUser ( const User &user );
std::vector<User> loadUsers ( );
bool userExists ( const std::string &username );