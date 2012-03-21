/*****************************************************************************
 * Authors: g-coders
 * Created: March 10, 2012
 * Revised: March 18, 2012
 * Description: This file includes unit tests for all functions which are part
 * of the convergence system.
 ******************************************************************************/

#include "notary.h"
#include <malloc.h>
#include "connection.h"
#include "certificate.h"
#include "response.h"

/* A macro that defines an enhanced assert statement. */
#define test(exp) \
do { ++__tests; \
if (! (exp)) { ++__fails; fprintf (stderr, "Test failed: %s at line: %d\n", \
                                   #exp, __LINE__); }} \
while (0)

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Globals
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int __tests = 0;
int __fails = 0;

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
  /* STUB */

  /* Start the daemon. */

  /* Create a curl request. */

  /* Set the appropriate request parameters. */

  /* Send the request. */
  
  /* Check the return value and record test results. */

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
  FILE *valids, *invalids;
  int max_len = 255;
  char *url = malloc (sizeof (char) * max_len);

  valids = fopen ("valid_urls.txt", "r");
  if (valids == NULL)
    fprintf (stderr, "Could not open valid_urls.txt\n");

  invalids = fopen ("invalid_urls.txt", "r");
  if (invalids == NULL)
    fprintf (stderr, "Could not open invalid_urls.txt\n");

  /* Read valid urls from a file and retrieve a certificate from each. */
  while (fgets (url, max_len, valids) != NULL)
  {
    test (request_certificate (url) != NULL);
  }

  /* Read invalid urls from a file and request a certificate from each. */
  while (fgets (url, max_len, invalids) != NULL)
  {
    test (request_certificate (url) == NULL);
  }

  fclose (valids);
  fclose (invalids);
} // test_request_certificate

void 
test_verify_certificate ()
{
  int fpt_length = 59;
  char valid_client_fpt[] =
    "b6:a4:6d:54:bc:9a:04:fe:2a:ff:8b:b7:0a:3a:a9:b1:0f:66:73:a8";
  char valid_website_fpt[] =
  "b6:a4:6d:54:bc:9a:04:fe:2a:ff:8b:b7:0a:3a:a9:b1:0f:66:73:a8";

  char *invalid_client_fpt = malloc (sizeof (char) * fpt_length);
  char *invalid_website_fpt = malloc (sizeof (char) * fpt_length);
    
  /* Fingerprints match and are valid. */
  test (verify_certificate (valid_client_fpt, valid_website_fpt) == 1);
  
  /* Fingerprints are valid format, but do not match. */

  /* One of inner characters does not match. */
  sprintf
    (invalid_client_fpt, "b6:a4:6d:54:bc:9a:04:fe:2a:ff:8b:b7:5a:3a:a9:b1:0f:66:73:a8");
  sprintf
    (invalid_website_fpt, "b6:a4:6d:54:bc:9a:04:fe:2a:ff:8b:b7:0a:3a:a9:b1:0f:66:73:a8");
  test (verify_certificate (invalid_client_fpt, invalid_website_fpt) == 0);

  /* First character does not match. */
  sprintf
    (invalid_client_fpt, "b6:a4:6d:54:bc:9a:04:fe:2a:ff:8b:b7:5a:3a:a9:b1:0f:66:73:a8");
  sprintf
    (invalid_website_fpt, "a6:a4:6d:54:bc:9a:04:fe:2a:ff:8b:b7:5a:3a:a9:b1:0f:66:73:a8");
  test (verify_certificate (invalid_client_fpt, invalid_website_fpt) == 0);

  /* Two characters do not match. */
  sprintf
    (invalid_client_fpt, "b6:a4:6d:54:bc:9a:04:f4:2a:ff:8b:b7:5a:3a:a9:b1:0f:66:73:a8");
  sprintf
    (invalid_website_fpt, "a6:a4:6d:54:bc:9a:04:fe:2a:ff:8b:b7:5a:3a:a9:b1:0f:66:73:a8");
  test (verify_certificate (invalid_client_fpt, invalid_website_fpt) == 0);

} // test_verify_certificate

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
  char *invalid_fpt = malloc (sizeof (char) * fpt_length);
  sprintf
    (invalid_fpt, "b6:a4:6d:54:bc:9a:04:fe:2a:ff:8b:b7:5a:3a:a9:b1:0f:66:73:a8");
  
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

/* Cache will be added later as an extension. */
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
  test_verify_fingerprint_format();
  test_is_in_cache ();
  test_cache_remove ();
  test_cache_insert ();
  test_cache_update ();
} // main
