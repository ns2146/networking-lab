#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 64

void die(char *msg) {
    fprintf(stderr, "DEAD: %s\n", msg);
    exit(1);
}

int main (int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server ip> <port num> <message>", argv[0]);
        exit(1);
    }


    char *server_ip = argv[1];
    int port_num = atoi(argv[2]);
    char *msg = argv[3];
    int sock;

    if ( (sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ) {
        die("socket() failed");
    }

    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = inet_addr(server_ip);
    s_addr.sin_port = htons(port_num);

    connect(sock, (struct sockaddr *) &s_addr, sizeof(s_addr));

    int msg_len = strlen(msg);

    //send the message to the server
    send(sock, msg, msg_len, 0);

    int total_recv = 0;
    char buffer[BUFSIZE];

    while (total_recv < msg_len) {
        int rcvd;

        if ( (rcvd = recv(sock, buffer, BUFSIZE - 1, 0)) <= 0 ) {
            die("recv() failed or connection closed");
        }
        total_recv += rcvd;
        buffer[rcvd] = '\0';
        printf("%s", buffer);
    }

    close(sock);
    return 0;
}