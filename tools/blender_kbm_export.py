import bpy
import bmesh

from datetime import datetime
import struct

def log(msg, *args, **kwargs):
	prefix = '[katha-binary-mesh]'
	if ('sep' in kwargs):
		kwargs['sep'] = kwargs['sep'].replace('\n', f'\n{prefix} ')
	print(f"{prefix} {msg}", *args, **kwargs)

def measure(fn, *args, **kwargs):
	start = datetime.now()
	res = fn(*args, **kwargs)
	end = datetime.now()
	elapsed = end - start
	log(f"elapsed {elapsed.total_seconds()}")
	return res
	pass

def should_export(obj):
	if 'MESH' != obj.type:
		return False
	if not obj.visible_get():
		return False
	return True
	pass

class Vec3:
	def __init__(self, x, y, z):
		self.x = x
		self.y = y
		self.z = z
		pass

	def __str__(self):
		return f'({self.x}, {self.y}, {self.z})'
	pass

class Vertex:
	def __init__(self, position, uv, normal):
		self.position = Vec3(position.x, position.y, position.z)
		self.uv = Vec3(uv.x, uv.y, uv.z)
		self.normal = Vec3(normal.x, normal.y, normal.z)
		pass

	def __str__(self):
		return f'Vertex {{ position: {self.position}, uv: {self.uv}, normal: {self.normal} }}'
	
	def write(self, f):
		f.write(struct.pack('<fff', self.position.x, self.position.y, self.position.z))
		
		U16_MAX = 65535
		s = int(self.uv.x * U16_MAX) % U16_MAX
		t = int(self.uv.y * U16_MAX) % U16_MAX
		f.write(struct.pack('<HHH', s, t, 0))

		I16_MAX = 32767
		nx = int(self.normal.x * I16_MAX)
		ny = int(self.normal.y * I16_MAX)
		nz = int(self.normal.z * I16_MAX)
		f.write(struct.pack('<hhh', nx, ny, nz))
		pass
	pass

def bv_to_vertex(v, uv, face_normal):
	vertex = Vertex(v.co.xzy, Vec3(uv.x, uv.y, 0), face_normal.xzy)
	return vertex

def triangulate_mesh(obj):
	vertices = []

	obj.data.calc_loop_triangles()
	mesh_verts = obj.data.vertices
	if not obj.data.uv_layers:
		has_uv = False
	else:
		has_uv = True
		uvs = obj.data.uv_layers[0].data
		pass
	
	face_count = 0
	max_faces = len(obj.data.loop_triangles)
	for face in obj.data.loop_triangles:
		log(f"triangle {face_count}/{max_faces}", end='\r')
		for i, u in zip(face.vertices, face.loops):
			if has_uv:
				uv = uvs[u].uv
			else:
				uv = Vec3(0, 0, 0)
			v = bv_to_vertex(mesh_verts[i], uv, face.normal)
			vertices.append(v)
			pass
		face_count += 1
	
	return vertices
	pass

def process_mesh(obj, kbm):
	vertices = triangulate_mesh(obj)

	kbm.write(struct.pack('<I', len(vertices)))

	for v in vertices:
		v.write(kbm)
		pass
	pass

def process_object(obj, kbm):
	log(f"{obj.type} {obj.name} {obj.location}")

	if 'MESH' == obj.type:
		process_mesh(obj, kbm)
	pass

def export(dst):
	objects = []
	for obj in bpy.context.scene.objects:
		if should_export(obj):
			objects.append(obj)
	if len(objects) >= 128:
		log("individual meshes must be less than 128")
		return False
	
	with open(dst, 'wb') as kbm:
		for c in '\0KBM\0':
			kbm.write(struct.pack('<B', ord(c)))
		kbm.write(struct.pack('<B', len(objects)))

		log(f'objects: {len(objects)}')
		for obj in objects:
			process_object(obj, kbm)
			pass # for objects
		pass # open(.kbm)
	return True
	pass # def export

if '__main__' == __name__:
	log('source', bpy.data.filepath)
	measure(export, 'out.kbm')