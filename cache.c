/******************************************************************************
 * authors: g-coders
 * created: February 21, 2012
 * revised: April 18, 2012
 * Description: This file contains functions which are responsible for
 * managing the cache of verified websites.
 ******************************************************************************/
#include "notary.h"
#include <string.h>
#include <mysql/mysql.h>
#include <time.h>
#include <stdbool.h>

#define CACHE_TIME 1; //FIXME one day

/* Structure of table
 *   trusted
 *   +--------------------------------------------------------------+
 *   | id  | url                  | fingerprint      |  time added  | 
 *   +--------------------------------------------------------------+
 *   | 1   | https://facebook.com | BL:AH:BL:AH: ..  | TIME         |
 *   | 2   | https://chase.com    | ME:OW:WO:OF: ..  | TIME         |
 *   |   ....                                                       |
 *   +--------------------------------------------------------------+
 * */

// TODO
// separate making a connection to trusted database and blacklisted database

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Helpers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Opens an SQL conenction, and returns a pointer to it. Exits and returns 
 * an error if connection cannot be established.
 * @return a mysql connection pointer
 */
MYSQL* 
start_mysql_connection()
{
  //Create a new MYSQL connection struct and set it to the result of the 
  //connection call.
  MYSQL *conn;
  conn = mysql_init(NULL);

  //If the connection wasn't made correctly, return an error
  if (!conn) 
    {
      printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
      return NULL;
    }
  if (mysql_real_connect(conn, "localhost", "zetcode", "passwd", 
                         NULL, 0, NULL, 0) == NULL) {
    printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
    exit(1);
  }

  //Return the connection struct
  return conn;
} //start_mysql_connection

/* Closes the db connection.
 * @param connection a pointer to MYSQL connection
 */ 
void 
close_mysql_connection(MYSQL *connection)
{
  mysql_close(connection);
} // close_mysql_connection

/* Determines whether given fingerprint can be safely inserted.
 * @return 1 if it is safe, 0 otherwise
 */
int is_fingerprint_safe(char *fingerprint)
{
  if (verify_fingerprint_format(fingerprint))
    return 1;
  else
    return 0;
} // is_fingerprint_safe

/* Determines whether given url can be safely inserted.
 * Returns 1 if it is safe. Otherwise returns 0.
 */ 
int is_url_safe(char *url) 
{
  /* STUB */
} // is_url_safe

/**
 * Checks if a particular fingerprint is in the cache already, 
 * either as the fingerprint of a trusted url or a blacklisted url. 
 * Returns 1 if the cache has a fingerprint for the url, 
 * otherwise returns 0. Returns -1 if error is encountered.
 */
int is_in_cache (char *fingerprint)
{
  MYSQL_RES *result;
  MYSQL_ROW row;
  int num_fields;
  MYSQL *conn = start_mysql_connection();

  if (conn)
    {
      char *query_string = strcat ("SELECT url FROM trusted WHERE ", url);
      query_string = strcat(query_string, ";"); 

      mysql_query(conn, query_string);
      result = mysql_store_result(conn);
      num_fields = mysql_num_fields(result);

      close_mysql_connection(conn);
  
      if (num_fields || is_blacklisted(url))
	return 1;
      else 
	return 0;
    }
  else
  {
    fprintf(stderr, "Could not connect to database.\n");
    return -1;
  }
} //is_in_cache

/* Checks if we have a record of a url in the blacklist. Returns 1 if 
 * the url is in the blacklist and 0 if it is not. 
 * Returns -1 if error is encountered.
 */
int is_blacklisted (char *url)
{
 MYSQL *conn = start_mysql_connection();

 if (conn)
 {
   MYSQL_RES *result;
   unsigned int num_fields;

   char *query_string = strcat ("SELECT url FROM blacklist WHERE ", url);
   query_string = strcat(query_string, ";"); 

   mysql_query(conn, query_string);
   result = mysql_store_result(conn);
   num_fields = mysql_num_fields(result);

   close_mysql_connection(conn);
   
   return num_fields;

 }

 exit(1);
} //is_blacklisted

/* Inserts a certificate fingerprint into the cache.
 * Inserts into trusted cache is trusted_db is set to true;
 * otherwise, inserts into blacklist cache.
 * Returns 1 if insert is successful. Otherwise, returns 0.
 */ 
int cache_insert (char* url, char* fingerprint, bool trusted_db)
{
  char *query_string = NULL;
  char *db = malloc(sizeof(char) * 9+1);
  char *timestamp;
  int return_val;
  MYSQL *conn = start_mysql_connection();
  if (conn)
    {
      /* Compute timestamp. */

      /* Determine which db to insert into. */
      if(trusted_db)
      {
	strncpy(db, "trusted", 7+1);
      }
      else 
      {
	strncpy(db, "blacklist", 9+1);
      }

      /* Construct the query string. */
      asprintf(&query_string, "INSERT INTO %s VALUES (%s, %s, %s )", db, url, fingerprint, timestamp);

      return_val = mysql_query(conn, query_string);
      close_mysql_connection(conn);

      return !return_val;
    } // if

  return 0;
} //cache_insert

/* Remove a specific certificate fingerprint from the cache. 
 * Removes from trusted cache if trusted_db is set to true;
 * otherwise, removes from blacklist cache.
 * Returns 0 if removal is successful, otherwise returns -1.
 */
int cache_remove (char* fingerprint, bool trusted_db)
{
  int return_val;
  MYSQL *conn = start_mysql_connection();

  if (conn)
    {
      char *query_string = NULL;
      asprintf(&query_string, "DELETE FROM trusted( %s )", fingerprint);
      return_val = mysql_query(conn, query_string); //FIXME
      close_mysql_connection(conn);
      return !return_val;
    }

  return 0;
} //cache_remove

/* Removes cache entries that have expired from trusted cache. 
 * @return 1 on success, 0 on failure
 */
int cache_update ()
{
  MYSQL *conn = start_mysql_connection();
  int return_val;
  
  if(conn) 
    {
      /* THIS CODE IS INCOMPLETE! */
      char *query_string = NULL;
      asprintf(&query_string, "SELECT * FROM trusted WHERE timestamp < ");

      return_val = mysql_query(conn, "SELECT * FROM __ WHERE timestamp >__");
      return_val = 
        mysql_query(conn, "DELETE FROM trusted(fingerprint)"); //FIXME
      
      close_mysql_connection(conn);

      return !return_val;
    }

  return 0;
} //cache_update
