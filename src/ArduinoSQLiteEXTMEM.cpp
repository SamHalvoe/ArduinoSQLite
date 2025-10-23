#include <Arduino.h> // for: Teensy extmem functions

#include "sqlite3.h" // for: SQLite related stuff

/* This information may be useful (from SQLite documentation):

**** The default memory allocator ****

By default, SQLite uses the malloc(), realloc(), and free() routines from the standard C library for its memory allocation needs.
These routines are surrounded by a thin wrapper that also provides a "memsize()" function that will return the size of an existing allocation.
The memsize() function is needed to keep an accurate count of the number of bytes of outstanding memory; memsize() determines how many bytes to remove from the outstanding count when an allocation is freed.
The default allocator implements memsize() by always allocating 8 extra bytes on each malloc() request and storing the size of the allocation in that 8-byte header.

*/

// Our allocator implements memsize() (=> sqlite3_extmem_size()) by always allocating 4 extra bytes
// on each malloc() request and storing the size of the allocation in that 4-byte header.
// => SizeHeaderType is the type for storing the size of the allocation, at the beginning of the allocation.
using SizeHeaderType = uint32_t; // sizeof(uint32_t) == 4 bytes

static inline void* excludeHeader(void* in_pointer)
{
  return static_cast<void*>(++static_cast<SizeHeaderType*>(in_pointer));
}

static inline void* includeHeader(void* in_pointer)
{
  return static_cast<void*>(--static_cast<SizeHeaderType*>(in_pointer));
}

// SQLite malloc wrapper for EXTMEM
static void* sqlite3_extmem_malloc(int in_size)
{
  Serial.println("sqlite3_extmem_malloc");
  void* pointer = extmem_malloc(c_sizeHeaderSize + in_size);
  *static_cast<SizeHeaderType*>(pointer) = static_cast<SizeHeaderType>(in_size);
  return excludeHeader(pointer);
}

// SQLite free wrapper for EXTMEM
static void sqlite3_extmem_free(void* in_pointer)
{
  Serial.println("sqlite3_extmem_free");
  extmem_free(includeHeader(in_pointer));
}

// SQLite realloc wrapper for EXTMEM
static void* sqlite3_extmem_realloc(void* in_pointer, int in_newSize)
{
  Serial.println("sqlite3_extmem_realloc");
  return extmem_realloc(includeHeader(in_pointer), in_newSize);
}

// Return the size of an allocation
static int sqlite3_extmem_size(void* in_pointer)
{
  Serial.println("sqlite3_extmem_size");
  if (in_pointer == nullptr) { return 0; }
  return static_cast<int>(*(--static_cast<SizeHeaderType*>(in_pointer)));
}

// Round up request size to allocation size
static int sqlite3_extmem_roundup(int in_size)
{
  Serial.println("sqlite3_extmem_roundup");
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
