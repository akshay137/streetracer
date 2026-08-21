ROOT="$(dirname $0)/.."
cd $ROOT
pwd

BUILD_DIR="build-desktop/windows"
cmake --build $BUILD_DIR --parallel