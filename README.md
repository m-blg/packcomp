# packcomp

## Description

Package consinsts of shared library packcomp.so.

## Dependencies

libcurl.so.4\
libjson-c.so.5\
libcriterion.so.3 - for testing


## Installation

To install shared library:
```bash
make
sudo make install
```

## Removal

```bash
sudo make uninstall
```

## Updating

```bash
sudo make uninstall
make clean
git pull

# See installation paragraph 
make
sudo make install
sudo make install-dev # instead for additionally installing headers
```

## Testing

```bash
make test # to run all tests
make run test=<test_name_no_ext> # to run specific test
make run test=rpmveccmp_tests

make test-bin # quick cli utility test
```


## Usage example

```bash
packcomp p9 p10 -va x86_64,armh -o tests/out.txt
packcomp --help # for more options
```


## Frontend writing guidelines


```bash
make
sudo make install-dev # to additionally install headers to /usr/include
```

```cpp
#include "packcomp/packcomp.h"
```