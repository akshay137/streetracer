ROOT="$(dirname $0)/.."
cd $ROOT
pwd

BUILD_DIR="build-desktop/windows"
rm -rvf $BUILD_DIR

# TODO: make build type a parameter
cmake -DCMAKE_BUILD_TYPE=release \
	-DCMAKE_TOOLCHAIN_FILE=win_cc_toolchain.cmake \
	-DKATHA_XR=0 \
	-S devops \
	-B $BUILD_DIR