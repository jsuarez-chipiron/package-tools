# package-tools

## merge-packages

### compile

```sh
mkdir build && cd build
cmake ..
make
```

### execute

```sh
./build/merge-packages package1.xml package2.xml 57.0
```

The command outputs the merge of both input files. The types are sorted by name and the members also sorted in each type.

The last arg is the version to be printed

