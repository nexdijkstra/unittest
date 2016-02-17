#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<err.h>

#define CODE404 404
#define CODE200 200
#define CODE400 400

#define PHRASE200 "OK"
#define PHRASE404 "FILE NOT FOUND"
#define PHRASE400 "Bad Request"

#define URLSIZ 100


#ifndef _CONST_H_
#define _CONST_H_


#endif
