1/******************************************************************************
 * Authors: g-coders
 * Created: February 21, 2012
 * Revised: March 11, 2012
 * Description: Handle sending a certificate request to the website and
 *              receiving its response.
 ******************************************************************************/
#ifndef CERTIFICATE_H
#define CERTIFICATE_H

#include "notary.h"

/* Determine if a character is a hexadecimal character. 
 * Return 1 if it is, else return 0.
 */
int is_hex_char (char c);

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
static size_t wrfu (void *ptr,  size_t  size,  size_t  nmemb,  void *stream);

/**
 * Combines curl cleanup calls. 
 */
static void curl_cleanup(CURL* curl_handle);

/**
 * Changes fingerprint (hex characters) to upper case.
 */
static void to_upper_case(char* fingerprint);

/**
 * Changes fingerprint (hex characters) to lower case.
 */
static void to_lower_case(char* fingerprint);

/**
 * This function converts the PEM certificate to its corresponding SHA1 
 * fingerprint 
 * Returns pointer to the fingerprint. 
 */
static char* get_fingerprint_from_cert (char* cert);

/* Requests a certificate from the website given by the url. Saved the
 * fingerprint of the certificate into fingerprint_from_website. 
 * Returns pointer to the fingerprint if successful, otherwise returns an error.
 */
char* request_certificate (const char *url);

/* Verifies that the received certificate from the website matches with the
 * fingerprint from the user. Returns 1 if fingerprints match. Otherwise,
 * returns 0.
 */
int verify_certificate (const char *fingerprint_from_client, char *fingerprint_from_website);

/* Verifies that a fingerprint has the correct format. */
int verify_fingerprint_format (char *fingerprint);
#endif // CERTIFICATE_H
