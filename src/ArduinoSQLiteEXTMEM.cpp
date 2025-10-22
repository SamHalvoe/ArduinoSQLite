#include <Arduino.h> // for: Teensy extmem functions

#include <smalloc.h> // for: size_t sm_szalloc(const void *); /* get size of allocation */

#include "sqlite3.h" // for: SQLite related stuff

/* This information may be useful:

**** The default memory allocator ****

By default, SQLite uses the malloc(), realloc(), and free() routines from the standard C library for its memory allocation needs.
These routines are surrounded by a thin wrapper that also provides a "memsize()" function that will return the size of an existing allocation.
The memsize() function is needed to keep an accurate count of the number of bytes of outstanding memory; memsize() determines how many bytes to remove from the outstanding count when an allocation is freed.
The default allocator implements memsize() by always allocating 8 extra bytes on each malloc() request and storing the size of the allocation in that 8-byte header.

*/


// SQLite malloc wrapper for EXTMEM
static void* sqlite3_extmem_malloc(int size)
{
  Serial.println("sqlite3_extmem_malloc");
  return extmem_malloc(size);
}

// SQLite free wrapper for EXTMEM
static void sqlite3_extmem_free(void* ptr)
{
  Serial.println("sqlite3_extmem_free");
  extmem_free(ptr);
}

// SQLite realloc wrapper for EXTMEM
static void* sqlite3_extmem_realloc(void* ptr, int new_size)
{
  Serial.println("sqlite3_extmem_realloc");
  return extmem_realloc(ptr, new_size);
}

// Return the size of an allocation
static int sqlite3_extmem_size(void* ptr)
{
  Serial.println("sqlite3_extmem_size");
  if (ptr == nullptr) { return 0; }
  return static_cast<int>(sm_szalloc(ptr));
}

// Round up request size to allocation size
static int sqlite3_extmem_roundup(int size)
{
  Serial.println("sqlite3_extmem_roundup");
  return size;
}

// Initialize the memory allocator
static int sqlite3_extmem_init(void* pAppData)
{
  // is already initialized by Teensy
  return SQLITE_OK;
}

// Shutdown the memory allocator
static void sqlite3_extmem_shutdown(void* pAppData)
{
  // Nothing to do - managed by Teensy
}

// SQLite memory methods structure
static const sqlite3_mem_methods sqlite3_extmem_methods = {
    sqlite3_extmem_malloc,
    sqlite3_extmem_free,
    sqlite3_extmem_realloc,
    sqlite3_extmem_size,
    sqlite3_extmem_roundup,
    sqlite3_extmem_init,
    sqlite3_extmem_shutdown,
    nullptr  // pAppData - not needed
};

// Function to configure SQLite to use EXTMEM allocators
int sqlite3_use_extmem()
{
  return sqlite3_config(SQLITE_CONFIG_MALLOC, &sqlite3_extmem_methods);
}
