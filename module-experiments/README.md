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

## How to build clang
```bash
git clone https://github.com/llvm/llvm-project.git
# in llvm-project src dir
mkdir my_build && mkdir my_install && cd my_build
cmake -S ../llvm/ -B ../my_build/ -DLLVM_ENABLE_PROJECTS="clang" -DCMAKE_INSTALL_PREFIX=../my_install/ -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=On -DLLVM_TARGETS_TO_BUILD=X86 -DLLVM_INCLUDE_TESTS=OFF -DLLVM_INCLUDE_EXAMPLES=OFF ../
make -j4
make install
```


## How to build cmake
