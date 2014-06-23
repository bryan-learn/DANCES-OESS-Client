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


#ifndef coua_h
#define coua_h

extern void coua_SendRequest(char *in_filepath, FILE *result, char *out_filepath);

#endif
