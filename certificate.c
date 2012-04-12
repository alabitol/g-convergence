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

static void curl_cleanup(CURL* curl_handle)
{
  curl_easy_cleanup(curl_handle);
  curl_global_cleanup();
}

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


/*
This function converts the PEM certificate to its corresponding SHA1 fingerprint
 */
static void get_fingerprint_from_cert (char** cert, char** fingerprints, int num_of_certs)
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

  //initialize OpenSSL
  //this allows us to access error messages
  SSL_load_error_strings();

  //this initializes the library for ssl (algorithms)
  SSL_library_init();

  char* temp = malloc(sizeof(char) * 4);
  int i;
  for(i=0; i<num_of_certs; i++)
    {
      len = strlen(cert[i]);
      //create BIO buffer for SSL, this buffer contains the certificate, buff
      bio_buffer = BIO_new_mem_buf(cert[i], len);

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

      //concatenate the fingerprint to the return string
      for(pos=0; pos < 19; pos++)
        {
          sprintf(temp, "%02x:", md[pos]);
          strcat(fingerprints[i], temp);
        }

      //then concatenate the final two characters
      sprintf(temp, "%02x", md[pos]);
      strcat(fingerprints[i], temp);

      //end the string with a null character
      fingerprints[i][FPT_LENGTH] = '\0';
      //printf("Fingerprint: %s\n", fingerprints[i]);
    }

  //free all memory
  BIO_free(bio_buffer);
  ERR_free_strings();
  free(temp);

}//get_fingerprint_from_cert

/* Requests the certificates from the website given by the url, and stores
 * the fingerprints of the corresponding certificates in the output
 * parameter fingerprints. Returns the number of fingerprints retrieved.
 */
int request_certificate (const char *url, char** fingerprints)
{  
  CURL *curl;
  CURLcode res;
  //variable to determine the number of certificates retrieved
  int number_of_certs;

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

      number_of_certs = ci->num_of_certs;
      //array to store the retrieved certificates
      char* certificates[number_of_certs];

      int i;
      if((!res) && ci)
        {
          for(i=0; i<number_of_certs; i++)
            {
              struct curl_slist *slist;
              for(slist = ci->certinfo[i]; slist; slist = slist->next)
                if(!strncmp(slist->data, "Cert:", 5))
                  {
                    certificates[i] = slist->data+5;
                    //printf("%s\n", certificates[i]);
                  }
            }

          //retrieve the fingerprints from these certificates
          get_fingerprint_from_cert(certificates, fingerprints, number_of_certs);

          //Cleanup Functions
          //Note that curl_easy_cleanup(curl) is not called here in case another
          //connection is being made
          curl_global_cleanup();
          return number_of_certs;
        }
      else
        {
          fprintf(stderr, "Could not retrieve certificates from server\n");
          curl_cleanup(curl);
          return 0;
        }
    }
    else
      {
        fprintf(stderr, "Could not establish a connection\n");
        curl_cleanup(curl);
        return 0;
      }
  }
  else
    {
      fprintf(stderr, "Could not initialize CURL\n");
      curl_cleanup(curl);
      return 0;
    }
} // request_certificate

/* Verifies that the fingerprint from the website matches the
 * fingerprint from the user. Returns 1 if fingerprints match. Otherwise,
 * returns 0.
 */
int
verify_certificate (const char **fingerprints_from_client, int num_of_client_certs, char **fingerprints_from_website, int num_of_website_certs)
{
  //Change the case of the fingerprints_from_website to ensure that it
  //is similar to the case of fingerprints_from_client
  int case_check;
  //this index will iterate over fingerprint_from_client
  int i = 0;
  //this index will iterate over fingerprint_from_website
  int j = 0;
  int result_of_comparison;
  
  while( (i < num_of_client_certs) )
    {
      result_of_comparison = 1;
      while( (j < num_of_website_certs) && (result_of_comparison != 0) )
        {
          case_check = strcmp(fingerprints_from_client[i], fingerprints_from_website[j]);
          if (case_check < 0)
            {
              to_upper_case(fingerprints_from_website[j]);
            }
          else
            if(case_check > 0)
              {
                to_lower_case(fingerprints_from_website[j]);
              }

          //compare the two fingerprints
          result_of_comparison = strcmp(fingerprints_from_client[i], fingerprints_from_website[j]);

          j++;
        }

      j = 0;
      i++;
    }

  return result_of_comparison;
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

/* int main() */
/* { */
/*   char* fingerprint[7]; */
/*   int i; */
/*   for(i=0; i< 7; i++) */
/*     { */
/*       fingerprint[i] = calloc(FPT_LENGTH * sizeof(char), 1); */
/*     } */

/*   int certs = request_certificate("https://www.github.com:443", fingerprint); */

/*   return 0; */
/* } */
