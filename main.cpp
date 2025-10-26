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

// Функции для меню
void executeSendToAll ( User *user ) {
    system ( "cls" );
    string text, tag;
    cout << "Введите сообщение: ";
    getline ( cin, text );
    cout << "Укажите важность вашего сообщения (Low, Medium, High): ";
    getline ( cin, tag );

    static const set<string> validTags = { "Low", "Medium", "High" };
    if ( validTags.find ( tag ) == validTags.end ( ) ) {
        system ( "cls" );
        cout << "Неверный приоритет сообщения\n";
        cout << "Для продолжения нажмите на END" << endl;
        while ( !GetAsyncKeyState ( VK_END ) ) { }
        return;
    }

    addMessage ( "messages.txt", Message ( "ALL", user->getUsername ( ), text, tag ) );
    cout << "Сообщение отправлено всем!\n";
}

void executeSendPrivate ( User *user ) {
    system ( "cls" );
    string getter, text, tag;
    cout << "Введите логин получателя: ";
    getline ( cin, getter );
    cout << "Введите сообщение: ";
    getline ( cin, text );
    cout << "Укажите важность вашего сообщения (Low, Medium, High): ";
    getline ( cin, tag );

    static const set<string> validTags = { "Low", "Medium", "High" };
    if ( validTags.find ( tag ) == validTags.end ( ) ) {
        system ( "cls" );
        cout << "Неверный приоритет сообщения\n";
        cout << "Для продолжения нажмите на END" << endl;
        while ( !GetAsyncKeyState ( VK_END ) ) { }
        return;
    }

    addMessage ( "messages.txt", Message ( getter, user->getUsername ( ), text, tag ) );
    cout << "Сообщение отправлено!\n";
}

void executeViewMessages ( User *user ) {
    system ( "cls" );
    vector<Message> messages = getMessage ( "messages.txt" );

    // Группируем сообщения по тегам
    map<string, vector<Message>> groupedMessages;
    for ( const auto &msg : messages ) {
        if ( ( msg.getGetter ( ) == user->getUsername ( ) || msg.getGetter ( ) == "ALL" ) &&
            msg.getSender ( ) != user->getUsername ( ) ) {
            groupedMessages [msg.getTag ( )].push_back ( msg );
        }
    }

    cout << "--- Сообщения ---\n";

    // Порядок отображения приоритетов
    vector<string> tagOrder = { "Low", "Medium", "High" };

    for ( const auto &tag : tagOrder ) {
        auto it = groupedMessages.find ( tag );
        if ( it != groupedMessages.end ( ) && !it->second.empty ( ) ) {
            cout << "\n" << tag << " priority:\n";
            cout << string ( 50, '-' ) << "\n";

            for ( const auto &msg : it->second ) {
                cout << "От: " << msg.getSender ( )
                    << "\nТекст: " << msg.getText ( )
                    << "\n\n";
            }
        }
    }

    cout << "Для продолжения нажмите на END" << endl;
    while ( !GetAsyncKeyState ( VK_END ) ) { }
}

// Регистрация
void registerUser ( ) {
    system ( "cls" );
    string username, password, name;
    cout << "Введите логин: ";
    getline ( cin, username );
    cout << "Введите пароль: ";
    getline ( cin, password );
    cout << "Введите имя: ";
    getline ( cin, name );

    if ( userExists ( username ) || username == "ALL" ) {
        cout << "Логин занят!\n";
        cout << "Для продолжения нажмите на END" << endl;
        while ( !GetAsyncKeyState ( VK_END ) ) { }
        return;
    }

    User newUser ( username, password, name );
    saveUser ( newUser );
    cout << "Регистрация успешна!\n";
}

// Вход
unique_ptr<User> loginUser ( ) {
    system ( "cls" );
    string username, password;
    cout << "Введите логин: ";
    getline ( cin, username );
    cout << "Введите пароль: ";
    getline ( cin, password );

    vector<User> users = loadUsers ( );
    for ( const auto &u : users ) {
        if ( u.getUsername ( ) == username && u.getPassword ( ) == password ) {
            return make_unique<User> ( u.getUsername ( ), u.getPassword ( ), u.getName ( ) );
        }
    }

    system ( "cls" );
    cout << "Неверный логин или пароль!\n";
    cout << "Для продолжения нажмите на END" << endl;
    while ( !GetAsyncKeyState ( VK_END ) ) { }
    return nullptr;
}

// Структура для хранения действий меню
struct MenuAction {
    string name;
    void ( *action )( User * );
};

// Выбор действия в чате
void chatMenu ( User *user ) {
    vector<MenuAction> menuActions = {
        { "Отправить всем", executeSendToAll },
        { "Личное сообщение", executeSendPrivate },
        { "Просмотр сообщений", executeViewMessages }
    };

    while ( true ) {
        system ( "cls" );
        cout << "1. Отправить всем\n2. Личное сообщение\n3. Просмотр сообщений\n4. Выход\nВыберите: ";
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
            cout << "Неверный выбор!\n";
            system ( "cls" );
            cout << "Для продолжения нажмите на END" << endl;
            while ( !GetAsyncKeyState ( VK_END ) ) { }
        }
    }
}

// Точка входа программы
int main ( ) {
    setlocale ( LC_ALL, "RUS" );

    while ( true ) {
        system ( "cls" );
        cout << "1. Регистрация\n2. Вход\n3. Выход\nВыберите: ";
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
            cout << "Неверный выбор!\n";
            system ( "cls" );
            cout << "Для продолжения нажмите на END" << endl;
            while ( !GetAsyncKeyState ( VK_END ) ) { }
        }
    }
    return 0;
}