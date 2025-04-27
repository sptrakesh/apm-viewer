set(Target_Name "apm-app")                        
file(GLOB_RECURSE apm_app_SRCS *.cpp *.cc *.hpp *.h)
add_executable(${Target_Name} ${apm_app_SRCS})

target_compile_definitions(${Target_Name}
    PRIVATE _FORTIFY_SOURCE=2
)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  message(STATUS "Debug build, not enabling IPO/LTO")
else()
  include(CheckIPOSupported)
  check_ipo_supported(RESULT result OUTPUT output LANGUAGES CXX)
  if (result)
    message(STATUS "IPO is supported: ${output}")
    set_property(TARGET ${Target_Name} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized -flto=auto")
  else()
    message(STATUS "IPO is not supported: ${output}")
  endif (result)
endif (CMAKE_BUILD_TYPE MATCHES Debug)

if (APPLE)
  target_link_libraries(${Target_Name}
  PRIVATE  Boost::boost Boost::iostreams Boost::json Boost::url
    fmt::fmt range-v3::range-v3 Wt::HTTP ${LIBPQ_LIBRARY} ${LIBPQXX_LIBRARY}
    ${JEMALLOC_LIBRARY} ${OPENSSL_LIBRARIES} gssapi_krb5 -rdynamic
)
else()
  target_link_libraries(${Target_Name}
  PRIVATE Boost::boost Boost::iostreams Boost::json Boost::url
    fmt::fmt range-v3::range-v3 Wt::HTTP pq pqxx
    jemalloc ssl crypto -export-dynamic
)
endif (APPLE)

install(TARGETS ${Target_Name} DESTINATION bin)
