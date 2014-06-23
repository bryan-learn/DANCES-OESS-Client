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
#include <stdint.h>
#include <sysexits.h>
#include "coua.h"
#include "cJSON.h"
#include "parser.h"
#include <iperf_api.h>

int main(int argc, char** argv)
{

	/* Process and execute OESS method call from infile */

	FILE *result = NULL;	// File to hold the method call response
	char *filepath = NULL;	// relative path to the result file

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

	// Perform the request. Store response in result
	coua_SendRequest(argv[1], result, filepath);


	return 0;
}
