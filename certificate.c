/******************************************************************************
 * Authors: Tolu Alabi
 *          Zach Butler
 *          Martin Dluhos
 *
 * Created: February 21, 2012
 * Revised: March 1, 2012
 * Description: Handle sending a certificate request to the website and
 *              receiving its response.
 ******************************************************************************/
#include<curl/curl.h>

/* Requests a certificate from the website given by the url. Saved the
 * fingerprint of the certificate into fingerprint_from_website. Return 1 if
 * certificate was obtained. Otherwise, return 0.
 */
char *
request_certificate (char *url)
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
}

/* Verifies that the fingerprint from the website matches the
 * fingerprint from the user. Returns 1 if fingerprints match. Otherwise,
 * returns 0.
 */
int
verify_certificate (char *fingerprint_from_client, char *fingerprint_from_website)
{
  return !strcmp(fingerprint_from_client, fingerprint_from_website);
}

/* Verifies that a fingerprint has the correct format. */
int
verify_fingerprint_format (char *fingerprint)
{
  /* STUB */
  return 1;
}
