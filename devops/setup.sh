ROOT="$(dirname $0)/.."
cd $ROOT
pwd

BUILD_DIR="build-desktop/host"
rm -rvf $BUILD_DIR

# TODO: make build type a parameter
cmake -DCMAKE_BUILD_TYPE=debug \
	-S devops \
	-B $BUILD_DIR