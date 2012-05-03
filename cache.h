/******************************************************************************
 * Authors: g-coders
 * Created: March 11, 2012
 * Revised: April 22, 2012
 * Description: This is the header file which contains functions which are 
 * responsible for managing the cache of verified websites.
 ******************************************************************************/
#ifndef CACHE_H
#define CACHE_H

#include "notary.h"
#include <mysql.h> //FIXME?

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Helpers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Opens an SQL conenction, and returns a pointer to it. Exits and returns 
 * an error if connection cannot be established.
 */
MYSQL* start_mysql_connection();

/* Closes the db connection.
 */
void close_mysql_connection();

/* Determines whether given fingerprint can be safely inserted.
 * Returns true if it is safe, false otherwise.
 */
boolean is_fingerprint_safe(char *fingerprint);

/* Determines whether given url can be safely inserted.
 * Returns true if it is safe, false otherwise.
 */ 
boolean is_url_safe(char *url);



/* Checks if a particular fingerprint is in the cache already, 
 * either as the fingerprint of a trusted url or a blacklisted url. 
 * Returns true if the cache has a fingerprint for the url, 
 * returns false if it's not. Returns an error if we couldn't connect to db. 
 */
boolean is_in_cache (char *url);

/* Checks if we have a record of a url in the blacklist. Returns true if 
 * the url is in the blacklist and false if it's not. Returns an error
 * if we could not connect to db.
 */
boolean is_blacklisted (char *url); 

/* Inserts a certificate fingerprint into the cache.
 * Inserts into trusted cache is location is set to true,
 *  inserts into blacklisted cache if location is false.
 * Returns 0 if insert is successful, otherwise returns an error.
 */ 
int cache_insert (char* url, char* fingerprint, boolean location);

/* Remove a specific certificate fingerprint from the cache. 
 * Returns 0 if removal is successful, otherwise returns an error. 
 */
int cache_remove (char* fingerprint);

/* Removes cache entries that are older than 1 day.
 */
int cache_update ();

#endif // CACHE_H
