function(AbiName_get outputVar flags)
    try_compile(compileResult
        ${CMAKE_BINARY_DIR}/okcompilers/
        ${CMAKE_SOURCE_DIR}/okcompilers/abiname_xcompile.c
        CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${flags}
        OUTPUT_VARIABLE compileOut
    )
    message("Testing ABI type returned [${compileOut}]")
    string(REGEX MATCH "ABI_IS_[^ ]*__" abi "${compileOut}")
    string(REGEX REPLACE "ABI_IS_(.*)__" "\\1" abi "${abi}")
    set(${outputVar} ${abi} PARENT_SCOPE)
endfunction()
