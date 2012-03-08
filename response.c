/******************************************************************************
 * Authors: Tolu Alabi
 *          Zach Butler
 *          Martin Dluhos
 *
 * Created: February 21, 2012
 * Revised: March 7, 2012
 * Description: This file contains functions which format the verification
 * response and functions which send the response back to the client.
 ******************************************************************************/

/* Figure out RESPONSE_LEN and define it here. */

/* Obtains a response to a POST request. This function is defined in
 * libmicrohttpd with typedef, so it needs to be declared with all of these
 * parameters even if they are not needed.
 */

int 
retrieve_post_response (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
    const char *filename, const char *content_type,
    const char *transfer_encoding, const char *data, uint64_t off,
    size_t size)
{
  int verified, requested; // was certificate verified?
  char *fingerprint_from_client, fingerprint_from_website;
  char *json_fingerprint_list; // the response to send to client

  struct connection_info_struct *con_info = coninfo_cls;
  
  requested = request_certificate (url, fingerprint_from_website);
  
  if (requested == 0)
  {
    /* The notary could not obtain the certificate from the website. */
  }

  /* The fingerprint from client should be in data (which should refer to
     POST data).
     NOTE: Need to figure out how to retrieve the fingerprint_from_data.
  */
  fingeprint_from_client = data;
  
  verified =
    verify_certificate (fingerprint_from_client, fingerprint_from_website);

  if (verified == 1)
    con_info->answercode = MHD_HTTP_OK; // 200
  else
    con_info->answercode = MHD_HTTP_CONFLICT;  // 409

  /* Format the response which will be sent to client. The JSON format
   * of the response is available at
   * https://github.com/moxie0/Convergence/wiki/Notary-Protocol
   */
  json_fingerprint_list = malloc (RESPONSE_LEN * sizeof (char));
  sprintf (json_fingerprint_list, 
      "{\n \
      \t\"fingerprintList\":\n \
      \t[\n \
      \t  {\n \
      \t     \"timestamp\": {\"start\": \"%s\", \"finish\": \"%s\"},\n \
      \t     \"fingerprint\": \"%s\"\n \
      \t  }\n \
      \t],\n \
      \t\"signature\": \"%s\"\n \
      }\n",);

con_info->answer_string = json_fingerprint_list;


return 1;
}

/* Obtains a response to a GET request. */ 
  int
retrieve_get_response (void *coninfo_cls, )
{
  request_certificate (url, fingerprint_from_website);

}
/* Sends response back to the client. This function could be a wrapper for
 * send_page.
 */
int
send_response ()
{
  return 1;
}


