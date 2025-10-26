#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <memory>
#include "user.h"
#include "message.h"
#include <windows.h>

using namespace std;

// ������� ��� ����
void executeSendToAll ( User *user ) {
    system ( "cls" );
    string text, tag;
    cout << "������� ���������: ";
    getline ( cin, text );
    cout << "������� �������� ������ ��������� (Low, Medium, High): ";
    getline ( cin, tag );

    static const set<string> validTags = { "Low", "Medium", "High" };
    if ( validTags.find ( tag ) == validTags.end ( ) ) {
        system ( "cls" );
        cout << "�������� ��������� ���������\n";
        cout << "��� ����������� ������� �� END" << endl;
        while ( !GetAsyncKeyState ( VK_END ) ) { }
        return;
    }

    addMessage ( "messages.txt", Message ( "ALL", user->getUsername ( ), text, tag ) );
    cout << "��������� ���������� ����!\n";
}

void executeSendPrivate ( User *user ) {
    system ( "cls" );
    string getter, text, tag;
    cout << "������� ����� ����������: ";
    getline ( cin, getter );
    cout << "������� ���������: ";
    getline ( cin, text );
    cout << "������� �������� ������ ��������� (Low, Medium, High): ";
    getline ( cin, tag );

    static const set<string> validTags = { "Low", "Medium", "High" };
    if ( validTags.find ( tag ) == validTags.end ( ) ) {
        system ( "cls" );
        cout << "�������� ��������� ���������\n";
        cout << "��� ����������� ������� �� END" << endl;
        while ( !GetAsyncKeyState ( VK_END ) ) { }
        return;
    }

    addMessage ( "messages.txt", Message ( getter, user->getUsername ( ), text, tag ) );
    cout << "��������� ����������!\n";
}

void executeViewMessages ( User *user ) {
    system ( "cls" );
    vector<Message> messages = getMessage ( "messages.txt" );

    // ���������� ��������� �� �����
    map<string, vector<Message>> groupedMessages;
    for ( const auto &msg : messages ) {
        if ( ( msg.getGetter ( ) == user->getUsername ( ) || msg.getGetter ( ) == "ALL" ) &&
            msg.getSender ( ) != user->getUsername ( ) ) {
            groupedMessages [msg.getTag ( )].push_back ( msg );
        }
    }

    cout << "--- ��������� ---\n";

    // ������� ����������� �����������
    vector<string> tagOrder = { "Low", "Medium", "High" };

    for ( const auto &tag : tagOrder ) {
        auto it = groupedMessages.find ( tag );
        if ( it != groupedMessages.end ( ) && !it->second.empty ( ) ) {
            cout << "\n" << tag << " priority:\n";
            cout << string ( 50, '-' ) << "\n";

            for ( const auto &msg : it->second ) {
                cout << "��: " << msg.getSender ( )
                    << "\n�����: " << msg.getText ( )
                    << "\n\n";
            }
        }
    }

    cout << "��� ����������� ������� �� END" << endl;
    while ( !GetAsyncKeyState ( VK_END ) ) { }
}

// �����������
void registerUser ( ) {
    system ( "cls" );
    string username, password, name;
    cout << "������� �����: ";
    getline ( cin, username );
    cout << "������� ������: ";
    getline ( cin, password );
    cout << "������� ���: ";
    getline ( cin, name );

    if ( userExists ( username ) || username == "ALL" ) {
        cout << "����� �����!\n";
        cout << "��� ����������� ������� �� END" << endl;
        while ( !GetAsyncKeyState ( VK_END ) ) { }
        return;
    }

    User newUser ( username, password, name );
    saveUser ( newUser );
    cout << "����������� �������!\n";
}

// ����
unique_ptr<User> loginUser ( ) {
    system ( "cls" );
    string username, password;
    cout << "������� �����: ";
    getline ( cin, username );
    cout << "������� ������: ";
    getline ( cin, password );

    vector<User> users = loadUsers ( );
    for ( const auto &u : users ) {
        if ( u.getUsername ( ) == username && u.getPassword ( ) == password ) {
            return make_unique<User> ( u.getUsername ( ), u.getPassword ( ), u.getName ( ) );
        }
    }

    system ( "cls" );
    cout << "�������� ����� ��� ������!\n";
    cout << "��� ����������� ������� �� END" << endl;
    while ( !GetAsyncKeyState ( VK_END ) ) { }
    return nullptr;
}

// ��������� ��� �������� �������� ����
struct MenuAction {
    string name;
    void ( *action )( User * );
};

// ����� �������� � ����
void chatMenu ( User *user ) {
    vector<MenuAction> menuActions = {
        { "��������� ����", executeSendToAll },
        { "������ ���������", executeSendPrivate },
        { "�������� ���������", executeViewMessages }
    };

    while ( true ) {
        system ( "cls" );
        cout << "1. ��������� ����\n2. ������ ���������\n3. �������� ���������\n4. �����\n��������: ";
        string choice;
        getline ( cin, choice );

        if ( choice == "1" ) {
            executeSendToAll ( user );
        }
        else if ( choice == "2" ) {
            executeSendPrivate ( user );
        }
        else if ( choice == "3" ) {
            executeViewMessages ( user );
        }
        else if ( choice == "4" ) {
            break;
        }
        else {
            cout << "�������� �����!\n";
            system ( "cls" );
            cout << "��� ����������� ������� �� END" << endl;
            while ( !GetAsyncKeyState ( VK_END ) ) { }
        }
    }
}

// ����� ����� ���������
int main ( ) {
    setlocale ( LC_ALL, "RUS" );

    while ( true ) {
        system ( "cls" );
        cout << "1. �����������\n2. ����\n3. �����\n��������: ";
        string choice;
        getline ( cin, choice );

        if ( choice == "1" ) {
            registerUser ( );
        }
        else if ( choice == "2" ) {
            auto user = loginUser ( );
            if ( user != nullptr ) {
                chatMenu ( user.get ( ) );
            }
        }
        else if ( choice == "3" ) {
            break;
        }
        else {
            cout << "�������� �����!\n";
            system ( "cls" );
            cout << "��� ����������� ������� �� END" << endl;
            while ( !GetAsyncKeyState ( VK_END ) ) { }
        }
    }
    return 0;
}