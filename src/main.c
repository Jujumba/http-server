#include "socket.h" 

HttpResponse* sample(HttpRequest*) {
    HttpResponse* res = malloc(sizeof(HttpResponse));
    res->header = "HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nContent-Type: text/html\r\n\r\n";
    res->body = "<h1>Hello world!</h1>";
    return res;
}

int main() {
    HttpSocket* socket = create_socket(8888);
    put_listener(socket, "/", sample);
    socket->start(socket);
    return 0;
}
