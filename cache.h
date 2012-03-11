/******************************************************************************
 * Authors: g-coders
 * Created: March 11, 2012
 * Revised: March 11, 2012
 * Description: This is the header file which contains functions which are 
 * responsible for managing the cache of verified websites.
 ******************************************************************************/
#ifndef CACHE_H
#define CACHE_H

#include "notary.h"

/* Check if the certificate is already in cache. */
int is_in_cache ();

/* Insert a certificate fingerprint into the cache. */
int cache_insert ();

/* Remove a specific certificate fingerprint from the cache. */
int cache_remove ();

/* Check the cache periodically to remove outdated certificates. */
int cache_update ();

#endif // CACHE_H
