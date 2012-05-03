/******************************************************************************
 * authors: Tolu Alabi
 *          Zach Butler
 *          Martin Dluhos
 *          Chase Felker
 *          Radhika Krishna
 * created: February 21, 2012
 * revised: April 18, 2012
 * Description: This file contains functions which are responsible for
 * managing the cache of verified websites.
 ******************************************************************************/
#include <string.h>
#include <mysql.h>
#include <time.h>

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

//TODO
//get a MYSQL C library, change functions
//create the cache DB table
//retrieve from cache (given fingerprint).
//make sure SQL commands with user-text are safe.

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Helpers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Opens an SQL conenction, and returns a pointer to it. Exits and returns 
 * an error if connection cannot be established.
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
}//start_mysql_connection

/* Close the db connection */
void 
close_mysql_connection(conn)
{
  mysql_close(conn);
}

/* Determines whether given fingerprint can be safely inserted.
 * Returns true if it is safe, false otherwise.
 */
boolean 
is_fingerprint_safe(char *fingerprint)
{
  return if (!verify_fingerprint_format(fingerprint)); 
}

/* Determines whether given url can be safely inserted.
 * Returns true if it is safe, false otherwise.
 */ 
boolean 
is_url_safe(char *url) 
{
  
}



/* 
 * Checks if a particular fingerprint is in the cache already, 
 * either as the fingerprint of a trusted url or a blacklisted url. 
 * Returns true if the cache has a fingerprint for the url, 
 * returns false if it's not. Returns an error if we couldn't connect to db. 
 */
boolean 
is_in_cache (char *url)
{
  MYSQL *conn = start_mysql_connection();
  MYSQL_RES *result;
  MYSQL_ROW row;
  int num_fields;

  if(conn)
    {
      char *query_string = strcat ("SELECT url FROM trusted WHERE ", url);
      query_string = strcat(query_string, ";"); 

      mysql_query(conn, query_string);
      result = mysql_store_result(conn);
      num_fields = mysql_num_fields(result);

      close_mysql_connection(conn);
  
      return if(num_fields || is_blacklisted(url));
    }

    exit(1);
}//is_in_cache

/* Checks if we have a record of a url in the blacklist. Returns true if 
 * the url is in the blacklist and false if it's not. Returns an error
 * if we could not connect to db.
 */
boolean 
is_blacklisted (char *url) {
  MYSQL *conn = start_mysql_connection();
 
  if (conn)
    {
      char *query_string = strcat ("SELECT url FROM blacklist WHERE ", url);
      query_string = strcat(query_string, ";"); 

      mysql_query(conn, query_string);
      result = mysql_store_result(conn);
      num_fields_blacklist = mysql_num_fields(result);

      close_mysql_connection(conn);
      return if(num_fields_blacklist);
    }

  exit(1);
}//is_blacklisted

/* Inserts a certificate fingerprint into the cache.
 * Inserts into trusted cache is location is set to true,
 *  inserts into blacklisted cache if location is false.
 * Returns 0 if insert is successful, otherwise returns an error.
 */ 
int
cache_insert (char* url, char* fingerprint, boolean location)
{
  time_t clock;
  int return_value;
  char *time;
  MYSQL *conn = start_mysql_connection();
  if (conn)
    {
      clock = time(NULL);
      *time = &clock;

      if(location)
        char *query_string = strcat("INSERT INTO trusted (url, fingerprint, timestamp) VALUES(", url, fingerprint, time, ")");
      else {
        char *query_string = strcat("INSERT INTO blacklist (url, fingerprint, timestamp) VALUES(", url, fingerprint, time, ")");
      }

      return_value = mysql_query(conn, query_string);
  
      close_mysql_connection(conn);
      return return_value;
    }

  exit(1);
}//cache_insert

/* Remove a specific certificate fingerprint from the cache. 
 * Returns 0 if removal is successful, otherwise returns an error. 
 */
int 
cache_remove (char* fingerprint)
{
  MYSQL *conn = start_mysql_connection();
  if (conn)
    {
      char *query_string = strcat("DELETE FROM trusted(", fingerprint);
      query_string = strcat(query_string, ")");
      int return_value = 
        mysql_query(conn, query_string); //FIXME
      close_mysql_connection(conn);
      return return_value;
    }

  exit(1);
}//cache_remove

/* Remove outdated certificates. */
int 
cache_update ()
{
  MYSQL *conn = start_mysql_connection();
  time_t clock = time(NULL);
  char *time = &clock;
  int return_value;
  
  if(conn) 
    {
      char *query_string = strcat("SELECT * FROM trusted WHERE timestamp < ");
      query_string = strcat(query_string, );

      return_value = mysql_query(conn, "SELECT * FROM __ WHERE timestamp >__");
      return_value = 
        mysql_query(conn, "DELETE FROM trusted(fingerprint)"); //FIXME
      
      close_mysql_connection(conn);
      return return_value;
    }

  exit(1);
}//cache_update
