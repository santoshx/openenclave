# Getting Started with Open Enclave on Windows with SGX1-FLC mode

## Platform requirements

- Windows client with windows 10 update august 2019 or Windows Server 2016
- SGX1 capable system with Flexible Launch Control support. Most likely this will be an Intel Coffeelake system.

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

```scripts/install-windows-prereqs.ps1```

```powershell
cd scripts
.\install-windows-prereqs.ps1 -InstallPath PATH_TO_OE_REPO -WithFLC $true -WithAzureDCAPClient $true
```

As an example, if you cloned Open Enclave SDK repo into c:\openenclave, you would run the following

```powershell
cd scripts
.\install-windows-prereqs.ps1 -InstallPath c:\openenclave -WithFLC $true -WithAzureDCAPClient $true
```

If you prefer to manually install prerequisites, please refer to this [document](WindowsManualInstallPrereqs.md).

## Build

To build, first create a build directory ("build" in the example below) and change directory into it.
Then run `cmake` to configure the build and generate the Makefiles, and then build by running `ninja'.

```cmd
cd C:\openenclave
mkdir build\x64-Debug
cd build\x64-Debug
cmake -G Ninja -DBUILD_ENCLAVES=1 -DUSE_LIBSGX=1 ../..
ninja
```

Open Enclave will support attestation workflows outside of Azure using DCAP in an upcoming release.

Refer to the [Advanced Build Information](AdvancedBuildInfo.md) documentation for further information.

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
