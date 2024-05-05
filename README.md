# Codec 2 README

Codec 2 is an open source (LGPL 2.1) low bit rate speech codec: http://rowetel.com/codec2.html written in C99 standard C.

Also included:

  + The FreeDV API for digital voice over radio. FreeDV is an open source digital voice protocol that integrates modems, codecs, and FEC [README_freedv](README_freedv.md)
  + HF OFDM and FSK modems, FEC used in the FreeDV API
  + APIs for packet data over radio [README_data](README_data.md)
  + An STM32 embedded version of FreeDV 1600/700D/700E for the [SM1000](stm32/README.md)

## Old Code and Deprecated Features

In July 2023 this repo was refactored, older code can be found in https://github.com/drowe67/codec2-dev. 

We are currently conducting a major re-development of Codec 2, new speech coding, modems, and FreeDV modes are under active development.  We have limited resources available for maintenance or development on features that may soon be replaced (with the exception of major bugs).  We'd rather put our efforts into new features! Modes and Features we are not actively maintaining at present, and likely to be superseded in the near future include:
1. FreeDV 1600, 700C, 700D, 700E, 2020.
1. Codec 2 modes with the exception of Codec 2 3200 for M17.
1. Our fork of LPCNet.
1. The stm32/SM1000 firmware (paused until new modes available).

The code supporting these modes won't be going away any time soon (and we will continue to include any modes/code in popular use), but we have chosen not to actively develop it at this time.

## Pull Requests, Feature Requests

We have a process for considering Feature Requests and Pull Requests that we will guide you through.

If you have a Feature Request, please answer the questions in the [Feature Request Form](https://freedv.org/wp-content/uploads/sites/8/2024/03/FreeDV-027-Feature-Request-Form-V1.1.pdf), and submit your answers as a GitHub Issue.

Before writing any code or submitting a PR - **please discuss** the PR with developers by raising a GitHub Issue. We have many years of experience and a carefully considered plan for Codec 2 development, and can guide you on work that will most benefit this project.

Some key guidelines about the code in the `codec2` repo:
1. Only code that is required to build, test, or document libcodec2 goes in codec2.
2. Experimental work, code used for algorithm development, should probably go into some other repo.
3. Only widely used “production” code goes in codec2. If it has an user base of < 2 (e.g. personal projects, early R&D) - it should probably be application code or a fork.

## Ports to non C99 Compilers

For Windows applications (built with MSVC or any compiler) we recommend linking with our [cross-compiled](#building-for-windows) Codec 2 DLLs.  This lets you enjoy the benefits of our carefully developed, tested and maintained codebase without having to develop and maintain your own fork.

We have standardized on C99 and develop and test using gcc on a Linux platform.  Our focus needs to be on what’s unique about our project – the speech codec and modem waveforms, and we lack the resources to support multiple compilers.  If you want to build Codec 2 using a non-standard compiler like MSVC and certain embedded compilers you will need to maintain your own Codec 2 fork (a very large commitment).  If you decide to fork Codec 2 to a non C99 compiler - please ensure you port the ctests and that they all pass.  If the tests have not been ported or do not pass - it's not Codec 2.

## Quickstart

1. Install packages (Debian/Ubuntu):
   ```
   sudo apt install git build-essential cmake
   ```
   Fedora/RH distros:
   ```
   sudo dnf groupinstall "Development Tools" "C Development Tools and Libraries"
   sudo dnf install cmake
   ```
   
1. Build Codec 2:
   ```
   git clone https://github.com/drowe67/codec2.git
   cd codec2
   mkdir build_linux
   cd build_linux
   cmake ..
   make
   ```

1. Listen to Codec 2:
   ```
   cd codec2/build_linux
   ./demo/c2demo ../raw/hts1a.raw hts1a_c2.raw
   aplay -f S16_LE ../raw/hts1a.raw
   aplay -f S16_LE hts1a_c2.raw
   ```
1. Compress, decompress and then play a file using Codec 2 at 2400 bit/s:
   ```
   ./src/c2enc 2400 ../raw/hts1a.raw hts1a_c2.bit
   ./src/c2dec 2400 hts1a_c2.bit hts1a_c2_2400.raw 
   ```
   which can be played with:
   ```
   aplay -f S16_LE hts1a_c2_2400.raw
   ```
   Or using Codec 2 using 700C (700 bits/s):
   ```
   ./src/c2enc 700C ../raw/hts1a.raw hts1a_c2.bit
   ./src/c2dec 700C hts1a_c2.bit hts1a_c2_700.raw
   aplay -f S16_LE hts1a_c2_700.raw
   ```
1. If you prefer a one-liner without saving to files:
   ```
   ./src/c2enc 1300 ../raw/hts1a.raw - | ./src/c2dec 1300 - - | aplay -f S16_LE
   ```

1. Or you can use your microphone and headphones to encode and listen to the result on the fly:
   ```
   br=1300; arecord -f S16_LE -c 1 -r 8000 | ./src/c2enc $br - - | ./src/c2dec $br - - | aplay -f S16_LE -
   ```
   
## FreeDV 2020 support (building with LPCNet)

1. Build LPCNet:
   ```
   cd ~
   git clone https://github.com/drowe67/LPCNet
   cd LPCNet && mkdir build_linux && cd build_linux
   cmake .. 
   make
   ```

1. Build Codec 2 with LPCNet support:
   ```
   cd ~/codec2/build_linux && rm -Rf *
   cmake -DLPCNET_BUILD_DIR=~/LPCNet/build_linux ..
   make
   ```

## Documentation

An algorithm description can be found in `doc/codec2.pdf`.

## Programs

+ See `demo` directory for simple examples of using Codec and the FreeDV API.

+ `c2demo` encodes a file of speech samples, then decodes them and saves the result.

+ `c2enc` encodes a file of speech samples to a compressed file of encoded bits.  `c2dec` decodes a compressed file of bits to a file of speech samples.

+ `c2sim` is a simulation/development version of Codec 2.  It allows selective use of the various Codec 2 algorithms.  For example switching phase modelling or quantisation on and off.

+ `freedv_tx` & `freedv_rx` are command line implementations of the FreeDV protocol, which combines Codec 2, modems, and Forward Error Correction (FEC).
  
+ `cohpsk_*` are coherent PSK (COHPSK) HF modem command line programs.

+ `fdmdv_*` are differential PSK HF modem command line programs (README_fdmdv).

+ `fsk_*` are command line programs for a non-coherent FSK modem (README_fsk).

+ `ldpc_*` are LDPC encoder/decoder command line programs, based on the CML library.

+ `ofdm_*` are OFDM PSK HF modem command line programs (README_ofdm).

## Building and Running Unit Tests

CTest is used as a test framework, with support from [GNU Octave](https://www.gnu.org/software/octave/) scripts.

1. Install GNU Octave and libraries on Ubuntu with:
   ```
   sudo apt install octave octave-common octave-signal liboctave-dev gnuplot python3-numpy sox valgrind clang-format texmaker texlive-bibtex-extra texlive-science
   ```
1. To build and run the tests:
   ```
   cd ~/codec2
   rm -Rf build_linux && mkdir build_linux
   cd build_linux
   cmake -DUNITTEST=1 ..
   make
   ```

1. To just run tests without rebuilding:
   ```
   ctest
   ```

1. To get a verbose run (e.g. for test debugging):
   ```
   ctest -V
   ```

1. To just run a single test:
   ```
   ctest -R test_OFDM_modem_octave_port
   ```

1. To list the available tests:
   ```
   ctest -N
   ```

1. Many Octave scripts rely on the CML LDPC library.  To run these from the Octave CLI, you need to set
   the `CML_PATH` environment variable.  A convenient way to do this is using a `.octaverc` file
   in your `codec/octave` directory.  For example on a Linux machine, create a `.octaverc` file:
   ```
   setenv("CML_PATH","../build_linux/cml")
   ```  
   
## Directories
```
cmake       - cmake support files
demo        - Simple Codec 2 and FreeDv API demo applications
doc         - documentation
octave      - Octave scripts used to support ctests
src         - C source code for Codec 2, FDMDV modem, COHPSK modem, FreeDV API
raw         - speech files in raw format (16 bits signed linear 8 kHz)
stm32       - STM32F4 microcontroller and SM1000 FreeDV Adaptor support
unittest    - Code to perform and support testing. Part of Debug build.
wav         - speech files in wave file format
```
## GDB and Dump Files

1. To compile with debug symbols for using gdb:
   ```
   cd ~/codec2
   rm -Rf build_linux && mkdir build_linux
   cd build_linux
   CFLAGS=-g cmake ..
   make
   ```

## Building for Windows

We develop and test on Linux to the [C99 standard](#ports-to-non-c99-compilers). We recommend using MinGW to cross compile for Windows.

On Ubuntu Linux:
   ```
   sudo apt-get install mingw-w64
   mkdir build_windows && cd build_windows
   cmake .. -DCMAKE_TOOLCHAIN_FILE=/home/david/freedv-dev/cmake/Toolchain-Ubuntu-mingw32.cmake -DUNITTEST=FALSE -DGENERATE_CODEBOOK=/home/david/codec2/build_linux/src/generate_codebook 
   make
   ```
   
This will create a working `libcodec2.dll` file for use with other applications (e.g. FreeDV GUI which is in wide spread use on Windows).  Please note the utility/development command line applications (e.g. `freedv_rx.exe`) may not work exactly the same on the Windows CLI compared to running on a Unix machine/shell.  For example pipes may not function as expected, and ctests are not supported.  Our primary development and test environment is Linux, and we lack the resources to support and maintain these applications for other operating systems.

## Including Codec 2 in an Android project

In an Android Studio 'NDK' project (a project that uses 'native' code)
Codec 2 can be added to the project in the following way.

1. Add the Codec 2 source tree to your app (e.g. in app/src/main/codec2)
   (e.g. as a git sub-module).

1. Add Codec 2 to the CMakeList.txt (app/src/main/cpp/CMakeLists.txt):

    ```
    # Sets lib_src_DIR to the path of the target CMake project.
    set( codec2_src_DIR ../codec2/ )
    # Sets lib_build_DIR to the path of the desired output directory.
    set( codec2_build_DIR ../codec2/ )
    file(MAKE_DIRECTORY ${codec2_build_DIR})

    add_subdirectory( ${codec2_src_DIR} ${codec2_build_DIR} )

    include_directories(
	    ${codec2_src_DIR}/src
	    ${CMAKE_CURRENT_BINARY_DIR}/../codec2
    )
    ```
     
1. Add Codec 2 to the target_link_libraries in the same file.

## Building Codec 2 for Microcontrollers

Codec 2 requires a hardware Floating Point Unit (FPU) to run in real time.

Two build options have been added to support building on microcontrollers:
1. Setting the `cmake` variable MICROCONTROLLER_BUILD disables position independent code (-fPIC is not used).  This was required for the IMRT1052 used in Teensy 4/4.1).

1. On ARM machines, setting the C Flag \_\_EMBEDDED\_\_ and linking with the ARM CMSIS library will improve performance on ARM-based microcontrollers. \_\_REAL\_\_ and FDV\_ARM\_MATH are additional ARM-specific options that can be set to improve performance if required, especially with OFDM modes.

A CMakeLists.txt example for a microcontroller is below:

```
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(MICROCONTROLLER_BUILD 1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mlittle-endian -ffunction-sections -fdata-sections -g -O3")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ffunction-sections -fdata-sections")

add_definitions(-DCORTEX_M7 -D__EMBEDDED__)
add_definitions(-DFREEDV_MODE_EN_DEFAULT=0 -DFREEDV_MODE_1600_EN=1 -DFREEDV_MODE_700D_EN=1 -DFREEDV_MODE_700E_EN=1 -DCODEC2_MODE_EN_DEFAULT=0 -DCODEC2_MODE_1300_EN=1 -DCODEC2_MODE_700C_EN=1)
                    
FetchContent_Declare(codec2
    GIT_REPOSITORY https://github.com/drowe67/codec2.git
    GIT_TAG origin/main
    GIT_SHALLOW ON
    GIT_PROGRESS ON
)
FetchContent_GetProperties(codec2)
if(NOT ${codec2_POPULATED})
    FetchContent_Populate(codec2)
endif()
set(CMAKE_REQUIRED_FLAGS "")

set(LPCNET OFF CACHE BOOL "")
add_subdirectory(${codec2_SOURCE_DIR} ${codec2_BINARY_DIR} EXCLUDE_FROM_ALL)
```

## Building Debian packages

To build Debian packages, simply run the "cpack" command after running "make". This will generate the following packages:

+ codec2: Contains the .so and .a files for linking/executing applications dependent on Codec2.
* codec2-dev: Contains the header files for development using Codec2.

Once generated, they can be installed with "dpkg -i" (once LPCNet is installed). If LPCNet is not desired, CMakeLists.txt can be modified to remove that dependency.
