if (TOKYOCABINET_INCLUDE_DIRS AND TOKYOCABINET_LIBRARIES)
    set(TOKYOCABINET_FOUND TRUE)
else (TOKYOCABINET_INCLUDE_DIRS AND TOKYOCABINET_LIBRARIES)
    find_path(TOKYOCABINET_INCLUDE_DIRS
        NAMES
            tctdb.h
        PATHS
            ${TOKYOCABINET_PREFIX}/include
            /usr/include
            /usr/local/include
        NO_DEFAULT_PATH
    )

    find_library(TOKYOCABINET_LIBRARIES
        NAMES
            tokyocabinet
        PATHS
            ${TOKYOCABINET_PREFIX}/lib
            /usr/lib
            /usr/local/lib
        NO_DEFAULT_PATH
    )
    if(TOKYOCABINET_INCLUDE_DIRS AND TOKYOCABINET_LIBRARIES)
       set(TOKYOCABINET_FOUND TRUE)
    endif(TOKYOCABINET_INCLUDE_DIRS AND TOKYOCABINET_LIBRARIES)
endif (TOKYOCABINET_INCLUDE_DIRS AND TOKYOCABINET_LIBRARIES)

if (TOKYOCABINET_FOUND)
    message(STATUS "Found tokyocabinet: includes ${TOKYOCABINET_INCLUDE_DIRS} libs ${TOKYOCABINET_LIBRARIES}")
else (TOKYOCABINET_FOUND)
    if (Tokyocabinet_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find tokyocabinet, try to setup TOKYOCABINET_PREFIX accordingly")
    endif (Tokyocabinet_FIND_REQUIRED)
endif (TOKYOCABINET_FOUND)
