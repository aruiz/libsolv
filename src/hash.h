/*
 * Copyright (c) 2007, Novell Inc.
 *
 * This program is licensed under the BSD license, read LICENSE.BSD
 * for further information
 */

/*
 * hash.h
 * generic hash functions
 */

#ifndef LIBSOLV_HASH_H
#define LIBSOLV_HASH_H

#include "pooltypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* value of a hash */
typedef unsigned int Hashval;

/* inside the hash table, Ids are stored. Hash maps: string -> hash -> Id */
typedef Id *Hashtable;

/* hash chain */
#define HASHCHAIN_START 7
#define HASHCHAIN_NEXT(h, hh, mask) (((h) + (hh)++) & (mask))

/* FNV-1a string hash */

#define STRHASH_INIT 2166136261u
#define STRHASH_PRIME 16777619u

static inline Hashval
strhash(const char *str)
{
  Hashval h = STRHASH_INIT;
  unsigned int c;
  while ((c = *(const unsigned char *)str++) != 0)
    {
      h ^= c;
      h *= STRHASH_PRIME;
    }
  return h;
}

static inline Hashval
strnhash(const char *str, unsigned int len)
{
  const unsigned char *p = (const unsigned char *)str;
  Hashval h = STRHASH_INIT;
  while (len >= 8)
    {
      h ^= p[0]; h *= STRHASH_PRIME;
      h ^= p[1]; h *= STRHASH_PRIME;
      h ^= p[2]; h *= STRHASH_PRIME;
      h ^= p[3]; h *= STRHASH_PRIME;
      h ^= p[4]; h *= STRHASH_PRIME;
      h ^= p[5]; h *= STRHASH_PRIME;
      h ^= p[6]; h *= STRHASH_PRIME;
      h ^= p[7]; h *= STRHASH_PRIME;
      p += 8;
      len -= 8;
    }
  while (len--)
    {
      h ^= *p++;
      h *= STRHASH_PRIME;
    }
  return h;
}

static inline Hashval
strhash_cont(const char *str, Hashval h)
{
  unsigned int c;
  while ((c = *(const unsigned char *)str++) != 0)
    {
      h ^= c;
      h *= STRHASH_PRIME;
    }
  return h;
}


/* hash for rel
 * rel -> hash
 */
static inline Hashval
relhash(Id name, Id evr, int flags)
{
  return name + 7 * evr + 13 * flags;
}


/* compute bitmask for value
 * returns smallest (2^n-1) > 2 * num + 3
 *
 * used for Hashtable 'modulo' operation
 */
static inline Hashval
mkmask(unsigned int num)
{
  num = num * 2 + 3;
  while (num & (num - 1))
    num &= num - 1;
  return num * 2 - 1;
}

#ifdef __cplusplus
}
#endif

#endif /* LIBSOLV_HASH_H */
