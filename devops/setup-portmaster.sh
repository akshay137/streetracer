ROOT="$(dirname $0)/.."
cd $ROOT
pwd

BUILD_DIR="build-portmaster"
rm -rvf $BUILD_DIR

# TODO: make build type a parameter
cmake -DCMAKE_BUILD_TYPE=release \
	-DKATHA_PORTMASTER=1 \
	-DKATHA_XR=0 \
	-S devops \
	-B $BUILD_DIR