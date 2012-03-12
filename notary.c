/*****************************************************************************
 * Authors: g-coders
 * Created: February 27, 2012
 * Revised: March 5, 2012
 * Description: This program starts the notary server which listens for
 * verification requests from clients.
 ******************************************************************************/

#include <microhttpd.h>
#include <stdbool.h>

#include "connection.h"
#include "certificate.h"
#include "response.h"

/* Begins logging of server activity to a log file. 
 * IMPLEMENT LATER.
 */
int 
initiateLogging ()
{
  return 1;
}

/* Print a helpful usage message if the user calls notary incorrectly or
 * if she invokes it with --help flag. */
void 
print_usage ()
{

} // print_help

int main (int argc, char *argv[])
{
  int i;

  struct MHD_Daemon *daemon;
  unsigned short port;

  /* Set sensible defaults for the server. */
  int http_port = 80;
  int ssl_port = 443;
  char *ip;
  char *certificate_file;
  char *key_file;
  char * username;
  char * group;
  bool debug;
  bool foreground;
  char *backend;

  /* Process command line arguments. */
  for (i = 1; i < argc; i++)
  {
    if ((! strcmp (argv[i], "-help")) || (! strcmp (argv[i], "--help")))
    {
      print_usage ();
    }
    else if (! strcmp (argv[i], "-p"))
    {
      http_port = atoi (argv[++i]);
    }
    else if (! strcmp (argv[i], "-s"))
    {
      ssl_port = atoi (argv[++i]);
    }
    else if (! strcmp (argv[i], "-i"))
    {
      ip = argv[++i];
    }
    else if (! strcmp (argv[i], "-c"))
    {
      certificate_file = argv[++i];
    }
    else if (! strcmp (argv[i], "-k"))
    {
      key_file = argv[++i];
    }
    else if (! strcmp (argv[i], "-u"))
    {
      username = argv[++i];
    }
    else if (! strcmp (argv[i], "-g"))
    {
      group = argv[++i];
    }
    else if (! strcmp (argv[i], "-d"))
    {
      debug = 1;
    }
    else if (! strcmp (argv[i], "-f"))
    {
      foreground = 1;
    }
    else if (! strcmp (argv[i], "-b"))
    {
      backend = argv[++i];
    }
    else if (! strcmp (argv[i], "-h"))
    {
     print_usage ();
    }
  }


  initiateLogging ();

  /* Make sure we can start the daemon in the background. */

  /* Start the MHD daemon to listen for client requests. 
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
  daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, 
                             port, 
			     NULL, 
			     NULL, 
			     &answer_to_connection, NULL, MHD_OPTION_NOTIFY_COMPLETED, 
			     request_completed, NULL, MHD_OPTION_END);

  if (daemon == NULL)
  {
    fprintf (stderr, "Error: Failed to start the MHD daemon\n")
    return 1;
  }
  else
    printf ("MHD daemon is listening on port %d\n", port);

  /* Prevent the server from stopping immediately after starting. */
  get_char ();

  /* Stop the  MHD daemon. */
  MHD_stop_daemon (daemon);

  return 0;
}
