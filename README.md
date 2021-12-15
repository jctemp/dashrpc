# DASHRPC

Wrapper library implementing grpc to communicate with dash network.

## Building

```bash
  # setup conan profile
  pip install conan
  conan user

  # change conan to download modern abi for C++ (change in C++11)
  sed -i 's/compiler.libcxx=libstdc++/compiler.libcxx=libstdc++11/g' \
      "${HOME}/.conan/profiles/default"

  # prepare build directory
  rm -rf build
  mkdir build && cd cd build
  conan install ..

  # build project inside the build directory
  cmake .. && cmake --build .
```
