# The choice of C and C++

A file system driver, if it is to be accepted into the mainline linux kernel,
must be written in C. So from the start we're designing the file system to
allow this to be accomplished relatively quickly as needed. Because we use
OpenSSL v3 quite extensively, all that functionality will be implemented via
C functions, and the C++ classes will be thin wrappers. 

Initially the higher level C++ classes will be in C++ only for convenience
of developing the proof of concept, but later they will move to C functions
with the C++ classes being kept as wrappers.

## Statically linked CLI

The crosscryptfs CLI is statically linked with OpenSSL v3 and any other utility
libraries required. This is to allow them to be ran in user space from a single
binary file without any additional system or administrator privileges.

## Stable ABI

The classes and C functions themself will have a stable ABI. This is because in
future there may be C++ GUI applications or similar linked to a dynamically
linked library for user space applications. 

This has also then forced the use
of class inheritance and virtual pointers to allow easy embedding and a
consistent ABI. 

*NOTE* The API will only be considered stable from version 1.0.

Smart pointers are used in preference to templates for this
reason. Where templates are used, they are always instantiated within the
library itself to ensure they are always there for client applications.

## NoExcept

All C++ classes shall be declared noexcept and will use error codes where
necessary and not use std::exception.

## Embedded devices

It is recommended that embedded devices make use of the C API rather than
the C++ API. This is because embedded compilers have poor support for
dynamic memory allocation and generally do not support std::exception either.

In particular, the C API will work on Zephyr devices, especially devices
like the Nordic Semiconductor Bluetooth chips.