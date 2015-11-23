#include <rpc/rpc.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include "Cookie.h"
/**
 * Assignment 6: Judy client code
 * Author: Hayden Schmackpfeffer
 */
#define JUDY 2

char *server_hostname;
char machine[200];
CLIENT *client2;
time_t t;
int flag;
int cookies_rcved;

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
	cookies_rcved = 0; //track how many cookies Judy got

	if (client2 = clnt_create(server_hostname, COOKIE_JAR, COOKIE_JAR_VERSION, "udp") == NULL) {
		printInfo();
		printf("---JUDY: connecting to server: %s \n", server_hostname);
		clnt_pcreateerror("Error creating client\n");
		exit(EXIT_FAILURE);
	}

	//seed the random number generator
    srand((unsigned int)time(&t));

	while (flag == 1) {
		if ( (rand() % 2) == 0 ){
			getCookie();
		}
		sleep(1);
	} 

	printInfo();
	printf("---JUDY: done requesting cookies\n Total Cookies Recieved: %d\n", cookies_rcved);

	return (0);
}

void getCookie() {
	struct CookieRequest *status;
	struct CookieRequest par;

	//set up the request
	par.sister=JUDY;

	//ensure that the client is not null before we try to send our request
	if (client2 == NULL) {
		printInfo();
		printf("---JUDY: reconnecting to server: %s \n", server_hostname);
		if ((client2 = clnt_create(server_hostname, COOKIE_JAR, COOKIE_JAR_VERSION, "udp")) == NULL) {
			clnt_pcreateerror("Error creating client\n");
			exit(EXIT_FAILURE);
		}
	}
	printInfo();
	printf("---JUDY: making cookie request\n");
	//make the cookie request
	if ((status=get_cookie_1(&par, client2)) ==NULL) {
	    clnt_perror(client2,server_hostname);
	    clnt_destroy(client2);
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
		printf("---JUDY: cookie jar is EMPTY oh no!!!\n");
	}
	else if (status->err==-1){
		printf("---JUDY: Judy must wait for Tina to get 2 cookies in a row\n");
	}
	else if (status->err== 1) {
		cookies_rcved++;
		printf("---JUDY: successfully got a cookie!!!\n");
	}
}

void printInfo() {
	t = time(NULL);
	printf("\n Machine: %s, Time: %ld \n", machine, t);
}
