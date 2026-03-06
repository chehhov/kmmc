mkdir build
cd build

cmake ..

cmake --build .

ctest -V

cmake --build . --target run_tests
