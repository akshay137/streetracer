ROOT="$(dirname $0)/.."
cd $ROOT
pwd

$HOME/dev/programs/blender-5.2.0-linux-x64/blender --background \
	/mnt/WD-2TB/art/downloads/Lara\ Croft/SOTR\ -\ Desert\ Tanktop/tr10_laracroft_deserttanktop_sotr_00.blend \
	--python tools/blender_kbm_export.py