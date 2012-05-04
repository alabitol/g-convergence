/*****************************************************************************
 * Authors: g-coders
 * Created: March 10, 2012
 * Revised: April 12, 2012
 * Description: This file includes unit tests for all functions which are part
 * of the convergence system.
 ******************************************************************************/

#include <malloc.h>
#include <stdio.h>
#include "notary.h"
#include "connection.h"
#include "certificate.h"
#include "response.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Globals
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define MAX_NO_OF_CERTS 7
#define NUM_OF_CONNECTIONS 10
int __tests = 0;
int __fails = 0;

/* Identify different connection types. */
#define GET 0
#define POST 1
#define CUSTOM 2

/* A macro that defines an enhanced assert statement. */
#define test(exp) do { ++__tests; if (! (exp)) { ++__fails; fprintf (stderr, "Test failed: %s at line: %d\n", #exp, __LINE__); } } while (0)

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Helpers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
 * Get information about the amount of dynamically allocated address space
 * in bytes.
 */
static int
mem_allocated ()
{
  struct mallinfo info = mallinfo ();
  return info.uordblks;
} // mem_allocated


/**
 * Write Function: tell the curl functions where to write the data they
 * receive from a network
 *
 * @param ptr a pointer to the location where the data will be stored
 * @param size,
 * @param nmemb, the size of the data written
 * @param stream
 * @return 
 */
static size_t write_function (void *ptr,  size_t  size,  size_t  nmemb,  void *stream)
{
  (void) stream;
  (void) ptr;
  return size * nmemb;
}

/**
 * Combines curl cleanup calls. 
 */
static void curl_cleanup(CURL* curl_handle)
{
  curl_easy_cleanup(curl_handle);
  curl_global_cleanup();
}

/**
 *Helper to create a post request
 */
static CURL* create_post_request(char* url, char* fingerprint)
{
  CURL* curl_handle;
  int ssl_port = 7000;

  /* Initialize curl. */
  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init();

  char *host = malloc(strlen(url) * sizeof(char));
  strcpy(host, url);

  curl_easy_setopt(curl_handle, CURLOPT_URL, host);
  curl_easy_setopt(curl_handle, CURLOPT_PORT, ssl_port);
  curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);
  curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, fingerprint);
  curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1L);
      
  return curl_handle;
}

/**
 * Helper to create a GET request
 */
static CURL* create_get_request(char* url)
{
  CURL* curl_handle;
  int ssl_port = 7000;
  
  /* Initialize curl. */
  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init();

  char *host = malloc(strlen(url) * sizeof(char));
  strcpy(host, url);

  curl_easy_setopt(curl_handle, CURLOPT_URL, host);
  curl_easy_setopt(curl_handle, CURLOPT_PORT, ssl_port);
  curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1L);

  return curl_handle;
}

/**
 * Helper to create a CUSTOM request
 */
static CURL* create_custom_request(char* url)
{
  CURL* curl_handle;
  int ssl_port = 7000;
  
  /* Initialize curl. */
  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init();

  char *host = malloc(strlen(url) * sizeof(char));
  strcpy(host, url);

  curl_easy_setopt(curl_handle, CURLOPT_URL, host);
  curl_easy_setopt(curl_handle, CURLOPT_PORT, ssl_port);
  curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "CUSTOM");
  curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1L);
      
  return curl_handle;
}

/* Function that sends HTTP requests to a running MHD_Daemon. */
void
send_curl_requests()
{
  int connection_type[NUM_OF_CONNECTIONS] = {GET, POST, CUSTOM, POST, GET, CUSTOM, POST, POST, GET, GET};

  /* Send requests to the daemon with curl. */
  CURL* curl;
  CURLcode res;
  int i;

  for(i=0; i<NUM_OF_CONNECTIONS; i++)
    {
      if(connection_type[i] == POST)
        curl = create_post_request("http://localhost/target/https://www.wikipedia.org+443",
                                   "03:47:7F:F5:F6:3B:F5:B6:10:C0:7D:65:9A:7B:A9:12:D3:20:83:68");
      else
        {
          if(connection_type[i] == GET)
            {
              curl = create_get_request("http://localhost/target/https://www.wikipedia.org+443");
            }
          else
            curl = create_custom_request("http://localhost/target/https://www.wikipedia.org+443");
        }
      
      /* Set curl options. */
      if (curl)
        {
          /* Make the curl request. */
          res = curl_easy_perform(curl);
          
          if (!res)
            {
              /* Verify if the response was received by the client. */
              curl_cleanup(curl);
            }
          else
            {
              /* Curl did not succeed in sending the request. */
              fprintf(stderr, "here Could not send request to server\n");
              curl_cleanup(curl);
            } // if (!res)
        }
      else
        {
          fprintf(stderr, "Could not initialize CURL\n");
          curl_cleanup(curl);
        } // if (curl)
    }//for
} // send_curl_requests

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Unit tests
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/** 
 * Test the entire system. 
 */
void
test_convergence ()
{
  /* STUB */

  /* Start the daemon. */

  /* Create a curl request. */

  /* Set the appropriate request parameters. */

  /* Send the request. */
  
  /* Check the return value and record test results. */

} // test_convergence

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Tests for functions in connection.c.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int
test_answer_to_connection_helper (void *cls, struct MHD_Connection *connection,
                                  const char *url, const char *method,
                                  const char *version, const char *upload_data,
                                  size_t *upload_data_size, void **con_cls)
{
  int ret_val;

  /* The first time through only headers are processed. Only the second time
   * this function is called can we access upload_data.
   */
  if (*con_cls == NULL)
    {
      struct connection_info_struct *con_info;
      con_info = malloc (sizeof (struct connection_info_struct));

      /* Process POST and GET request separately. Signal an error
       * on any other method.
       */
      if  (strcmp (method, "POST") == 0)
        {
          con_info->connection_type = POST;
        }
      else if (strcmp (method, "GET") == 0)
        {
          *upload_data_size = 0;
          con_info->connection_type = GET;
        }
      *con_cls = (void *) con_info;
      return MHD_YES;
    } // if

  if(*upload_data_size != 0)
    {
      *con_cls = NULL;
      
      //first time through answer_to_ssl_connection
      ret_val = answer_to_SSL_connection(cls, connection, url, method, version, upload_data, upload_data_size, con_cls);
      test(ret_val == MHD_YES);
      
      ret_val = answer_to_SSL_connection(cls, connection, url, method, version, upload_data, upload_data_size, con_cls);
      test(ret_val == MHD_YES);
      
      *upload_data_size = 0;
      return MHD_YES;
    }
  else
    {
      ret_val = answer_to_SSL_connection(cls, connection, url, method, version, upload_data, upload_data_size, con_cls);
      test(ret_val == MHD_YES);

      struct connection_info_struct *con_info = *con_cls;
      if(strcmp (method, "CUSTOM") == 0)
        {
          test(con_info->answer_code == MHD_HTTP_BAD_REQUEST);
        }
      else
        {
          test(con_info->answer_code == MHD_HTTP_OK);
        }

      return MHD_YES;
    }
  
  return MHD_YES;
}

void 
test_answer_to_connection ()
{
  int ssl_port = 7000;
  struct MHD_Daemon *ssl_daemon;

  /* To test answer_to_connection, we need MHD_Connection, which is one of its
   * arguments. To obtain MHD_Connection, we start the MHD_Daemon, make a
   * request to it using curl, and instruct the daemon to call
   answer_to_connection for testing
  */
  ssl_daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, 
                                 ssl_port, 
                                 NULL, 
                                 NULL, 
                                 &test_answer_to_connection_helper,
                                 NULL, 
                                 MHD_OPTION_END
                                 );

  if (ssl_daemon == NULL)
    {
      fprintf (stderr, "Error: Failed to start the MHD SSL daemon.\n");
    }
  else // for debugging
    {
      printf("The daemon is up and running!\n");
    }
  
  send_curl_requests();

  MHD_stop_daemon(ssl_daemon);
} // test_answer_to_connection


int
test_request_completed_helper(void *cls, struct MHD_Connection *connection,
                              const char *url, const char *method,
                              const char *version, const char *upload_data,
                              size_t *upload_data_size, void **con_cls)
{
  int ret_val;

  /* The first time through only headers are processed. Only the second time
   * this function is called can we access upload_data.
   */
  if (*con_cls == NULL)
    {
      struct connection_info_struct *con_info;
      con_info = malloc (sizeof (struct connection_info_struct));

      /* Process POST and GET request separately. Signal an error
       * on any other method.
       */
      if  (strcmp (method, "POST") == 0)
        {
          con_info->connection_type = POST;
        }
      else if (strcmp (method, "GET") == 0)
        {
          *upload_data_size = 0;
          con_info->connection_type = GET;
        }
      *con_cls = (void *) con_info;
      return MHD_YES;
    } // if

  if(*upload_data_size != 0)
    {
      *upload_data_size = 0;

      return MHD_YES;
    }
  else
    {
      const char *json_fingerprint_list = 
        "{\n \
    \t\"fingerprintList\":\n \
\t[\n \
\t {\n \
\t \"timestamp\": {\"start\": \"1292636531\", \"finish\": \"1292754629\"},\n \
\t \"fingerprint\": \"BF:E1:FE:03:10:E9:CB:DC:96:BF:3D:AA:6E:C6:03:E5:31:CD:A9:9C\"\n \
\t }\n \
\t],\n \
}\n";
      struct connection_info_struct *con_info = *con_cls;
      
      con_info->answer_string = malloc(sizeof(char) * strlen(json_fingerprint_list) + 1);
      strcpy((char*)con_info->answer_string, json_fingerprint_list);
      con_info->answer_code = MHD_HTTP_OK;

      ret_val = send_response(connection, json_fingerprint_list, con_info->answer_code);
      
      request_completed(cls, connection, con_cls, con_info->answer_code);

      //check if request_completed did its job
      test(*con_cls == NULL);
      test(con_info->answer_string == NULL);

      return MHD_YES;
    }

  return MHD_YES;
}


void 
test_request_completed ()
{ 

  int ssl_port = 7000;
  struct MHD_Daemon *ssl_daemon;

  /* To test answer_to_connection, we need MHD_Connection, which is one of its
   * arguments. To obtain MHD_Connection, we start the MHD_Daemon, make a
   * request to it using curl, and instruct the daemon to call
   answer_to_connection for testing
  */
  ssl_daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION,
                                 ssl_port,
                                 NULL,
                                 NULL,
                                 &test_request_completed_helper,
                                 NULL,
                                 MHD_OPTION_END
                                 );

  if (ssl_daemon == NULL)
    {
      fprintf (stderr, "Error: Failed to start the MHD SSL daemon.\n");
    }
  else // for debugging
    {
      printf("The daemon is up and running!\n");
    }
  
  send_curl_requests();

  MHD_stop_daemon(ssl_daemon);

} // test_request_completed

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Tests for functions in response.c.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void 
test_retrieve_response ()
{
  /* Construct arguments to pass to retrieve_response. */
  FILE *valid_urls;
  FILE *invalid_urls;
  FILE *invalid_fingerprints;
  int size = 250;
  char input_line[size];
  char *url;
  char *fingerprint;
  int index_of_last_char;

  int result = 0;
  host *host_to_verify = malloc (sizeof(host));

  //Set the port
  host_to_verify->port = 443;

  valid_urls = fopen("valid_urls.txt", "r");  
  if (valid_urls == NULL)
    {
      fprintf(stderr, "Could not open valid_urls.txt\n");
      exit(1);
    }

  invalid_urls = fopen("invalid_urls.txt", "r");  
  if (invalid_urls == NULL)
    {
      fprintf(stderr, "Could not open invalid_urls.txt\n");
      exit(1);
    }

  invalid_fingerprints = fopen("invalid_fingerprints.txt", "r");  
  if (invalid_fingerprints == NULL)
    {
      fprintf(stderr, "Could not open invalid_fingerprints.txt\n");
      exit(1);
    }
  
  /* Construct coninfo_cls. */
  struct connection_info_struct *coninfo_cls;
  coninfo_cls = malloc (sizeof (struct connection_info_struct));

  /* Get url and corresponding fingerprint from valid_urls.txt. */
  /* If fingerprint_from_client matches the fingerprint from website, expect MHD_YES as the return value and answer code as MHD_HTTP_OK */
  while (fgets (input_line, size, valid_urls) != NULL)
    {
      //First get rid of the newline character at the end of each line in the file
      index_of_last_char = strlen(input_line) - 1;
      if( input_line[index_of_last_char] == '\n')
        input_line[index_of_last_char] = '\0';

      url = strtok(input_line, " ");
      fingerprint = strtok(NULL, " ");

      host_to_verify->url = url;
      
      /* Call retrieve_response and check its return value. */
      result = retrieve_response (coninfo_cls, host_to_verify, fingerprint);
    
      test(result == MHD_YES);
      test(coninfo_cls->answer_code == MHD_HTTP_OK);
    }//while

  /* If URL is not valid, then expect MHD_NO. */
  while (fgets (input_line, size, invalid_urls) != NULL)
    {
      //First get rid of the newline character at the end of each line in the file
      index_of_last_char = strlen(input_line) - 1;
      if( input_line[index_of_last_char] == '\n')
        input_line[index_of_last_char] = '\0';

      url = strtok(input_line, " ");
      fingerprint = strtok(NULL, " ");
      
      host_to_verify->url = url;
 
      /* Call retrieve_response and check its return value. */
      result = retrieve_response(coninfo_cls, host_to_verify, fingerprint);

      /* Check if result is MHD_NO */
      test(result == MHD_NO);
    }
      
  /* If fingerprint_from_client does not match any fingerprint from website, expect MHD_YES as the return value and answer_code as MHD_HTTP_CONFLICT. */
  while (fgets (input_line, size, invalid_fingerprints) != NULL)
    {
      //First get rid of the newline character at the end of each line in the file
      index_of_last_char = strlen(input_line) - 1;
      if( input_line[index_of_last_char] == '\n')
        input_line[index_of_last_char] = '\0';

      url = strtok(input_line, " ");
      fingerprint = strtok(NULL, " ");
      
      host_to_verify->url = url;
 
      /* Call retrieve_response and check its return value. */
      result = retrieve_response(coninfo_cls, host_to_verify, fingerprint);

      test(result == MHD_YES);
      test(coninfo_cls->answer_code == MHD_HTTP_CONFLICT);
    }

  /* If the URL is valid but no certificates are returned from the website, expect MHD_NO and answer code is MHD_HTTP_SERVICE_UNAVAILABLE. This is not possible to test directly given our implementation of retrieve_response. */

  //Close valid_urls.txt
  fclose(valid_urls);
  fclose(invalid_urls);
  fclose(invalid_fingerprints);
} // test_retrieve_post_response

void
test_verify_certificate ()
{
  char *c1 = "BF:E1:FE:03:10:E9:CB:DC:96:BF:3D:AA:6E:C6:03:E5:31:CD:A9:9C";
  char *c2 = "E2:9E:46:29:A0:FD:3C:57:A0:68:30:C5:0A:45:97:63:BF:8D:75:FC";
  char *c3 = "BF:E1:FE:03:10:E9:CB:DC:96:BF:3D:AA:6E:C6:03:E5:31:CD:A9:9C";
  
  int result = -1;
  
  char *c5 = "EA:9D:EF:D6:33:61:D9:76:71:E1:6C:68:9F:54:A6:59:D7:F1:0E:66";
  char *c6 = "68:C8:18:57:3D:39:55:B6:60:34:14:54:70:32:CB:41:A0:9D:0B:06";
  char *c7 = "45:24:40:53:4F:B4:7C:A6:C6:09:F4:B3:FA:DE:6A:DD:21:56:35:ED";
  char *c8 = "C2:2A:30:8D:49:DE:49:32:3B:F1:AF:D9:F8:41:79:E6:A8:ED:65:A6";
  char *c9 = "03:47:7F:F5:F6:3B:F5:B6:10:C0:7D:65:9A:7B:A9:12:D3:20:83:68";

  char *c10[1] = {c3};
  char *c11[1] = {c2};
  char *no_match[5] = {c5, c6, c7, c8, c9};
  char *last_match[6] = {c5, c6, c7, c8, c9, c1};
  char *second_match[4] = {c5, c1, c6, c7};
  char *first_match[4] = {c1, c6, c7, c9};

  //If there is only one certificate from the website:
  //If the certificate matches
  result = verify_certificate(c1, c10, 1);
  test(result == 1);
  //If the certificate doesn't match
  result = verify_certificate(c1, c11, 1);
  test(result == 0);

  //If there are multiple certificates from the website:
  //If the first certificate matches
  result = verify_certificate(c3, first_match, 4);
  test (result == 1);
  //If the second certificate matches
  result = verify_certificate(c3, second_match, 4);
  test (result == 1);
  //If the last certificate matches
  result = verify_certificate(c1, last_match, 6);
  test (result == 1);
  //If none of the certificates match
  result = verify_certificate(c1, no_match, 5);
  test (result == 0);

  //Additional tests: lower case fingerprints 
 
} // test_verify_certificate


int
test_send_response_helper (void *cls, struct MHD_Connection *connection,
    const char *url, const char *method,
    const char *version, const char *upload_data,
    size_t *upload_data_size, void **con_cls) 
{
  int ret_val;
  char request_num;
    
  /* The first time through only headers are processed. Only the second time
   * this function is called can we access upload_data.
   */
  if (*con_cls == NULL)
    {
      struct connection_info_struct *con_info;
      con_info = malloc (sizeof (struct connection_info_struct));

      /* Process POST and GET request separately. Signal an error
       * on any other method.
       */
      if  (strcmp (method, "POST") == 0)
        con_info->connection_type = POST;
      else if (strcmp (method, "GET") == 0)
        con_info->connection_type = GET;

      *con_cls = (void *) con_info;
      return MHD_YES;
    } // if
  
  /* This gets executed the second time the function is called. Only now can
   * we access upload_data.
   */
  
  /* Response strings for the server to return. */
  const char *busy_page = 
    "The server is too busy to handle the verification request.\n";

  const char *unsupported_method_page = 
  "The server received a request with unsupported method.\n";

  const char *json_fingerprint_list = 
    "{\n \
    \t\"fingerprintList\":\n \
\t[\n \
\t {\n \
\t \"timestamp\": {\"start\": \"1292636531\", \"finish\": \"1292754629\"},\n \
\t \"fingerprint\": \"BF:E1:FE:03:10:E9:CB:DC:96:BF:3D:AA:6E:C6:03:E5:31:CD:A9:9C\"\n \
\t }\n \
\t],\n \
\t\"signature\": \"MHu+jKdTJhRiNqTZleFnTYhKF/l0F4Nch8il/mG4GNoQ911VgLSAv1WICfoZ8E9+GFeNWZJMX6B7dMOVeG/mYbPdm7jH4XvJoIPFT+OoihcS0XjonUXEhYslVbmVviAiIkhESVcuuE1QZwXYHaDY21WBo4UMHBu6bVotx6Y3vgyBeA++5+yKXqVe7Tc+d/1GRcPqMCgYTpFxOTzlRSqceHUNiGx1X1HjUAGb7DqayR75cGtcIjZ9ONc0UBwJq8SYzF/j0DqB7HX4AdRyFyT8qN8ONFh9Vp04plwhuiXTmwiIM3dmDt+lD16D1RDYcPWuWBawQcReP3N82y54ZcEl3g==\"\n \
}\n";

  /* Upload size needs to be set to 0 before calling send response. */
  if(*upload_data_size != 0)
    {
      *upload_data_size = 0;
      return MHD_YES;
    }
  else
    {
      /* This is the third time this function is called. */

      /* Get the test number which determines the appropriate response. */
      request_num = url[33];
      switch (atoi(&request_num))
        {
          // Potential scenarios
        case 0:
          /* The server is too busy to respond. */
          ret_val = send_response(connection, busy_page, MHD_HTTP_SERVICE_UNAVAILABLE);
          test (ret_val == MHD_YES);
          break;
        case 1:
          /* The request from the client was not POST or GET. */
          ret_val = 
            send_response(connection, unsupported_method_page, MHD_HTTP_BAD_REQUEST);
          test (ret_val == MHD_YES);
          break;  
        case 2:
          /* Request suceeded and fingerprint was not verified: answer_code 409. */
          ret_val =
            send_response(connection, json_fingerprint_list, MHD_HTTP_CONFLICT);
          test (ret_val == MHD_YES);
          break;
        case 3:
          /* Request succeeded and fingerprint was verified: answer_code 200. */
          ret_val =
            send_response(connection, json_fingerprint_list, MHD_HTTP_OK);
          test (ret_val == MHD_YES);
          break;
        }
    }
  return MHD_YES;
} // test_send_response_helper

void 
test_send_response ()
{
  long int ssl_port = 7000;
  int i, num_tests = 4;
  struct MHD_Daemon *ssl_daemon;
  
  /* To test send_response, we need MHD_Connection, which is one of its
   * arguments. To obtain MHD_Connection, we start the MHD_Daemon, make a
   * request to it using curl, instruct the deamon to call a
   * test_send_response_helper function, which will perform the actual
   * testing.
   */
  ssl_daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, 
                                 ssl_port, 
                                 NULL, 
                                 NULL, 
                                 &test_send_response_helper,
                                 NULL, // we do not need to call request_completed
                                 MHD_OPTION_END
                             );

  if (ssl_daemon == NULL)
    {
      fprintf (stderr, "Error: Failed to start the MHD SSL daemon.\n");
    }
  else // for debugging
    {
      printf("The daemon is up and running!\n");
    }
  
  /* Send requests to the daemon with curl. */
  CURL *curl;
  CURLcode res;

  /* Send multiple requests. */
  for (i = 0; i < num_tests; i++)
    {
      /* We want to send a POST request to the daemon. */
      char *host = malloc(56 * sizeof(char));
      sprintf(host, "http://localhost/target/https://www.wikipedia.org%d+443", i);
      char* finger =
        "03:47:7F:F5:F6:3B:F5:B6:10:C0:7D:65:9A:7B:A9:12:D3:20:83:68";

      curl = create_post_request(host, finger);
      free(host);

      /* Set curl options. */
      if (curl)
        {
          /* Make the curl request. */
          res = curl_easy_perform(curl);
          if (!res)
            {
              /* Verify if the response was received by the client. */
              curl_cleanup(curl);
            }
          else
            {
              /* Curl did not succeed in sending the request. */
              fprintf(stderr, "Could not send request to server\n");
              curl_cleanup(curl);
            } // if (!res)
        }
      else
        {
          fprintf(stderr, "Could not initialize CURL\n");
          curl_cleanup(curl);
        } // if (curl)
    } // for
  
  MHD_stop_daemon(ssl_daemon);
  
} // test_send_response

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Tests for functions in certificate.c.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void 
test_request_certificate ()
{
  FILE *valids, *invalids;
  int max_len = 256;
  char *string_read = malloc (sizeof(char) * max_len);
  char *url;
  char* correct_fingerprint;
  char* retrieved_fingerprints[MAX_NO_OF_CERTS];
  int index_of_last_char, number_of_certs;
  host *host_to_verify = malloc (sizeof(host));

  //Set the port to connect to  
  host_to_verify->port = 443;

  valids = fopen ("valid_urls.txt", "r");
  if (valids == NULL)
  {
      fprintf (stderr, "Could not open valid_urls.txt\n");
      exit(1);
    }

  invalids = fopen ("invalid_fingerprints.txt", "r");
  if (invalids == NULL)
    {
      fprintf (stderr, "Could not open invalid_fingerprints.txt\n");
      exit(1);
    }

  //allocate space for the fingerprints
  int i,j;
  for(i=0; i<MAX_NO_OF_CERTS; i++)
    retrieved_fingerprints[i] = calloc(FPT_LENGTH * sizeof(char), 1);

  /* Read valid urls from a file and retrieve a certificate from each. */
  while (fgets (string_read, max_len, valids) != NULL)
    {
      //First get rid of the newline character at the end of each line in the file
      index_of_last_char = strlen(string_read) - 1;
      if( string_read[index_of_last_char] == '\n')
        string_read[index_of_last_char] = '\0';

      //get the url from the string gotten from the file and construct host
      url = strtok(string_read, "' '");
      host_to_verify->url = url;

      //then get the fingerprint
      correct_fingerprint = strtok(NULL, "' '");
      printf("Its: %s\n", correct_fingerprint);

      //Get the fingerprint by calling request_certificate
      number_of_certs = request_certificate(host_to_verify, retrieved_fingerprints);
      printf("%d certs found\n", number_of_certs);

      for(i=0; i<number_of_certs; i++)
        printf("%s\n", retrieved_fingerprints[i]);

      test (verify_certificate(correct_fingerprint, retrieved_fingerprints, number_of_certs) == 1);

      //reset all the characters in each string to null to allow for the
      //next iteration of fingerprint conversion
      for(i=0; i<MAX_NO_OF_CERTS; i++)
	retrieved_fingerprints[i][0] = '\0';
    }

  /* Read invalid urls from a file and request a certificate from each. */
  while (fgets (string_read, max_len, invalids) != NULL)
    {
      //First get rid of the newline character at the end of each line in the file
      index_of_last_char = strlen(string_read) - 1;
      if( string_read[index_of_last_char] == '\n')
        string_read[index_of_last_char] = '\0';

      //get the url from the string gotten from the file
      url = strtok(string_read, "' '");
      host_to_verify->url = url;

      //then get the fingerprint
      correct_fingerprint = strtok(NULL, "' '");

      //Get the fingerprint by calling request_certificate
      number_of_certs = request_certificate(host_to_verify, retrieved_fingerprints);

      //Check that fingerprints do not match
      test (verify_certificate(correct_fingerprint, retrieved_fingerprints, number_of_certs) == 0);

      //reset all the characters in each string to null to allow for the
      //next iteration of fingerprint convers
      for(i=0; i<MAX_NO_OF_CERTS; i++)
	retrieved_fingerprints[i][0] = '\0';
    }

  fclose (valids);
  fclose (invalids);
  free(host_to_verify);

  //free memory used for fingerprints
  for(i=0; i<MAX_NO_OF_CERTS; i++)
    free(retrieved_fingerprints[i]);
} // test_request_certificate


void
test_verify_fingerprint_format ()
{
  int i, fpt_length = 59;
  char temp, temp1;
  
  typedef struct char_at_pos
  {
    int pos;
    char invalid_char;
    int pos1;
    char invalid_char1;
  } char_at_pos;

  /* Initialize the fingerprint. */
  char *invalid_fpt = malloc (sizeof (char) * (fpt_length + 1) );
  strcpy (invalid_fpt, "b6:a4:6d:54:bc:9a:04:fe:2a:ff:8b:b7:5a:3a:a9:b1:0f:66:73:a8");

  char *valid_fpt = malloc(sizeof(char) * (fpt_length + 1));
  strcpy (valid_fpt, "51:1f:8e:c6:22:82:5b:ed:a2:75:cb:3e:95:ab:63:7f:69:d3:18:1c");

  test (verify_fingerprint_format (valid_fpt) == 1);
  
  /* Fingerprint is longer. */
  char longer_fpt1[] =
    "b6:a4:6d:54:bc:9a:04:fe:2a:ff:8b:b7:0a:3a:a9:b1:0f:66:73:a8:43:51:43:a1:b5:fc";
  test (verify_fingerprint_format (longer_fpt1) == 0);
  char longer_fpt2[] =
    "b6:a4:6d:54:bc:9a:04:fe:2a:ff:8b:b7:0a:3a:a9:b1:0f:66:73:a8:43";
  test (verify_fingerprint_format (longer_fpt2) == 0);

  /* Fingerprint is shorter. */
  char shorter_fpt1[] =
  "b6:a4:6d:54:bc:9a:04:fe:2a:ff:8b:b7:0a:b6:a4:6d:54:bc:9a";
  test (verify_fingerprint_format (shorter_fpt1) == 0);
  char shorter_fpt2[] = "b6:a4:6d:54:bc:9a:04:fe:2a:ff:8b:b7:0a";
  test (verify_fingerprint_format (shorter_fpt2) == 0);

  /* Fingerprint includes nonhex characters. */
  char_at_pos test0 = { .pos = 2, .invalid_char = 'a', .pos1 = -1 };
  char_at_pos test1 = { .pos = 0, .invalid_char = 'g', .pos1 = -1 };
  char_at_pos test2 = { .pos = 0, .invalid_char = 'z', .pos1 = -1 };
  char_at_pos test3 = { .pos = 58, .invalid_char = '*', .pos1 = -1};
  char_at_pos test4 = { .pos = 58, .invalid_char = ' ', .pos1 = -1};
  char_at_pos test5 = { .pos = 58, .invalid_char = '/', .pos1 = -1};
  char_at_pos test6 = { .pos = 1, .invalid_char = ':', .pos1 = -1 };
  /* Fingerprint valid characters are not in correct order. */
  char_at_pos test7 = { .pos = 2, .invalid_char = 'a', .pos1 = 3,
                        .invalid_char1 = ':' };
  char_at_pos test8 = { .pos = 56, .invalid_char = 'a', .pos1 = 58,
                        .invalid_char1 = ':' };

  char_at_pos *test_array[] = 
    {&test0, &test1, &test2, &test3, &test4, &test5, &test6, &test7, &test8};

  for (i = 0; i < 9; i++)
    {
      temp = invalid_fpt[test_array[i]->pos];
      invalid_fpt[test_array[i]->pos] = test_array[i]->invalid_char;
      if (test_array[i]->pos1 != -1)
        {
          temp1 = invalid_fpt[test_array[i]->pos1];
          invalid_fpt[test_array[i]->pos1] = test_array[i]->invalid_char1;
          test (verify_fingerprint_format (invalid_fpt) == 0);
          invalid_fpt[test_array[i]->pos1] = temp1;
        }
      else
        test (verify_fingerprint_format (invalid_fpt) == 0);
      
      invalid_fpt[test_array[i]->pos] = temp;
    }
  
} // test_verify_fingerprint_format

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Tests for functions in cache.c
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void
test_is_fingerprint_safe()
{
} // test_is_fingerprint_safe

void
test_is_url_safe()
{
} // test_is_url_safe

void
test_is_in_cache ()
{
} // test_verify_certificate

void
test_is_blacklisted()
{
} // test_is_blacklisted

void 
test_cache_insert ()
{
} // test_cache_insert

void
test_cache_remove ()
{
} // test_cache_remove

void 
test_cache_update ()
{
} // test_cache_update

int
main (int argc, char *argv[])
{
  /* Variables to keep track of allocated memory. */
  int before, after;

  /* Test all functions here. */
  //test_convergence ();
  //test_answer_to_connection();

  /* Check if the system is leaking memory. */
  before = mem_allocated();
  test_request_completed ();
  after = mem_allocated();
  printf ("BEFORE: %d\nAFTER: %d", before, after);
  test(before==after);
  //test_retrieve_response ();
  //test_send_response ();
  //test_retrieve_post_response ();
  //test_send_response ();
  //test_request_certificate ();
  //test_verify_fingerprint_format();
  //test_is_in_cache ();
  //test_cache_remove ();
  //test_cache_insert ();
  //test_cache_update ();
  // test_verify_certificate();

  printf("tests: %d,  failed: %d\n", __tests, __fails);

  return 0;
} // main
