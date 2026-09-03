ROOT="$(dirname $0)/.."
cd $ROOT
pwd

$HOME/dev/programs/blender-5.2.0-linux-x64/blender --background \
	cube.blend \
	--python tools/blender_kbm_export.py