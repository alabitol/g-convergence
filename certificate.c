/******************************************************************************
 * Authors: Tolu Alabi
 *          Zach Butler
 *          Martin Dluhos
 *
 * Created: February 21, 2012
 * Revised: March 19, 2012
 * Description: Handle sending a certificate request to the website and
 *              receiving its response.
 ******************************************************************************/
#include "certificate.h"
#include <curl/curl.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Helpers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/* Determine if a character is a hexadecimal character. Return 1 if it does,
 * else return 0.
 */
int
is_hex_char (char c)
{
  if ((c >= '0' && c <= '9') ||
      (c >= 'a' && c <= 'f') ||
      (c >= 'A' && c <= 'F'))
  {
    return 1;
  }
  else
  {
    return 0;
  }
} // is_hex_char


static size_t wrfu (void *ptr,  size_t  size,  size_t  nmemb,  void *stream)
{
  (void) stream;
  (void) ptr;
  return size * nmemb;
}

/* Requests a certificate from the website given by the url. Saved the
 * fingerprint of the certificate into fingerprint_from_website. Return 1 if
 * certificate was obtained. Otherwise, return 0.
 */
char *
request_certificate (const char *url)
{
  
  CURL *curl;
  CURLcode res;
 
  curl_global_init(CURL_GLOBAL_DEFAULT);
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wrfu);
 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
 
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_CERTINFO, 1L);
 
    res = curl_easy_perform(curl);
 
    if(!res) {
      struct curl_certinfo *ci = NULL;
 
      res = curl_easy_getinfo(curl, CURLINFO_CERTINFO, &ci);
 
      if(!res && ci) {
        struct curl_slist *slist;
        for(slist = ci->certinfo[0]; slist; slist = slist->next)
          if(!strncmp(slist->data, "Cert:", 5))
            printf("%s\n", slist->data+5);
      }
    }
  
 
    curl_easy_cleanup(curl);
  }
 
  curl_global_cleanup();
} // request_certificate

/* Verifies that the fingerprint from the website matches the
 * fingerprint from the user. Returns 1 if fingerprints match. Otherwise,
 * returns 0.
 */
int
verify_certificate (const char *fingerprint_from_client, char *fingerprint_from_website)
{
  return !strcmp(fingerprint_from_client, fingerprint_from_website);
} // verify_certificate

/* Verifies that a fingerprint has the correct format. */
int
verify_fingerprint_format (char *fingerprint)
{
  /* Length of SHA-1 fingerprint */
  int fpt_length = 59;
  int i;

  /* Check the length of the fingerprint. */
  if (strlen (fingerprint) != fpt_length)
    return 0;

  /* Check three characters at a time until reaching the last colon. There are
   * 19 sequences of two hex and one colon characters.
   */
  for (i = 0; i < fpt_length-2; i++)
  {
    /* Check if the first two characters are hex and the third a colon. */
    if (is_hex_char (fingerprint[i]))
      i++;
    else 
      return 0;

    if (is_hex_char (fingerprint[i]))
      i++;
    else
      return 0;

    if (! (fingerprint[i] == ':') )
      return 0;

  }

  /* Check the last two characters. */
  if (is_hex_char (fingerprint[i]))
    i++;
  else 
    return 0;

  if (is_hex_char (fingerprint[i]))
    return 1;
  else
    return 0;
} // verify_fingerprint_format 

