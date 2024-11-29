LD_LIBRARY_PATH=$HOME/github/AMCAX/point-cloud-reverse-algorithm/amcax-trans/3rd/AMCAXKernel_Release/lib:$LD_LIBRARY_PATH

rm -rf build

mkdir build
cd build

cmake ..
make -j
