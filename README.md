# Chess engine
Please refer to our [wiki](https://github.com/Shibe/chess-engine/wiki) for useful papers, links, background information and techniques used in this project.

## build instructions
On Unix-like systems it should be possible to compile the source code with the Makefile.
Before compiling the source, make sure to have a ```bin``` and ```obj``` folder in the root of the project:
```
mkdir bin obj
```
To compile the source code, run the following command:
```
make build
```
Running the program can be done using make:
```
make run
```
Or simply do:
```
./bin/program
```

## running the tests
Tests are ran using [greatest](https://github.com/silentbicycle/greatest). A seperate main is made for running tests.
To run tests on Unix-like systems, make sure to have the ```bin``` and ```obj``` folders in the root of the project. Use the following commands to run the tests:

```
make test
make run-test
```


