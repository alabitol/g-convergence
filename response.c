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

#include "response.h"
#include "certificate.h"

//WE ARE NOT SURE ABOUT THIS
#define RESPONSE_LEN 201
const char* page = "Your request was successful!";

/* Figure out RESPONSE_LEN and define it here. */
/* SEEMS TO BE ABOUT 201 CHARACTERS LONG */

/* Obtains a response to a POST/GET request.
 */
int 
retrieve_response (void *coninfo_cls, const char* url, const char *fingerprint_from_client)
{
  int verified, requested; // was certificate verified?
  char *fingerprint_from_website;
  char *json_fingerprint_list; // the response to send to client

  struct connection_info_struct *con_info = coninfo_cls;

  char* requested_fingerprint = request_certificate(url);
  fingerprint_from_website = (char*)malloc(sizeof(char) * strlen(requested_fingerprint));

  strcpy(fingerprint_from_website, requested_fingerprint);
  
  if (requested == 0)
    {
      /* The notary could not obtain the certificate from the website
       * for some reason
       */
      con_info->answer_code = MHD_HTTP_SERVICE_UNAVAILABLE; //503
      return MHD_NO;
    }

  if(fingerprint_from_client != NULL)
    {
      verified =
        verify_certificate (fingerprint_from_client, fingerprint_from_website);

      if (verified == 1)
        con_info->answer_code = MHD_HTTP_OK; // 200
      else
        con_info->answer_code = MHD_HTTP_CONFLICT;  // 409
    }

  /* Format the response which will be sent to client. 
   * Note that this response is sent both on a successful verification
   * and on a failed verification.
   * The JSON format of the response is available at
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
      }\n");

  con_info->answer_string = json_fingerprint_list;

  return MHD_YES;
}


/* Sends response back to the client. This function could be a wrapper for
 * send_page.
 */
int
send_response (struct MHD_Connection *connection, const char *response_data,
               int status_code)
{
  int return_value;
  struct MHD_Response *response;

  response = MHD_create_response_from_data (strlen(page), (void*)page,
                                            MHD_NO, MHD_NO);
  if(response == NULL)
    return MHD_NO;

  return_value = MHD_queue_response (connection, status_code, response);
  MHD_destroy_response (response);

  return return_value;
}


