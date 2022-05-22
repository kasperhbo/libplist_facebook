## libplist

libplist is a library to maniuplate .plist file. The [plist file format](https://en.wikipedia.org/wiki/Property_list) is commonly used on Apple's operating systems.

The source code is extracted from the Facebook [xcbuild](https://github.com/facebookarchive/xcbuild) project.

### Requirements

* cmake
* libxml2-dev or libexpat-dev (Linux/BSD)

### Build

```sh
git clone https://github.com/soflare/libplist
cd libplist
git submodule update --init
mkdir build
cd build
cmake ..
make
```

Build options:

* -DBUILD_TESTING      : Build unit tests (default OFF)
* -DBUILD_SHARED_LIBS  : Build shared library (default ON)
* -DUSE_EXPAT          : Link with expat (default ON)
