# packcomp

## Description

Package consinsts of shared library packcomp.so and cli frontend packcomp-cli.

packcomp fetches two branches, finds unique packages in both of them and also packages version of which in the first branch is greater than version of the corresponding packages in the second branch. This operation is performed for every specified package architecture (-a) or for every common architecture if none specified.

## Output

For each specified arch packcomp outputs json of the following format:
```json
{
    "arch": "", 
    "length1": 0, 
    "length2": 0, 
    "length3": 0, 
    "packages1": [
        { "name": "", "epoch": 0, "version": "", 
          "release": "", "arch": "", "disttag": "", 
          "buildtime": 0, "source": "" 
          }
    ],
    "packages2": [
        { "name": "", "epoch": 0, "version": "", 
          "release": "", "arch": "", "disttag": "", 
          "buildtime": 0, "source": "" 
          }
    ],
    "packages3": [
        { "name": "", "epoch": 0, "version": "", 
          "release": "", "arch": "", "disttag": "", 
          "buildtime": 0, "source": "" 
          }
    ]
}
```

where 'length1' is the length of 'packages1', 'length2' ...,\
arrray 'packages1' constains of all packages of branch1, that are not in branch2,\
arrray 'packages2' constains of all packages of branch2, that are not in branch1,\
arrray 'packages3' constains of all packages of branch1, the version of which is greater than the version of the same packages in branch2.

Several jsons are wrote sequentially one after another.




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