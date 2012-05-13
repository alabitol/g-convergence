/**
 * @file
 * @author Tolu Alabi
 *         Zach Butler
 *         Martin Dluhos
 *
 * @date Created: March 1, 2012
 *       Modified: May 6th, 2012
 *
 * @section DESCRIPTION
 * Handle sending a certificate request to the website and
 * receiving its response.
 */

#include "connection.h"
#include "response.h"
#include "certificate.h"
#include "notary.h"

#define MAX_HOST_LEN 10

/* Keep track of the number of clients with active requests. */
unsigned int number_active_clients = 0;

/* Response strings for the server to return. */
const char busy_page[] = 
  "The server is too busy to handle the verification request.\n";

const char unsupported_method_page[] = 
  "The server received a request with an unsupported method.\n";


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Helpers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
 * @brief Extract the actual url and the host from the input string
 * @param url string of the form "/target/https://www..."
 * @param host_to_verify A struct which serves as an output parameter 
 *        containing the url and the host to connect to
 */
static void
extract_host (char *url, host *host_to_verify)
{
  char *actual_url = malloc(sizeof(char) * (strlen(url) + 1) );
  char host[MAX_HOST_LEN];

  //Get the url and the host from the input string
  sscanf(url, "/target/%99[^ ] %99[^\n]", actual_url, host);

  //set the fields in host_to_verify
  host_to_verify->url = malloc(sizeof(char) * (strlen(actual_url) + 1) );
  strcpy(host_to_verify->url, actual_url);

  host_to_verify->port = atol(host);

  //free used memory
  free(actual_url);

}// extract_host

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
 * @brief Handles the connection of a client. The address of this function needs to
 *        be passed to MHD_start_daemon.
 *
 * @param cls 
 * @param connection
 * @param url The url with which we are trying to connect
 * @param method
 * @param version
 * @param upload_data 
 * @param upload_data_size
 * @param con_cls
 *
 * @return Returns MHD_YES if the a connection was made with the url, 
 *         and MHD_NO if no connection could be made.
 */
int
answer_to_SSL_connection (void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
                          const char *version, const char *upload_data,
                          size_t *upload_data_size, void **con_cls)
{
  host *host_to_verify = malloc(sizeof(host)); // website the user wants to verify

  /* Url to be parsed cannot be a constant string. */
  char *requested_url = malloc(sizeof(char) * ( strlen(url) + 1) );
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

    /* Process POST and GET request separately. 
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

      if(verify_fingerprint_format(upload_data) == 1)
        retrieve_response(con_info, host_to_verify, upload_data);
      else
        {
          fprintf(stderr, "Incorrect fingerprint format\n");
          exit(1);
        }

      *upload_data_size = 0;

      free(host_to_verify->url);
      free(host_to_verify);
      free(requested_url);

      return MHD_YES;
    }
    else
      {
        /* Send response of the POST request to the client */
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

          free(host_to_verify->url);
          free(host_to_verify);
          free(requested_url);

          /* We send the response of the GET request to the client*/
          return send_response (connection, con_info->answer_string, con_info->answer_code);
        }
      else
        { 
          struct connection_info_struct *con_info = *con_cls;

          //free used memory
          free(host_to_verify);
          free(requested_url);

          /* We received a request with unsupported method, so we return the
           * appropriate error code. 
           */
          con_info->answer_code = MHD_HTTP_BAD_REQUEST;
          return send_response (connection, unsupported_method_page, con_info->answer_code);
        }
    }
}//answer_to_SSL_connection

/* Clean up after the request completes closing the connection. Taken from the microhttpd tutorial 
   (Example 5).
 */

/**
 * @brief 
 *
 * @param cls
 * @param connection
 * @param url
 * @param method
 * @param version
 * @param upload_data
 * @param upload_data_size
 * @param con_cls
 *
 * @return Returns 0.
 */
int answer_to_HTTP_connection(void *cls, struct MHD_Connection *connection,
    const char *url, const char *method,
    const char *version, const char *upload_data,
    size_t *upload_data_size, void **con_cls)
{ return 0; }

/**
 * @brief 
 *
 * @param cls
 * @param connection
 * @param url
 * @param method
 * @param version
 * @param upload_data
 * @param upload_data_size
 * @param con_cls
 *
 * @return Returns 0.
 */
int answer_to_4242_connection(void *cls, struct MHD_Connection *connection,
    const char *url, const char *method,
    const char *version, const char *upload_data,
    size_t *upload_data_size, void **con_cls)
{ return 0; }

/**
 * @brief This function is called by MHD_start_daemon when a request completes to
 *        ensure that memory for datastructures we create to store information 
 *        about the connection are properly freed at the end.
 * 
 * @param cls
 * @param connection
 * @param con_cls
 * @param toe
 */
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
} // request_completed

