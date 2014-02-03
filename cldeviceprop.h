#ifndef CL_DEVICE_PROPERTY
#error need to define CL_DEVICE_PROPERTY before including device property table
#endif

//
// Device property table for automatic code generation (by including with appropriately defined CL_DEVICE_PROPERTY)
//
// Basic groups (and the associated types) for differentiation are
//
//   DeviceId         -- cl_device_id
//   PlatformId       -- cl_platform_id
//   DeviceType       -- cl_device_type
//   FPConfig         -- cl_device_fp_config
//   MemCacheType     -- cl_device_mem_cache_type
//   MemLocalType     -- cl_device_local_mem_type
//   ExecCapabilities -- cl_device_exec_capabilities
//   QueueProperty    -- cl_command_queue_properties
//   AffinityDomain   -- cl_device_affinity_domain
//
//   Bool  -- cl_bool
//   UInt  -- cl_uint
//   ULong -- cl_ulong
//   Size -- size_t
//
//   String                  -- char[]
//   VectorSize              -- size_t[]
//   VectorColon             -- char[]
//   VectorSpace             -- char[]
//   VectorPartitionProperty -- cl_device_partition_property[]
//


// CL_DEVICE_PROPERTY(ID, IDENT, TYPE, GROUP, DESC) 

CL_DEVICE_PROPERTY(CL_DEVICE_TYPE,      Type,     cl_device_type, DeviceType, "Type")
CL_DEVICE_PROPERTY(CL_DEVICE_VENDOR_ID, VendorId, cl_uint,        UInt,       "Vendor id")

CL_DEVICE_PROPERTY(CL_DEVICE_MAX_COMPUTE_UNITS, MaxComputeUnits, cl_uint, UInt, "Maximum compute units")
CL_DEVICE_PROPERTY(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, MaxWorkItemDimensions, cl_uint,    UInt,       "Maximum work item dimensions")
CL_DEVICE_PROPERTY(CL_DEVICE_MAX_WORK_ITEM_SIZES,      MaxWorkItemSizes,      VectorSize, VectorSize, "Maximum work item sizes")
CL_DEVICE_PROPERTY(CL_DEVICE_MAX_WORK_GROUP_SIZE,      MaxWorkGroupSize,      size_t,     Size,       "Maximum work group size")

CL_DEVICE_PROPERTY(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,   PreferredVectorWidthChar,   cl_uint, UInt, "Preferred vector width char")
CL_DEVICE_PROPERTY(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,  PreferredVectorWidthShort,  cl_uint, UInt, "Preferred vector width short")
CL_DEVICE_PROPERTY(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,    PreferredVectorWidthInt,    cl_uint, UInt, "Preferred vector width int")
CL_DEVICE_PROPERTY(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,   PreferredVectorWidthLong,   cl_uint, UInt, "Preferred vector width long")
CL_DEVICE_PROPERTY(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,  PreferredVectorWidthFloat,  cl_uint, UInt, "Preferred vector width float")
CL_DEVICE_PROPERTY(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, PreferredVectorWidthDouble, cl_uint, UInt, "Preferred vector width double")
CL_DEVICE_PROPERTY(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF,   PreferredVectorWidthHalf,   cl_uint, UInt, "Preferred vector width half")
CL_DEVICE_PROPERTY(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR,   NativeVectorWidthChar,   cl_uint, UInt, "Native vector width char")
CL_DEVICE_PROPERTY(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT,  NativeVectorWidthShort,  cl_uint, UInt, "Native vector width short")
CL_DEVICE_PROPERTY(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT,    NativeVectorWidthInt,    cl_uint, UInt, "Native vector width int")
CL_DEVICE_PROPERTY(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG,   NativeVectorWidthLong,   cl_uint, UInt, "Native vector width long")
CL_DEVICE_PROPERTY(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT,  NativeVectorWidthFloat,  cl_uint, UInt, "Native vector width float")
CL_DEVICE_PROPERTY(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, NativeVectorWidthDouble, cl_uint, UInt, "Native vector width double")
CL_DEVICE_PROPERTY(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF,   NativeVectorWidthHalf,   cl_uint, UInt, "Native vector width half")

CL_DEVICE_PROPERTY(CL_DEVICE_MAX_CLOCK_FREQUENCY, MaxClockFrequency, cl_uint, UInt, "Maximum clock frequency")

CL_DEVICE_PROPERTY(CL_DEVICE_ADDRESS_BITS, AddressBits, cl_uint, UInt, "Address bits")

CL_DEVICE_PROPERTY(CL_DEVICE_MAX_MEM_ALLOC_SIZE, MaxMemAllocSize, cl_ulong, ULong, "Maximum memory alloc size")

CL_DEVICE_PROPERTY(CL_DEVICE_IMAGE_SUPPORT, ImageSupport, cl_bool, Bool, "Image support")
CL_DEVICE_PROPERTY(CL_DEVICE_MAX_READ_IMAGE_ARGS,  MaxReadImageArgs,  cl_uint, UInt, "Maximum read image args")
CL_DEVICE_PROPERTY(CL_DEVICE_MAX_WRITE_IMAGE_ARGS, MaxWriteImageArgs, cl_uint, UInt, "Maximum write image args")
CL_DEVICE_PROPERTY(CL_DEVICE_IMAGE2D_MAX_WIDTH,  Image2DMaxWidth,  size_t, Size, "Image2D maximum width")
CL_DEVICE_PROPERTY(CL_DEVICE_IMAGE2D_MAX_HEIGHT, Image2DMaxHeight, size_t, Size, "Image2D maximum height")
CL_DEVICE_PROPERTY(CL_DEVICE_IMAGE3D_MAX_WIDTH,  Image3DMaxWidth,  size_t, Size, "Image3D maximum width")
CL_DEVICE_PROPERTY(CL_DEVICE_IMAGE3D_MAX_HEIGHT, Image3DMaxHeight, size_t, Size, "Image3D maximum height")
CL_DEVICE_PROPERTY(CL_DEVICE_IMAGE3D_MAX_DEPTH,  Image3DMaxDepth,  size_t, Size, "Image3D maximum depth")
#ifdef CL_VERSION_1_2
CL_DEVICE_PROPERTY(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE, ImageMaxBufferSize, size_t, Size, "Image maximum buffer size")
CL_DEVICE_PROPERTY(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE,  ImageMaxArraySize,  size_t, Size, "Image maximum array size")
#endif // CL_VERSION_1_2

CL_DEVICE_PROPERTY(CL_DEVICE_MAX_SAMPLERS, MaxSamplers, cl_uint, UInt, "Maximum samplers")

CL_DEVICE_PROPERTY(CL_DEVICE_MAX_PARAMETER_SIZE, MaxParameterSize, size_t, Size, "Maximum parameter size")

CL_DEVICE_PROPERTY(CL_DEVICE_MEM_BASE_ADDR_ALIGN, MemBaseAddrAlign, cl_uint, UInt, "Memory base addr align")

CL_DEVICE_PROPERTY(CL_DEVICE_SINGLE_FP_CONFIG, SingleFpConfig, cl_device_fp_config, FPConfig, "Single fp config")
CL_DEVICE_PROPERTY(CL_DEVICE_DOUBLE_FP_CONFIG, DoubleFpConfig, cl_device_fp_config, FPConfig, "Double fp config")

CL_DEVICE_PROPERTY(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, GlobalMemCacheType, cl_device_mem_cache_type, MemCacheType, "Global memory cache type")
CL_DEVICE_PROPERTY(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, GlobalMemCachelineSize, cl_uint,  UInt,  "Global memory cacheline size")
CL_DEVICE_PROPERTY(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,     GlobalMemCacheSize,     cl_ulong, ULong, "Global memory cache size")
CL_DEVICE_PROPERTY(CL_DEVICE_GLOBAL_MEM_SIZE,           GlobalMemSize,          cl_ulong, ULong, "Global memory size")

CL_DEVICE_PROPERTY(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, MaxConstantBufferSize, cl_ulong, ULong, "Maximum constant buffer size")
CL_DEVICE_PROPERTY(CL_DEVICE_MAX_CONSTANT_ARGS, MaxConstantArgs, cl_uint, UInt, "Maximum constant args")

CL_DEVICE_PROPERTY(CL_DEVICE_LOCAL_MEM_TYPE, LocalMemType, cl_device_local_mem_type, MemLocalType, "Local memory type")
CL_DEVICE_PROPERTY(CL_DEVICE_LOCAL_MEM_SIZE, LocalMemSize, cl_ulong, ULong, "Local memory size")
CL_DEVICE_PROPERTY(CL_DEVICE_ERROR_CORRECTION_SUPPORT, ErrorCorrectionSupport, cl_bool, Bool, "Error correction support")
CL_DEVICE_PROPERTY(CL_DEVICE_HOST_UNIFIED_MEMORY, HostUnifiedMemory, cl_bool, Bool, "Host unified memory")

CL_DEVICE_PROPERTY(CL_DEVICE_PROFILING_TIMER_RESOLUTION, ProfilingTimerResolution, size_t, Size, "Profiling timer resolution")

CL_DEVICE_PROPERTY(CL_DEVICE_ENDIAN_LITTLE, EndianLittle, cl_bool, Bool, "Little endian")

CL_DEVICE_PROPERTY(CL_DEVICE_AVAILABLE,          Available,         cl_bool, Bool, "Device available")
CL_DEVICE_PROPERTY(CL_DEVICE_COMPILER_AVAILABLE, CompilerAvailable, cl_bool, Bool, "Compiler available")
#ifdef CL_VERSION_1_2
CL_DEVICE_PROPERTY(CL_DEVICE_LINKER_AVAILABLE,   LinkerAvailable,   cl_bool, Bool, "Linker available")
#endif // CL_VERSION_1_2

CL_DEVICE_PROPERTY(CL_DEVICE_EXECUTION_CAPABILITIES, ExecutionCapabilities, cl_device_exec_capabilities, ExecCapabilities, "Execution capabilities")
CL_DEVICE_PROPERTY(CL_DEVICE_QUEUE_PROPERTIES,       QueueProperties,       cl_command_queue_properties, QueueProperties,  "Queue properties")

#ifdef CL_VERSION_1_2
CL_DEVICE_PROPERTY(CL_DEVICE_BUILT_IN_KERNELS, BuiltInKernels, VectorString, VectorColon, "Built in kernels")
#endif // CL_VERSION_1_2

CL_DEVICE_PROPERTY(CL_DEVICE_PLATFORM, Platform, cl_platform_id, PlatformId, "Platform")

CL_DEVICE_PROPERTY(CL_DEVICE_NAME,   Name,   String, String, "Name")
CL_DEVICE_PROPERTY(CL_DEVICE_VENDOR, Vendor, String, String, "Vendor")

CL_DEVICE_PROPERTY(CL_DRIVER_VERSION,          DriverVersion,  String, String, "Driver")
CL_DEVICE_PROPERTY(CL_DEVICE_PROFILE,          Profile,        String, String, "Profile")
CL_DEVICE_PROPERTY(CL_DEVICE_VERSION,          Version,        String, String, "Version")
CL_DEVICE_PROPERTY(CL_DEVICE_OPENCL_C_VERSION, OpenCLCVersion, String, String, "C version")

CL_DEVICE_PROPERTY(CL_DEVICE_EXTENSIONS, Extensions, VectorString, VectorSpace, "Extensions")

#ifdef CL_VERSION_1_2
CL_DEVICE_PROPERTY(CL_DEVICE_PRINTF_BUFFER_SIZE, PrintfBufferSize, size_t, Size, "Printf buffer size")

CL_DEVICE_PROPERTY(CL_DEVICE_PREFERRED_INTEROP_USER_SYNC, PreferredInteropUserSync, cl_bool, Bool, "Preferred interop user sync")

CL_DEVICE_PROPERTY(CL_DEVICE_PARENT_DEVICE, ParentDevice, cl_device_id, DeviceId, "Parent device")
CL_DEVICE_PROPERTY(CL_DEVICE_PARTITION_MAX_SUB_DEVICES, PartitionMaxSubDevices, cl_uint, UInt, "Partition maximum sub devices")
CL_DEVICE_PROPERTY(CL_DEVICE_PARTITION_PROPERTIES,      PartitionProperties,     VectorCLPartitionProperty, VectorPartitionProperty, "Partition properties")
CL_DEVICE_PROPERTY(CL_DEVICE_PARTITION_AFFINITY_DOMAIN, PartitionAffinityDomain, cl_device_affinity_domain, AffinityDomain,          "Partition affinity domain")
CL_DEVICE_PROPERTY(CL_DEVICE_PARTITION_TYPE,            PartitionType,           VectorCLPartitionProperty, VectorPartitionProperty, "Partition type")

CL_DEVICE_PROPERTY(CL_DEVICE_REFERENCE_COUNT, ReferenceCount, cl_uint, UInt, "Reference count")
#endif // CL_VERSION_1_2



/*
SINGLE cl_device_type CL_DEVICE_TYPE      "type"
SINGLE cl_uint        CL_DEVICE_VENDOR_ID "vendor id"

SINGLE cl_uint CL_DEVICE_MAX_COMPUTE_UNITS "maximum compute units"
SINGLE cl_uint CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS "maximum work item dimensions"
VECTOR size_t  CL_DEVICE_MAX_WORK_ITEM_SIZES      "maximum work item sizes"
SINGLE size_t  CL_DEVICE_MAX_WORK_GROUP_SIZE      "maximum work group size"

SINGLE cl_uint CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR   "preferred vector width char"
SINGLE cl_uint CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT  "preferred vector width short"
SINGLE cl_uint CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT    "preferred vector width int"
SINGLE cl_uint CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG   "preferred vector width long"
SINGLE cl_uint CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT  "preferred vector width float"
SINGLE cl_uint CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE "preferred vector width double"
SINGLE cl_uint CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF   "preferred vector width half"
SINGLE cl_uint CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR   "native vector width char"
SINGLE cl_uint CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT  "native vector width short"
SINGLE cl_uint CL_DEVICE_NATIVE_VECTOR_WIDTH_INT    "native vector width int"
SINGLE cl_uint CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG   "native vector width long"
SINGLE cl_uint CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT  "native vector width float"
SINGLE cl_uint CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE "native vector width double"
SINGLE cl_uint CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF   "native vector width half"

SINGLE cl_uint CL_DEVICE_MAX_CLOCK_FREQUENCY "maximum clock frequency"

SINGLE cl_uint CL_DEVICE_ADDRESS_BITS "address bits"

SINGLE cl_ulong CL_DEVICE_MAX_MEM_ALLOC_SIZE "maximum memory alloc size"

SINGLE cl_bool CL_DEVICE_IMAGE_SUPPORT "image support"
SINGLE cl_uint CL_DEVICE_MAX_READ_IMAGE_ARGS  "maximum read image args"
SINGLE cl_uint CL_DEVICE_MAX_WRITE_IMAGE_ARGS "maximum write image args"
SINGLE size_t CL_DEVICE_IMAGE2D_MAX_WIDTH  "image2d maximum width"
SINGLE size_t CL_DEVICE_IMAGE2D_MAX_HEIGHT "image2d maximum height"
SINGLE size_t CL_DEVICE_IMAGE3D_MAX_WIDTH  "image3d maximum width"
SINGLE size_t CL_DEVICE_IMAGE3D_MAX_HEIGHT "image3d maximum height"
SINGLE size_t CL_DEVICE_IMAGE3D_MAX_DEPTH  "image3d maximum depth"
SINGLE size_t CL_DEVICE_IMAGE_MAX_BUFFER_SIZE "image maximum buffer size"
SINGLE size_t CL_DEVICE_IMAGE_MAX_ARRAY_SIZE  "image maximum array size"

SINGLE cl_uint CL_DEVICE_MAX_SAMPLERS "maximum samplers"

SINGLE size_t CL_DEVICE_MAX_PARAMETER_SIZE "maximum parameter size"

SINGLE cl_uint CL_DEVICE_MEM_BASE_ADDR_ALIGN "memory base addr align"

SINGLE cl_device_fp_config CL_DEVICE_SINGLE_FP_CONFIG "single fp config"
SINGLE cl_device_fp_config CL_DEVICE_DOUBLE_FP_CONFIG "double fp config"

SINGLE cl_device_mem_cache_type CL_DEVICE_GLOBAL_MEM_CACHE_TYPE     "global memory cache type"
SINGLE cl_uint                  CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE "global memory cacheline size"
SINGLE cl_ulong                 CL_DEVICE_GLOBAL_MEM_CACHE_SIZE     "global memory cache size"
SINGLE cl_ulong                 CL_DEVICE_GLOBAL_MEM_SIZE           "global memory size"

SINGLE cl_ulong                 CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE "maximum constant buffer size"
SINGLE cl_uint                  CL_DEVICE_MAX_CONSTANT_ARGS "maximum constant args"

SINGLE cl_device_local_mem_type CL_DEVICE_LOCAL_MEM_TYPE "local memory type"
SINGLE cl_ulong                 CL_DEVICE_LOCAL_MEM_SIZE "local memory size"
SINGLE cl_bool                  CL_DEVICE_ERROR_CORRECTION_SUPPORT "error correction support"

SINGLE cl_bool CL_DEVICE_HOST_UNIFIED_MEMORY "host unified memory"

SINGLE size_t  CL_DEVICE_PROFILING_TIMER_RESOLUTION "profiling timer resolution"

SINGLE cl_bool CL_DEVICE_ENDIAN_LITTLE "little endian"

SINGLE cl_bool CL_DEVICE_AVAILABLE          "device available"
SINGLE cl_bool CL_DEVICE_COMPILER_AVAILABLE "compiler available"
SINGLE cl_bool CL_DEVICE_LINKER_AVAILABLE   "linker available"

SINGLE cl_device_exec_capabilities CL_DEVICE_EXECUTION_CAPABILITIES "execution capabilities"
SINGLE cl_command_queue_properties CL_DEVICE_QUEUE_PROPERTIES       "queue properties"
COLON  char                        CL_DEVICE_BUILT_IN_KERNELS       "built in kernels"
SINGLE cl_platform_id              CL_DEVICE_PLATFORM               "platform"

STRING char CL_DEVICE_NAME   "name"
STRING char CL_DEVICE_VENDOR "vendor"

STRING char CL_DRIVER_VERSION          "driver"
STRING char CL_DEVICE_PROFILE          "profile"
STRING char CL_DEVICE_VERSION          "version"
STRING char CL_DEVICE_OPENCL_C_VERSION "C version"

STRING char CL_DEVICE_EXTENSIONS "extensions"

SINGLE size_t CL_DEVICE_PRINTF_BUFFER_SIZE "printf buffer size"

SINGLE cl_bool CL_DEVICE_PREFERRED_INTEROP_USER_SYNC "preferred interop user sync"

SINGLE cl_device_id                 CL_DEVICE_PARENT_DEVICE             "parent device"
SINGLE cl_uint                      CL_DEVICE_PARTITION_MAX_SUB_DEVICES "partition maximum sub devices"
VECTOR cl_device_partition_property CL_DEVICE_PARTITION_PROPERTIES      "partition properties"
SINGLE cl_device_affinity_domain    CL_DEVICE_PARTITION_AFFINITY_DOMAIN "partition affinity domain"
VECTOR cl_device_partition_property CL_DEVICE_PARTITION_TYPE            "partition type"

SINGLE cl_uint CL_DEVICE_REFERENCE_COUNT "reference count"
*/
