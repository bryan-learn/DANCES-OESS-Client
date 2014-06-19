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

#ifndef parser_h
#define parser_h

/* States */
#define Q 0	// start state
#define S 1	// service
#define A 2	// action (accept state)
#define P 3	// parameter (accept state)

/* State String Values */
#define strS "service"
#define strA "action"

/*
 * Opens file at 'filepath' and parses it a method request (See manual for formatting details).
 * Returns a string ready to append to the service URL.
 * Return string format: <service url>action=<value>&[parameter=<value>&value=<value>&]*
 */
extern void parser_buildRequestURL(char* filepath);
extern int parser_getURLSize();
extern char* parser_getRequestURL();

#endif
