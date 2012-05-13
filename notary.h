/******************************************************************************
 * Authors: g-coders
 * Created: March 11, 2012
 * Revised: April 20, 2012
 * Description: This is a header file which will be included in all other
 * header files of the project
 ******************************************************************************/

#ifndef NOTARY_H
#define NOTARY_H

#define _GNU_SOURCE

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <microhttpd.h>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/bio.h>
#include <curl/curl.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


/* Global variables representing the locations of the key file and
 * certificate file */
char *keyfile, *certfile;
/* The number of simultaneous requests. */
#define MAX_CLIENTS 15
#define POST_BUFFER_SIZE 512

#define PORT 8888

char *key_filename;
char *certificate_file;

/* GET and POST are the only requests notary handles. */
enum connection_type
  {
    GET = 0,
    POST = 1
  };

/* This datastructure contains information about an individual connection from
 * a client. 
 */
struct connection_info_struct
{
  enum connection_type connection_type;
  const char *answer_string;
  int answer_code;
};

/* This datastructure contains the url and port of the host we need to
   verify. */
typedef struct
{
  char *url;
  long port;
} host;

/* Length of certificate fingerprints we are dealing with including the
   trailing null character */
#define FPT_LENGTH (59+1)

#endif // NOTARY_H
