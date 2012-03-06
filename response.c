/******************************************************************************
 * Authors: Tolu Alabi
 *          Zach Butler
 *          Martin Dluhos
 *
 * Created: February 21, 2012
 * Revised: March 1, 2012
 * Description: This file contains functions which format the verification
 * response and functions which send the response back to the client.
 ******************************************************************************/

/* Gets a response that will be sent to the client. */
/* NOTE: ARE ALL PARAMS REQUIRED? DO WE NEED TO PASS FILENAME??? */
int 
get_response (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
    const char *filename, const char *content_type,
    const char *transfer_encoding, const char *data, uint64_t off,
    size_t size)
{
  request_certificate ();

  verify_certificate ();

  /* Format the response which will be sent to client. This involves setting
   * the answercode and answerstring */

  return 1;
}

/* Sends response back to the client. This function could be a wrapper for
 * send_page.
 */
int
send_response ()
{
  return 1;
}


