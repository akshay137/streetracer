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
	
	def __eq__(self, rhs):
		return (self.x == rhs.x) and (self.y == rhs.y) and (self.z == rhs.z)
	
	def get_ivec3(self, precision: int = 3):
		factor = 10 ** precision
		x = int(self.x * factor)
		y = int(self.y * factor)
		z = int(self.z * factor)
		return Vec3(x, y, z)
	
	def is_same(self, rhs, precision: int = 3):
		return self.get_ivec3(precision) == rhs.get_ivec3(precision)
	pass

class Vertex:
	def __init__(self, position, uv, normal):
		self.position = Vec3(position.x, position.y, position.z)
		self.uv = Vec3(uv.x, uv.y, uv.z)
		self.normal = Vec3(normal.x, normal.y, normal.z)
		pass

	def __str__(self):
		return f'Vertex {{ position: {self.position}, uv: {self.uv}, normal: {self.normal} }}'
	
	def __eq__(self, rhs):
		pos = self.position.is_same(rhs.position, 3)
		uv = self.uv.is_same(rhs.uv, 3)
		normal = self.normal.is_same(rhs.normal, 3)
		return pos and uv and normal
	
	def write(self, f):
		f.write(struct.pack('<fff', self.position.x, self.position.y, self.position.z))
		
		U16_MAX = 65535
		s, t = int(self.uv.x * U16_MAX), int(self.uv.y * U16_MAX)
		f.write(struct.pack('<HHH', s, t, 0))

		I16_MAX = 32767
		nx = int(self.normal.x * I16_MAX)
		ny = int(self.normal.y * I16_MAX)
		nz = int(self.normal.z * I16_MAX)
		f.write(struct.pack('<hhh', nx, ny, nz))
		pass
	pass

def find_vertex(vertex: Vertex, vertices: list[Vertex]):
	i = 0
	for v in vertices:
		if v == vertex:
			return i
		i += 1
	return -1
	pass

def bv_to_vertex(v, uv, face_normal):
	vertex = Vertex(v.co, Vec3(uv.x, uv.y, 0), face_normal)
	return vertex

def calculate_face_normal(a, b, c):
	ba = b - a
	ca = c - a
	n = ba.cross(ca)
	n.normalize()
	return n
	pass

def triangulate_mesh(obj):
	vertices = []
	indices = []

	obj.data.calc_loop_triangles()
	mesh_verts = obj.data.vertices
	if not obj.data.uv_layers:
		has_uv = False
	else:
		has_uv = True
		uvs = obj.data.uv_layers[0].data
		pass
	for face in obj.data.loop_triangles:
		for i, u in zip(face.vertices, face.loops):
			if has_uv:
				uv = uvs[u].uv
			else:
				uv = Vec3(0, 0, 0)
			v = bv_to_vertex(mesh_verts[i], uvs[u].uv, face.normal)
			vi = find_vertex(v, vertices)
			if -1 == vi:
				indices.append(len(vertices))
				vertices.append(v)
			else:
				indices.append(vi)
			pass
	
	print('triangulate result:')
	print('\ttotal vertices:', len(obj.data.vertices))
	print('\tmerged:', len(vertices))
	print('\tindices:', len(indices))
	return (vertices, indices)
	pass

def process_mesh(obj, kbm):
	vertices, indices = triangulate_mesh(obj)

	kbm.write(struct.pack('<I', len(vertices)))
	kbm.write(struct.pack('<I', len(indices)))

	for v in vertices:
		# log(dir(v))
		# log(f'position: {v.position} normal: {v.normal} uv: {v.uv}')
		v.write(kbm)
		pass

	for i in indices:
		kbm.write(struct.pack('<H', i))
	pass

def process_object(obj, kbm):
	# log(*dir(obj.data), sep='\n')
	log(f"{obj.type} {obj.name} {obj.location}")

	if 'MESH' == obj.type:
		process_mesh(obj, kbm)
	# log((obj.data.uv_layers.active.data.values()[0].uv), sep='\n')
	# return

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
			break
			pass # for bpy.context.scene.objects
		pass # open(.kbm)
	pass # def export

if '__main__' == __name__:
	log('source', bpy.data.filepath)
	log('target', '')
	measure(export, 'lara.kbm')