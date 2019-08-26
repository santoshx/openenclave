# Getting Started with Open Enclave on Windows with SGX1-FLC mode

## Platform requirements

The following are prerequisites for building and running Open Enclave on
Windows.

- Intel® X86-64bit architecture with SGX1 or SGX2
- A version of Windows OS with native support for SGX features:
   - For server: Windows Server 2016 (or newer)
   - For client: Windows 10 64-bit with Fall Creators Update (1709) or newer

## Install Git and Clone the Open Enclave SDK repo

Download and install Git for Windows from [here](https://git-scm.com/download/win)

Clone the Open Enclave SDK

```powershell
cd c:\
git clone https://github.com/openenclave/openenclave
```

This creates a source tree under the directory called openenclave.

## Install project prerequisites

First, change directory into the openenclave repository:

```powershell
cd openenclave
```

To deploy all the prerequisities for building Open Enclave including Intel's DCAP primitives and Azure's DCAP library, you can run the  following from powershell.

```powershell
cd scripts
.\install-windows-prereqs.ps1 -InstallPath PATH_TO_OE_REPO -WithFLC $false
```

As an example, if you cloned Open Enclave SDK repo into c:\openenclave, you would run the following

```powershell
cd scripts
.\install-windows-prereqs.ps1 -InstallPath c:\openenclave -WithFLC $false
```

If you prefer to manually install prerequisites, please refer to this [document](WindowsManualInstallPrereqs.md).

## Intel® SGX Platform Software for Windows (PSW)

The PSW should be installed automatically on Windows 10 with the Fall Creators
Update installed, or on a Windows Server 2016 image for an Azure Confidential
Compute VM. You can verify that is the case on the command line as follows:

```cmd
sc query aesmservice
```

The state of the service should be "running" (4). Follow Intel's documentation for troubleshooting.

Note that Open Enclave is only compatible with the Intel PSW 2.2.
To use Intel PSW 2.3 and higher, please refer _Building with Intel Data Center Attestation
Primitives (DCAP) libraries_ below.

## Building on Windows using Developer Command Prompt

1. Launch the [x64 Native Tools Command Prompt for VS 2017](
https://docs.microsoft.com/en-us/dotnet/framework/tools/developer-command-prompt-for-vs)
Normally this is accessible under the `Visual Studio 2017` folder in the Start Menu.

2. At the x64 Native Tools command prompt, use cmake and ninja to build the debug version:

   ```cmd
   cd C:\openenclave
   mkdir build\x64-Debug
   cd build\x64-Debug
   cmake -G Ninja -DBUILD_ENCLAVES=1 ../..
   ninja
   ```

   Similarly, build the release version with:

    ```cmd
   cd C:\openenclave
   mkdir build\x64-Release
   cd build\x64-Release
   cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_ENCLAVES=1 ../..
   ninja
   ```

## Run unittests

After building, run all unit test cases using `ctest` to confirm the SDK is built and working as expected.

Run the following command from the build directory:

```cmd
c:\openenclave\build\x64-Debug>ctest
```

You will see test logs similar to the following:

```cmd
  Test project C:/Users/radhikaj/openenclave/build/x64-Debug
        Start   1: tests/lockless_queue
  1/107 Test   #1: tests/lockless_queue ..................................   Passed    3.49 sec
        Start   2: tests/mem
  2/107 Test   #2: tests/mem .............................................   Passed    0.01 sec
  ...
  ....
100% tests passed, 0 tests failed out of 107
```

A clean pass of the above unitests run is an indication that your Open Enclave setup was successful. You can start playing with the Open Enclave samples after following the instructions in the "Install" section below to configure samples for building,

For more information refer to the [Advanced Test Info](AdvancedTestInfo.md) document.

## Packaging into Nuget Package

Instructions coming soon

## Known Issues

Samples have not yet been ported to Windows.

Not all tests currently run on Windows. See tests/CMakeLists.txt for a list of supported tests.
