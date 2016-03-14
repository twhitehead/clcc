#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>
#include <sysexits.h>

#include <fcntl.h>
#include <unistd.h>

#include <argp.h>
#include <CL/opencl.h>


#define STRINGIFY(x) STRINGIFY_EXPANDED(x)
#define STRINGIFY_EXPANDED(x) #x


//---------------------------------------------------------------------------------------------------------------//
typedef struct Vector_ Vector;
typedef struct MVector_ MVector;

typedef struct MString_ MString;
typedef struct String_ String;

typedef struct MaybeString_* MaybeString;

typedef struct MVectorString_ MVectorString;
typedef struct VectorString_ VectorString;

typedef enum Command_ Command;

typedef enum Settings_CL_ Settings_CL;
typedef struct MSettings_ MSettings;
typedef struct Settings_ Settings;

typedef struct VectorSize_ VectorSize;

typedef struct VectorCLPlatform_ VectorCLPlatform;
typedef struct VectorCLDevice_ VectorCLDevice;

#ifdef CL_VERSION_1_2
typedef struct VectorCLPartitionProperty_ VectorCLPartitionProperty;
#endif // CL_VERSION_1_2


//---------------------------------------------------------------------------------------------------------------//
// Vector (all instances are just type specialized)
#define Vector_BLOCK 16

struct MVector_ {
  size_t number;
  void* elements;
};

struct Vector_ {
  size_t number;
  const void* elements;
};


// String (no terminating 0)
#define MString_BLOCK 16

struct MString_ {
  size_t number;
  char* elements;
};

struct String_ {
  size_t number;
  const char* elements;
};

struct MaybeString_ {
  String value;
};

#define MVectorString_BLOCK 16

struct MVectorString_ {
  size_t number;
  String* elements;
};

struct VectorString_ {
  size_t number;
  const String* elements;
};


// Types for argp parser
enum Command_ {
  Command_UNSET = 0,
  Command_LIST
};

struct MSettings_ {
  Command command;
  MVectorString sources;
  MaybeString platform;
  MaybeString device;
  MVectorString options;
};

struct Settings_ {
  Command command;
  VectorString sources;
  MaybeString platform;
  MaybeString device;
  VectorString options;
};


// Lists returned by OpenCL
struct VectorSize_ {
  size_t number;
  const size_t* elements;
};

struct VectorCLPlatform_ {
  size_t number;
  const cl_platform_id* elements;
};

struct VectorCLDevice_ {
  size_t number;
  const cl_device_id* elements;
};

#ifdef CL_VERSION_1_2
struct VectorCLPartitionProperty_ {
  size_t number;
  const cl_device_partition_property* elements;
};
#endif // CL_VERSION_1_2


//---------------------------------------------------------------------------------------------------------------//
// String routines

// CString
static const char* CString_string(String string);
static void CString_free(const char* cstring);

// Mutable String
static MString MString_raw(size_t number, char* elements);
static MString MString_empty();
static MString MString_string(String string);
static MString MString_cstring(const char* cstring);
static String MString_freeze(MString string);

static MString MString_push(MString mstring0, char element);
static MString MString_append(MString mstring0, String string1);
static MString MString_cappend(MString mstring0, const char* cstring1);

// String
static String String_raw(size_t number, const char* elements);
static String String_string(String string);
static String String_cstring(const char* cstring);
static void String_free(String string);

static String String_append(String string0, String string1);
static String String_cappend(String string0, const char* cstring1);

static String String_intercalate(String deliminator, VectorString source);
static String String_cintercalate(const char* deliminator, VectorString source);

static VectorString String_split(String deliminator, String source);
static VectorString String_csplit(const char* deliminator, String source);

static String String_file(String name);

int String_compare(String string0, String string1);
int String_ccompare(String string0, const char* cstring1);

// Maybe String
static MaybeString MaybeString_raw(String string);
static MaybeString MaybeString_string(String string);
static MaybeString MaybeString_cstring(const char*  cstring);
static MaybeString MaybeString_nothing();
static void MaybeString_free( MaybeString maybe);

static int MaybeString_isJust(MaybeString maybe);
static int MaybeString_IsNothing(MaybeString maybe);
static String MaybeString_assert(MaybeString maybe);

static MaybeString MaybeString_append(MaybeString maybe0, MaybeString maybe1);
static MaybeString MaybeString_cappend(MaybeString maybe0, const char* cstring1);

// Mutable Vector String
static MVectorString MVectorString_raw(size_t number, String* elements);
static MVectorString MVectorString_empty();
static VectorString MVectorString_freeze(MVectorString vector);

static MVectorString MVectorString_push(MVectorString mvector, String string);
static MVectorString MVectorString_cpush(MVectorString mvector, const char* cstring);
static MVectorString MVectorString_append(MVectorString mvector0, VectorString vector1);

// Vector String
static VectorString VectorString_raw(size_t number, const String* elements);
static void VectorString_free(VectorString vector);

//---------------------------------------------------------------------------------------------------------------//
// Error handling routines

static void Error_die(int value, const char* format, ...)
  __attribute__((format (printf,2,3)));
static void Error_vdie(int value, const char* format, va_list args);

// Errno
static void Error_dieErrno(int status, int value, const char* format, ...)
  __attribute__((format (printf,3,4)));
static void Error_vdieErrno(int status, int value, const char* format, va_list args);

// OpenCL
static const char* Error_stringCL(cl_int status);
static void Error_dieCL(cl_int status, int value, const char* format, ...)
  __attribute__((format (printf,3,4)));
static void Error_vdieCL(cl_int status, int value, const char* format, va_list args);

//---------------------------------------------------------------------------------------------------------------//
// Argp parser routines
static Settings Settings_initial();
static void Settings_free(Settings settings);

static error_t Settings_parser(int key, char* arg, struct argp_state* state);

//---------------------------------------------------------------------------------------------------------------//
// OpenCL routines
static VectorSize VectorSize_raw(size_t number, const size_t* elements);
static void VectorSize_free(VectorSize vector);

static VectorCLPlatform VectorCLPlatform_raw(size_t number, const cl_platform_id* elements);
static void VectorCLPlatform_free(VectorCLPlatform platforms);

static VectorCLDevice VectorCLDevice_raw(size_t number, const cl_device_id* elements);
static void VectorCLDevice_free(VectorCLDevice devices);

#ifdef CL_VERSION_1_2
static VectorCLPartitionProperty VectorCLPartitionProperty_raw(size_t number,
                                                              const cl_device_partition_property* elements);
static void VectorCLPartitionProperty_free(VectorCLPartitionProperty vector);
#endif // CL_VERSION_1_2

static VectorCLPlatform CL_platformsQuery();
static String CL_platformName(cl_platform_id platform_id);

static VectorCLDevice CL_devicesQuery(cl_platform_id platform_id);

static void CL_deviceProperty_Singleton(cl_device_id device_id, cl_device_info property,
                                        void* value, size_t value_size);
static void CL_deviceProperty_Vector(cl_device_id device_id, cl_device_info property,
                                     size_t* value_number, void** value,
                                     size_t value_size);
static void CL_deviceProperty_Vector0(cl_device_id device_id, cl_device_info property,
                                      size_t* value_number, void** value,
                                      size_t value_size);

static cl_device_id CL_deviceProperty_DeviceId(cl_device_id device_id, int property);
static cl_platform_id CL_deviceProperty_PlatformId(cl_device_id device_id, int property);
static cl_device_type CL_deviceProperty_DeviceType(cl_device_id device_id, int property);
static cl_device_fp_config CL_deviceProperty_FPConfig(cl_device_id device_id, int property);
static cl_device_mem_cache_type CL_deviceProperty_MemCacheType(cl_device_id device_id, int property);
static cl_device_local_mem_type CL_deviceProperty_MemLocalType(cl_device_id device_id, int property);
static cl_device_exec_capabilities CL_deviceProperty_ExecCapabilities(cl_device_id device_id, int property);
static cl_command_queue_properties CL_deviceProperty_QueueProperties(cl_device_id device_id, int property);
#ifdef CL_VERSION_1_2
static cl_device_affinity_domain CL_deviceProperty_AffinityDomain(cl_device_id device_id, int property);
#endif // CL_VERSION_1_2
static cl_bool CL_deviceProperty_Bool(cl_device_id device_id, int property);
static cl_uint CL_deviceProperty_UInt(cl_device_id device_id, int property);
static cl_ulong CL_deviceProperty_ULong(cl_device_id device_id, int property);
static size_t CL_deviceProperty_Size(cl_device_id device_id, int property);
static String CL_deviceProperty_String(cl_device_id device_id, int property);
static VectorSize CL_deviceProperty_VectorSize(cl_device_id device_id, int property);
static VectorString CL_deviceProperty_VectorColon(cl_device_id device_id, int property);
static VectorString CL_deviceProperty_VectorSpace(cl_device_id device_id, int property);
#ifdef CL_VERSION_1_2
static VectorCLPartitionProperty CL_deviceProperty_VectorPartitionProperty(cl_device_id device_id, int property);
#endif // CL_VERSION_1_2

#define CL_DEVICE_PROPERTY(ID, IDENT, TYPE, GROUP, DESC) \
  static TYPE CL_deviceProperty##IDENT(cl_device_id device_id);
#include "cldeviceprop.h"
#undef CL_DEVICE_PROPERTY

static cl_context CL_contextCreate(cl_platform_id platform, cl_device_id device);
static void CL_contextFree(cl_context context);

static cl_program CL_programCreate(cl_context context, cl_device_id device,
                                   VectorString codes, VectorString options);
static void CL_programFree(cl_program program);

//---------------------------------------------------------------------------------------------------------------//
// Action routines
static void Print_device_DeviceId(unsigned int indent, cl_device_id value);
static void Print_device_PlatformId(unsigned int indent, cl_platform_id value);
static void Print_device_FPConfig(unsigned int indent, cl_device_fp_config value);
static void Print_device_MemCacheType(unsigned int indent, cl_device_mem_cache_type value);
static void Print_device_MemLocalType(unsigned int indent, cl_device_local_mem_type value);
static void Print_device_ExecCapabilities(unsigned int indent, cl_device_exec_capabilities value);
static void Print_device_QueueProperties(unsigned int indent, cl_command_queue_properties value);
#ifdef CL_VERSION_1_2
static void Print_device_PartitionProperty(unsigned int indent, cl_device_partition_property value);
static void Print_device_AffinityDomain(unsigned int indent, cl_device_affinity_domain value);
#endif // CL_VERSION_1_2
static void Print_device_Bool(unsigned int indent, cl_bool value);
static void Print_device_UInt(unsigned int indent, cl_ulong value);
static void Print_device_ULong(unsigned int indent, cl_uint value);
static void Print_device_Size(unsigned int indent, size_t value);
static void Print_device_String(unsigned int indent, String value);
static void Print_device_VectorSize(unsigned int indent, VectorSize value);
static void Print_device_VectorString(unsigned int indent, VectorString value);
static void Print_device_VectorColon(unsigned int indent, VectorString value);
static void Print_device_VectorSpace(unsigned int indent, VectorString value);
#ifdef CL_VERSION_1_2
static void Print_device_VectorPartitionProperty(unsigned int indent, VectorCLPartitionProperty value);
#endif // CL_VERSION_1_2

#define CL_DEVICE_PROPERTY(ID, IDENT, TYPE, GROUP, DESC)                \
  static void Print_device##IDENT(unsigned int indent, TYPE value);
#include "cldeviceprop.h"
#undef CL_DEVICE_PROPERTY

static void Action_compile(Settings settings);
static void Action_list(Settings settings);

//---------------------------------------------------------------------------------------------------------------//
// CStrings (terminating 0)

// Construction/destruction
static const char* CString_string(const String string) {
  char *cstring;

  if ( (cstring = malloc(sizeof *cstring * (string.number+1) )) == 0 )
    Error_dieErrno(errno, EX_OSERR, "Unable to duplicate string of length %zd", string.number);
  memcpy(cstring, string.elements, sizeof *cstring * string.number);
  cstring[string.number] = 0;

  return cstring;
}


static void CString_free(const char* const cstring) {
  free((void*)cstring);
}


//---------------------------------------------------------------------------------------------------------------//
// Mutable String (no terminating 0)

// Construct/destruct string
static MString MString_raw(const size_t number, char* elements) {
  MString mstring = { number, elements };
  return mstring;
}


static MString MString_empty() {
  return MString_raw(0, 0);
}


static MString MString_string(const String string) {
  char* elements;

  if ( (elements = (char*)malloc(sizeof *elements * (string.number+MString_BLOCK-1) /
                                 MString_BLOCK * MString_BLOCK)) == 0)
    Error_dieErrno(errno, EX_OSERR, "Unable to allocate %zd bytes for MString",
                   sizeof *elements * (string.number+MString_BLOCK-1)/MString_BLOCK * MString_BLOCK);
  memcpy(elements, string.elements, sizeof *elements * string.number);

  return MString_raw(string.number, elements);
}


static MString MString_cstring(const char* const cstring) {
  return MString_string(String_raw(strlen(cstring), cstring));
}


static String MString_freeze(MString mstring) {
  // Release extra memory
  if ( (mstring.elements = (char*)realloc(mstring.elements, sizeof *mstring.elements * mstring.number)) == 0 )
    Error_dieErrno(errno, EX_OSERR, "Unable to reduce MString allocation to %zd bytes",
                   sizeof *mstring.elements * mstring.number);

  return *(String*)&mstring;
}


// Extend string by character/string
static MString MString_push(MString mstring, const char element) {
  // Expand allocation by block size if required
  if (mstring.number % MString_BLOCK == 0)
    if ( (mstring.elements = (char*)realloc(mstring.elements,
                                            sizeof *mstring.elements * (mstring.number+MString_BLOCK))) == 0 )
      Error_dieErrno(errno, EX_OSERR, "Unable to expand MString allocation to %zd bytes",
                     sizeof *mstring.elements * (mstring.number+MString_BLOCK));

  // Append element
  mstring.elements[mstring.number] = element;
  ++mstring.number;

  return mstring;
}


static MString MString_append(MString mstring0, const String string1) {
  // Expand allocation by multiples of block size if required
  if ( (mstring0.number+MString_BLOCK-1)/MString_BLOCK <
       (mstring0.number+string1.number+MString_BLOCK-1)/MString_BLOCK ) {
    if ( (mstring0.elements = (char*)realloc(mstring0.elements, sizeof *mstring0.elements *
                                             (mstring0.number+string1.number+MString_BLOCK-1) /
                                             MString_BLOCK * MString_BLOCK)) == 0 )
      Error_dieErrno(errno, EX_OSERR, "Unable to expand MString allocation to %zd bytes",
                     sizeof *mstring0.elements *
                     (mstring0.number+string1.number+MString_BLOCK-1)/MString_BLOCK * MString_BLOCK);
  }

  // Append elements
  memcpy(&mstring0.elements[mstring0.number], string1.elements, sizeof *mstring0.elements * string1.number);
  mstring0.number += string1.number;

  return mstring0;
}


static MString MString_cappend(MString mstring0, const char* const cstring1) {
  return MString_append(mstring0, String_raw(strlen(cstring1), cstring1));
}


//---------------------------------------------------------------------------------------------------------------//
// String (no terminating 0)

// Construct/destruct string
static String String_raw(const size_t number, const char* const elements) {
  const String string = { number, elements };
  return string;
}


static String String_string(const String string) {
  char* elements;

  if ( (elements = (char*)malloc(sizeof *elements * string.number)) == 0 )
    Error_dieErrno(errno, EX_OSERR, "Unable to allocate %zd bytes for string", sizeof *elements * string.number);
  memcpy(elements, string.elements, sizeof *elements * string.number);

  return String_raw(string.number, elements);
}


static String String_cstring(const char* const cstring) {
  return String_string(String_raw(strlen(cstring), cstring));
}


static void String_free(const String string) {
  free((void*)string.elements);
}


// Append two strings
static String String_append(const String string0, const String string1) {
  return MString_freeze(MString_append(MString_string(string0), string1));
}


static String String_cappend(const String string0, const char* cstring1) {
  return String_append(string0, String_raw(strlen(cstring1), cstring1));
}


// Merge strings
static String String_intercalate(const String deliminator, const VectorString source) {
  MString target = MString_empty();

  if (source.number > 0) {
    target = MString_append(target, source.elements[0]);

    for (size_t iterator = 1; iterator < source.number; ++iterator) {
      target = MString_append(target, deliminator);
      target = MString_append(target, source.elements[iterator]);
    }
  }

  return MString_freeze(target);
}


static String String_cintercalate(const char* cstring, const VectorString source) {
  return String_intercalate(String_raw(strlen(cstring), cstring), source);
}


// Split string
static VectorString String_split(const String deliminator, const String source) {
  MVectorString target = MVectorString_empty();

  size_t source_start = 0;
  size_t source_iterator = 0;
  for ( ; source_iterator < source.number; ++source_iterator ) {

    // Compare deliminator to current portion of source
    size_t deliminator_iterator = 0;
    for ( ;
          deliminator_iterator < deliminator.number &&
            source_iterator+deliminator_iterator < source.number &&
            ( deliminator.elements[deliminator_iterator] ==
              source.elements[source_iterator + deliminator_iterator] );
          ++deliminator_iterator );

    // If they match, add deliminated string to vector and skip over
    if (deliminator_iterator == deliminator.number) {
      target = MVectorString_push(target, String_raw(source_iterator-source_start,
                                                     &source.elements[source_start]));
      source_iterator += deliminator_iterator;
      source_start = source_iterator;
    }
  }

  target = MVectorString_push(target, String_raw(source_iterator-source_start,
                                                 &source.elements[source_start]));
  
  return MVectorString_freeze(target);
}


static VectorString String_csplit(const char* const cdeliminator, const String source) {
  return String_split(String_raw(strlen(cdeliminator), cdeliminator), source);
}


// File as string
static String String_file(const String name) {
  int file;
  char* buffer;
  size_t buffer_fill;

  // Open the file
  {
    const char* cname = CString_string(name);

    if ( (file = open(cname, O_RDONLY)) < 0)
      Error_dieErrno(errno, EX_NOINPUT, "Unable to open \"%.*s\" for reading into slurp buffer",
                     (int)name.number, name.elements);

    CString_free(cname);
  }

  // Read in file
  {
    size_t buffer_size;
    ssize_t buffer_inc;

    // Allocate an initial slurp buffer
    buffer_fill = 0;
    buffer_size = 4096;
    if ( (buffer = (char*)malloc(sizeof *buffer * buffer_size*2)) == 0 )
      Error_dieErrno(errno, EX_OSERR, "Unable to allocate initial slurp buffer of %zd bytes for \"%.*s\"",
                     sizeof *buffer * buffer_size, (int)name.number, name.elements);

    // Read in everything by doubling slurp buffer everytime it fills up
    do {
      // Maintain minimal buffer size to make kernel call worthwhile
      if (buffer_size - buffer_fill < 4096) {
        if ( (buffer = (char*)realloc(buffer, sizeof *buffer * buffer_size*2)) == 0 )
          Error_dieErrno(errno, EX_OSERR, "Unable to expand slurp buffer to %zd bytes for \"%.*s\"",
                         sizeof *buffer * buffer_size*2, (int)name.number, name.elements);
        buffer_size *= 2;
      }

      // Attempt to read enough to fill up rest of slurp buffer
      if ( (buffer_inc = read(file, &buffer[buffer_fill], buffer_size - buffer_fill)) < 0 )
        Error_dieErrno(errno, EX_OSERR, "Unable to read all of \"%.*s\" into slurp buffer",
                       (int)name.number, name.elements);
      buffer_fill += buffer_inc;
    } while (buffer_inc > 0);

    // Resize slurp buffer to fit exactly
    buffer = (char*)realloc(buffer, sizeof *buffer * buffer_fill);
  }

  // Close file
  {
    int status;

    while ( (status = close(file)) < 0 && errno == EINTR );
    if (status < 0)
      Error_dieErrno(errno, EX_OSERR, "Unable to close \"%.*s\" after reading into slurp buffer",
                     (int)name.number, name.elements);
  }

  return String_raw(buffer_fill, buffer);
}


// Compare strings
int String_compare(const String string0, const String string1) {
  if (string0.number > string1.number) {
    const int results = memcmp(string0.elements, string1.elements, string1.number * sizeof *string1.elements);
    return results == 0 ?  1 : results;
  }
  else if (string0.number < string1.number) {
    const int results = memcmp(string0.elements, string1.elements, string0.number * sizeof *string0.elements);
    return results == 0 ? -1 : results;
  }
  else
    return memcmp(string0.elements, string1.elements, string0.number * sizeof *string0.elements);
}


int String_ccompare(const String string0, const char* const cstring1) {
  return String_compare(string0, String_raw(strlen(cstring1), cstring1));
}


//---------------------------------------------------------------------------------------------------------------//
// Maybe String

// Construct/destruct maybe string
static MaybeString MaybeString_raw(const String string) {
  struct MaybeString_* maybe;

  if ( (maybe = (struct MaybeString_*)malloc(sizeof(struct MaybeString_))) == 0 )
    Error_dieErrno(errno, EX_OSERR, "Unable to allocate %zd for MaybeString", sizeof(struct MaybeString_));
  maybe->value = string;

  return maybe;
}


static MaybeString MaybeString_string(const String string) {
  return MaybeString_raw(String_string(string));
}


static MaybeString MaybeString_cstring(const char* const cstring) {
  return MaybeString_raw(String_cstring(cstring));
}


static MaybeString MaybeString_nothing() {
  return 0;
}


static void MaybeString_free(const MaybeString maybe) {
  if (maybe) {
    String_free(maybe->value);
    free(maybe);
  }
}


// Extract string
static int MaybeString_isJust(const MaybeString maybe) {
  return maybe != 0;
}


static int MaybeString_isNothing(const MaybeString maybe) {
  return maybe == 0;
}


static String MaybeString_assert(const MaybeString maybe) {
  if ( maybe == 0 )
    Error_die(EX_SOFTWARE, "Expecting valid string");
  return maybe->value;
}


// Lifted string functions
static MaybeString MaybeString_append(const MaybeString maybe0, const MaybeString maybe1) {
  if ( maybe0 == 0 && maybe1 == 0 )
    return MaybeString_nothing();
  if ( maybe0 == 0 && maybe1 != 0 )
    return MaybeString_string(maybe1->value);
  if ( maybe0 != 0 && maybe1 == 0 )
    return MaybeString_string(maybe0->value);
  else
    return MaybeString_raw(String_append(maybe0->value, maybe1->value));
}


static MaybeString MaybeString_cappend(const MaybeString maybe0, const char* const cstring1) {
  if ( maybe0 == 0 )
    return MaybeString_cstring(cstring1);
  else
    return MaybeString_raw(String_cappend(maybe0->value, cstring1));
}


//---------------------------------------------------------------------------------------------------------------//
// Mutable Vector String

// Construct/destruct vector string
static MVectorString MVectorString_raw(const size_t number, String* const elements) {
  MVectorString vector = { number, elements };
  return vector;
}


static MVectorString MVectorString_empty() {
  return MVectorString_raw(0, 0);
}


static VectorString MVectorString_freeze(MVectorString mvector) {
  // Release extra memory
  if ( (mvector.elements = (String*)realloc(mvector.elements,
                                            sizeof *mvector.elements * mvector.number)) == 0 )
    Error_dieErrno(errno, EX_OSERR, "Unable to reduce MVectorString allocation to %zd bytes",
                   sizeof *mvector.elements * mvector.number);

  return *(VectorString*)&mvector;
}


// Extend vector string by string/strings
static MVectorString MVectorString_push(MVectorString mvector, const String string) {
  // Expand allocation by block size if required
  if (mvector.number % MVectorString_BLOCK == 0)
    if ( (mvector.elements = (String*)realloc(mvector.elements, sizeof *mvector.elements *
                                              (mvector.number+MVectorString_BLOCK))) == 0 )
      Error_dieErrno(errno, EX_OSERR, "Unable to expand MVectorString allocation to %zd bytes",
                     sizeof *mvector.elements * (mvector.number+MVectorString_BLOCK));

  // Append element
  mvector.elements[mvector.number] = String_string(string);
  ++mvector.number;

  return mvector;
}


static MVectorString MVectorString_cpush(MVectorString mvector, const char* const cstring) {
  return MVectorString_push(mvector, String_raw(strlen(cstring), cstring));
}


static MVectorString MVectorString_append(MVectorString mvector0, const VectorString vector1) {
  // Expand allocation by multiples of block size if required
  if ( (mvector0.number+MVectorString_BLOCK-1)/MVectorString_BLOCK 
       < (mvector0.number+vector1.number+MVectorString_BLOCK-1)/MVectorString_BLOCK ) {
    if ( (mvector0.elements = (String*)realloc(mvector0.elements, sizeof *mvector0.elements *
                                               (mvector0.number+vector1.number+MVectorString_BLOCK-1) /
                                               MVectorString_BLOCK * MVectorString_BLOCK)) == 0 )
      Error_dieErrno(errno, EX_OSERR, "Unable to expand MVectorString allocation to %zd bytes",
                     sizeof *mvector0.elements * (mvector0.number+vector1.number+MVectorString_BLOCK-1) /
                     MVectorString_BLOCK * MVectorString_BLOCK);
  }

  // Append elements
  memcpy(&mvector0.elements[mvector0.number], vector1.elements, sizeof *mvector0.elements * vector1.number);
  mvector0.number += vector1.number;

  return mvector0;
}


//---------------------------------------------------------------------------------------------------------------//
// Vector String

// Construct/destruct vector string
static VectorString VectorString_raw(const size_t number, const String* const elements) {
  const VectorString vector = { number, elements };
  return vector;
}

static void VectorString_free(const VectorString vector) {
  for (size_t iterator = 0; iterator < vector.number; ++iterator)
    String_free(vector.elements[iterator]);
  free((void*)vector.elements);
}


//---------------------------------------------------------------------------------------------------------------//
// Print format .../args message, system description for errno, and then exit with value
static void Error_die(const int value, const char* const format, ...) {
  va_list args;

  va_start(args, format);
  Error_vdie(value, format, args);
  va_end(args);
}

static void Error_vdie(const int value, const char* const format, va_list args) {
  if (format) {
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
  }

  exit(value);
}


// Print format .../args message, system description for errno, and then exit with value
static void Error_dieErrno(const int status, const int value, const char* const format, ...) {
  va_list args;

  va_start(args, format);
  Error_vdieErrno(status, value, format, args);
  va_end(args);
}

static void Error_vdieErrno(const int status, const int value, const char* const format, va_list args) {
  if (format) {
    vfprintf(stderr, format, args);
    fprintf(stderr, ": ");
  }
  fprintf(stderr, "%s\n", strerror(status));

  exit(value);
}


// Convert OpenCL status codes into strings
static const char* Error_stringCL(const cl_int status) {
  switch(status) {
  case CL_SUCCESS:                                   return "Success";
  case CL_DEVICE_NOT_FOUND:                          return "Device not found";
  case CL_DEVICE_NOT_AVAILABLE:                      return "Device not available";
  case CL_COMPILER_NOT_AVAILABLE:                    return "Compiler not available";
  case CL_MEM_OBJECT_ALLOCATION_FAILURE:             return "Mem object allocation failure";
  case CL_OUT_OF_RESOURCES:                          return "Out of resources";
  case CL_OUT_OF_HOST_MEMORY:                        return "Out of host memory";
  case CL_PROFILING_INFO_NOT_AVAILABLE:              return "Profiling info not available";
  case CL_MEM_COPY_OVERLAP:                          return "Mem copy overlap";
  case CL_IMAGE_FORMAT_MISMATCH:                     return "Image format mismatch";
  case CL_IMAGE_FORMAT_NOT_SUPPORTED:                return "Image format not supported";
  case CL_BUILD_PROGRAM_FAILURE:                     return "Build program failure";
  case CL_MAP_FAILURE:                               return "Map failure";
#ifdef CL_VERSION_1_1
  case CL_MISALIGNED_SUB_BUFFER_OFFSET:              return "Misaligned sub buffer offset";
  case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: return "Exec status error for events in wait list";
#ifdef CL_VERSION_1_2
  case CL_COMPILE_PROGRAM_FAILURE:                   return "Compile program failure";
  case CL_LINKER_NOT_AVAILABLE:                      return "Linker not available";
  case CL_LINK_PROGRAM_FAILURE:                      return "Link program failure";
  case CL_DEVICE_PARTITION_FAILED:                   return "Device partition failed";
  case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:             return "Kernel arg info not available";
#endif // CL_VERSION_1_2
#endif // CL_VERSION_1_1
  case CL_INVALID_VALUE:                             return "Invalid value";
  case CL_INVALID_DEVICE_TYPE:                       return "Invalid device type";
  case CL_INVALID_PLATFORM:                          return "Invalid platform";
  case CL_INVALID_DEVICE:                            return "Invalid device";
  case CL_INVALID_CONTEXT:                           return "Invalid context";
  case CL_INVALID_QUEUE_PROPERTIES:                  return "Invalid queue properties";
  case CL_INVALID_COMMAND_QUEUE:                     return "Invalid command queue";
  case CL_INVALID_HOST_PTR:                          return "Invalid host ptr";
  case CL_INVALID_MEM_OBJECT:                        return "Invalid mem object";
  case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:           return "Invalid image format descriptor";
  case CL_INVALID_IMAGE_SIZE:                        return "Invalid image size";
  case CL_INVALID_SAMPLER:                           return "Invalid sampler";
  case CL_INVALID_BINARY:                            return "Invalid binary";
  case CL_INVALID_BUILD_OPTIONS:                     return "Invalid build options";
  case CL_INVALID_PROGRAM:                           return "Invalid program";
  case CL_INVALID_PROGRAM_EXECUTABLE:                return "Invalid program executable";
  case CL_INVALID_KERNEL_NAME:                       return "Invalid kernel name";
  case CL_INVALID_KERNEL_DEFINITION:                 return "Invalid kernel definition";
  case CL_INVALID_KERNEL:                            return "Invalid kernel";
  case CL_INVALID_ARG_INDEX:                         return "Invalid arg index";
  case CL_INVALID_ARG_VALUE:                         return "Invalid arg value";
  case CL_INVALID_ARG_SIZE:                          return "Invalid arg size";
  case CL_INVALID_KERNEL_ARGS:                       return "Invalid kernel args";
  case CL_INVALID_WORK_DIMENSION:                    return "Invalid work dimension";
  case CL_INVALID_WORK_GROUP_SIZE:                   return "Invalid work group size";
  case CL_INVALID_WORK_ITEM_SIZE:                    return "Invalid work item size";
  case CL_INVALID_GLOBAL_OFFSET:                     return "Invalid global offset";
  case CL_INVALID_EVENT_WAIT_LIST:                   return "Invalid event wait list";
  case CL_INVALID_EVENT:                             return "Invalid event";
  case CL_INVALID_OPERATION:                         return "Invalid operation";
  case CL_INVALID_GL_OBJECT:                         return "Invalid gl object";
  case CL_INVALID_BUFFER_SIZE:                       return "Invalid buffer size";
  case CL_INVALID_MIP_LEVEL:                         return "Invalid mip level";
  case CL_INVALID_GLOBAL_WORK_SIZE:                  return "Invalid global work size";
#ifdef CL_VERSION_1_2
  case CL_INVALID_PROPERTY:                          return "Invalid property";
  case CL_INVALID_IMAGE_DESCRIPTOR:                  return "Invalid image descriptor";
  case CL_INVALID_COMPILER_OPTIONS:                  return "Invalid compiler options";
  case CL_INVALID_LINKER_OPTIONS:                    return "Invalid linker options";
  case CL_INVALID_DEVICE_PARTITION_COUNT:            return "Invalid device partition count";
#endif // CL_VERSION_1_2
  default:
    {
      static __thread char static_buffer[] = "Unknown status code " STRINGIFY(LONG_MIN);

      snprintf(static_buffer, sizeof static_buffer/sizeof *static_buffer,
               "Unknown status code %ld", (long)status);

      return static_buffer;
    }
  }
}


// Print format .../args message, description for OpenCL error, and then exit with passed value
static void Error_dieCL(const cl_int status, const int value, const char* const format, ...) {
  va_list args;

  va_start(args, format);
  Error_vdieCL(status, value, format, args);
  va_end(args);
}


static void Error_vdieCL(const cl_int status, const int value, const char* const format, va_list args) {
  if (format) {
    vfprintf(stderr, format, args);
    fprintf(stderr, ": ");
  }
  fprintf(stderr, "%s\n", Error_stringCL(status));

  exit(value);
}


//---------------------------------------------------------------------------------------------------------------//
// Command line argp parser data
static char Settings_doc[] = "Invoke the OpenCL compiler from the command line";

static char Settings_args[] = "-l\nSOURCE";
 
enum Settings_CL_ {
  Settings_CL_LB = 0x0fff,                          // Has to not overlap with ARGP_KEY_* or ASCII

  Settings_CL_STD,
  Settings_CL_KERNEL_ARG_INFO,

  Settings_CL_SINGLE_PRECISION_CONSTANT,
  Settings_CL_DENORMS_ARE_ZERO,
  Settings_CL_FP32_CORRECTLY_ROUNDED_DIVIDE_SQRT,
  Settings_CL_OPT_DISABLE,
  Settings_CL_MAD_ENABLE,
  Settings_CL_NO_SIGNED_ZEROS,
  Settings_CL_UNSAFE_MATH_OPTIMIZATIONS,
  Settings_CL_FINITE_MATH_ONLY,
  Settings_CL_FAST_RELAXED_MATH,

  Settings_CL_UB
};

static struct argp_option Settings_options[] = {
  { "list",     'l', 0,             0, "List platforms and devices",         0 },
  { "platform", 'p', "platform",    0, "Only compile against given plaform", 1 },
  { "device",   'd', "device",      0, "Only compile against given device",  1 },

  { 0,          'D', "name[=defn]", 0, "Predefine name as definition (default defn is 1)",     2 },
  { 0,          'I', "dir...",      0, "Add to list of directories searched for header files", 2 },
  { 0,          'w', 0,             0, "Disable all warnings",                                 2 },
  { 0,          'W', "error",       0, "Make all warnings errors",                             2 },

  { "cl-std",             Settings_CL_STD,             "CL1.1|CL1.2", 0,
    "Version of OpenCL language standard to use",       3 },
  { "cl-kernel-arg-info", Settings_CL_KERNEL_ARG_INFO, 0,             0,
    "Store kernel argument information for use in API", 3 },

  { "cl-single-precision-constant",          Settings_CL_SINGLE_PRECISION_CONSTANT,          0, 0,
    "Treat double precision floating-point constants as single",                           4 },
  { "cl-denorms-are-zero",                   Settings_CL_DENORMS_ARE_ZERO,                   0, 0,
    "Single and double denormalized floating-point number may be flushed to zero",         4 },
  { "cl-fp32-correctly-rounded-divide-sqrt", Settings_CL_FP32_CORRECTLY_ROUNDED_DIVIDE_SQRT, 0, 0,
    "Correctly round division and square root (see specificaiton for accuracy otherwise)", 4 },
  { "cl-opt-disable",                        Settings_CL_OPT_DISABLE,                        0, 0,
    "Disable all optimizations",                                                           4 },
  { "cl-mad-enable",                         Settings_CL_MAD_ENABLE,                         0, 0,
    "Enable (potentially less accurate) combined multiple add instructions",               4 },
  { "cl-no-signed-zeros",                    Settings_CL_NO_SIGNED_ZEROS,                    0, 0,
    "Allow optimizations that ignore the signedness of zero",                              4 },
  { "cl-unsafe-math-optimizations",          Settings_CL_UNSAFE_MATH_OPTIMIZATIONS,          0, 0,
    "Enable cl-no-signed-zeros and cl-mad-enable and further unsafe optmizations",         4 },
  { "cl-finite-math-only",                   Settings_CL_FINITE_MATH_ONLY,                   0, 0,
    "Allow optimizations that assume arguments and results are finite (not nan or inf)",   4 },
  { "cl-fast-relaxed-math",                  Settings_CL_FAST_RELAXED_MATH,                  0, 0,
    "Enable cl-finite-math-only and cl-unsafe-math-optimizations",                         4 },

  { 0 }
};

static struct argp Settings_argp = { Settings_options, Settings_parser, Settings_args, Settings_doc };


// Command line argp parser function
static error_t Settings_parser(int key, char* arg, struct argp_state* state) {
  MSettings* msettings = (MSettings*)state->input;

  switch (key) {
  case ARGP_KEY_INIT:
    break;
  case ARGP_KEY_NO_ARGS:
    break;
  case ARGP_KEY_END:
    break;
  case ARGP_KEY_SUCCESS:
    break;
  case ARGP_KEY_ERROR:
    break;
  case ARGP_KEY_FINI:
    break;

  case ARGP_KEY_ARG:
    msettings->sources = MVectorString_cpush(msettings->sources, arg);
    break;

  case 'l':
    if (msettings->command != Command_UNSET)
      argp_error(state, "multiple operations specified");
    msettings->command = Command_LIST;
    break;

  case 'p':
    if (MaybeString_isJust(msettings->platform))
      argp_error(state, "multiple platforms specified");
    msettings->platform = MaybeString_cstring(arg);
    break;
  case 'd':
    if (MaybeString_isJust(msettings->device))
      argp_error(state, "multiple devices specified");
    msettings->device = MaybeString_cstring(arg);
    break;

  case 'D':
    msettings->options = MVectorString_cpush(msettings->options, "-D");
    msettings->options = MVectorString_cpush(msettings->options, arg);
    break;
  case 'I':
    msettings->options = MVectorString_cpush(msettings->options, "-I");
    msettings->options = MVectorString_cpush(msettings->options, arg);
    break;

  case 'w':
    msettings->options = MVectorString_cpush(msettings->options, "-w");
    break;
  case 'W':
    if ( strcmp(arg, "error") != 0 )
      argp_error(state, "invalid warning mode specified");
    msettings->options = MVectorString_cpush(msettings->options, "-Werror");
    break;

  case Settings_CL_STD: {
    if ( strcmp(arg, "CL1.1") != 0 && strcmp(arg, "CL1.2") != 0 )
      argp_error(state, "invalid OpenCL language standard");
    String string;
    {
      MString mstring = MString_cstring("-cl-std=");
      mstring = MString_cappend(mstring, arg);
      string = MString_freeze(mstring);
    }
    msettings->options = MVectorString_push(msettings->options, string);
    String_free(string);
    break;
  }
  case Settings_CL_KERNEL_ARG_INFO:
    msettings->options = MVectorString_cpush(msettings->options, "-cl-kernel-arg-info");
    break;

  case Settings_CL_SINGLE_PRECISION_CONSTANT:
    msettings->options = MVectorString_cpush(msettings->options, "-cl-single-precision-constant");
    break;
  case Settings_CL_DENORMS_ARE_ZERO:
    msettings->options = MVectorString_cpush(msettings->options, "-cl-denorms-are-zero");
    break;
  case Settings_CL_FP32_CORRECTLY_ROUNDED_DIVIDE_SQRT:
    msettings->options = MVectorString_cpush(msettings->options, "-cl-fp32-correctly-rounded-divide-sqrt");
    break;
  case Settings_CL_OPT_DISABLE:
    msettings->options = MVectorString_cpush(msettings->options, "-cl-opt-disable");
    break;
  case Settings_CL_MAD_ENABLE:
    msettings->options = MVectorString_cpush(msettings->options, "-cl-mad-disable");
    break;
  case Settings_CL_NO_SIGNED_ZEROS:
    msettings->options = MVectorString_cpush(msettings->options, "-cl-no-signed-zeros");
    break;
  case Settings_CL_UNSAFE_MATH_OPTIMIZATIONS:
    msettings->options = MVectorString_cpush(msettings->options, "-cl-unsafe-math-optimizations");
    break;
  case Settings_CL_FINITE_MATH_ONLY:
    msettings->options = MVectorString_cpush(msettings->options, "-cl-finite-math-only");
    break;
  case Settings_CL_FAST_RELAXED_MATH:
    msettings->options = MVectorString_cpush(msettings->options, "-cl-fast-relaxed-math");
    break;

  default:
    return ARGP_ERR_UNKNOWN;
    break;
  };

  return 0;
}


// Initialize and free settings
static MSettings MSettings_initial() {
  MSettings msettings = {
    Command_UNSET,
    MVectorString_empty(),
    MaybeString_nothing(),
    MaybeString_nothing(),
    MVectorString_empty()
  };
  return msettings;
}

static Settings MSettings_freeze(MSettings msettings) {
  const Settings settings = {
    msettings.command,
    MVectorString_freeze(msettings.sources),
    msettings.platform,
    msettings.device,
    MVectorString_freeze(msettings.options)
  };
  return settings;
}

static void Settings_free(const Settings settings) {
  VectorString_free(settings.sources);
  MaybeString_free(settings.platform);
  MaybeString_free(settings.device);
  VectorString_free(settings.options);
}



//---------------------------------------------------------------------------------------------------------------//
// List of sizes
static VectorSize VectorSize_raw(const size_t number, const size_t* const elements) {
  const VectorSize vector = { number, elements };
  return vector;
}

static void VectorSize_free(const VectorSize vector) {
  free((void*)vector.elements);
}

// VList of platforms type
static VectorCLPlatform VectorCLPlatform_raw(const size_t number, const cl_platform_id* const elements) {
  const VectorCLPlatform vector = { number, elements };
  return vector;
}

static void VectorCLPlatform_free(const VectorCLPlatform vector) {
  free((void*)vector.elements);
}


// List of devices type
static VectorCLDevice VectorCLDevice_raw(const size_t number, const cl_device_id* const elements) {
  const VectorCLDevice vector = { number, elements };
  return vector;
}

static void VectorCLDevice_free(const VectorCLDevice vector) {
  free((void*)vector.elements);
}


// List of partition properties
#ifdef CL_VERSION_1_2
static VectorCLPartitionProperty VectorCLPartitionProperty_raw(size_t number,
                                                               const cl_device_partition_property* elements) {
  const VectorCLPartitionProperty vector = { number, elements };
  return vector;
}

static void VectorCLPartitionProperty_free(VectorCLPartitionProperty vector) {
  free((void*)vector.elements);
}
#endif // CL_VERSION_1_2


// Platforms
static VectorCLPlatform CL_platformsQuery() {
  cl_uint number;
  cl_platform_id* elements;

  {
    cl_int status;
    if ( (status = clGetPlatformIDs(0, 0, &number) != CL_SUCCESS) )
      Error_dieCL(status, EX_SOFTWARE, "Unable to get number of platform ids");
    if ( (elements = (cl_platform_id*)malloc(sizeof *elements * number)) == 0  && number != 0 )
      Error_dieErrno(errno, EX_OSERR, "Unable to allocate %zd bytes for platform ids", 
                     sizeof *elements * number);
    if ( (status = clGetPlatformIDs(number, elements, 0)) != CL_SUCCESS )
      Error_dieCL(status, EX_SOFTWARE, "Unable to get platform ids");
  }

  return VectorCLPlatform_raw(number, elements);
}

// Platforms
static String CL_platformName(const cl_platform_id platform_id) {
  size_t size_0;
  char* name;

  {
    cl_int status;
    if ( (status = clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, 0, 0, &size_0)) != CL_SUCCESS )
      Error_dieCL(status, EX_SOFTWARE, "Unable to get size of name for platform");
    if ( (name = (char*)malloc(size_0)) == 0 && size_0 != 0 )
      Error_dieErrno(errno, EX_OSERR, "Unable to allocate %zd bytes for name of platform", size_0);
    if ( (status = clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, size_0, name, 0)) != CL_SUCCESS )
      Error_dieCL(status, EX_SOFTWARE, "Unable to get name for platform");
  }

  return String_raw(size_0-1, name);
}


// Devices
static VectorCLDevice CL_devicesQuery(const cl_platform_id platform_id) {
  cl_uint number;
  cl_device_id* elements;

  {
    cl_int status;
    if ( (status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 0, 0, &number)) != CL_SUCCESS )
      Error_dieCL(status, EX_SOFTWARE, "Unable to get number of devices for platform");
    if ( (elements = (cl_device_id*)malloc(sizeof *elements * number)) == 0 && number != 0 )
      Error_dieErrno(errno, EX_OSERR, "Unable to allocate %zd bytes for device ids for platform",
                     sizeof *elements * number);
    if ( (status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, number, elements, 0)) != CL_SUCCESS )
      Error_dieCL(status, EX_SOFTWARE, "Unable to get device ids for platform");
  }

  return VectorCLDevice_raw(number, elements);
}


// Device properties
static void CL_deviceProperty_Singleton(const cl_device_id device_id, const cl_device_info property,
                                        void* const value, const size_t value_size) {
  cl_int status;
  size_t size;

  if ( (status = clGetDeviceInfo(device_id, property, 0, 0, &size)) != CL_SUCCESS )
    Error_dieCL(status, EX_SOFTWARE, "Unable to get size of device property");
  if (size != value_size)
    Error_die(EX_SOFTWARE, "Device property size %zd is not type size %zd", size, value_size);
  if ( (status = clGetDeviceInfo(device_id, property, size, value, 0)) != CL_SUCCESS )
    Error_dieCL(status, EX_SOFTWARE, "Unable to get device property");
}

static void CL_deviceProperty_Vector(const cl_device_id device_id, const cl_device_info property,
                                     size_t* const value_number, void** const value,
                                     const size_t value_size) {
  cl_int status;
  size_t size;

  if ( (status = clGetDeviceInfo(device_id, property, 0, 0, &size)) != CL_SUCCESS )
    Error_dieCL(status, EX_SOFTWARE, "Unable to get size of device property");
  if (size % value_size != 0)
    Error_die(EX_SOFTWARE, "Device property size %zd is not a multiple of type size %zd", size, value_size);
  *value_number = size / value_size;
  if ( (*value = malloc(size)) == 0 )
    Error_dieErrno(errno, EX_OSERR, "Unable to allocate %zd bytes for property", size);
  if ( (status = clGetDeviceInfo(device_id, property, size, *value, 0)) != CL_SUCCESS )
    Error_dieCL(status, EX_SOFTWARE, "Unable to get device property");
}

static void CL_deviceProperty_Vector0(const cl_device_id device_id, const cl_device_info property,
                                      size_t* const value_number, void** const value,
                                      const size_t value_size) {
  cl_int status;
  size_t size_0;

  if ( (status = clGetDeviceInfo(device_id, property, 0, 0, &size_0)) != CL_SUCCESS )
    Error_dieCL(status, EX_SOFTWARE, "Unable to get size of device property");
  if (size_0 % value_size != 0)
    Error_die(EX_SOFTWARE, "Device property size %zd is not a multiple of type size %zd", size_0, value_size);
  *value_number = size_0 / value_size - 1;
  if ( (*value = malloc(size_0)) == 0 )
    Error_dieErrno(errno, EX_OSERR, "Unable to allocate %zd bytes for property", size_0);
  if ( (status = clGetDeviceInfo(device_id, property, size_0, *value, 0)) != CL_SUCCESS )
    Error_dieCL(status, EX_SOFTWARE, "Unable to get device property");
}

static cl_device_id CL_deviceProperty_DeviceId(const cl_device_id device_id, int property) {
  cl_device_id value;
  CL_deviceProperty_Singleton(device_id, property, &value, sizeof value);
  return value;
}

static cl_platform_id CL_deviceProperty_PlatformId(const cl_device_id device_id, int property) {
  cl_platform_id value;
  CL_deviceProperty_Singleton(device_id, property, &value, sizeof value);
  return value;
}

static cl_device_type CL_deviceProperty_DeviceType(const cl_device_id device_id, int property) {
  cl_device_type value;
  CL_deviceProperty_Singleton(device_id, property, &value, sizeof value);
  return value;
}

static cl_device_fp_config CL_deviceProperty_FPConfig(const cl_device_id device_id, int property) {
  cl_device_fp_config value;
  CL_deviceProperty_Singleton(device_id, property, &value, sizeof value);
  return value;
}

static cl_device_mem_cache_type CL_deviceProperty_MemCacheType(const cl_device_id device_id, int property) {
  cl_device_mem_cache_type value;
  CL_deviceProperty_Singleton(device_id, property, &value, sizeof value);
  return value;
}

static cl_device_local_mem_type CL_deviceProperty_MemLocalType(const cl_device_id device_id, int property) {
  cl_device_local_mem_type value;
  CL_deviceProperty_Singleton(device_id, property, &value, sizeof value);
  return value;
}

static cl_device_exec_capabilities CL_deviceProperty_ExecCapabilities(const cl_device_id device_id, int property) {
  cl_device_exec_capabilities value;
  CL_deviceProperty_Singleton(device_id, property, &value, sizeof value);
  return value;
}

static cl_command_queue_properties CL_deviceProperty_QueueProperties(const cl_device_id device_id, int property) {
  cl_command_queue_properties value;
  CL_deviceProperty_Singleton(device_id, property, &value, sizeof value);
  return value;
}

#ifdef CL_VERSION_1_2
static cl_device_affinity_domain CL_deviceProperty_AffinityDomain(const cl_device_id device_id, int property) {
  cl_device_affinity_domain value;
  CL_deviceProperty_Singleton(device_id, property, &value, sizeof value);
  return value;
}
#endif // CL_VERSION_1_2

static cl_bool CL_deviceProperty_Bool(const cl_device_id device_id, int property) {
  cl_bool value;
  CL_deviceProperty_Singleton(device_id, property, &value, sizeof value);
  return value;
}

static cl_uint CL_deviceProperty_UInt(const cl_device_id device_id, int property) {
  cl_uint value;
  CL_deviceProperty_Singleton(device_id, property, &value, sizeof value);
  return value;
}

static cl_ulong CL_deviceProperty_ULong(const cl_device_id device_id, int property) {
  cl_ulong value;
  CL_deviceProperty_Singleton(device_id, property, &value, sizeof value);
  return value;
}

static size_t CL_deviceProperty_Size(const cl_device_id device_id, int property) {
  size_t value;
  CL_deviceProperty_Singleton(device_id, property, &value, sizeof value);
  return value;
}

static String CL_deviceProperty_String(const cl_device_id device_id, int property) {
  String value;
  CL_deviceProperty_Vector0(device_id, property, &value.number, (void**)&value.elements, sizeof *value.elements);
  return value;
}

static VectorSize CL_deviceProperty_VectorSize(const cl_device_id device_id, int property) {
  VectorSize value;
  CL_deviceProperty_Vector(device_id, property, &value.number, (void**)&value.elements, sizeof *value.elements);
  return value;
}

static VectorString CL_deviceProperty_VectorColon(const cl_device_id device_id, int property) {
  const String string = CL_deviceProperty_String(device_id, property);
  const VectorString vector = String_csplit(":", string);
  String_free(string);
  return vector;
}

static VectorString CL_deviceProperty_VectorSpace(const cl_device_id device_id, int property) {
  const String string = CL_deviceProperty_String(device_id, property);
  const VectorString vector = String_csplit(" ", string);
  String_free(string);
  return vector;
}

#ifdef CL_VERSION_1_2
static VectorCLPartitionProperty CL_deviceProperty_VectorPartitionProperty(const cl_device_id device_id, int property) {
  VectorCLPartitionProperty value;
  CL_deviceProperty_Vector(device_id, property, &value.number, (void**)&value.elements, sizeof *value.elements);
  return value;
}
#endif // CL_VERSION_1_2


#define CL_DEVICE_PROPERTY(ID, IDENT, TYPE, GROUP, DESC)               \
  static TYPE CL_deviceProperty##IDENT(const cl_device_id device_id) { \
    return CL_deviceProperty_##GROUP(device_id, ID);                   \
  }
#include "cldeviceprop.h"
#undef CL_DEVICE_PROPERTY


// Context
static cl_context CL_contextCreate(const cl_platform_id platform, const cl_device_id device) {
  cl_context context;

  {
    cl_int status;
    const cl_context_properties context_properties[] =
      { CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0 };
    const cl_device_id devices[] = { device };
    context = clCreateContext(context_properties, sizeof devices/sizeof *devices, devices, 0, 0, &status);
    if (status != CL_SUCCESS)
      Error_dieCL(status, EX_SOFTWARE, "Unable to create context");
  }

  return context;
}

static void CL_contextFree(const cl_context context) {
  cl_int status;
  if ( (status = clReleaseContext(context)) != CL_SUCCESS )
    Error_dieCL(status, EX_SOFTWARE, "Unable to release context");
}


// Program
static cl_program CL_programCreate(const cl_context context, const cl_device_id device,
                                   const VectorString codes, const VectorString options) {
  cl_program program;

  // Load program
  {
    // Build code lists
    const char* strings[codes.number];
    size_t strings_length[codes.number];

    for (size_t iterator = 0; iterator < codes.number; ++iterator) {
      strings[iterator] = codes.elements[iterator].elements;
      strings_length[iterator] = codes.elements[iterator].number;
    }

    // Call OpenCL routine
    cl_int status;

    program = clCreateProgramWithSource(context, sizeof strings/sizeof *strings, strings, strings_length,
                                        &status);
    if (status != CL_SUCCESS)
      Error_dieCL(status, EX_SOFTWARE, "Unable to create program");
  }

  // Build program
  {
    // Build option
    const char* coption;

    {
      String option = String_cintercalate(" ", options);
      coption = CString_string(option);
      String_free(option);
    }

    // Build device list
    const cl_device_id devices[] = { device };

    // Call OpenCL routine
    cl_int status;

    if ( (status = clBuildProgram(program, sizeof devices/sizeof *devices, devices,
                                  coption, 0, 0)) != CL_SUCCESS )
      if (status == CL_BUILD_PROGRAM_FAILURE) {
        cl_int status;
        size_t log_size_0;
        char* log;

        if ( (status = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
                                             0, 0, &log_size_0)) != CL_SUCCESS )
          Error_dieCL(status, EX_SOFTWARE, "Unable to get size of program build log");
        if ( (log = (char*)malloc(log_size_0)) == 0 )
          Error_dieErrno(errno, EX_OSERR, "Unable to allocate %zd bytes for program build log", log_size_0);
        if ( (status = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
                                             log_size_0, log, 0)) != CL_SUCCESS )
          Error_dieCL(status, EX_SOFTWARE, "Unable to get program build log");

        Error_die(EX_DATAERR, "Compilation failure:\n%s", log);
        
        free(log);
      }
      else
        Error_dieCL(status, EX_SOFTWARE, "Unable to build program");

    CString_free(coption);
  }

  return program;
}

static void CL_programFree(const cl_program program) {
  cl_int status;
  if ( (status = clReleaseProgram(program)) != CL_SUCCESS )
    Error_dieCL(status, EX_SOFTWARE, "Unable to release program");
}


//---------------------------------------------------------------------------------------------------------------//
static void Print_device_DeviceId(const unsigned int indent, const cl_device_id value) {
  printf("%lu", (unsigned long)value);
}

static void Print_device_PlatformId(const unsigned int indent, const cl_platform_id value) {
  printf("%lu", (unsigned long)value);
}

static void Print_device_DeviceType(const unsigned int indent, const cl_device_type value) {
  VectorString values;
  {
    MVectorString mvalues = MVectorString_empty();

    if (value & CL_DEVICE_TYPE_CPU)
      mvalues = MVectorString_push(mvalues, String_cstring("CPU"));
    if (value & CL_DEVICE_TYPE_GPU)
      mvalues = MVectorString_push(mvalues, String_cstring("GPU"));
    if (value & CL_DEVICE_TYPE_GPU)
      mvalues = MVectorString_push(mvalues, String_cstring("Accelerator"));
    if (value & CL_DEVICE_TYPE_GPU)
      mvalues = MVectorString_push(mvalues, String_cstring("Custom"));

    values = MVectorString_freeze(mvalues);
  }

  Print_device_VectorString(indent, values);
  VectorString_free(values);
}

static void Print_device_FPConfig(const unsigned int indent, const cl_device_fp_config value) {
}

static void Print_device_MemCacheType(const unsigned int indent, const cl_device_mem_cache_type value) {
}

static void Print_device_MemLocalType(const unsigned int indent, const cl_device_local_mem_type value) {
}

static void Print_device_ExecCapabilities(const unsigned int indent, const cl_device_exec_capabilities value) {
}

static void Print_device_QueueProperties(const unsigned int indent, const cl_command_queue_properties value) {
}

#ifdef CL_VERSION_1_2
static void Print_device_PartitionProperty(const unsigned int indent, const cl_device_partition_property value) {
}

static void Print_device_AffinityDomain(const unsigned int indent, const cl_device_affinity_domain value) {
}
#endif // CL_VERSION_1_2

static void Print_device_Bool(const unsigned int indent, const cl_bool value) {
  printf("%s", value ? "True" : "False");
}

static void Print_device_UInt(const unsigned int indent, const cl_ulong value) {
  printf("%lu", (unsigned long)value);
}

static void Print_device_ULong(const unsigned int indent, const cl_uint value) {
  printf("%lu", (unsigned long)value);
}

static void Print_device_Size(const unsigned int indent, const size_t value) {
  printf("%zu", value);
}

static void Print_device_String(const unsigned int indent, const String value) {
  printf("%.*s", (int)value.number, value.elements);
}

static void Print_device_VectorSize(const unsigned int indent, const VectorSize value) {
  printf("[");
  if (value.number > 0) {
    printf(" ");
    Print_device_Size(indent+2, value.elements[0]);

    for (size_t iterator = 1; iterator < value.number; ++iterator) {
      printf(", ");
      Print_device_Size(indent+2, value.elements[iterator]);
    }
    printf(" ");
  }
  printf("]");
}

static void Print_device_VectorString(const unsigned int indent, const VectorString value) {
  printf("[");
  if (value.number > 0) {
    printf(" ");
    Print_device_String(indent+2, value.elements[0]);

    for (size_t iterator = 1; iterator < value.number; ++iterator) {
      printf(", ");
      Print_device_String(indent+2, value.elements[iterator]);
    }
    printf(" ");
  }
  printf("]");
}

static void Print_device_VectorColon(const unsigned int indent, const VectorString value) {
  Print_device_VectorString(indent, value);
}

static void Print_device_VectorSpace(const unsigned int indent, const VectorString value) {
  Print_device_VectorString(indent, value);
}

#ifdef CL_VERSION_1_2
static void Print_device_VectorPartitionProperty(const unsigned int indent, const VectorCLPartitionProperty value) {
  printf("[");
  if (value.number > 0) {
    printf(" ");
    Print_device_PartitionProperty(indent+2, value.elements[0]);

    for (size_t iterator = 1; iterator < value.number; ++iterator) {
      printf(", ");
      Print_device_PartitionProperty(indent+2, value.elements[iterator]);
    }
    printf(" ");
  }
  printf("]");
}
#endif // CL_VERSION_1_2

#define CL_DEVICE_PROPERTY(ID, IDENT, TYPE, GROUP, DESC)                \
  static void Print_device##IDENT(const unsigned int indent, const TYPE value) { \
    printf("%*.0s%s = ", indent, "", DESC);                             \
    Print_device_##GROUP(indent, value);                                \
    printf("\n");                                                       \
  }
#include "cldeviceprop.h"
#undef CL_DEVICE_PROPERTY


//---------------------------------------------------------------------------------------------------------------//
int main(const int argc, char **const argv) {
  Settings settings;

  // Parse command line
  {
    MSettings msettings = MSettings_initial();
    int status;

    if ( (status = argp_parse(&Settings_argp, argc, argv, ARGP_LONG_ONLY, 0, &msettings)) )
      Error_dieErrno(status, EX_SOFTWARE, "Error encountered by argp parser");

    settings = MSettings_freeze(msettings);
  }

  // Perform requested action
  switch (settings.command) {
  case Command_UNSET:
    Action_compile(settings);
    break;
  case Command_LIST:
    Action_list(settings);
    break;
  default:
    Error_die(EX_SOFTWARE, "Unhandled command mode %d", settings.command);
    break;
  }

  // Release settings
  Settings_free(settings);

  return 0;
}

// Compile the given source
static void Action_compile(const Settings settings) {
  // Load the source codes
  if (settings.sources.number < 1 )
    Error_die(EX_USAGE, "Compilation mode requires source file");

  VectorString sources;
  {
    MVectorString msources = MVectorString_empty();
    for (size_t iterator = 0; iterator < settings.sources.number; ++iterator) {
      msources = MVectorString_cpush(msources, "#line 1 \"");
      msources = MVectorString_push(msources, settings.sources.elements[iterator]);
      msources = MVectorString_cpush(msources, "\"\n");
      msources = MVectorString_push(msources, String_file(settings.sources.elements[iterator]));
    }
    sources = MVectorString_freeze(msources);
  }

  // For all the platforms
  const VectorCLPlatform platforms = CL_platformsQuery();

  for (size_t platforms_iterator = 0; platforms_iterator < platforms.number; ++platforms_iterator) {
    const cl_platform_id platform_id = platforms.elements[platforms_iterator];

    // If platform selected, filter out ones that don't match
    const String platform_name = CL_platformName(platform_id);
    if (MaybeString_isNothing(settings.platform) ||
        String_compare(MaybeString_assert(settings.platform), platform_name) == 0) {

      // For all the devices
      const VectorCLDevice devices = CL_devicesQuery(platform_id);

      for (size_t devices_iterator = 0; devices_iterator < devices.number; ++devices_iterator) {
        const cl_device_id device_id = devices.elements[devices_iterator];

        // If device selected, filter out ones that don't match
        const String device_name = CL_devicePropertyName(device_id);

        if (MaybeString_isNothing(settings.device) ||
            String_compare(MaybeString_assert(settings.device), device_name) == 0) {

          cl_context context = CL_contextCreate(platform_id, device_id);
          cl_program program = CL_programCreate(context, device_id, sources, settings.options);

          CL_programFree(program);
          CL_contextFree(context);

          //          status = clBuildProgram(program, 0, 0, options, 0, 0);
          //          status = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, 0, &log_len);
          //          status = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_len, log, 0);
        }

        String_free(device_name);
      }

      VectorCLDevice_free(devices);
    }

    String_free(platform_name);
  }

  VectorCLPlatform_free(platforms);

  VectorString_free(sources);
}


// List the platforms and their devices
static void Action_list(const Settings settings) {
  // For all the platforms
  const VectorCLPlatform platforms = CL_platformsQuery();

  for (size_t platforms_iterator = 0; platforms_iterator < platforms.number; ++platforms_iterator) {
    const cl_platform_id platform_id = platforms.elements[platforms_iterator];

    // Print the platform
    const String platform_name = CL_platformName(platform_id);
    printf("Platform %d: %.*s\n", (int)platforms_iterator, (int)platform_name.number, platform_name.elements);

    // For all the devices
    const VectorCLDevice devices = CL_devicesQuery(platform_id);

    for (size_t devices_iterator = 0; devices_iterator < devices.number; ++devices_iterator) {
      const cl_device_id device_id = devices.elements[devices_iterator];

      // Print the device
      {
        const String device_name = CL_devicePropertyName(device_id);
        printf("  Device %d: %.*s\n", (int)devices_iterator, (int)device_name.number, device_name.elements);
        String_free(device_name);
      }

      // Print some properties
      {
        const cl_device_type value = CL_devicePropertyType(device_id);
        Print_deviceType(6, value);
      }
      {
        const cl_uint value = CL_devicePropertyMaxComputeUnits(device_id);
        Print_deviceMaxComputeUnits(6, value);
      }
      {
        const cl_uint value = CL_devicePropertyMaxWorkItemDimensions(device_id);
        Print_deviceMaxWorkItemDimensions(6, value);
      }
      {
        const VectorSize value = CL_devicePropertyMaxWorkItemSizes(device_id);
        Print_deviceMaxWorkItemSizes(6, value);
        VectorSize_free(value);
      }
      {
        const size_t value = CL_devicePropertyMaxWorkGroupSize(device_id);
        Print_deviceMaxWorkGroupSize(6, value);
      }
      {
        const cl_bool value = CL_devicePropertyImageSupport(device_id);
        Print_deviceImageSupport(6, value);
        if (value) {
          {
            const size_t value = CL_devicePropertyImage2DMaxWidth(device_id);
            Print_deviceImage2DMaxWidth(8, value);
          }
          {
            const size_t value = CL_devicePropertyImage2DMaxHeight(device_id);
            Print_deviceImage2DMaxHeight(8, value);
          }
          {
            const size_t value = CL_devicePropertyImage3DMaxWidth(device_id);
            Print_deviceImage3DMaxWidth(8, value);
          }
          {
            const size_t value = CL_devicePropertyImage3DMaxHeight(device_id);
            Print_deviceImage3DMaxHeight(8, value);
          }
          {
            const size_t value = CL_devicePropertyImage3DMaxDepth(device_id);
            Print_deviceImage3DMaxDepth(8, value);
          }
        }
      }
      {
        const cl_ulong value = CL_devicePropertyGlobalMemSize(device_id);
        Print_deviceGlobalMemSize(6, value);
      }
      {
        const cl_ulong value = CL_devicePropertyLocalMemSize(device_id);
        Print_deviceLocalMemSize(6, value);
      }
    }

    VectorCLDevice_free(devices);
    String_free(platform_name);
  }

  VectorCLPlatform_free(platforms);
}
