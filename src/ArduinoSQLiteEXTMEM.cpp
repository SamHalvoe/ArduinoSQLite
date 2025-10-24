#include <Arduino.h> // for: Teensy extmem functions

#include <smalloc.h> // for: sm_szalloc_pool(&extmem_smalloc_pool, in_pointer) /* get size of allocation */

#include "sqlite3.h" // for: SQLite related stuff

// SQLite malloc wrapper for EXTMEM
static void* sqlite3_extmem_malloc(int in_size)
{
  //Serial.println("sqlite3_extmem_malloc");
  return extmem_malloc(in_size);
}

// SQLite free wrapper for EXTMEM
static void sqlite3_extmem_free(void* in_pointer)
{
  //Serial.println("sqlite3_extmem_free");
  extmem_free(in_pointer);
}

// SQLite realloc wrapper for EXTMEM
static void* sqlite3_extmem_realloc(void* in_pointer, int in_newSize)
{
  //Serial.println("sqlite3_extmem_realloc");
  return extmem_realloc(in_pointer, in_newSize);
}

// Return the size of an allocation
static int sqlite3_extmem_size(void* in_pointer)
{
  //Serial.println("sqlite3_extmem_size");
  return static_cast<int>(sm_szalloc_pool(&extmem_smalloc_pool, in_pointer));
}

// Round up request size to allocation size
static int sqlite3_extmem_roundup(int in_size)
{
  //Serial.println("sqlite3_extmem_roundup");
  return in_size;
}

// Initialize the memory allocator
static int sqlite3_extmem_init(void* in_appData)
{
  // is already initialized by Teensy
  return SQLITE_OK;
}

// Shutdown the memory allocator
static void sqlite3_extmem_shutdown(void* in_appData)
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
    nullptr  // in_appData - not needed
};

// Function to configure SQLite to use EXTMEM allocators
int sqlite3_use_extmem()
{
  return sqlite3_config(SQLITE_CONFIG_MALLOC, &sqlite3_extmem_methods);
}
