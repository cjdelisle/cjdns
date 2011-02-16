if (GCRYPT_INCLUDE_DIRS AND GCRYPT_LIBRARIES)
    set(GCRYPT_FOUND TRUE)
else (GCRYPT_INCLUDE_DIRS AND GCRYPT_LIBRARIES)
    find_path(GCRYPT_INCLUDE_DIRS
        NAMES
            gcrypt.h
        PATHS
            ${GCRYPT_PREFIX}/include
            /usr/include
            /usr/local/include
        NO_DEFAULT_PATH
    )

    find_library(GCRYPT_LIBRARIES
        NAMES
            gcrypt
        PATHS
            ${GCRYPT_PREFIX}/lib
            /usr/lib
            /usr/local/lib
        NO_DEFAULT_PATH
    )
    if(GCRYPT_INCLUDE_DIRS AND GCRYPT_LIBRARIES)
       set(GCRYPT_FOUND TRUE)
    endif(GCRYPT_INCLUDE_DIRS AND GCRYPT_LIBRARIES)
endif (GCRYPT_INCLUDE_DIRS AND GCRYPT_LIBRARIES)

if (GCRYPT_FOUND)
    message(STATUS "Found libgcrypt: includes ${GCRYPT_INCLUDE_DIRS} libs ${GCRYPT_LIBRARIES}")
else (GCRYPT_FOUND)
    if (GCrypt_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find libgcrypt, try to setup GCRYPT_PREFIX accordingly")
    endif (GCrypt_FIND_REQUIRED)
endif (GCRYPT_FOUND)
