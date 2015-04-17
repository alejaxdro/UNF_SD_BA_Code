// socket_fail_1.c
//
// Alejandro Lepervanche using kaiwwii ho code from stackoverflow.com
// Date: 3/9/2015
//
// This code creates and provides functions for socket ip communication


#include <string.h>
#include <jni.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

JNIEXPORT void JNICALL Java_gz_kaiwii_NativeSocketActivity_start
  (JNIEnv *env, jclass jclss)
{
    int sockfd;
    int len;
    struct sockaddr_un address;
    int result;
    char ch = 'A';

/*  Create a socket for the client.  */

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

/*  Name the socket, as agreed with the server.  */

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, "server_socket");
    len = sizeof(address);

/*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("oops: client1");
        exit(1);
    }
    

/*  We can now read/write via sockfd.  */

    write(sockfd, &ch, 1);
    read(sockfd, &ch, 1);
    
    printf("char from server = %c\n", ch);
    
    close(sockfd);
}