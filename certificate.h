/******************************************************************************
 * Authors: Tolu Alabi
 *          Zach Butler
 *          Martin Dluhos
 *
 * Created: February 21, 2012
 * Revised: March 4, 2012
 * Description: Handle sending a certificate request to the website and
 *              receiving its response.
 ******************************************************************************/

/* Requests a certificate from the website given by the url. */
int 
request_certificate ();

/* Verifies that the received certificate from the website matches with the
 * fingerprint from the user.
 */
int
verify_certificate ();
