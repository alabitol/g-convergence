/******************************************************************************
 * Authors: g-coders
 * Created: March 4, 2012
 * Revised: March 11, 2012
 * Description: Handle sending a certificate request to the website and
 *              receiving its response.
 ******************************************************************************/
#ifndef CONNECTION_H
#define CONNECTION_H

#include "notary.h"

/* Handles the connection of a client. The address of this function needs to
 * be passed to MHD_start_daemon.
 */
int
answer_to_connection (void *cls, struct MHD_Connection *connection,
    const char *url, const char *method,
    const char *version, const char **upload_data,
    size_t *upload_data_size, void **con_cls);

/* Clean up after the request completes. 
 */
void
request_completed (void *cls, struct MHD_Connection *connection,
    void **con_cls, enum MHD_RequestTerminationCode toe);

#endif // CONNECTION_H
