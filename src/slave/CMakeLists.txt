set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.52 REQUIRED serialization system)

find_package(OpenSSL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

set(SOURCES
    Discussion.cxx
    Changeset.cxx
    Database.cxx
    ClientRequest.cxx
    main.cxx)

add_executable(slave ${SOURCES})

set_target_properties(slave PROPERTIES COMPILE_FLAGS "-std=c++11")

target_link_libraries(slave ${Boost_LIBRARIES} ConnCS ConnMS shared ${OPENSSL_LIBRARIES})

install(TARGETS slave RUNTIME DESTINATION slave/bin)
