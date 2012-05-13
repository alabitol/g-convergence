/** @file 
 *
    @brief  Certificate: Handle sending a certificate request to the website 
            and receiving its response.

    @author 
    Tolu Alabi, Zach Butler, Martin Dluhos, Chase Felker, Radhika Krishnar

    @date
    Created February 21, 2012 <br />
    Last revised: May 6, 2012 

*/

#include "certificate.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Helpers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
 * @brief Tell the curl functions where to write the data they receive from a network
 * 
 * @remark wrfu is shorthand for Write Function.
 *
 * @param ptr     a pointer to the location where the data will be stored
 * @param size    FIXME
 * @param nmemb   the size of the data written
 * @param stream  FIXME 
 * 
 * @return  FIXME  
 */
static size_t wrfu (void *ptr,  size_t  size,  size_t  nmemb,  void *stream)
{
  (void) stream;
  (void) ptr;
  return size * nmemb;
}

/**
 * @brief Combines curl cleanup calls.
 * @param curl_handle ___. FIXME
 */
static void curl_cleanup(CURL* curl_handle)
{
  curl_easy_cleanup(curl_handle);
  curl_global_cleanup();
}

/**
 * @brief Changes fingerprint (hex characters) to upper case.
 * @param fingerprint  pointer to the string to be changed into upper case.
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
 * @brief converts the PEM certificate to its corresponding SHA1 fingerprint
 * 
 * @param cert          array of certificates to be converted.
 * @param fingerprints  pointer to the array to which @c get_fingerprint_from_cert writes the SHA1 fingerprints     
 * @param num_of_certs  size of the @c cert array
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

  //temporary value to store parts of the fingerprint
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
      //strcat adds a terminating null character to the end
      sprintf(temp, "%02x", md[pos]);
      strcat(fingerprints[i], temp);

      //free all used memory
      X509_free(decoded_certificate);
      BIO_free(bio_buffer);
      ERR_free_strings();
    }

  //free all memory
  free(temp);

}//get_fingerprint_from_cert


/** 
 * @brief Requests the certificates from the website given by the url, 
 * and stores the fingerprints of the corresponding certificates. 
 *
 * @param host_to_verify  FIXME  
 * @param fingerprints    pointer to the array to which @c request_certificate writes the fingerprints from the host. 
 *
 * @return  the number of fingerprints retrieved.
 */
int 
request_certificate (host *host_to_verify, char** fingerprints)
{  
  CURL *curl;
  CURLcode res;
  //variable to determine the number of certificates retrieved
  int number_of_certs;

  //initialize curl
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  
  //If curl has been initialized, set curl options.
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, host_to_verify->url);
    curl_easy_setopt(curl, CURLOPT_PORT, host_to_verify->port);
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

      //allocate memory for the array to store the retrieved certificates
      char** certificates;
      certificates = (char**)calloc(number_of_certs, sizeof *certificates);

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

          //Cleanup curl
          curl_cleanup(curl);

          return number_of_certs;
        }
      else
        {
          fprintf(stderr, "Could not retrieve certificate from server\n");
          curl_cleanup(curl);
          return 0;
        } //If the certificate cannot be retrieved from the server, return 0
    }
    else
      {
        fprintf(stderr, "Could not establish a connection with the server\n");
        curl_cleanup(curl);
        return 0;
      } //If curl could not establish a connection with server, return 0
  }
  else
    {
      fprintf(stderr, "Could not initialize CURL\n");
      curl_cleanup(curl);
      return 0;
    } //if curl could not be initialized, return 0
}
// request_certificate


/** 
 * @brief Verifies that the fingerprint from the website matches the fingerprint from the user. 
 *
 * @return 1 if fingerprints match, 0 otherwise.
 */
int 
verify_certificate (const char *fingerprint_from_client, char **fingerprints_from_website, int num_of_website_certs)
{
  //variable to store the result of the fingerprint comparison
  int result_of_comparison = 0;

  //this variable is used to store the fingerprint from the client
  //in order for the fingerprint to be changed to uppercase
  char* client_fingerprint = malloc(sizeof(char) * FPT_LENGTH);
  strcpy(client_fingerprint, fingerprint_from_client);
  to_upper_case(client_fingerprint);

  int i;
  for(i=0; i< num_of_website_certs; i++)
    {
      to_upper_case(fingerprints_from_website[i]);
      if(strcmp(client_fingerprint, fingerprints_from_website[i]) == 0)
        {
          result_of_comparison = 1;
          break;
        }
    }

  free(client_fingerprint);
  return result_of_comparison;
} // verify_certificate


/** 
 * @brief Verifies that a fingerprint has the correct format. 
 *
 * @return 1 if the fingerprint is correctly formatted, 0 otherwise. 
 */
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
      if (isxdigit (fingerprint[i]))
        i++;
      else
        return 0;

      if (isxdigit (fingerprint[i]))
        i++;
      else
        return 0;

      if (! (fingerprint[i] == ':') )
        return 0;

    }

  /* Check the last two characters. */
  if (isxdigit (fingerprint[i]))
    i++;
  else
    return 0;

  if (isxdigit (fingerprint[i]))
    return 1;
  else
    return 0;
} // verify_fingerprint_format
