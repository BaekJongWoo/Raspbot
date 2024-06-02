#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"

int connect_to_server(const char *server_ip, int server_port);
int send_action(int sock, ClientAction *cAction);
int receive_update(int sock, DGIST *dgist);
void* receive_updates(void* arg);

#endif // CLIENT_H
