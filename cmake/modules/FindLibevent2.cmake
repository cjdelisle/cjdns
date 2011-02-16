if (LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
    set(LIBEVENT2_FOUND TRUE)
else (LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
    find_path(LIBEVENT2_INCLUDE_DIRS
        NAMES
            event2/dns.h
        PATHS
            ${LIBEVENT2_PREFIX}/include
            /usr/include
            /usr/local/include
        NO_DEFAULT_PATH
    )

    find_library(LIBEVENT2_LIBRARIES
        NAMES
            event
        PATHS
            ${LIBEVENT2_PREFIX}/lib
            /usr/lib
            /usr/local/lib
        NO_DEFAULT_PATH
    )
    if(LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
       set(LIBEVENT2_FOUND TRUE)
    endif(LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
endif (LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)

if (LIBEVENT2_FOUND)
    message(STATUS "Found libevent2: includes ${LIBEVENT2_INCLUDE_DIRS} libs ${LIBEVENT2_LIBRARIES}")
else (LIBEVENT2_FOUND)
    if (Libevent2_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find libevent2, try to setup LIBEVENT2_PREFIX accordingly")
    endif (Libevent2_FIND_REQUIRED)
endif (LIBEVENT2_FOUND)
