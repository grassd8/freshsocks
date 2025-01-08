// Grass D8 Freshsock project.
// Small web server
// $ gcc main.c -o fs

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>


#define NODE_PORT  8880

static char buffer[2048];

void dispatch(int fd);

// =========================================

void dispatch(int fd)
{
    int r_bytes=0;

    if (fd<=0)
        return;

    memset(buffer,0,2048);

    r_bytes = (int) read (fd, buffer, 2048);
    if (r_bytes <= 0)
        return;

// Print
    printf ("Data: {%s}\n",buffer);
}


int main( int argc, char** argv)
{
    struct sockaddr_in  serverAddress;
    serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serverAddress.sin_port = htons(NODE_PORT);
    serverAddress.sin_family = AF_INET;
    size_t host_addrlen = sizeof(serverAddress);

    printf("Hello from Freshsocks\n");

// Create socket
    int server_fd = -1;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd <0)
        goto fail;

// Bind
// Bind the socket to the address
    if ( bind(server_fd, (struct sockaddr *) &serverAddress, host_addrlen ) != 0 ) 
    {
        perror("On bind\n");
        goto fail;
    }

// Listen for incoming connections
    if (listen(server_fd, SOMAXCONN) != 0) 
    {
        perror("On listen\n");
        goto fail;
    }

// Accept
    printf("Listening for connections...\n");
    int newsockfd = -1;
    //while (TRUE)
    while (1)
    {
        // Accept incoming connections
        newsockfd = 
            accept( 
                server_fd, 
                (struct sockaddr *)&serverAddress,
                (socklen_t *)&host_addrlen);
        
        if (newsockfd > 0)
        {
            printf("connection accepted\n");
            dispatch(newsockfd);
            close(newsockfd);          
        }
    };

    printf("Done\n");
    //close(server_fd);
    return 0;

fail:
    return 1;
}
