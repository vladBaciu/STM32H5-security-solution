# Install script for directory: D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/Middlewares/mbed-crypto/programs/ssl

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/dtls_client.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dtls_client.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dtls_client.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dtls_client.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/CMakeFiles/dtls_client.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/dtls_server.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dtls_server.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dtls_server.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/dtls_server.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/CMakeFiles/dtls_server.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/mini_client.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mini_client.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mini_client.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/mini_client.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/CMakeFiles/mini_client.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/ssl_client1.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_client1.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_client1.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_client1.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/CMakeFiles/ssl_client1.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/ssl_client2.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_client2.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_client2.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_client2.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/CMakeFiles/ssl_client2.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/ssl_context_info.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_context_info.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_context_info.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_context_info.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/CMakeFiles/ssl_context_info.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/ssl_fork_server.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_fork_server.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_fork_server.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_fork_server.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/CMakeFiles/ssl_fork_server.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/ssl_mail_client.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_mail_client.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_mail_client.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_mail_client.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/CMakeFiles/ssl_mail_client.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/ssl_server.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_server.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_server.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_server.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/CMakeFiles/ssl_server.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/ssl_server2.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_server2.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_server2.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_server2.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/CMakeFiles/ssl_server2.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/ssl_pthread_server.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_pthread_server.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_pthread_server.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/Installs/stmcubeide/STM32CubeIDE_1.15.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32/tools/bin/arm-none-eabi-strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ssl_pthread_server.elf")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("D:/PhD/Workspace/Git_phd/phd/research/embedded_security/attest_app/build/Debug/Middlewares/mbed-crypto/programs/ssl/CMakeFiles/ssl_pthread_server.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

