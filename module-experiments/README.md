## How to build gcc
```bash
git clone git@github.com:mathstuf/gcc.git
cd gcc
git checkout p1689r5
./contrib/download_prerequisites
cd ..
mkdir install
mkdir objdir && cd objdir
../gcc/configure --prefix=$PWD/../install --disable-multilib
make # it requires ~ 6-7 hours
make install
cd ../install/
./bin/g++ --version
```
output:
```
g++ (GCC) 13.0.0 20220608 (experimental)
Copyright (C) 2022 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## How to build cmake
