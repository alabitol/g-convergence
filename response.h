/******************************************************************************
 * Authors: g-coders
 * Created: March 4, 2012
 * Revised: May 2, 2012
 * Description: This file contains functions which format the verification
 * response and functions which send the response back to the client.
 ******************************************************************************/

#ifndef RESPONSE_H
#define RESPONSE_H

#include "notary.h"

/**
 * Generates a signature of a list of fingerprints using the notary's private
 * key.
 */
int
generate_signature(unsigned char *fingerprint_list, unsigned char *signature,
                   unsigned int *signature_size, RSA *private_key);

/* Obtains a response to a POST/GET request. */
int retrieve_response (void *coninfo_cls, host *host_to_verify, const char *fingerprint_from_client);

/* Sends response back to the client. This function could be a wrapper for
 * send_page.
 */
int send_response (struct MHD_Connection *connection, const char *response_data,
               int status_code);


#endif // RESPONSE_H
