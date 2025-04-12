mkdir -p build && cd build
qmake ../NeuroCards.pro
make
cd NeuroCards.app/Contents/MacOS/
./NeuroCards