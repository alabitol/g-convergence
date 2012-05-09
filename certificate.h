/******************************************************************************
 * Authors: g-coders
 * Created: February 21, 2012
 * Revised: March 11, 2012
 * Description: Handle sending a certificate request to the website and
 *              receiving its response.
 ******************************************************************************/
#ifndef CERTIFICATE_H
#define CERTIFICATE_H

#include "notary.h"
#include <regex.h>
/* Requests a certificate from the website given by the url
   This function calculates and returns the fingerprint of the 
   requested certificate
*/
int 
request_certificate (host *host_to_verify, char** fingerprints);


/* Verifies that the received certificate from the website matches with the
 * fingerprint from the user. Returns 1 if fingerprints match. Otherwise,
 * returns 0.
 */
int verify_certificate (const char *fingerprint_from_client, char **fingerprints_from_website, int num_of_website_certs);



/* Verifies that a fingerprint has the correct format. */
int verify_fingerprint_format (char *fingerprint);
#endif // CERTIFICATE_H
