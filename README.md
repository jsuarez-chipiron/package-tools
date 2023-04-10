# package-tools

### compile

```sh
mkdir build && cd build
cmake ..
make
```

## merge-packages

### execute

```sh
./build/merge-packages package1.xml package2.xml 57.0
```

The command outputs the merge of both input files. The types are sorted by name and the members also sorted in each type.

The last arg is the version to be printed

## extract-tests

### execute

```sh
./build/extract-tests package.xml
```

Return a list of the test classes included in the package for use as RunSpecifiedTests content
