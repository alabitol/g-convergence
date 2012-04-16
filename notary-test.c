/*****************************************************************************
 * Authors: g-coders
 * Created: March 10, 2012
 * Revised: April 12, 2012
 * Description: This file includes unit tests for all functions which are part
 * of the convergence system.
 ******************************************************************************/

#include "notary.h"
#include <malloc.h>
#include <stdio.h>
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
#define MAX_NO_OF_CERTS 7
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

  int result = 0;

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
      url = strtok(input_line, " ");
      fingerprint = strtok(NULL, " ");

      /* Call retrieve_response and check its return value. */
      result = retrieve_response (coninfo_cls, url, fingerprint);
    
      test(result == MHD_YES);
      printf ("code: %d\n", coninfo_cls->answer_code);
      test(coninfo_cls->answer_code == MHD_HTTP_OK);
    }//while

  /* If URL is not valid, then expect MHD_NO. */
  while (fgets (input_line, size, invalid_urls) != NULL)
    {
      url = strtok(input_line, " ");
      fingerprint = strtok(NULL, " ");

      /* Call retrieve_response and check its return value. */
      result = retrieve_response(coninfo_cls, url, fingerprint);

      /* Check if result is MHD_NO */
      test(result == MHD_NO);
    }
      
  /* If fingerprint_from_client does not match any fingerprint from website, expect MHD_YES as the return value and answer_code as MHD_HTTP_CONFLICT. */
  while (fgets (input_line, size, invalid_fingerprints) != NULL)
    {
      url = strtok(input_line, " ");
      fingerprint = strtok(NULL, " ");

      /* Call retrieve_response and check its return value. */
      result = retrieve_response(coninfo_cls, url, fingerprint);

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
  result = verify_certificate(c1, c11, 1);x
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
} // test_verify_certificate

void 
test_send_response ()
{
} // test_send_response

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

      //get the url from the string gotten from the file
      url = strtok(string_read, "' '");
      //then get the fingerprint
      correct_fingerprint = strtok(NULL, "' '");

      //Get the fingerprint by calling request_certificate
      number_of_certs = request_certificate(url, retrieved_fingerprints);
      printf("%d certs found\n", number_of_certs);

      test (verify_certificate(correct_fingerprint, retrieved_fingerprints, number_of_certs) == 0);
      printf("tests: %d,  failed: %d\n", __tests, __fails);

      //reset all the characters in each string to null to allow for the
      //next iteration of fingerprint conversion
      for(i=0; i<MAX_NO_OF_CERTS; i++)
        for(j=0; j<FPT_LENGTH; j++)
          retrieved_fingerprints[i][j] = '\0';
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

      //then get the fingerprint
      correct_fingerprint = strtok(NULL, "' '");

      //Get the fingerprint by calling request_certificate
      number_of_certs = request_certificate(url, retrieved_fingerprints);

      //Check if the fingerprints are the same
      test (verify_certificate(correct_fingerprint, retrieved_fingerprints, number_of_certs) != 0);
      printf("tests: %d,  failed: %d\n", __tests, __fails);

      //reset all the characters in each string to null to allow for the
      //next iteration of fingerprint convers
      for(i=0; i<MAX_NO_OF_CERTS; i++)
        for(j=0; j<FPT_LENGTH; j++)
          retrieved_fingerprints[i][j] = '\0';
    }

  fclose (valids);
  fclose (invalids);

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
  //test_convergence ();
  //test_request_completed ();
  //test_retrieve_response ();
  //test_send_response ();
  //test_request_certificate ();
  //test_verify_fingerprint_format();
  //test_is_in_cache ();
  //test_cache_remove ();
  //test_cache_insert ();
  //test_cache_update ();
  test_verify_certificate();

  return 0;
} // main
