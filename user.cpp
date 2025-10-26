#include "user.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

User::User ( const string &UN, const string &PW, const string &Name )
    : username ( UN ), password ( PW ), name ( Name ) { }

string User::getUsername ( ) const { return this->username; }
string User::getPassword ( ) const { return this->password; }
string User::getName ( ) const { return this->name; }

// Добавление пользователя
void saveUser ( const User &user ) {
    ofstream file ( "users.txt", ios::app );
    if ( file.is_open ( ) ) {
        file << user.getUsername ( ) << ";"
            << user.getPassword ( ) << ";"
            << user.getName ( ) << "\n";
    }
}

// Получение пользователей
vector<User> loadUsers ( ) {
    vector<User> users;
    ifstream file ( "users.txt" );

    if ( !file.is_open ( ) ) {
        return users;
    }

    string line;
    while ( getline ( file, line ) ) {
        if ( line.empty ( ) ) continue;

        vector<string> tokens;
        stringstream ss ( line );
        string token;

        while ( getline ( ss, token, ';' ) ) {
            tokens.push_back ( token );
        }

        if ( tokens.size ( ) == 3 ) {
            users.emplace_back ( tokens [0], tokens [1], tokens [2] );
        }
    }

    return users;
}

// Проверка существования пользователя
bool userExists ( const string &username ) {
    vector<User> users = loadUsers ( );
    for ( const auto &u : users ) {
        if ( u.getUsername ( ) == username ) {
            return true;
        }
    }
    return false;
}