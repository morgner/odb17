Building with the documentation
===============================

(Docu is already here: http://odb17.readthedocs.io/en/latest/classes/odb.html)

required: 
 * boost-containers

sample code dependencies:
 * libjsoncpp       https://github.com/nlohmann/json
 * asio (non-boost) https://github.com/chriskohlhoff/asio
 * linenoise        https://github.com/antirez/linenoise

```
cd <project_root>
virtualenv env
. env/bin/activate
pip install -r docs/requirements.txt
mkdir -p build
cd build
cmake ..
cmake --build .
or:
cmake --build . --target all -- -j4 (amount of cpu's to use)
```

Building **WITHOUT** the documentation
======================================

required:
 * boost-containers

sample code dependencies:
 * libjsoncpp       https://github.com/nlohmann/json
 * asio (non-boost) https://github.com/chriskohlhoff/asio
 * linenoise        https://github.com/antirez/linenoise

```
cd <project_root>
mkdir -p build
cd build
cmake .. -DBUILD_DOCS=Off
cmake --build .
or
cmake --build . --target all -- -j4 (amount of cpu's to use)
```

------------------- OR (e.g.)

mkdir -p build/CodeBlocks
cd build/CodeBlocks
cmake ../../ -G "CodeBlocks - Unix Makefiles"
codeblocks odb.cbp

------------------- OR

use a different CMAKE Generator suitable to you environment

