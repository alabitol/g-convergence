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

/* Requests a certificate from the website given by the url. Saved the
 * fingerprint of the certificate into fingerprint_from_website. Return 1 if
 * certificate was obtained. Otherwise, return 0.
 */
int 
request_certificate (char *url, char *fingerprint_from_website)
{
  /* We need to figure out how to send an HTTP request to the
   * website. There is no microhttpd function for this.
   */
  return 1;
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
