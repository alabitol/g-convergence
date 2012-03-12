/*****************************************************************************
 * Authors: g-coders
 * Created: March 10, 2012
 * Revised: March 11, 2012
 * Description: This file includes unit tests for all functions which are part
 * of the convergence system.
 ******************************************************************************/

#include <stdio.h>
#include <malloc.h>
#include "connection.h"
#include "certificate.h"
#include "response.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Helpers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Get information about the amount of dynamically allocated address space
 * in bytes.
 */
static int
mem_allocated ()
{
  struct mallinfo info = mallinfo ();
  return info.uordblks;
} // mem_allocated

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Unit tests for functions implemted in connection.c, certificate.c,
// response.c, and cache.c.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Test the entire system. */
void
test_convergence ()
{
  /* Start the daemon. */
  MHD_start_daemon ();

  /* Create a curl request. */
  request = curl_easy_init ();

  /* Set the appropriate request parameters. */
  curl_easy_setopt (request, CURLOPT_URL, "http://127.0.0.1:1080/hello_world");

  /* Send the request. */
  return_val = curl_easy_perform (request);
  
  /* Check the return value and record test results. */
  if (return_val == CURLE_OK)
  {
  }

} // test_convergence

void 
test_answer_to_connection ()
{
} // test_answer_to_connection

void 
test_request_completed ()
{ 
} // test_request_completed

void 
test_retrieve_post_response ()
{
} // test_retrieve_post_response

void 
test_send_response ()
{
} // test_send_response

void 
test_request_certificate ()
{
} // test_request_certificate

void 
test_verify_certificate ()
{
} // test_verify_certificate

void
test_is_in_cache ()
{
} // test_verify_certificate

void
test_cache_remove ()
{
} // test_cache_remove

void 
test_cache_insert ()
{
} // test_cache_insert

void 
test_cache_update ()
{
} // test_cache_update

int
main (int argc, char *argv[])
{
  /* Test all functions here. */
  test_convergence ();
  test_request_completed ();
  test_retrieve_post_response ();
  test_send_response ();
  test_request_certificate ();
  test_verify_certificate ();
  test_is_in_cache ();
  test_cache_remove ();
  test_cache_insert ();
  test_cache_update ();
} // main
