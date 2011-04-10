find_package(GCrypt REQUIRED)

include_directories(${GCRYPT_INCLUDE_DIRS})

add_library(seccure
    aes256ctr.c
    curves.c
    ecc.c
    numtheory.c
    protocol.c
    #seccure.c
    serialize.c
)

target_link_libraries(seccure ${GCRYPT_LIBRARIES})
