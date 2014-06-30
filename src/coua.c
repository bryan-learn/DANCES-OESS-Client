/*
	Copyright © 2014, Pittsburgh Supercomputing Center (PSC).  All Rights
	Reserved.

	Permission to use this software and its documentation without fee for
	personal or non-commercial use within your organization is hereby
	granted, provided that the above copyright notice is preserved in all
	copies and that the copyright and this permission notice appear in
	supporting documentation.  Permission to redistribute this software
	to other organizations or individuals is strictly prohibited without
	the written permission of the Pittsburgh Supercomputing Center.  PSC
	makes no representations about the suitability of this software for
	any purpose.  It is provided "as is" without express or implied warranty.
*/

/*
 * CURL OESS User Agent (COUA)
 * COUA handles OESS user authentication as well as issuing method requests and receives the response.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <termios.h>
#include <unistd.h>
#include <curl/curl.h>
#include "cJSON.h"
#include "parser.h"
#include "coua.h"

/*
 * Builds request URL for the method call specified in the input file (see 
 * MANUAL for file format details). Issues the OESS method request storing the
 * response in the default file, or provided output file if one is given.
 * Returns a FILE pointer to the output file.
 */
FILE* coua_SendRequest(char *in_filepath, FILE *result, char *res_filepath)
{
	/* Output variables */
	FILE *headerRes;	// File to hold header output

	/* CURL library variables */
	CURL *handle = NULL;	// Handle for connection
	CURLcode res;		// result code of requests

	/* Init URL strings */
	const char *baseURL = "https://al2s.net.internet2.edu/oess/services-kerb/";
	char *requestURL = NULL;	// URL to issue the service request

	/* Build the requestURL - Append parsed requestURL to baseURL */
	parser_buildRequestURL(in_filepath);
	requestURL = (char*) malloc( parser_getURLSize() + sizeof(baseURL) ); // allocate enough space for entire URL
	sprintf(requestURL, "%s%s", baseURL, parser_getRequestURL());
	
	printf("complete url: %s\n", requestURL);

	/* Open output files */
	headerRes = fopen("output/headerRecv", "w");
	result = fopen(res_filepath, "w");

	if(result == NULL){ // Failed to open file
		printf("Failed to open file: %s", res_filepath);
		exit( EXIT_FAILURE );
	}

	/* Initilize cURL & get handle for connection */
	curl_global_init(CURL_GLOBAL_DEFAULT);
	handle = curl_easy_init();

	if(handle) {

		/* Basic User Agent Options */
		curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);		// follow redirects
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, result);		// set data output pointer
		curl_easy_setopt(handle, CURLOPT_WRITEHEADER, headerRes);	// set header output file
		curl_easy_setopt(handle, CURLOPT_VERBOSE, 0);			// turn off verbose output

		/* SSL Options */
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);		// skip peer verfiication (for now)
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 1);		// verify host
		curl_easy_setopt(handle, CURLOPT_SSLVERSION, 3);		// need to specify SSL version 3?
	
		/* Prompt user for credentials */
		coua_input_credentials();
	 
		/* Authentication Options */
		curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
		curl_easy_setopt(handle, CURLOPT_USERNAME, coua_creds.usr);		// TODO pull username and password from infile
		curl_easy_setopt(handle, CURLOPT_PASSWORD, coua_creds.pass);

		/* Request URL */
		curl_easy_setopt(handle, CURLOPT_URL, requestURL);

		/* Perform the request */
		printf("Performing request: %s\n", requestURL);
		res = curl_easy_perform(handle);

		/* Check for errors */ 
		if(res != CURLE_OK)
		  fprintf(stderr, "curl_easy_perform() failed: %s\n",
			  curl_easy_strerror(res));
		else
			printf("\nSee %s for result of the request.\n", res_filepath);

		/* Output results to stdout */
		// read results from file & store into cJSON struct
		//root = cJSON_CreateObject();	// Create empty cJSON object
		//root = cJSON_Parse();	// Populate cJSON struct from result
		
		/* Cleanup */ 
		curl_easy_cleanup(handle);
	}
	
	/* Cleanup */ 
	free(requestURL);
	fclose(headerRes);
	fclose(result);
	curl_global_cleanup();

	return result;	 
}

/* Prompt user to input Credentials (coua_creds) */
void coua_input_credentials(){
	// Setup Termios variables for stop terminal echoing password
	struct termios oflags, nflags;
	tcgetattr(fileno(stdin), &oflags);
	nflags = oflags;		// save original flags
	nflags.c_lflag &= ~ECHO;	// set disable echo flag
	nflags.c_lflag |= ECHONL;

	// Setup buffers
	size_t buff_size = 100;		// buffer char count
	//size_t bytes_read;		// number of bytes read
	char *usr_buff, *pass_buff;	// string buffers

	// Prompt for user name
	printf("\nPlease enter your credentials.\n");
	printf("User: ");

	// Store into buffer
	usr_buff = (char *) malloc(buff_size+1);
	/*bytes_read = */getline(&usr_buff, &buff_size, stdin);

	// remove trailing '\n' if present
	size_t last = strlen(usr_buff)-1;
	if( usr_buff[last] == '\n' ){ usr_buff[last] = '\0'; }


	// Echo disabled
        if( tcsetattr(fileno(stdin), TCSADRAIN, &nflags) != 0 ) {
                perror("tcsetattr");
                exit( EXIT_FAILURE );
        }

	// Prompt for password
	printf("Password: ");

	// Store into buffer
	pass_buff = (char *) malloc(buff_size+1);
	/*bytes_read = */getline(&pass_buff, &buff_size, stdin);

	// remove trailing '\n' if present
	last = strlen(pass_buff)-1;
      	if( pass_buff[last] == '\n' ){ pass_buff[last] = '\0'; }

        // Echo enabled
        if( tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0 ) {
                perror("tcsetattr");
                exit( EXIT_FAILURE );
        }


	coua_set_credentials(usr_buff, pass_buff);

	// Cleanup
	free(usr_buff);
	free(pass_buff);
}

/* Set Credentials (coua_creds) from input strings usr and passwd */
void coua_set_credentials(char *usr, char *pass){
	
	// if credentials exist, free memory
	if(coua_creds.usr != NULL){
		free(coua_creds.usr);
	}
	if(coua_creds.pass != NULL){
		free(coua_creds.pass);
	}

	// (re-)allocate memory
	coua_creds.usr = (char *) malloc(strlen(usr)+1);
	coua_creds.pass = (char *) malloc(strlen(pass)+1);

	// Copy memory from parameters to coua_creds
	strncpy(coua_creds.usr, usr, strlen(usr));
	strncpy(coua_creds.pass, pass, strlen(pass));
}

/* Frees resources allocated by coua functions */
void coua_cleanup(){
	// TODO free any dynamically allocated resources
}
