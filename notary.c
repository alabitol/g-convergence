/*****************************************************************************
 * Authors: g-coders
 * Created: February 27, 2012
 * Revised: April 15, 2012
 * Description: This program starts the notary server which listens for
 * verification requests from clients.
 ******************************************************************************/

#include "notary.h"
#include "connection.h"
#include "certificate.h"
#include "response.h"

/* Begins logging of server activity to a log file.  */
static void
initiate_logging ()
{
  
} // initiate_logging

/* Print a helpful usage message if the user calls notary incorrectly or
 * if she invokes it with --help flag. */
static void 
print_usage ()
{
  printf ("usage: notary <options>\n \
           Options:\n \
	   -p <http_port>   HTTP port to listen on (defaults to 80).\n \
	   -s <ssl_port>    SSL port to listen on (defaults to 443).\n \
	   -i <address>     IP address to listen on for incoming connections (optional).\n \
	   -c <certificate> SSL certificate location.\n \
	   -k <ssl_key>     SSL private key location.\n \
	   -u <username>    Name of user to drop privileges to (defaults to 'nobody')\n \
	   -g <group>       Name of group to drop privileges to (defaults to 'nogroup')\n \
	   -b <backend>     Verifier backend [perspective|google] (defaults to 'perspective')\n \
	   -f               Run in foreground.\n \
	   -d               Run in debug mode.\n \
	   -h               Print this help message.\n");

} // print_help


/*Set the default notary option */
static char*
set_default_notary_option (char* default_string)
{
  char* option;
  option = (char*) malloc(sizeof(char) * (strlen(default_string) + 1) );

  strcpy(option, default_string);
  return option;
}


/* Set the appropriate notary option. */
static void
set_notary_option (char *option, char* argument)
{
  /* Reallocate the string. */
  char *temp;
  temp = realloc(option, sizeof(char) * (strlen(argument) + 1));
  if (temp == NULL)
    {
      /* Figure out how to print out the option for which memory could not be
       * allocated.
       */
      fprintf (stderr, "Could not allocate memory for %s\n", argument);
      exit(1);
    }

  strcpy(option, argument);
}

int main (int argc, char *argv[])
{
  int i;
  struct MHD_Daemon *ssl_daemon, *http_daemon, *fourtwo_daemon;

  /* Set sensible defaults for the server. */
  int http_port = 80;
  int ssl_port = 443;
  char* ip;
  ip = set_default_notary_option("");
  char *certificate_file;
  certificate_file = set_default_notary_option("/etc/ssl/certs/convergence.pem");
  char *key_file;
  key_file = set_default_notary_option("/etc/ssl/private/convergence.key");
  char *username;
  username = set_default_notary_option("nobody");
  char *group;
  group = set_default_notary_option("nogroup");
  bool debug = false;
  bool foreground = false;

  char c;
  opterr = 0;
  
  while ((c = getopt (argc, argv, "p:s:i:c:k:u:g:df")) != -1)
    {
      switch (c)
        {
        case 'p':
          http_port = atoi (optarg);
          break;
        case 's':
          ssl_port = atoi (optarg);
          break;
        case 'i':
          set_notary_option (ip, optarg);
          break;
        case 'c':
          set_notary_option (certificate_file, optarg);
          break;
        case 'k':
          printf("%s\n", optarg);
          set_notary_option (key_file, optarg);
          break;
        case 'u':
          set_notary_option (username, optarg);
          break;
        case 'g':
          set_notary_option (group, optarg);
          break;
        case 'd':
          debug = true;
          break;
        case 'f':
          foreground = true;
          break;
        case '?': //an unknown character was encountered
          print_usage();
          return 1;
        default:
          abort ();
        }
    }

  /* Find a logging c library. */
  initiate_logging ();

  /* Make sure we can start the daemon in the background. */

  /* Start the MHD daemons to listen for client requests. 
   * We have 3 daemons to listen on 3 ports: the SSL port 
   * (for standard traffic), the HTTP port (for proxy traffic) and 4242 traffic
   * (for other notaries that are serving as proxies to query us)
   *
   * Parameters: 
   * MHD_USE_THREAD_PER_CONNECTION: use one thread per connection 
   * port: port to listen on
   * NULL: allow connection from any IP
   * NULL: additional arguments to preceding param
   * &answer_to_connection: call this function to handle a new connection
   * NULL: arguments to answer_to_connection,
   * MHD_OPTION_NOTIFY_COMPLETED: indicate that request_completed is
   * registered
   * request_completed: function to call when a request completes
   * NULL: arguments to the request_completed function
   * MHD_OPTION_END: indicate that there are no more options
   */
  ssl_daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, 
                             ssl_port, 
			     NULL, 
			     NULL, 
			     &answer_to_SSL_connection,
                             NULL,
                             MHD_OPTION_NOTIFY_COMPLETED, 
			     request_completed,
                             NULL, 
                             MHD_OPTION_END
                             );

  if (ssl_daemon == NULL)
    {
      fprintf (stderr, "Error: Failed to start the MHD SSL daemon\n");
    return 1;
    }
  else
    {
      printf ("MHD SSL daemon is listening on port %d\n", ssl_port);
    }
  
 
  http_daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, 
                             http_port, 
			     NULL, 
			     NULL, 
			     &answer_to_HTTP_connection,
                             NULL,
                             MHD_OPTION_NOTIFY_COMPLETED, 
			     request_completed,
                             NULL, 
                             MHD_OPTION_END
                             );

  if (http_daemon == NULL)
    {
      fprintf (stderr, "Error: Failed to start the MHD HTTP daemon\n");
      return 1;
    }
  else
    {
      printf ("MHD HTTP daemon is listening on port %d\n", http_port);
    }
  
  fourtwo_daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, 
                             4242, 
			     NULL, 
			     NULL, 
			     &answer_to_4242_connection,
                             NULL,
                             MHD_OPTION_NOTIFY_COMPLETED, 
			     request_completed,
                             NULL, 
                             MHD_OPTION_END
                             );

  if (fourtwo_daemon == NULL)
  {
    fprintf (stderr, "Error: Failed to start the MHD 4242 daemon\n");
    return 1;
  }
  else
    {
      printf ("MHD 4242 daemon is listening on port 4242\n");
    }

  /* Prevent the server from stopping immediately after starting. We might
   * want to change this approach in the future. */
  getchar ();

  /* Stop the  MHD daemon. */
  MHD_stop_daemon (ssl_daemon);
  printf ("SSL daemon has terminated\n");
  MHD_stop_daemon (http_daemon);
  printf ("HTTP daemon has terminated\n");
  MHD_stop_daemon (fourtwo_daemon);
  printf ("4242 daemon has terminated\n");

  return 0;
}
