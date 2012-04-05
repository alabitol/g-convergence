/*****************************************************************************
 * Authors: g-coders
 * Created: February 27, 2012
 * Revised: March 12, 2012
 * Description: This program starts the notary server which listens for
 * verification requests from clients.
 ******************************************************************************/

#include "notary.h"
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
int
set_notary_option (char *option, int *i, char *argv[])
{
  /* Reallocate the string. */
  char *temp;
  temp = realloc (option, sizeof (char) * strlen (argv[++(*i)]));
  if (temp == NULL)
  {
    /* Figure out how to print out the option for which memory could not be
     * allocated.
     */
    fprintf (stderr, "Could not allocate memory for ...\n");
    return 0;
  }

  option = temp;
  strcpy(option, argv[*i]);

  return 1;
}

int main (int argc, char *argv[])
{
  int i;
  struct MHD_Daemon *daemon;

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
  /* Implement the backend option as an extension */
  // char backend[]; 

  /* Process command line arguments. */
  for (i = 1; i < argc; i++)
  {
    if ((! strcmp (argv[i], "-help")) || 
        (! strcmp (argv[i], "--help")) ||
        (! strcmp (argv[i], "-h")))
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
      set_notary_option (ip, &i, argv);
    }
    else if (! strcmp (argv[i], "-c"))
    {
      set_notary_option (certificate_file, &i, argv);
    }
    else if (! strcmp (argv[i], "-k"))
    {
      set_notary_option (key_file, &i, argv);
    }
    else if (! strcmp (argv[i], "-u"))
    {
      set_notary_option (username, &i, argv);
    }
    else if (! strcmp (argv[i], "-g"))
    {
      set_notary_option (group, &i, argv);
    }
    else if (! strcmp (argv[i], "-d"))
    {
      debug = true;
    }
    else if (! strcmp (argv[i], "-f"))
    {
      foreground = true;
    }
   // else if (! strcmp (argv[i], "-b"))
   // {
   //   set_notary_option (backend, &i, argv);
   // }
  }

  /* Find a logging c library. */
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
                             ssl_port, 
			     NULL, 
			     NULL, 
			     &answer_to_connection, NULL, MHD_OPTION_NOTIFY_COMPLETED, 
			     request_completed, NULL, MHD_OPTION_END);

  if (daemon == NULL)
  {
    fprintf (stderr, "Error: Failed to start the MHD daemon\n");
    return 1;
  }
  else
    printf ("MHD daemon is listening on port %d\n", ssl_port);

  /* Prevent the server from stopping immediately after starting. We might
   * want to change this approach in the future. */
  getchar ();

  /* Stop the  MHD daemon. */
  MHD_stop_daemon (daemon);

  return 0;
}
