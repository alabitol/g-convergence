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
#include <mysql.h> 

#define TRUSTED true
#define BLACKLISTED false

/* Determines whether given fingerprint can be safely inserted.
 * Returns 1 if it is safe. Otherwise returns 0.
 */
int is_fingerprint_safe(char *fingerprint);

/* Determines whether given url can be safely inserted.
 * Returns 1 if it is safe. Otherwise returns 0.
 */ 
int is_url_safe(char *url);

/* Checks if a particular fingerprint is in the cache already, 
 * either as the fingerprint of a trusted url or a blacklisted url. 
 * Returns 1 if the cache has a fingerprint for the url, 
 * otherwise returns 0. Returns -1 if error is encountered.
 */
int is_in_cache (char *fingerprint);

/* Checks if we have a record of a url in the blacklist. Returns 1 if 
 * the url is in the blacklist and 0 if it is not. 
 * Returns -1 if error is encountered.
 */
int is_blacklisted (char *url); 

/* Inserts a certificate fingerprint into the cache.
 * Inserts into trusted cache is trusted_db is set to true;
 * otherwise, inserts into blacklist cache.
 * Returns 1 if insert is successful. Otherwise, returns 0.
 */ 
int cache_insert (char* url, char* fingerprint, boolean trusted_db);

/* Remove a specific certificate fingerprint from the cache. 
 * Removes from trusted cache if trusted_db is set to true;
 * otherwise, removes from blacklist cache.
 * Returns 1 if removal is successful, 0 if fingerprint cannot be removed,
 * and -1 if the fingerprint does not exist in the database. 
 */
int cache_remove (char* fingerprint, boolean trusted_db);

/* Removes cache entries that have expired from trusted cache.
 */
int cache_update ();

#endif // CACHE_H
