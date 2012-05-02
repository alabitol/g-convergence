/******************************************************************************
 * Authors: Tolu Alabi
 *          Zach Butler
 *          Martin Dluhos
 *
 * Created: March 1, 2012
 * Description: Handle sending a certificate request to the website and
 *              receiving its response.
 ******************************************************************************/
#include "connection.h"
#include "response.h"
#include "certificate.h"
#include "notary.h"

/* Keep track of the number of clients with active requests. */
unsigned int number_active_clients = 0;

/* Response strings for the server to return. */
const char *busy_page = 
  "The server is too busy to handle the verification request.\n";

const char *unsupported_method_page = 
  "The server received a request with an unsupported method.\n";


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Helpers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Extract the host which we need to verify from the requested url. */
static host *
extract_host (char *url, host *host_to_verify)
{
  //set the url to point to the beggining of the actual website address
  url = url + 8;

  /* Extract the host and port from the url. */
  host_to_verify->url = strtok(url, " ");
  host_to_verify->port = atol(strtok(NULL, " "));

  return host_to_verify;
} // extract_host

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Handles the connection of a client. The address of this function needs to
 * be passed to MHD_start_daemon.
 *
 */
int
answer_to_SSL_connection (void *cls, struct MHD_Connection *connection,
    const char *url, const char *method,
    const char *version, const char *upload_data,
    size_t *upload_data_size, void **con_cls)
{
  host *host_to_verify = malloc(sizeof(host)); // website the user wants to verify

  /* Url to be parsed cannot be a constant string. */
  char *requested_url = malloc(sizeof(char) * strlen(url));
  strcpy(requested_url,url);

  /* The first time the function is called, only headers are processed. */
  if (*con_cls == NULL)
  {
    struct connection_info_struct *con_info;

    /* If there are too many clients connected, refuse a new connection. */
    if (number_active_clients >= MAX_CLIENTS)
      return send_response (connection, busy_page, MHD_HTTP_SERVICE_UNAVAILABLE);

    con_info = malloc (sizeof (struct connection_info_struct));

    if (con_info == NULL)
      return MHD_NO;

    /* Process POST and GET request separately. Signal an error on any other
     * method.
     */
    if  (strcmp (method, "POST") == 0)
      con_info->connection_type = POST;
    else if (strcmp (method, "GET") == 0)
      con_info->connection_type = GET;
    
    number_active_clients++;
    *con_cls = (void *) con_info;
    return MHD_YES;
  }

  /* The second time through we want to send a response back to the client. */
  if (strcmp (method, "POST") == 0)
  {
    struct connection_info_struct *con_info = *con_cls;

    /* Keep processing the upload data until there is no data to process. */
    if (*upload_data_size != 0)
    {
      extract_host(requested_url, host_to_verify);
      retrieve_response(con_info, host_to_verify, upload_data);
      *upload_data_size = 0;

      free(host_to_verify);
      free(requested_url);

      return MHD_YES;
    }
    else
      {
        /* Retrieve response attributes from con_info to send a response. */
        return send_response (connection, con_info->answer_string,
                              con_info->answer_code);
      }
  }
  else
    {
      if (strcmp (method, "GET") == 0)
        {
          struct connection_info_struct *con_info = *con_cls;
          
          extract_host(requested_url, host_to_verify);
          /* If the fingerprint is not included in the request, call the method retrieve_response 
             without a fingerprint from the client */
          retrieve_response(con_info, host_to_verify, NULL);
          
          free(host_to_verify);
          free(requested_url);

          /* We probably want to call get_response here as well. Hmmm... */
          return send_response (connection, con_info->answer_string, con_info->answer_code);
        }
      else
        { 
          struct connection_info_struct *con_info = *con_cls;

          /* We received a request with unsupported method, so we return the
           * appropriate error code. 
           */
          con_info->answer_code = MHD_HTTP_BAD_REQUEST;
          return send_response (connection, unsupported_method_page, con_info->answer_code);
        }
    }
}

/* Clean up after the request completes closing the connection. Taken from the microhttpd tutorial 
   (Example 5).
 */

int answer_to_HTTP_connection(void *cls, struct MHD_Connection *connection,
    const char *url, const char *method,
    const char *version, const char *upload_data,
    size_t *upload_data_size, void **con_cls)
{ return 0; }

int answer_to_4242_connection(void *cls, struct MHD_Connection *connection,
    const char *url, const char *method,
    const char *version, const char *upload_data,
    size_t *upload_data_size, void **con_cls)
{ return 0; }

void
request_completed (void *cls, struct MHD_Connection *connection,
    void **con_cls, enum MHD_RequestTerminationCode toe)
{
  struct connection_info_struct *con_info = *con_cls;
  if (con_info == NULL)
    return;

  if ( (con_info->connection_type == POST) || (con_info->connection_type == GET))
    {
      if (con_info->answer_string)
        {
          free ((char*)con_info->answer_string);
        }
    }

  //free memory and set previously used pointers to NULL
  con_info->answer_string = NULL;
  free (*con_cls);
  *con_cls = NULL;
  con_info = NULL;
}

