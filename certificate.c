/******************************************************************************
 * Authors: Tolu Alabi
 *          Zach Butler
 *          Martin Dluhos
 *	    Chase Felker
 *	    Radhika Krishna
 * Created: February 21, 2012
 * Revised: April 11, 2012
 * Description: Handle sending a certificate request to the website and
 *              receiving its response.
 ******************************************************************************/
#include "certificate.h"

#define FPT_LENGTH 59

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Helpers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/* Determine if a character is a hexadecimal character. 
 * Return 1 if it is, else return 0.
 */
int is_hex_char (char c)
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
static size_t wrfu (void *ptr,  size_t  size,  size_t  nmemb,  void *stream)
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
 * Changes fingerprint (hex characters) to upper case.
 */
static void to_upper_case(char* fingerprint)
{
  int i=0;
  while(fingerprint[i])
    {
      if( (fingerprint[i] != ':') && 
          (fingerprint[i] >= 'a') && 
          (fingerprint[i] <= 'f'))
        fingerprint[i] -= 32;
      i++;
    }
}

/**
 * Changes fingerprint (hex characters) to lower case.
 */
 static void to_lower_case(char* fingerprint)
{
  int i=0;
  while(fingerprint[i])
    {
      if( (fingerprint[i] != ':') && 
          (fingerprint[i] >= 'A') && 
          (fingerprint[i] <= 'F'))
        fingerprint[i] += 32;
      i++;
    }
}


/**
 * This function converts the PEM certificate to its corresponding SHA1 fingerprint
 * returns pointer to the fingerprint. 
 */
static char* get_fingerprint_from_cert (char* cert)
{
  ssize_t len;
  BIO* bio_buffer;
  X509* decoded_certificate;
  const EVP_MD* digest;
  unsigned char md[EVP_MAX_MD_SIZE];
  unsigned int n;
  char errmsg[1024];
  unsigned err;
  int pos;
  char* fingerprint = calloc(FPT_LENGTH + 1, 1);

  //initialize OpenSSL
  //this allows us to access error messages
  SSL_load_error_strings();

  //this initializes the library for ssl (algorithms)
  SSL_library_init();


  len = strlen(cert);
  //create BIO buffer for SSL, this buffer contains the certificate, buff
  bio_buffer = BIO_new_mem_buf(cert, len);

  //decode the buffer by reading from the newly created BIO buffer
  if(! (decoded_certificate = PEM_read_bio_X509(bio_buffer, NULL, 0L, NULL)))
    {
      while( (err = ERR_get_error()))
        {
          errmsg[1023] = '\0';
          ERR_error_string_n(err, errmsg, 1023);
          fprintf(stderr, "peminfo: %s\n", errmsg);
        }

      BIO_free(bio_buffer);
      ERR_free_strings();
      exit(1);
    }

  //calculate and print out the fingerprint
  digest = EVP_get_digestbyname("sha1");
  X509_digest(decoded_certificate, digest, md, &n);

  char* temp = malloc(sizeof(char) * 4);
  //concatenate the fingerprint to the return string
  for(pos=0; pos < 19; pos++)
    {
      sprintf(temp, "%02x:", md[pos]);
      strcat(fingerprint, temp);
    }

  //then concatenate the final two characters
  sprintf(temp, "%02x", md[pos]);
  strcat(fingerprint, temp);

  //end the string with a null character
  fingerprint[FPT_LENGTH] = '\0';


  //free all memory
  BIO_free(bio_buffer);
  ERR_free_strings();
  free(temp);

  return fingerprint;
}//get_fingerprint_from_cert

/* Requests a certificate from the website given by the url. Saved the
 * fingerprint of the certificate into fingerprint_from_website. 
 * Returns pointer to the fingerprint if successful, otherwise returns an error.
 */
char* request_certificate (const char *url)
{  
  CURL *curl;
  CURLcode res;
  char* certificate;
  //variable to determine the number of certificates retrieved
  int number_of_certs;

  //initialize curl
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  
  //If curl has been initialized, set curl options.
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wrfu);
 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
 
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_CERTINFO, 1L);
    
    //res is 0 if curl_easy_perform succeeds.
    res = curl_easy_perform(curl);
    
    //Initialize ci and determine number of certificates.
    if(!res) {
      struct curl_certinfo *ci = NULL;
      res = curl_easy_getinfo(curl, CURLINFO_CERTINFO, &ci);
      number_of_certs = ci->num_of_certs;
      
      //Get the certificate. 
      if((!res) && ci)
        {
          struct curl_slist *slist;
          for(slist = ci->certinfo[0]; slist; slist = slist->next)
            if(!strncmp(slist->data, "Cert:", 5))
              {
                certificate = slist->data+5;
              }
        }
      else
        {
          fprintf(stderr, "Could not retrieve certificate from server\n");
          curl_cleanup(curl);
        exit(1);
      } //If res is an error, return an error and exit.
  }
  else
    {
      fprintf(stderr, "Could not initialize CURL\n");
      curl_cleanup(curl);
      exit(1);
    } //If ci has not been initialized, return an error and exit.

  //Extract fingerprint
  char* fingerprint = get_fingerprint_from_cert(certificate);

  //Cleanup Functions
  //Note that curl_easy_cleanup(curl) is not called here in case another
  //connection is being made
  curl_global_cleanup();

  return fingerprint;
} // request_certificate

/* Verifies that the fingerprint from the website matches the
 * fingerprint from the user. Returns 1 if fingerprints match. Otherwise,
 * returns 0.
 */
int
verify_certificate (const char *fingerprint_from_client, 
                    char *fingerprint_from_website)
{
  //Change the case of the fingerprint_from_website to ensure that it
  //is similar to the case of fingerprint_from_client
  int result_of_comparison = 
	strcmp(fingerprint_from_client, fingerprint_from_website);

  if (result_of_comparison < 0)
  {
    to_upper_case(fingerprint_from_website);
  }

  else
    if(result_of_comparison > 0)
    {
     to_lower_case(fingerprint_from_website);
    }

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

