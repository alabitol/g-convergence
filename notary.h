/******************************************************************************
 * Authors: g-coders
 * Created: March 11, 2012
 * Revised: March 11, 2012
 * Description: This is a header file which will be included in all other
 * header files of the project
 ******************************************************************************/

#ifndef NOTARY_H
#define NOTARY_H

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


/* The number of simultaneous requests. */
#define MAX_CLIENTS 15
#define POST_BUFFER_SIZE 512
#define GET 0
#define POST 1

#define PORT 8888

/* This datastructure contains information about an individual connection from
 * a client. 
 */
struct connection_info_struct
{
  int connection_type;
  const char *answer_string;
  int answer_code;
};


#endif // NOTARY_H