# DASHRPC

![dashrpc](./dashrpc.png)

This library is an implementation of the Dash *.proto files. It allows to do GRPC and uses the nlohmann library for JSON files. There is only the option to build from source. This library is not officially supported by the Dash.

It was developed to create stress testing for the Dash network. There is no guarantee that it will work for you **(。﹏。*)**

## Building

First of all, one has to clone the project from github. The repository is using submodules, however, those are handled in the next step. Now recursively update and init. the submodules, otherwise one will run into an error.

```bash
# clone the repository and be recursively update the submodules
git clone https://github.com/jctemp/dashrpc
git submodule update --init --recursive
```

After one has cloned the repository and took care of the submodules, one perpare the directory to build the library. If you want to adjust building parameters see [cmake.org](https://cmake.org/cmake/help/latest/manual/cmake.1.html).

```bash
# 1. create build directory
mkdir build

# 2. generate project build system
cmake --no-warn-unused-cli -S$(pwd) -B$(pwd)/build

# 3. build the project
#
# This part can take a lot of time due to the size of the grpc project.
# If you have pre-compiled grpc, then link agaist it manually.
#
# ┌──────────────────────────────┬───────────────────────┐
# │   cmake                      │                       │
# │     --build build            │ build directory       │
# │     --config [Debug|Release] │ build variant         │
# │     --parallel n             │ number of jobs        │
# │     --clean-first            │ clean build artefacts │
# └──────────────────────────────┴───────────────────────┘

cmake --build build --config Debug --parallel 8 --clean-first --target tools tools_c tools_cxx plugins libprotobuf libprotoc protoc dashrpc

# 3.1. create cmake string for targets
DASHRPC_CMAKE="--build build --config Debug --parallel 8 --clean-first" 

# 3.2. build grpc dependencies
cmake ${DASHRPC_CMAKE} --target tools tools_c tools_cxx plugins

# 3.3. build protoc dependencies
cmake ${DASHRPC_CMAKE} --target libprotobuf libprotoc protoc

# 3.4. build dashrpc
cmake ${DASHRPC_CMAKE} --target dashrpc

```
