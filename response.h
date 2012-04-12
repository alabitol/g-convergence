/******************************************************************************
 * Authors: g-coders
 * Created: March 4, 2012
 * Revised: April 11, 2012
 * Description: This file contains functions which format the verification
 * response and functions which send the response back to the client.
 ******************************************************************************/

#ifndef RESPONSE_H
#define RESPONSE_H

#include "notary.h"

/* Obtains a response to a POST/GET request. */
int retrieve_response (void *coninfo_cls, const char* url, const char *fingerprint_from_client);

/* Sends response back to the client. This function could be a wrapper for
 * send_page.
 */
int send_response (struct MHD_Connection *connection, const char *response_data,
               int status_code);


#endif // RESPONSE_H
