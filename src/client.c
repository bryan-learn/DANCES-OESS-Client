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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <curl/curl.h>
#include "cJSON.h"
#include "parser.h"

int main(int argc, char** argv)
{
	/* Output variables */
	FILE *headerRes;	// File to hold header output
	FILE *result;		// File to hold retrieved data
	char *filepath;		// Relative path to the result file

	/* CURL library variables */
	CURL *handle = NULL;	// Handle for connection
	CURLcode res;		// result code of requests

	/* Check arguments syntax */
	if( argc != 2 && argc != 3)
	{
		fprintf( stderr, "argc %d\nusage: %s infile [outfile]\n", argc, argv[0]);
		exit ( EXIT_FAILURE );
	}
	if(argv[2] != NULL)
		filepath = argv[2];
	else
		filepath = "output/result";

	/* Init URL strings */
	const char *baseURL = "https://al2s.net.internet2.edu/oess/services-kerb/";
	char *requestURL = NULL;	// URL to issue the service request

	/* Build the requestURL - Append parsed requestURL to baseURL */
	parser_buildRequestURL(argv[1]);
	requestURL = (char*) malloc( parser_getURLSize() + sizeof(baseURL) ); // allocate enough space for entire URL
	sprintf(requestURL, "%s%s", baseURL, parser_getRequestURL());
	
	printf("complete url: %s\n", requestURL);

	/* Open output files */
	headerRes = fopen("output/headerRecv", "w");
	result = fopen(filepath, "w");

	if(result == NULL)
		exit( EXIT_FAILURE ); // Failed to open file

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
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 1);		// peer verfiication
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 1);		// verify host
		curl_easy_setopt(handle, CURLOPT_SSLVERSION, 3);		// need to specify SSL version 3?
		 
		/* Authentication Options */
		curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
		curl_easy_setopt(handle, CURLOPT_USERNAME, "usr");		// TODO Add your OESS credentials. (Eventually need to remove credentials from source code)
		curl_easy_setopt(handle, CURLOPT_PASSWORD, "pass");

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
			printf("\nSee %s for result of the request.\n", filepath);

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
	 
	return 0;
}
