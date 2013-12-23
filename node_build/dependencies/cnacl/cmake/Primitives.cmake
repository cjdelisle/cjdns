##
# Get the primitives for each operation.
#
# @param operations a list of operation names.
# @param output the name of a variable which will be set to a list of all operations
#               and primitives, seperated by an underscore.
#               eg: crypto_sign_edwards25519sha512batch
##
function(Primitives_get operations output)
    foreach(operation ${OPERATIONS})
        file(GLOB primitive_paths "${CMAKE_SOURCE_DIR}/${operation}/*")
        foreach(primitive_path ${primitive_paths})
            if(IS_DIRECTORY "${primitive_path}")
                string(REGEX REPLACE ".*/" "" primitive ${primitive_path})
                list(APPEND outList "${operation}_${primitive}")
            endif()
        endforeach()
    endforeach()
    set(${output} ${outList} PARENT_SCOPE)
endfunction()
