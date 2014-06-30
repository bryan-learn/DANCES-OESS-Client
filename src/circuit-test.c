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
#include <stdbool.h>
#include "coua.h"
#include "cJSON.h"
#include "parser.h"
#include <iperf_api.h>

int main(int argc, char** argv)
{
	/* Provision new circuit */
	FILE *result = NULL;	// File to hold the method call response
	char *host;		// Host name for iperf test
	int port;		// Port number for iperf test
	struct iperf_test *test;
	bool prov_success = false;	// Did the circuit provision succeed?
	bool iperf_success = false;	// Did the iperf test succeed?
	bool rem_success = false;	// Did the circuit remove succeed?

	// Perform the circuit provision request
	coua_SendRequest("input/provision_circuit", result, "output/provision_result");

	// Parse result for success & circuit id
	// TODO
	prov_success = true;

	/* Test circuit with iperf */
	if(prov_success){
		// Setup iperf test
		host = "?";
		port = 5000;
	
		test = iperf_new_test();	// Instantiate a new iperf_test
		if (test == NULL ){
			printf("Failed to create test.\n");
			exit( EXIT_FAILURE );
		}

		// Set test parameters.
		iperf_defaults(test);
		iperf_set_verbose(test,1);
		iperf_set_test_role( test, 'c' );
		iperf_set_test_server_hostname( test, host );
		iperf_set_test_server_port( test, port );
		iperf_set_test_omit( test, 3 );
		iperf_set_test_duration( test, 5 );
		iperf_set_test_reporter_interval( test, 1 );
		iperf_set_test_stats_interval( test, 1 );
	
		printf("Do iperf test\n");
		// Perform iperf test
		if(iperf_run_client(test) < 0){
			printf("Error - %s\n", iperf_strerror(i_errno));
			exit( EXIT_FAILURE );
		}

	} // end if(prov_success)


	/* Remove the newly provisioned circuit */

	// Edit remove_circuit file to set the circuit id

	// Perform the circuit remove request
	//coua_SendRequest("input/remove_circuit", result, "output/remove_result");

	/* Clean-up */
	iperf_free_test(test);

	return 0;
}
