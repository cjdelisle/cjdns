set(impls
    "devurandom"
)
set(impls_Windows
    "CryptGenRandom"
)
if(NOT "${impls_${CMAKE_SYSTEM_NAME}}" STREQUAL "")
    list(INSERT impls 0 ${impls_${CMAKE_SYSTEM_NAME}})
endif()

function(success dir impl)
    message("randombytes() ${impl} -- Succeeded!")
    file(COPY "${dir}/randombytes-impl.h" DESTINATION "${CMAKE_BINARY_DIR}/include_internal")
    file(RENAME
        ${CMAKE_BINARY_DIR}/include_internal/randombytes-impl.h
        ${CMAKE_BINARY_DIR}/include_internal/randombytes.h
    )
    include_directories(${dir})
    add_library(randombytes ${dir}/${impl}.c)
endfunction()


file(READ "${CMAKE_SOURCE_DIR}/randombytes/test.c" testContent)
foreach(impl ${impls})
    set(dir "${CMAKE_BINARY_DIR}/randombytes/${impl}")
    file(MAKE_DIRECTORY ${dir})

    file(COPY "${CMAKE_SOURCE_DIR}/randombytes/${impl}.c" DESTINATION ${dir})
    file(RENAME "${dir}/${impl}.c" "${dir}/test.c")
    file(APPEND "${dir}/test.c" "${testContent}")

    file(COPY "${CMAKE_SOURCE_DIR}/randombytes/${impl}.c" DESTINATION ${dir})
    file(COPY "${CMAKE_SOURCE_DIR}/randombytes/${impl}.h" DESTINATION ${dir})
    file(RENAME "${dir}/${impl}.h" "${dir}/randombytes-impl.h")

    if (CMAKE_CROSSCOMPILING)
        try_compile(compileResult
                    ${dir}
                    ${dir}/test.c
                    OUTPUT_VARIABLE compileOut
        )
        if(NOT compileResult)
            message("randombytes() ${impl} -- Failed to compile [${compileOut}]")
        else()
            success("${dir}" "${impl}")
            break()
        endif()
    else()
        try_run(runResult compileResult
            ${dir}
            ${dir}/test.c
            COMPILE_OUTPUT_VARIABLE compileOut
        )
        if(NOT compileResult)
            message("randombytes() ${impl} -- Failed to compile [${compileOut}]")
        elseif(runResult)
            message("randombytes() ${impl} -- Failed")
        else()
            success(${dir} ${impl})
            break()
        endif()
    endif()
endforeach()
