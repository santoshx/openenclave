# SGX1 mode: Prereqs

## Intel SGX Platform Software for Windows (PSW) v2.2

The PSW should be installed automatically on Windows 10 with the Fall Creators
Update installed, or on a Windows Server 2016 image for an Azure Confidential
Compute VM. You can verify that is the case on the command line as follows:

```cmd
sc query aesmservice
```

The state of the service should be "running" (4). Follow Intel's documentation for troubleshooting.

Note that Open Enclave is only compatible with the Intel PSW 2.2.