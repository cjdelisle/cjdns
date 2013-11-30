#
# Public Domain.
#
project(nacl C)
cmake_minimum_required(VERSION 2.4)

# Generic hook to allow arbitrary cmake code to be run for preconfiguration.
# to pass assert that ARM NEON is available:
# set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -mfpu=neon")
if(CNACL_CONFIG_SCRIPT)
    include(${CNACL_CONFIG_SCRIPT})
endif()

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/include")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/include_internal")
add_subdirectory(randombytes)

if(CMAKE_VERSION VERSION_LESS 2.8.4)
    message("Parallel building (-j) will not be available.")
    message("To build in parallel, upgrade to cmake 2.8.4 or newer.")
    message("see: http://www.cmake.org/Bug/print_bug_page.php?bug_id=10395")
endif()

#################################

enable_language(ASM)
set(CMAKE_ASM_COMPILER_ARG1 "${MY_CMAKE_ASM_FLAGS} -c")

macro(print appendTo content)
    list(APPEND ${appendTo} "${content}\n")
endmacro()

##
# Create a header file for an operation/primitive such as crypto_stream/salsa20.
# NOTE: Not all primitives are required to behave the same for a given operation.
#       crypto_stream/salsa20 will obviously give you a different output
#       than crypto_stream/aes128ctr.
#
# @param operation the operation name, a generic name for the job which is done.
# @param primitive the name of the algorithm which is used to do the operation.
# @param output the name of a variable which will be set to a string containing the header file.
##
function(writeOperationHeader operation primitive output)
    set(op "${operation}_${primitive}")
    set(out "")
    print(out "#ifndef ${operation}_H")
    print(out "#define ${operation}_H")
    print(out "")
    print(out "#include \"${op}.h\"")
    print(out "")

    foreach(macro ${MACROS})
        if("${macro}" MATCHES "${operation}$|${operation}\\(|${operation}_")
            string(REGEX REPLACE "${operation}" "${op}" regexout "${macro}")
            print( out "#define ${macro} ${regexout}")
        endif()
    endforeach()

    print(out "#define ${operation}_PRIMITIVE \"${primitive}\"")
    print(out "#define ${operation}_IMPLEMENTATION ${op}_IMPLEMENTATION")
    print(out "#define ${operation}_VERSION ${op}_VERSION")
    print(out "")
    print(out "#endif")
    string(REGEX REPLACE "\n;" "\n" tmpoutStr "${out}")
    set(${output} ${tmpoutStr} PARENT_SCOPE)
endfunction()

##
# Create a header file for an operation/primitive/implementation
# such as crypto_stream/salsa20/amd64_xmm.
# This is a (potentially processor specific) implementation of an operation/primitive.
# All implementations must return the same output for a given input.
#
# @param operation the operation name, a generic name for the job which is done.
# @param primitive the name of the algorithm which is used to do the operation.
# @param implementation the name of the implementation of the operation/primitive.
# @param output the name of a variable which will be set to a string containing the header file.
##
function(writeOperationPrimitiveHeader operation primitive implementation output)
    set(op "${operation}_${primitive}")
    set(path "${operation}/${primitive}/${implementation}")
    string(REGEX REPLACE "[\\.-/]" "_" opi "${path}")

    set(tmpout "")
    print(tmpout "#ifndef ${op}_H")
    print(tmpout "#define ${op}_H")
    print(tmpout "")
    file(STRINGS "${CMAKE_SOURCE_DIR}/${path}/api.h" api)
    foreach(line ${api})
        string(REGEX REPLACE "[ \\t]CRYPTO_" " ${opi}_" out "${line}")
        print(tmpout "${out}")
    endforeach()

    # C++
    print(tmpout "#ifdef __cplusplus")
    print(tmpout "#include <string>")
    foreach(prototype ${PROTOTYPES_CPP})
        if("${prototype}" MATCHES "${operation}$|${operation}\\(|${operation}_")
            string(REGEX REPLACE "${operation}" "${opi}" out "${prototype}")
            print(tmpout "${out}")
        endif()
    endforeach()
    print(tmpout "extern \"C\" {")
    print(tmpout "#endif")

    # C
    foreach(prototype ${PROTOTYPES})
        if("${prototype}" MATCHES "${operation}$|${operation}\\(|${operation}_")
            string(REGEX REPLACE "${operation}" "${opi}" out "${prototype}")
            print(tmpout "${out}")
        endif()
    endforeach()

    print(tmpout "#ifdef __cplusplus")
    print(tmpout "}")
    print(tmpout "#endif")

    foreach(macro ${MACROS})
        if("${macro}" MATCHES "${operation}$|${operation}\\(|${operation}_")
            string(REGEX REPLACE "${operation}" "${opi}" mopi "${macro}")
            string(REGEX REPLACE "${operation}" "${op}" mop "${macro}")
            print(tmpout "#define ${mop} ${mopi}")
        endif()
    endforeach()

    print(tmpout "#define ${op}_IMPLEMENTATION \"${path}\"")
    print(tmpout "#ifndef ${opi}_VERSION")
    print(tmpout "#define ${opi}_VERSION \"-\"")
    print(tmpout "#endif")
    print(tmpout "#define ${op}_VERSION ${opi}_VERSION")
    print(tmpout "")
    print(tmpout "#endif")
    string(REGEX REPLACE "\n;" "\n" tmpoutStr "${tmpout}")
    set(${output} ${tmpoutStr} PARENT_SCOPE)
endfunction()

function(doOperationPrimitiveImplementation
    operation
    primitive
    implementation
    allOpPrimitives
    runTest
)
    #message("${operation} --- ${primitive} --- ${implementation}")
    #message("${allOpPrimitives}")
    set(opiPath "${operation}/${primitive}/${implementation}")
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${opiPath}")
    writeOperationPrimitiveHeader("${operation}" "${primitive}" "${implementation}" output)
    file(WRITE "${CMAKE_BINARY_DIR}/include/${operation}_${primitive}.h" "${output}")

    file(GLOB files "${CMAKE_SOURCE_DIR}/${opiPath}/*.[csS]")

    set(lib "${operation}_${primitive}_${implementation}")

    print(tmpout "set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})")
    print(tmpout "include_directories(
        \"${CMAKE_BINARY_DIR}/${operation}/${primitive}\"
        \"${CMAKE_BINARY_DIR}/${opiPath}\"
        \"${CMAKE_BINARY_DIR}/include\"
        \"${CMAKE_BINARY_DIR}/include_internal\"
    )")

    print(tmpout "add_library(${lib} ${files})")
    print(tmpout "set_target_properties(${lib} PROPERTIES LINKER_LANGUAGE C)")

    string(REGEX REPLACE "\n;" "\n" tmpoutStr "${tmpout}")
    file(WRITE "${CMAKE_BINARY_DIR}/${opiPath}/CMakeLists.txt" "${tmpoutStr}")
endfunction()

function(executePlan implementations)
    set(opPrimitives "")
    foreach(line ${implementations})
        string(REGEX REPLACE "/[^/]*$" "" opSlashPrim ${line})
        string(REPLACE "/" "_" opPrim ${opSlashPrim})
        list(APPEND opPrimitives ${opPrim})
    endforeach()

    set(operations "")

    foreach(line ${implementations})
        string(REPLACE "/" ";" newLine ${line})
        list(GET newLine 0 op)
        list(GET newLine 1 prim)
        list(GET newLine 2 impl)

        doOperationPrimitiveImplementation(${op} ${prim} ${impl} "${opPrimitives}" FALSE)

        set("${op}_primitives"
            "${${op}_primitives};${prim}"
            CACHE INTERNAL "primitives by operation" FORCE
        )
        set("${op}_${prim}_implementations"
            "${${op}_${prim}_implementations};${impl}"
            CACHE INTERNAL "implementations by primitivie" FORCE
        )

        list(APPEND operations "${op}")
    endforeach()

    set(libraries "")

    cmake_policy(SET CMP0007 NEW)
    list(REMOVE_DUPLICATES operations)
    foreach(op ${operations})
        list(REMOVE_DUPLICATES ${op}_primitives)
        foreach(prim ${${op}_primitives})
            list(REMOVE_DUPLICATES ${op}_${prim}_implementations)
            #message("${op} ---- ${prim}")
            foreach(impl ${${op}_${prim}_implementations})
                #message("${op} === ${prim} === ${impl}")
                print(tmpout "add_subdirectory(${impl})")
                list(APPEND libraries "${op}_${prim}_${impl}")
            endforeach()
            string(REGEX REPLACE "\n;" "\n" tmpoutStr "${tmpout}")
            file(WRITE "${CMAKE_BINARY_DIR}/${op}/${prim}/CMakeLists.txt" "${tmpoutStr}")
            set(tmpout "")
            print(opOut "add_subdirectory(${prim})")

            writeOperationHeader("${op}" "${prim}" output)
            file(WRITE "${CMAKE_BINARY_DIR}/${op}/${prim}/${op}.h" "${output}")
        endforeach()
        string(REGEX REPLACE "\n;" "\n" tmpoutStr "${opOut}")
        file(WRITE "${CMAKE_BINARY_DIR}/${op}/CMakeLists.txt" "${tmpoutStr}")
        set(opOut "")
        add_subdirectory("${CMAKE_BINARY_DIR}/${op}" "${CMAKE_BINARY_DIR}/${op}")
    endforeach()

    include("${CMAKE_SOURCE_DIR}/cmake/libutils.cmake")
    merge_static_libs(nacl nacl "${libraries}")
    #add_dependencies(nacl randombytes)
endFunction()

function(writeTypesHeaders types)
    foreach(intfile
        "crypto_uint8.h"
        "crypto_int8.h"
        "crypto_uint16.h"
        "crypto_int26.h"
        "crypto_uint32.h"
        "crypto_int32.h"
        "crypto_uint64.h"
        "crypto_int64.h"
    )
        file(WRITE "${CMAKE_BINARY_DIR}/include_internal/${intfile}" "#include <crypto_types.h>")
    endforeach()

    set(out)
    print(out "#ifndef crypto_types_h")
    print(out "#define crypto_types_h")
    foreach(type ${types})
        print(out "${type};")
    endforeach()
    print(out "#endif")

    string(REGEX REPLACE "\n;" "\n" tmpoutStr "${out}")
    file(WRITE "${CMAKE_BINARY_DIR}/include/crypto_types.h" "${tmpoutStr}")
endfunction()


include("${CMAKE_SOURCE_DIR}/cmake/AbiName.cmake")
AbiName_get(abi "${CMAKE_REQUIRED_FLAGS}")
message("Detected ABI as ${abi}")
set(planPath "${CMAKE_SOURCE_DIR}/cmake/plans/${abi}_plan.cmake")

## On apple systems, the assembler is different and it doesn't accept
## a number of the asm files in NaCl
if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Darwin")
    set(planPath "${CMAKE_SOURCE_DIR}/cmake/plans/apple_${abi}_plan.cmake")
endif()
message("Using a [${CMAKE_HOST_SYSTEM_NAME}] toolchain.")

if (NOT EXISTS "${planPath}")
    message("Could not find compile plan for this ABI, please wait while one is generated...")
    execute_process(COMMAND "${CMAKE_SOURCE_DIR}/do" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

    include("${CMAKE_SOURCE_DIR}/cmake/MakePlan.cmake")
    makePlan("${abi}" "${planPath}")

    if (NOT EXISTS "${planPath}")
        message("Can't find plan for the target ABI [${abi}].")
        message(FATAL_ERROR "Cross compiling is not supported without a premade plan.")
    endif()
    message("Created new plan for ${abi} at ${planPath}")
    file(READ "${planPath}" plan)
    message("${plan}")
else()
    message("Using preexisting build plan ${planPath}")
endif()

include("${planPath}")
writeTypesHeaders("${PLAN_TYPES}")

file(STRINGS "${CMAKE_SOURCE_DIR}/MACROS" MACROS)
file(STRINGS "${CMAKE_SOURCE_DIR}/PROTOTYPES.c" PROTOTYPES)
file(STRINGS "${CMAKE_SOURCE_DIR}/PROTOTYPES.cpp" PROTOTYPES_CPP)
file(STRINGS "${CMAKE_SOURCE_DIR}/OPERATIONS" OPERATIONS)
executePlan("${PLAN_IMPLEMENTATIONS}")


enable_testing()
add_subdirectory(tests)
