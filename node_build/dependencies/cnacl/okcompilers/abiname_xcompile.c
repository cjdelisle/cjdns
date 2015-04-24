#if defined(__amd64__) || defined(__x86_64__) || defined(__AMD64__) || defined(_M_X64) || defined(__amd64)
   #ifdef __AVX__
       #error ABI_IS_amd64_AVX__
   #else
       #error ABI_IS_amd64__
   #endif
#elif defined(__i386__) || defined(__x86__) || defined(__X86__) || defined(_M_IX86) || defined(__i386)
   #if defined(__AVX__)
       #error ABI_IS_x86_AVX__
   #elif defined(__SSE2__)
       #error ABI_IS_x86_SSE2__
   #else
       #error ABI_IS_x86__
   #endif
#elif defined(__ia64__) || defined(__IA64__) || defined(__M_IA64)
   #error ABI_IS_ia64__
#elif defined(__SPU__)
   #error ABI_IS_cellspu__
#elif defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) || defined(_ARCH_PPC64)
    #error ABI_IS_ppc64__
#elif defined(__powerpc__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
    #error ABI_IS_ppc32__
#elif defined(__sparcv9__) || defined(__sparcv9)
    #error ABI_IS_sparcv9__
#elif defined(__sparc_v8__)
    #error ABI_IS_sparcv8__
#elif defined(__sparc__) || defined(__sparc)
    // if false then: error: zero width for bit-field ‘ABI_IS_sparcv9__’
    struct isItV9 { int ABI_IS_sparcv9__ : (sizeof(long) == 4); };
    struct isItV8 { int ABI_IS_sparcv8__ : (sizeof(long) != 4); };
#elif defined(__aarch64__)
    #error ABI_IS_arm64__
#elif defined(__ARM_EABI__)
    #if defined(__ARM_NEON__)
        #error ABI_IS_armeabi_neon__
    #else
        #error ABI_IS_armeabi__
    #endif
#elif defined(__arm__)
    #error ABI_IS_arm__
#elif defined(__mips__) || defined(__mips) || defined(__MIPS__)
    #if defined(_ABIO32)
        #if defined(_MIPS_ARCH_MIPS32R2)
            #error ABI_IS_mipso32_r2__
        #else
            #error ABI_IS_mipso32__
        #endif
    #elif defined(_ABIN32)
        #error ABI_IS_mips32__
    #else
        #error ABI_IS_mips64__
    #endif
#else
    #error ABI_IS_default__
#endif
