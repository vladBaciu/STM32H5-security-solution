# Install script for directory: D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/Middlewares/mbed-crypto/programs/test

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ml_model_attestation")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/test/selftest.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/selftest.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/selftest.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/selftest.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/test/CMakeFiles/selftest.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/test/udp_proxy.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/udp_proxy.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/udp_proxy.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/udp_proxy.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/test/CMakeFiles/udp_proxy.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/test/benchmark.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/benchmark.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/benchmark.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/benchmark.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/test/CMakeFiles/benchmark.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/test/query_compile_time_config.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/query_compile_time_config.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/query_compile_time_config.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/query_compile_time_config.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/test/CMakeFiles/query_compile_time_config.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/test/zeroize.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/zeroize.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/zeroize.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/zeroize.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/test/CMakeFiles/zeroize.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

