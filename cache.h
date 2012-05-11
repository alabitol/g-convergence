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
#include "certificate.h"
#include <string.h>
#include <mysql/mysql.h>
#include <time.h>
#include <stdbool.h>

/* We maintain two tables in the db
   TRUSTED:     caches (url, fingerprint) pairs from trusted sites
   BLACKLISTED: stores blacklisted urls
*/
/* The time we want to keep records in the TRUSTED cache. */
#define CACHE_TIME 1 //FIXME one day
#define CACHE_TRUSTED 1 // Indicate element is in the trusted database
#define CACHE_BLACKLIST 2 // Indicate element is in the blacklisted database

/* Opens an SQL conenction, and returns a pointer to it. Exits and returns 
 * an error if connection cannot be established.
 * @returns a mysql connection pointer
 */
MYSQL* start_mysql_connection();

/* Closes the db connection.
 * @param connection a pointer to MYSQL connection
 */ 
void close_mysql_connection(MYSQL *connection);

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
int is_in_cache (char *url, char *fingerprint);

/* Checks if we have a record of a url in the blacklist. Returns 1 if 
 * the url is in the blacklist and 0 if it is not. 
 * Returns -1 if error is encountered.
 */
int is_blacklisted (char *url); 

/* Inserts a certificate fingerprint into the cache.
 * Inserts into trusted cache is db is set to true;
 * otherwise, inserts into blacklist cache.
 * Returns 1 if insert is successful. Otherwise, returns 0.
 */ 
int cache_insert (char* url, char* fingerprint, int db);

/* Remove a specific certificate fingerprint from the cache. 
 * Removes from trusted cache if db is set to true;
 * otherwise, removes from blacklist cache.
 * Returns 1 if removal is successful, 0 if fingerprint cannot be removed,
 * and -1 if the fingerprint does not exist in the database. 
 */
int cache_remove (char* fingerprint, int db);

/* Removes cache entries that have expired from trusted cache.
 * @returns 1  if update is successful, otherwise 0
 */
int cache_update_url (char *url, char *fingerprints);

#endif // CACHE_H
