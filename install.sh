git submodule init;
git submodule update;

rm -rf build
mkdir build

cd build

cmake ..

make -j 10

cd ..
