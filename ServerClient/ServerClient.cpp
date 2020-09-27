// SERVER
#include <stdio.h>
#include <iostream>
using namespace std;
#pragma comment(lib, "ws2_32.lib") // доступ к некоторым функциям
#include <winsock2.h>
#include <vector>
#pragma warning(disable: 4996)
#include <thread>
#include <conio.h>

const int KEY_ESC = 27;

void esc() // выход по клавише esc
{
    char symb;
    int code_symb;

    symb = getch();
    code_symb = symb;

    if (code_symb == 27)
    {
        exit(1);
        cout << "the server was stopped" << endl;
  
    }
}

SOCKET IncomingSockets[100];

void UserDisconnect(int index)
{
    while (true)
    {
        int r = recv(IncomingSockets[index], NULL, 0, 0);
        if (r == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET) {
            cout << "client: " << index << "disconnect from server" << endl;
        }
    }
   
}

int counter;

void GetRequest(int index)
{
    char msg[256];
    char answer_msg[256] = "your request was successfully sent to server";

    while (true)
    {
        recv(IncomingSockets[index], msg, sizeof(msg), NULL);
        cout << "client: " << counter << ": " <<  msg <<  endl;
    
        for (int i = 0; i < counter; i++)
        {
            if (i == index)
            {
                send(IncomingSockets[index], answer_msg, sizeof(answer_msg), NULL);
            }
        }
        
    }
}
int main()
{
     cout << "If you need to disconnect press ESC..." << endl;
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

    // Передаем всю информацию сокету
    SOCKET listener = socket(AF_INET, SOCK_STREAM, NULL);
    // привязываем 
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    // переводим в режим прослушки
    listen(listener, SOMAXCONN); // вторым параметром кол-во клиентов
    
    // построим новый сокет, чтобы удерживать соедение с клиентом
    SOCKET current_socket;
    for (int i = 0; i < 100; i++)
    {
        current_socket = accept(listener, (SOCKADDR*)&addr, &size);

        if (current_socket == 0)
        {
            cout << "Connection Error" << endl;
        }
        else
        {
            cout << "Connection succeed" << endl;
            Sleep(100);
            cout << "client: " << i + 1 << " connected" << endl;

            char msg1[256];

            IncomingSockets[i] = current_socket;

            counter++;

            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GetRequest, LPVOID(i), NULL, NULL);

            bool is_exit = false;

            while (true)
            {
                int r = recv(IncomingSockets[i], NULL, 0, 0);
                if (r == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET) {
                    cout << "client: " << i + 1 << " disconnect from server" << endl;
                    exit(1);
                }
            }
          

        }
    }
    return 0;

}


