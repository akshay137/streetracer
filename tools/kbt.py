import argparse
import struct
import os

from enum import Enum
from PIL import Image

class Format(Enum):
	NONE = 0
	GREYSCALE8 = 1
	RGBA8 = 2
	RGB8 = 3
	SRGBA8 = 4
	SRGB8 = 5

class Options:
	def __init__(self):
		self.source = ''
		self.dest = ''
		self.append = False
		pass

	def __str__(self):
		res = f'source: {self.source}\n'
		res += f'dest: {self.dest}'
		return res
		pass

	def set_dest_from_source(self):
		base = os.path.basename(self.source)
		base = base[:base.rfind('.')]
		dst = os.path.dirname(self.source)
		self.dest = os.path.join(dst, f'{base}.kbt')
		pass
	pass

def parse_command_line():
	parser = argparse.ArgumentParser('kbt',
		'kbt.py -i <file|dir> -o <file|dir>',
		'converts image to katha binary texture format'
	)

	parser.add_argument('-i', action='store', type=str, help='input image', required=True)
	parser.add_argument('-o', action='store', type=str, help='output texture', required=False)
	parser.add_argument('--mipmaps', action='store_true', help='generate mipmaps', required=False)
	parser.add_argument('--append', action='store_true', help='append to existing file', required=False)

	args = parser.parse_args()
	opts = Options()
	opts.source = args.i
	if args.o:
		opts.dest = args.o
	else:
		opts.set_dest_from_source()
	opts.append = args.append
	return opts
	pass

def get_image_format(img: Image):
	mode = img.mode
	if 'L' == mode:
		return Format.GREYSCALE8
	if 'RGB' == mode:
		return Format.RGB8
	if 'RGBA' == mode:
		return Format.RGBA8
	return Format.NONE
	pass

def get_pixel_size(format: Format):
	if Format.GREYSCALE8 == format: return 1
	if Format.RGBA8 == format: return 4
	if Format.RGB8 == format: return 3
	return 0
	pass

def get_row_size(width, format: Format):
	size = width * get_pixel_size(format)
	rem = size % 4
	if 0 == rem:
		return (size, 0)
	padding = 4 - rem
	return (size + padding, padding)
	pass

def write_image_to_file(img: Image, out):
	format = get_image_format(img)
	if Format.NONE == format:
		print('unsupported image format')
		return False
	print('format:', format)
	row_size, padding = get_row_size(img.size[0], format)
	print('padding:', padding, 'row_size:', row_size)

	for c in '\0KBT\0':
		out.write(struct.pack('<B', ord(c)))
		pass
	out.write(struct.pack('<B', format.value))
	out.write(struct.pack('<II', *img.size))
	out.write(struct.pack('<I', row_size))

	data = img.load()
	for y in range(img.size[1]):
		for x in range(img.size[0]):
			pix = data[x, y]
			if type(pix) == int:
				out.write(struct.pack('<B', pix))
				pass
			else:
				for c in pix:
					out.write(struct.pack('<B', c))
				pass
			for p in range(padding):
				out.write(struct.pack('<B', 0))
				pass
			pass # x
		pass # y
	pass # write_image_to_file

def convert(src: str, dst: str, opt: Options):
	try:
		print(f'converting {src}')
		img = Image.open(src)
		file_mode = 'wb'
		if opts.append:
			file_mode = 'ab'
		with open(dst, file_mode) as out:
			write_image_to_file(img, out)
		pass
	except Exception as e:
		print(f'exception for: convert({src}, {dst})')
		print(e)
	pass

if '__main__' == __name__:
	opts = parse_command_line()
	print(opts)
	convert(opts.source, opts.dest, opts)