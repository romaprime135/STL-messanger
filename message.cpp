#include "message.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

string Message::getGetter ( ) const { return this->Getter; }
string Message::getSender ( ) const { return this->Sender; }
string Message::getText ( ) const { return this->Text; }
string Message::getTag ( ) const { return this->Tag; }

Message::Message ( const string &Getter, const string &Sender,
                 const string &Text, const string &Tag )
    : Getter ( Getter ), Sender ( Sender ), Text ( Text ), Tag ( Tag ) {
}

Message::Message ( ) : Getter ( "None" ), Sender ( "None" ), Text ( "None" ), Tag ( "None" ) { }

// Отправка сообщения
void addMessage ( const string &Path, const Message &Msg ) {
    vector<Message> messages = getMessage ( Path );
    messages.push_back ( Msg );

    ofstream file ( Path );
    if ( !file.is_open ( ) ) {
        throw runtime_error ( "Cannot open file for writing" );
    }

    for ( const auto &message : messages ) {
        file << message.getGetter ( ) << ";"
            << message.getText ( ) << ";"
            << message.getSender ( ) << ";"
            << message.getTag ( ) << "\n";
    }
}

// Получение сообщений
vector<Message> getMessage ( const string &Path ) {
    vector<Message> result;
    ifstream file ( Path );

    if ( !file.is_open ( ) ) {
        return result;
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

        if ( tokens.size ( ) == 4 ) {
            result.emplace_back ( tokens [0], tokens [2], tokens [1], tokens [3] );
        }
    }

    return result;
}