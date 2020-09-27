
#include <iostream>
using namespace std;
#pragma comment(lib, "ws2_32.lib") // доступ к некоторым функциям
#include <winsock2.h>
#pragma warning(disable: 4996)


int main()
{
    // загржуаем нужную версию  .. 
    // создадим структуру
    WSADATA wsaData;
    WORD Version = MAKEWORD(2, 1); // версия 2.1
    // проверяем как загрузилась библиотека
    if (WSAStartup(Version, &wsaData) != 0)
    {
        cout << "Error load of library" << endl;
        exit(1);
    }
    // теперь соберем всю информацию для сокета
    SOCKADDR_IN addr;
    int size = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // local
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET; // семейство протоколов (TCP/UPD)

    SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);

    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
    {
        cout << "Connection to server return error" << endl;
    }
    else
    {
        cout << "Connection to server succed" << endl;
    }

    return 0;
}