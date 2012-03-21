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

/* Requests a certificate from the website given by the url. */
char * 
request_certificate (char *url);

/* Verifies that the received certificate from the website matches with the
 * fingerprint from the user.
 */
int
verify_certificate (char *fingerprint_from_client, char *fingerprint_from_website);

/* Verifies that a fingerprint has the correct format. */
int
verify_fingerprint_format (char *fingerprint);
#endif // CERTIFICATE_H
