# Husky-Kylin
Implement cubing algorithm in apache kylin using husky

[![Build Status](https://travis-ci.org/husky-team/husky-kylin.svg?branch=master)](https://travis-ci.org/husky-team/husky-kylin)

Dependencies
-------------
Husky-Kylin has the following minimal dependencies:

* CMake (Version >= 3.0.2, if >= 3.6.0, it should set `CMAKE_PREFIX_PATH` first when errors occur finding the following dependencies)
* Apache ORC (Version >= 1.5.2. `ORC_HOME` need to be set if you want to use existing installation of ORC. Otherwise it will be installed automatically)
* nlohmann/json (Version >= 3.2.0, it will be installed automatically)
* All dependencies of [husky](https://github.com/husky-team/husky)

Build
-------------
Download the latest source code of Husky-Kylin:

```bash
git clone --recursive https://github.com/husky-team/husky-kylin.git
cd husky-kylin
```

Do an out-of-source build using CMake:

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release # CMAKE_BUILD_TYPE: Release, Debug, RelWithDebInfo
make help                           # List all build target

make -j4 HuskyMaster                # Build the Husky master
```

Configuration & Running
-------------
For information of configuring and running Husky, please take a look at the [Husky README](https://github.com/husky-team/husky/blob/master/README.md).
