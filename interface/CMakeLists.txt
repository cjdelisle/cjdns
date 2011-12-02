find_package(Libevent2 REQUIRED)

add_library(interface
    UDPInterface.c
    TUNInterface.c
    InterfaceConnector.c
)
##target_link_libraries(interface benc benc_StandardBencSerializer util)#dhtstore)

# Everything must be tested.
enable_testing()
##add_subdirectory(test)
