find_package(GCrypt REQUIRED)

set(SECCURE_SOURCES
    aes256ctr.c
    curves.c
    ecc.c
    numtheory.c
    protocol.c
    seccure.c
    serialize.c
    )

include_directories(${GCRYPT_INCLUDE_DIRS})
add_library(seccure ${SECCURE_SOURCES})
target_link_libraries(seccure ${GCRYPT_LIBRARIES})
