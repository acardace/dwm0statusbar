/*
 ** dwm0statusbar, a simple dwm status bar daemon.
 ** Copyright (C) 2015  Antonio Cardace.
 **
 ** This file is part of dwm0statusbar.
 **
 ** dwm0statusbar is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** dwm0statusbar is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>

#define PATH "/tmp/dwm_statusbar"
#define BUFSIZE 256

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_un serv_path;
    char buffer[BUFSIZE];
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_path, sizeof(serv_path));
    serv_path.sun_family = AF_UNIX;
    strncpy(serv_path.sun_path, PATH, 20);
    if (connect(sockfd,(struct sockaddr *)&serv_path,sizeof(serv_path)) < 0)
        error("ERROR connecting");
    bzero(buffer,BUFSIZE);
    if (argc < 2) {
        fgets(buffer, BUFSIZE-1, stdin);
    }
    else {
        for (int i = 1, len = 0; i < argc; i++)
          len += snprintf (&buffer[len],sizeof(buffer)-len, "%s ", argv[i]);
    }
    n = send(sockfd, buffer, strlen(buffer), 0);
    if (n < 0)
        error("ERROR writing to socket");
    return 0;
}
