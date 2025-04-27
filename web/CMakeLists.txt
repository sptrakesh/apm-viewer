cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# For INTERPROCEDURAL_OPTIMIZATION
if (POLICY CMP0069)
  cmake_policy(SET CMP0069 NEW)
endif (POLICY CMP0069)

project(apm-app)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_USE_MULTITHREADED ON)
set(OPENSSL_USE_STATIC_LIBS TRUE)

if (APPLE)
  execute_process(
    COMMAND brew --prefix libpq
    RESULT_VARIABLE BREW_LIBPQ
    OUTPUT_VARIABLE BREW_LIBPQ_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  if (BREW_LIBPQ EQUAL 0 AND EXISTS "${BREW_LIBPQ_PREFIX}")
    message(STATUS "Found libpq keg installed by Homebrew at ${BREW_LIBPQ_PREFIX}")
    set(LIBPQ_ROOT_DIR "${BREW_LIBPQ_PREFIX}/")
    set(LIBPQ_INCLUDE_DIR "${BREW_LIBPQ_PREFIX}/include")
    set(LIBPQ_LIBRARIES "${BREW_LIBPQ_PREFIX}/lib")
    set(LIBPQ_LIBRARY "${BREW_LIBPQ_PREFIX}/lib/libpq.dylib")
  endif (BREW_LIBPQ EQUAL 0 AND EXISTS "${BREW_LIBPQ_PREFIX}")

  execute_process(
    COMMAND brew --prefix libpqxx
    RESULT_VARIABLE BREW_LIBPQXX
    OUTPUT_VARIABLE BREW_LIBPQXX_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  if (BREW_LIBPQXX EQUAL 0 AND EXISTS "${BREW_LIBPQXX_PREFIX}")
    message(STATUS "Found libpqxx keg installed by Homebrew at ${BREW_LIBPQXX_PREFIX}")
    set(LIBPQXX_ROOT_DIR "${BREW_LIBPQXX_PREFIX}/")
    set(LIBPQXX_INCLUDE_DIR "${BREW_LIBPQXX_PREFIX}/include")
    set(LIBPQXX_LIBRARIES "${BREW_LIBPQXX_PREFIX}/lib")
    set(LIBPQXX_LIBRARY "${BREW_LIBPQXX_PREFIX}/lib/libpqxx.dylib")
  endif (BREW_LIBPQXX EQUAL 0 AND EXISTS "${BREW_LIBPQXX_PREFIX}")

  execute_process(
    COMMAND brew --prefix OpenSSL
    RESULT_VARIABLE BREW_OPENSSL
    OUTPUT_VARIABLE BREW_OPENSSL_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  if (BREW_OPENSSL EQUAL 0 AND EXISTS "${BREW_OPENSSL_PREFIX}")
    message(STATUS "Found OpenSSL keg installed by Homebrew at ${BREW_OPENSSL_PREFIX}")
    set(OPENSSL_ROOT_DIR "${BREW_OPENSSL_PREFIX}/")
    set(OPENSSL_INCLUDE_DIR "${BREW_OPENSSL_PREFIX}/include")
    set(OPENSSL_LIBRARIES "${BREW_OPENSSL_PREFIX}/lib")
    set(OPENSSL_CRYPTO_LIBRARY "${BREW_OPENSSL_PREFIX}/lib/libcrypto.dylib")
    set(OPENSSL_LIBRARY "${BREW_OPENSSL_PREFIX}/lib/libssl.dylib")
  endif (BREW_OPENSSL EQUAL 0 AND EXISTS "${BREW_OPENSSL_PREFIX}")

  execute_process(
    COMMAND brew --prefix jemalloc
    RESULT_VARIABLE BREW_JEMALLOC
    OUTPUT_VARIABLE BREW_JEMALLOC_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  if (BREW_JEMALLOC EQUAL 0 AND EXISTS "${BREW_JEMALLOC_PREFIX}")
    message(STATUS "Found jemalloc keg installed by Homebrew at ${BREW_JEMALLOC_PREFIX}")
    set(JEMALLOC_ROOT_DIR "${BREW_JEMALLOC_PREFIX}/")
    set(JEMALLOC_LIBRARIES "${BREW_JEMALLOC_PREFIX}/lib")
    set(JEMALLOC_LIBRARY "${BREW_JEMALLOC_PREFIX}/lib/libjemalloc.dylib")
  endif (BREW_JEMALLOC EQUAL 0 AND EXISTS "${BREW_JEMALLOC_PREFIX}")
else(APPLE)
endif (APPLE)

find_package(Boost REQUIRED COMPONENTS filesystem iostreams json program_options thread url)
find_package(Wt REQUIRED Wt HTTP)
find_package(fmt REQUIRED)
find_package(range-v3 REQUIRED)
find_package(Threads)
find_package(OpenSSL REQUIRED)

INCLUDE(cmake/FindHPDF.cmake)
find_package(HPDF)

include_directories(
  ${Boost_INCLUDE_DIR}
  ${LIBPQXX_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

if (APPLE)
  set(CMAKE_MACOSX_RPATH 1)
  include_directories(
    /usr/local/spt/include
    /usr/local/wt/include
  )
  link_directories(
    /usr/local/spt/lib
    /usr/local/wt/lib
  )
else()
  include_directories(
    /opt/local/include
    /opt/spt/include
  )
  link_directories(
    /opt/local/lib
    /opt/spt/lib
  )
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX /opt/spt)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

add_subdirectory(src)
