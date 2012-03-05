/******************************************************************************
 * Authors: Tolu Alabi
 *          Zach Butler
 *          Martin Dluhos
 *
 * Created: February 27, 2012
 * Revised: March 4, 2012
 * Description: This program starts the notary server which listens for
 * verification requests from clients.
 ******************************************************************************/

#include <microhttpd.h>

#include "connection.h"
#include "certificate.h"
#include "response.h"

/* This is our primary datasructure for storing requests that arrive from
 * clients. Each request will contain a URL of the host to be verified. If the
 * client is sending a GET request, then there is no fingerprint to compare. 
 * Otherwise, it is a POST request which has to include the fingerprint.
 */ 
struct ClientRequest
{
  char *url;
  char *fingerprint;
}

/* Begins logging of server activity to a log file. 
 * IMPLEMENT LATER.
 */
int 
initiateLogging ()
{
  return 1;
}

int main (int argc, char *argv[])
{
  struct MHD_Daemon *daemon;

  /* Process command line arguments. */

  /* Print a helpful usage message if the user calls notary incorrectly or
   * if she invokes it with --help flag. */

  initiateLogging ();

  /* Start the MHD daemon to listen for client requests. We probably want to
   * use one thread per connection passing MHD_USE_THREAD_PER_CONNECTION as
   * the first argument.
   */
  daemon = MHD_start_daemon (...);

  /* Stop the  MHD daemon. */
  MHD_stop_daemon (daemon);

  return 0;
}
