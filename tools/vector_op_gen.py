import sys
import os

def repeat_expression(n: int, exp: str):
	components = 'xyzw'
	res = ''
	for i in range(n):
		res += exp.format(components[i]) + '\n\t\t'
		pass
	return res.strip()
	pass

def get_operator_expression(operator: str, param_count: int, retval: str = 'result', is_scalar = False):
	if (is_scalar):
		return f"{retval}.{{0}} = v1.{{0}} {operator} scalar;"
	
	if (1 == param_count):
		return f"{retval}.{{0}} = {operator}v1.{{0}};"
	if (2 == param_count):
		return f"{retval}.{{0}} = v1.{{0}} {operator} v2.{{0}};"
	pass

def get_operator_string(n: int, operator: str, op_exp: str):
	vt = f'Vector{n}<T>'
	param_str = f'const {vt}& v1'
	if 'v2' in op_exp:
		param_str += f', const {vt}& v2'
	elif 'scalar' in op_exp:
		param_str += f', const T& scalar'

	res = f'''
	template <typename T>
	constexpr {vt} operator {operator} ({param_str})
	{{
		{vt} result = {{}};
		{repeat_expression(n, op_exp)}
		return result;
	}}
	'''
	
	return res.strip()
	pass

def get_assignment_operator_string(n: int, operator: str, op_exp: str):
	vt = f'Vector{n}<T>'
	param_str = f'{vt}& v1'
	if 'v2' in op_exp:
		param_str += f', const {vt}& v2'
	elif 'scalar' in op_exp:
		param_str += f', const T& scalar'

	res = f'''
	template <typename T>
	{vt}& operator {operator}= ({param_str})
	{{
		{repeat_expression(n, op_exp)}
		return v1;
	}}
	'''
	return res.strip()
	pass

def get_function_string(n: int, fn: str):
	if fn in [ 'Sin', 'Cos', 'Tan', 'ACos', 'ASin' ]:
		mfn = f'std::{fn.lower()}'
	else:
		mfn = fn
	vt = f'Vector{n}<T>'
	fn_exp = f'result.{{0}} = {mfn}(v.{{0}});'
	res = f'''
	template <typename T>
	{vt} {fn}(const {vt}& v)
	{{
		{vt} result = {{}};
		{repeat_expression(n, fn_exp)}
		return result;
	}}
	'''
	return res.strip()
	pass

def get_binary_function_string(n: int, fn: str):
	mfn = fn
	if fn in [ 'ATan2']:
		mfn = f"std::{fn.lower()}"
	vt = f'Vector{n}<T>'
	fn_exp = f'result.{{0}} = {mfn}(v1.{{0}}, v2.{{0}});'
	res = f'''
	template <typename T>
	{vt} {fn}(const {vt}& v1, const {vt}& v2)
	{{
		{vt} result = {{}};
		{repeat_expression(n, fn_exp)}
		return result;
	}}
	'''
	return res.strip()
	pass

def get_misc_string(n: int):
	vt = f'Vector{n}<T>'
	components = 'xyzw'

	res = f'''
	template <typename T>
	T HorizontalSum(const {vt}& v)
	{{
		T result = {' + '.join([ f'v.{components[i]}' for i in range(n) ])};
		return result;
	}}

	template <typename T>
	T Dot(const {vt}& v1, const {vt}& v2)
	{{
		const {vt}& vv = v1 * v2;
		const T result = HorizontalSum(vv);
		return result;
	}}

	template <typename T>
	T SquaredLength(const {vt}& v)
	{{
		const T squared_length = Dot(v, v);
		return squared_length;
	}}

	template <typename T, typename RT=float>
	RT Length(const {vt}& v)
	{{
		const T squared_length = Dot(v, v);
		const RT length = std::sqrt(squared_length);
		return length;
	}}

	template <typename T, typename LT=float>
	{vt} Normalize(const {vt}& v)
	{{
		const LT length = Length<LT>(v);
		const {vt} normalized = v / length;
		return normalized;
	}}
	'''
	return res.strip()
	pass

def generate_vector_operations(n: int, root: str):
	file_path = os.path.join(root, f'vector{n}.gen.hpp')
	header = f'''
#pragma once
#ifndef KATHA_MATH_VECTOR_{n}_AUTO_GEN_H__
#define KATHA_MATH_VECTOR_{n}_AUTO_GEN_H__ 1

/*
	This is file is auto generated from `vector_op_gen.py`
	found at {{root}}/tools/vector_op_gen.py
*/

#include "../type/vector.hpp"

#include <cmath>

namespace katha
{{

'''
	footer = '}\n#endif'
	binary_operators = [ '+', '-', '*', '/', '%', '|', '&', '^', '<<', '>>' ]
	unary_operators = '-~'
	unary_functions = [ 'Abs', 'Radians', 'Degrees', 'Sin', 'Cos', 'Tan', 'ACos', 'ASin' ]
	binary_functions = [ 'ATan2', 'Min', 'Max', ]
	try:
		with open(file_path, 'wt') as out:
			out.write(header)

			for operator in unary_operators:
				fn = get_operator_string(
					n,
					operator,
					get_operator_expression(operator, 1)
				)
				print(f'\t{fn}\n', file=out)
				pass

			for operator in binary_operators:
				fn = get_operator_string(
					n,
					operator,
					get_operator_expression(operator, 2)
				)
				print(f'\t{fn}\n', file=out)
				
				fn = get_operator_string(
					n,
					operator,
					get_operator_expression(operator, 2, is_scalar=True)
				)
				print(f'\t{fn}\n', file=out)
				
				fn = get_assignment_operator_string(
					n,
					operator,
					get_operator_expression(operator, 2, 'v1')
				)
				print(f'\t{fn}\n', file=out)

				fn = get_assignment_operator_string(
					n,
					operator,
					get_operator_expression(operator, 2, 'v1', True)
				)
				print(f'\t{fn}\n', file=out)
				pass

			for uf in unary_functions:
				fn = get_function_string(n, uf)
				print(f'\t{fn}\n', file=out)

			for bf in binary_functions:
				fn = get_binary_function_string(n, bf)
				print(f'\t{fn}\n', file=out)

			misc = get_misc_string(n)
			print(f'\t{misc}\n', file=out)
			out.write(footer)
			pass
	except Exception as e:
		print(f'error: failed to generate operations for vector{n}')
		print(e)
	pass

if '__main__' == __name__:
	base = os.path.abspath(os.path.join(os.path.dirname(__file__), '../'))
	source = os.path.join(base, 'source/math')
	print(f'base_dir: {source}')

	generate_vector_operations(2, source)
	generate_vector_operations(3, source)
	generate_vector_operations(4, source)
	quit()