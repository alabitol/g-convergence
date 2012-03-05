/******************************************************************************
 * Authors: Tolu Alabi
 *          Zach Butler
 *          Martin Dluhos
 *
 * Created: March 4, 2012
 * Revised: March 4, 2012
 * Description: Handle sending a certificate request to the website and
 *              receiving its response.
 ******************************************************************************/

/* Handles the connection of a client. The address of this function needs to
 * be passed to MHD_start_daemon.
 *
 * NOTE: this was initially static- make sure this works
 */
int
answer_to_connection (void *cls, struct MHD_Connection *connection,
    const char *url, const char *method,
    const char *version, const char *upload_data,
    size_t *upload_data_size, void **con_cls);

/* Clean up after the request completes. Should this also close the
 * connection???
 */
void
request_completed (void *cls, struct MHD_Connection *connection,
    void **con_cls, enum MHD_RequestTerminationCode toe);
