#include "socket.h" 

int main() {
    HttpSocket* socket = create_socket(8888);
    socket->start(socket);
    return 0;
}
