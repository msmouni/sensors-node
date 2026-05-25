# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/marwan/.espressif/v6.0.1/esp-idf/components/bootloader/subproject"
  "/home/marwan/projects/esp-idf/sensors_node/build/bootloader"
  "/home/marwan/projects/esp-idf/sensors_node/build/bootloader-prefix"
  "/home/marwan/projects/esp-idf/sensors_node/build/bootloader-prefix/tmp"
  "/home/marwan/projects/esp-idf/sensors_node/build/bootloader-prefix/src/bootloader-stamp"
  "/home/marwan/projects/esp-idf/sensors_node/build/bootloader-prefix/src"
  "/home/marwan/projects/esp-idf/sensors_node/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/marwan/projects/esp-idf/sensors_node/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/marwan/projects/esp-idf/sensors_node/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
