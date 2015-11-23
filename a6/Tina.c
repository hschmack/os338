#include <rpc/rpc.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include "Cookie.h"
/**
 * Assignment 6: Tina client code
 * Author: Hayden Schmackpfeffer
 */
#define TINA 1

char *server_hostname;
char machine[200];
CLIENT *client1;
time_t t;
int flag;
cookies_rcved;

void getCookie();
void printInfo();

int main(int argc, char *argv[]) {

	if (argc < 2) {				//check that server is specified
    	printf("The server must be specified\n");
    	printf("Usage: %s <hostname> \n",argv[0]);
   		exit(1);
   	}

   	//hostname is something like eecslinab.case.edu
	server_hostname = argv[1];
	gethostname(machine,200);	//set machine name
	flag = 1; //flag will be set to -1 if jar is out of cookies
	cookies_rcved = 0;

	if (client1 = clnt_create(server_hostname, COOKIE_JAR, COOKIE_JAR_VERSION, "udp") == NULL) {
		printInfo();
		printf("---TINA: connecting to server: %s \n", server_hostname);
		clnt_pcreateerror("Error creating client\n");
		exit(EXIT_FAILURE);
	}

	while (flag == 1) {
		getCookie();
		sleep(1);
	} 

	printInfo();
	printf("---TINA: done requesting cookies\n Total Cookies Recieved: %d\n", cookies_rcved);

	return (0);
}

void getCookie() {
	struct CookieRequest *status;
	struct CookieRequest par;

	//set up the request
	par.sister=TINA;

	//ensure that the client is not null before we try to send our request
	if (client1 == NULL) {
		printInfo();
		printf("---TINA: reconnecting to server: %s \n", server_hostname);
		if ((client1 = clnt_create(server_hostname, COOKIE_JAR, COOKIE_JAR_VERSION, "udp")) == NULL) {
			clnt_pcreateerror("Error creating client\n");
			exit(EXIT_FAILURE);
		}
	}
	printInfo();
	printf("---TINA: making cookie request\n");
	//make the cookie request
	if ((status=get_cookie_1(&par, client1)) ==NULL) {
	    clnt_perror(client1,server_hostname);
	    clnt_destroy(client1);
	    exit(EXIT_FAILURE);
	}

	/*
	 * Return Codes
	 * -2: Cookie jar is empty
	 * -1 if it is judy, and judy cannot request
	 * +1 if the cookie is successfully returned
	 */
	printInfo();
	if (status->err== -2) {
		flag  = -1;
		printf("---TINA: cookie jar is EMPTY oh no!!!\n");
	}
	else if (status->err== 1) {
		cookies_rcved++;
		printf("---TINA: successfully got a cookie!!!\n");
	}
}

void printInfo() {
	t = time(NULL);
	printf("\n Machine: %s, Time: %ld \n", machine, t);
}
