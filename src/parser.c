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
#include <json/json.h>
#include "parser.h"

void trim(char*);
void expandBuff(char**, size_t);
char* methodBuff;
int buffSize = 256;

/*
 * Opens file at 'filepath' and parses it as a method request (See manual for formatting details).
 * Builds a string ready to append to the service URL.
 * URL string format: <service url>action=<value>&[<parameter>=<value>&]*
 * Must call parser_getRequestURL and parser_getURLSize after this to receive the string and it's size
 */
void parser_buildRequestURL(char* filepath){
	int curState = Q;	// Begin at start state
	int foundToken = Q;	// Start state services as null token

	int lineCnt = 0;	// Counter for number of lines in the parsed file

	FILE *fp;
	char *line = NULL;
	methodBuff = (char*) malloc(buffSize * sizeof(char));	// Buffer that holds a parsed method call.
	size_t len = 0;
	ssize_t read;

	fp = fopen(filepath, "r");
	if(fp == NULL)
		exit(EXIT_FAILURE);

	// Parse each line in the input file
	while ( (read = getline(&line, &len, fp)) != -1 ) {
		foundToken = Q; // Reset to null token
		lineCnt++;

		if(line[0] != '#'){ // if the line is not a comment
			trim(line); // remove whitespaces & line feeds
			int i=0;
			while(i<strlen(line)){ // search line for a token {service, action, parameter} before '='
				if(line[i] == '='){ // found '='
					if( strncmp(line, strS, i) == 0 ){ foundToken = S;}	// service
					else if( strncmp(line, strA, i) == 0 ){ foundToken = A;}	// action
					else { foundToken = P; }	// parameter
				}
				i++;
			}

			// Process tokens as a FSM
			switch (curState){
				case Q:
					if( foundToken == S ){
						expandBuff(&methodBuff, strlen(line) + strlen(".cgi?") - strlen("service="));
						sprintf(methodBuff +strlen(methodBuff), "%s.cgi?", line+strlen("service=")); // concatenate '<service type>.cgi?' to methodBuff
						curState = S;
					}
					break;
				case S:
					if( foundToken == A ){
						expandBuff(&methodBuff, strlen(line)+1);
						sprintf(methodBuff +strlen(methodBuff), "%s&", line); // concatenate '<action>=<value>&' to methodBuff
						curState = A;
					}
					break;
				case A:
					if( foundToken == P ){
						expandBuff(&methodBuff, strlen(line)+1);
						sprintf(methodBuff +strlen(methodBuff), "%s&", line); // concatenate '<parameter>=<value>&' to methodBuff
						curState = P;
					}
					break;
				case P:
					if( foundToken == P ){
						expandBuff(&methodBuff, strlen(line)+1);
						sprintf(methodBuff +strlen(methodBuff), "%s&", line); // concatenate '<parameter>=<value>&' to methodBuff
						curState = P;
					}
					break;
			}
		}//end if(not comment)
	}//end while loop

	if(line)
		free(line);
}

/*
 * Returns the size of the URL string created by parser_buildRequestURL
 */
int parser_getURLSize(){
	return buffSize;
}


/* 
 * Returns the URL string created by parser_buildRequestURL
 * Return string format: <service url>action=<value>&[<parameter>=<value>&]*
 */
char* parser_getRequestURL(){
	return methodBuff;
}

/* Trims all spaces, tabs, and newlines from str */
void trim(char *str)
{
	char tmp[strlen(str)+1];
	int i,j=0;

	for(i=0; str[i]!='\0'; i++){
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') 
			tmp[j++]=str[i]; // copy non-white space value into tmp
	} 

	tmp[j]='\0'; // add null terminator
	strcpy(str, tmp); // overwrite str as tmp
}

/*
 * Reallocates ptr to hold n more chars
 */
void expandBuff(char **ptr, size_t n){
	if(strlen(*ptr)+n >= buffSize-1){ // if not enough memory is allocated
		buffSize *= 2;
		*ptr = (char*) realloc( *ptr, buffSize*sizeof(char) ); // double allocated space
		
		if( *ptr == NULL ) { // check for error from realloc
			printf("Error reallocating memory.\n");
			free(*ptr);
			exit(EXIT_FAILURE);
		}
	}
}
