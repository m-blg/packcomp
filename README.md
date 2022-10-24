# packcomp

## Description

Package consinsts of shared library packcomp.so and cli frontend packcomp-cli.

packcomp fetches two branches, finds unique packages in both of them and also packages version of which in the first branch is greater than version of the corresponding packages in the second branch. This operation is performed for every specified package architecture (-a) or for every common architecture if none specified.

## Installation

```bash
make
sudo make install
```

## Removal

```bash
sudo make uninstall
```

## Usage example

```bash
packcomp p9 p10 -va x86_64,armh -o tests/out.txt
packcomp --help # for more options
```