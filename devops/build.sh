ROOT="$(dirname $0)/.."
cd $ROOT
pwd

BUILD_DIR="build-desktop/linux"
cmake --build $BUILD_DIR --parallel