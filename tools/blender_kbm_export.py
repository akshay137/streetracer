import bpy

from datetime import datetime
import struct

def log(msg, *args, **kwargs):
	prefix = '[katha-binary-mesh]'
	if ('sep' in kwargs):
		kwargs['sep'] = kwargs['sep'].replace('\n', f'\n{prefix} ')
	print(f"{prefix} {msg}", *args, **kwargs)

def measure(fn, *args, **kwargs):
	start = datetime.now()
	res = fn(*args, *kwargs)
	end = datetime.now()
	elapsed = end - start
	log(f"elapsed {elapsed.total_seconds()}")
	pass

def should_export(obj):
	if not obj.visible_get():
		return False
	return True
	pass

def process_object(obj, kbm):
	# log(*dir(obj.data), sep='\n')
	log(f"{obj.type} {obj.name} {obj.location}")

	if 'MESH' == obj.type:
		kbm.write(struct.pack('<I', len(obj.data.vertices)))
		kbm.write(struct.pack('<I', 0)) # index count

	# log((obj.data.uv_layers.active.data.values()[0].uv), sep='\n')
	return

	for v in obj.data.vertices:
		log(dir(v))
		log(f'position: {v.co}',
			f'normal: {v.normal}',
			f'index: {v.index}'
		)
		pass
	pass

def export(dst):
	with open(dst, 'wb') as kbm:
		for c in '\0KBM\0':
			kbm.write(struct.pack('<B', ord(c)))
		kbm.write(struct.pack('<B', 0)) # vertex_layout

		log(f'objects: {len(bpy.context.scene.objects)}')
		for obj in bpy.context.scene.objects:
			if not should_export(obj):
				continue
			process_object(obj, kbm)
			# break
			pass # for bpy.context.scene.objects
		pass # open(.kbm)
	pass # def export

if '__main__' == __name__:
	log('source', bpy.data.filepath)
	log('target', '')
	measure(export, 'lara.kbm')