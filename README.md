cpp_properties
==============

> This library provides a complete lexer to produce a property abstract syntax tree
according to the java properties grammar (https://docs.oracle.com/javase/8/docs/api/java/util/Properties.html).

Compiling
---------

cmake supports 'out of source' builds by default, to use it do this:

```shell
mkdir build
cd build
cmake ..
make
```

this should create everything into the `build/` directory

Test
----

Run the cpp_properties test suites. This is covered by cmake:

```shell
make test
```
