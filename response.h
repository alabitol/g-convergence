/******************************************************************************
 * Authors: Tolu Alabi
 *          Zach Butler
 *          Martin Dluhos
 *
 * Created: March 4, 2012
 * Revised: March 4, 2012
 * Description: This file contains functions which format the verification
 * response and functions which send the response back to the client.
 ******************************************************************************/

/* Formats the response that needs to be sent back to the client. */
int 
format_response ();

/* Sends response back to the client. This function could be a wrapper for
 * send_page.
 */
int
send_response ();

