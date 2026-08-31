ROOT="$(dirname $0)/.."
cd $ROOT
pwd

BUILD_DIR="build-desktop/host"
cmake --build $BUILD_DIR --parallel $@