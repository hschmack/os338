#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Cookie.h"

#define TINA 1
#define JUDY 2
#define EMPTY -2
#define NOTJUDYSTURN -1
#define SUCCESS 1
// total initial cookies = 20, tinaCookies tracks how many cookies tina has gotten since Judy
int cookies = 20, tinaCookies = 0;
time_t t;

/**
 * Assignment 6:
 * Author: Hayden Schmackpfeffer
 */
struct CookieRequest* get_cookie_1_svc(struct CookieRequest *request. struct svc_req *rqstp) {
	static struct CookieRequest response;
	int sister = request->sister;

	/*
	 * Return Codes
	 * -2: Cookie jar is empty
	 * -1 if it is judy, and judy cannot request
	 * +1 if the cookie is successfully returned
	 */
	if (cookies == 0) {
		response.err=-2;
		response.sister=TINA;
		printf("---MOTHER: Not enough cookies in the cookie jar.\n");
		return (&response);
	}
	if (sister == TINA) {
		cookies--;
		tinaCookies++;
		response.err=1;
		response.sister=TINA;
		printf("---MOTHER: Giving a cookie to TINA. Cookies left: %d, Tina's cookie streak: %d\n", cookies, tinaCookies);
		return (&response);
	}
	//else assume the sister is JUDY
	if (tinaCookies < 2) {
		response.err=-1;
		reponse.sister=JUDY;
		printf("---MOTHER: JUDY wants a cookie, but TINA has only gotten: %d \n", tinaCookies);
		return (&response);
	} else {
		cookies--;
		tinaCookies=0;
		reponse.err=1;
		reponse.sister=JUDY;
		printf("---MOTHER: Giving a cookie to JUDY. Cookies left: %d", cookies);
		return (&response);
	}

	response.err=0;
	reponse.sister=-1;
	prinft("---MOTHER: This case should never be reached (sister is not Judy or Tina");
	return response;
}