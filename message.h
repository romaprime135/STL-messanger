#pragma once
#include <string>
#include <vector>

class Message {
    std::string Sender;
    std::string Getter;
    std::string Text;
    std::string Tag;

public:
    std::string getText ( ) const;
    std::string getSender ( ) const;
    std::string getGetter ( ) const;
    std::string getTag ( ) const;
    Message ( const std::string &Getter, const std::string &Sender,
            const std::string &Text, const std::string &Tag );
    Message ( );
};

std::vector<Message> getMessage ( const std::string &Path );
void addMessage ( const std::string &Path, const Message &Msg );