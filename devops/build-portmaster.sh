ROOT="$(dirname $0)/.."
cd $ROOT
pwd

BUILD_DIR="build-portmaster"
cmake --build $BUILD_DIR --parallel