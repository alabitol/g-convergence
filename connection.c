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

#include "notary.h"

/* Keep track of the number of clients with active requests. */
unsigned int number_active_clients = 0;

const char *busy_page = "The server is too busy to handle the verification request.";
const char *unsupported_method_page = "The server received a request with unsupported method.";

/* Handles the connection of a client. The address of this function needs to
 * be passed to MHD_start_daemon.
 *
 */
int
answer_to_connection (void *cls, struct MHD_Connection *connection,
    const char *url, const char *method,
    const char *version, const char *upload_data,
    size_t *upload_data_size, void **con_cls)
{
  /* The first time the function is called, only headers are processed. */
  if (con_cls == NULL)
  {    
    struct connection_info_struct *con_info;

    /* If there are too many clients connected, refuse a new connection. */
    if (number_active_clients >= MAX_CLIENTS)
      return send_response (connection, busy_page, MHD_HTTP_SERVICE_UNAVAILABLE);

    con_info = malloc (sizeof (struct connection_info_struct));

    /* Do we want to return send_response or MHD_NO? */
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
    if (0 != *upload_data_size)
    {
      retrieve_response(con_info, url, upload_data);
      *upload_data_size = 0;

      /* Can we send the response right away instead of return MHD_YES? */
      return MHD_YES;
    }
    else
    {
      /* Retrieve response attributes from con_info to send a response. */
      return send_response (connection, con_info->answer_string,
	  con_info->answer_code);
    }
  }
  else if (strcmp (method, "GET") == 0)
  {
    struct connection_info_struct *con_info = *con_cls;

    /* If the fingerprint is not included in the request, call the method retrieve_response 
       without a fingerprint from the client */
    retrieve_response(con_info, url, NULL);
    
    /* We probably want to call get_response here as well. Hmmm... */
    return send_response (connection, con_info->answer_string, con_info->answer_code);
  }
  else
  {
    /* We received a request with unsupported method, so we return the
     * appropriate error code. 
     */
    return send_response (connection, unsupported_method_page, MHD_HTTP_BAD_REQUEST);
  }
}

/* Clean up after the request completes closing the connection. Taken from the microhttpd tutorial 
   (Example 5).
 */
void
request_completed (void *cls, struct MHD_Connection *connection,
    void **con_cls, enum MHD_RequestTerminationCode toe)
{
  struct connection_info_struct *con_info = *con_cls;
  if (con_info == NULL)
    return;

  if (con_info->connection_type == POST)
    {
      if (con_info->answer_string)
        free ((char*)con_info->answer_string);
    }
  
  free (con_info);
  *con_cls = NULL;
}
