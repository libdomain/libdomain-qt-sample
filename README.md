# Documentation for the Get-RootDSE Example Program using the libdomain Library

## Overview

The provided example is a C program that demonstrates the use of the libdomain library to perform an LDAP search operation.
The program establishes a connection to the LDAP server and executes a search reques to get RootDSE entry in the specified directory server.

## Compilation

To compile the program, you need to install the libdomain library.

```bash
apt-get install libdomain-devel libconfig-devel cmake rpm-build qt5-base-devel
```

Clone the example:

```bash
git clone https://github.com/libdomain/libdomain-qt-sample
```

Use the following compilation command:

```bash
mkdir build && cd build && cmake .. && make -j `nproc`
```

## Usage

The program has connection dialog to allow user to specify LDAP server connection parameters.

```bash
./libdomain-qt-sample
```

## Program Structure

TODO: Describe program structure.

## Error Handling

TODO: Describe error handling.

## Notes

TODO: Add description.

## Additional Information

## Version Information

- Program Version: 1.0.0

## License

This program is distributed under the GPLv2 license. See the accompanying LICENSE file for detailed information.
