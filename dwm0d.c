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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <X11/Xlib.h>
#include <signal.h>

#define PATH "/tmp/dwm_statusbar"
#define BUFSIZE 256

static Display *dpy;
int sockfd;

void error(char *msg)
{
    perror(msg);
    exit(1);
}

static void handler(int signum)
{
    close(sockfd);
    unlink(PATH);
    XCloseDisplay(dpy);
    exit(0);
}

    char *
smprintf(char *fmt, ...)
{
    va_list fmtargs;
    char *ret;
    int len;
    va_start(fmtargs, fmt);
    len = vsnprintf(NULL, 0, fmt, fmtargs);
    va_end(fmtargs);
    ret = malloc(++len);
    if (ret == NULL) {
	perror("malloc");
	exit(1);
    }
    va_start(fmtargs, fmt);
    vsnprintf(ret, len, fmt, fmtargs);
    va_end(fmtargs);
    return ret;
}

void trim(char *str)
{
    char *ptr = str;
    while(*ptr == ' ' || *ptr == '\t' || *ptr == '\r' || *ptr == '\n') ++ptr;
    char *end = ptr;
    while(*end) ++end;
    if (end > ptr)
    {
	for(--end; end >= ptr && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'); --end);
    }
    memmove(str, ptr, end-ptr);
    str[end-ptr+1] = 0;
}

void setstatus(char *str)
{
    XStoreName(dpy, DefaultRootWindow(dpy), str);
    XSync(dpy, False);
}

int main(int argc, char *argv[])
{
    int newsockfd, n;
    char buffer[BUFSIZE];
    struct sigaction sa;
    struct sockaddr_un serv_path;
    char *status;
    if (!(dpy = XOpenDisplay(NULL))) {
	fprintf(stderr, "dwmstatus: cannot open display.\n");
	return 1;
    }
    /*handle SIGTERM and SIGINT*/
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if( sigaction(SIGTERM, &sa, NULL) ){
	perror("sigaction()");
	exit(-1);
    }
    if( sigaction(SIGINT, &sa, NULL) ){
	perror("sigaction()");
	exit(-1);
    }
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
	error("ERROR opening socket");
    bzero((char *) &serv_path, sizeof(serv_path));
    serv_path.sun_family = AF_UNIX;
    strncpy(serv_path.sun_path, PATH, 20);
    if (bind(sockfd, (struct sockaddr *) &serv_path,
		sizeof(serv_path)) < 0)
	error("ERROR on binding");
    while(1){
	bzero(buffer,BUFSIZE);
	n = recv(sockfd, buffer, BUFSIZE-1, 0);
	if (n < 0)
	    error("ERROR reading from socket");
	trim(buffer);
	status = smprintf("%s",buffer);
	setstatus(status);
	free(status);
    }
    close(newsockfd);
    close(sockfd);
    unlink(PATH);
    XCloseDisplay(dpy);
    return 0;
}
