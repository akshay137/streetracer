#include "highway.hpp"
#include "../katha/graphics/graphics.hpp"

#include "../katha/math/vector2.hpp"
#include "../katha/math/vector3.hpp"
#include "../katha/physics/vertex.hpp"
#include "../katha/core.hpp"

#include <cstdlib>

float random_norm()
{
	const int r = rand();
	const float rn = r / static_cast<float>(RAND_MAX);
	return rn;
}

template <typename T>
T random_range(const T l, const T u)
{
	const float rn = random_norm();
	const T result = l + static_cast<T>(rn * (u - l));
	return result;
}

katha::highway_t::traffic_e get_random_traffic()
{
	const int traffic_type = random_range<int>(1, 4);
	return static_cast<katha::highway_t::traffic_e>(traffic_type);
}

katha::vec3 get_random_traffic_spawn_position()
{
	static float last_z = -200;
	static int last_lane = 0;
	constexpr int MAX_LANES = 5;
	constexpr float FIRST_LANE = (20.0f / MAX_LANES) * 0.5f;
	int lane = random_range<int>(0, MAX_LANES);
	while (abs(last_lane - lane) < 2)
	{
		lane = random_range<int>(0, MAX_LANES);
	}
	last_lane = lane;
	const int dist = random_range<int>(0, 5);
	const katha::vec3 position((-10 + FIRST_LANE) + lane * (MAX_LANES - 1), 0, -300 + lane * 50);
	katha::log_line("spawn: {i} {v3}", lane, position.array());
	return position;
}

katha::highway_t::highway_t()
{
	for (int i = 0; i < traffic_count; i++)
	{
		spawn_delay_seconds[i] = random_range(1, 6);
		despawn_time[i] = 0;
	}
}

katha::result_e katha::highway_t::load(graphics_i* gfx)
{
	constexpr vertex_t vertices[] = {
		{ .position = vec3(-1, -1,  1), .uv = vertex_t::unorm(vec2(0, 0)) },
		{ .position = vec3( 1, -1,  1), .uv = vertex_t::unorm(vec2(1, 0)) },
		{ .position = vec3( 1,  1,  1), .uv = vertex_t::unorm(vec2(1, 1)) },
		{ .position = vec3(-1, -1,  1), .uv = vertex_t::unorm(vec2(0, 0)) },
		{ .position = vec3( 1,  1,  1), .uv = vertex_t::unorm(vec2(1, 1)) },
		{ .position = vec3(-1,  1,  1), .uv = vertex_t::unorm(vec2(0, 1)) },
		{ .position = vec3( 1, -1, -1), .uv = vertex_t::unorm(vec2(0, 0)) },
		{ .position = vec3(-1, -1, -1), .uv = vertex_t::unorm(vec2(1, 0)) },
		{ .position = vec3(-1,  1, -1), .uv = vertex_t::unorm(vec2(1, 1)) },
		{ .position = vec3( 1, -1, -1), .uv = vertex_t::unorm(vec2(0, 0)) },
		{ .position = vec3(-1,  1, -1), .uv = vertex_t::unorm(vec2(1, 1)) },
		{ .position = vec3( 1,  1, -1), .uv = vertex_t::unorm(vec2(0, 1)) },
		{ .position = vec3( 1, -1,  1), .uv = vertex_t::unorm(vec2(0, 0)) },
		{ .position = vec3( 1, -1, -1), .uv = vertex_t::unorm(vec2(1, 0)) },
		{ .position = vec3( 1,  1, -1), .uv = vertex_t::unorm(vec2(1, 1)) },
		{ .position = vec3( 1, -1,  1), .uv = vertex_t::unorm(vec2(0, 0)) },
		{ .position = vec3( 1,  1, -1), .uv = vertex_t::unorm(vec2(1, 1)) },
		{ .position = vec3( 1,  1,  1), .uv = vertex_t::unorm(vec2(0, 1)) },
		{ .position = vec3(-1, -1, -1), .uv = vertex_t::unorm(vec2(0, 0)) },
		{ .position = vec3(-1, -1,  1), .uv = vertex_t::unorm(vec2(1, 0)) },
		{ .position = vec3(-1,  1,  1), .uv = vertex_t::unorm(vec2(1, 1)) },
		{ .position = vec3(-1, -1, -1), .uv = vertex_t::unorm(vec2(0, 0)) },
		{ .position = vec3(-1,  1,  1), .uv = vertex_t::unorm(vec2(1, 1)) },
		{ .position = vec3(-1,  1, -1), .uv = vertex_t::unorm(vec2(0, 1)) },
		{ .position = vec3(-1,  1,  1), .uv = vertex_t::unorm(vec2(0, 0)) },
		{ .position = vec3( 1,  1,  1), .uv = vertex_t::unorm(vec2(1, 0)) },
		{ .position = vec3( 1,  1, -1), .uv = vertex_t::unorm(vec2(1, 1)) },
		{ .position = vec3(-1,  1,  1), .uv = vertex_t::unorm(vec2(0, 0)) },
		{ .position = vec3( 1,  1, -1), .uv = vertex_t::unorm(vec2(1, 1)) },
		{ .position = vec3(-1,  1, -1), .uv = vertex_t::unorm(vec2(0, 1)) },
		{ .position = vec3(-1, -1, -1), .uv = vertex_t::unorm(vec2(0, 0)) },
		{ .position = vec3( 1, -1, -1), .uv = vertex_t::unorm(vec2(1, 0)) },
		{ .position = vec3( 1, -1,  1), .uv = vertex_t::unorm(vec2(1, 1)) },
		{ .position = vec3(-1, -1, -1), .uv = vertex_t::unorm(vec2(0, 0)) },
		{ .position = vec3( 1, -1,  1), .uv = vertex_t::unorm(vec2(1, 1)) },
		{ .position = vec3(-1, -1,  1), .uv = vertex_t::unorm(vec2(0, 1)) }
	};
	result_e result = gfx->create_array_buffer(&vertex_buffer, sizeof(vertices), vertices);
	if (!check_result(result, "gl::create_array_buffer"))
	{
		return result;
	}

	constexpr uint8_t pixels[] = {
		255, 255, 255, 0, 0, 0, 0, 0,
		0, 0, 0, 255, 255, 255, 0, 0
	};
	result = gfx->create_texture(
		&checker_board_texture,
		katha::uvec2(2), format_e::rgb8,
		pixels
	);
	if (!check_result(result, "gl_scene_t::create::checker_board_texture"))
	{
		return result;
	}

	return result_e::success;
}

void katha::highway_t::clear(graphics_i* gfx)
{
	gfx->delete_texture(&checker_board_texture);
	gfx->delete_buffer(&vertex_buffer);
}

void katha::highway_t::update(const action_map_t& action_map, const float delta)
{
	game_time += delta;

	constexpr float PLAYER_SPEED = 10.0f;
	if (action_map.movement)
	{
		player.x += action_map.movement * PLAYER_SPEED * delta;
	}
	player = clamp(player, -10.0f, 10.0f);

	constexpr float TRAFFIC_SPEED = PLAYER_SPEED * 1.75f;
	for (int i = 0; i < traffic_count; i++)
	{
		traffic_t& t = traffic[i];
		if (traffic_e::none == t.type)
		{
			if (game_time - despawn_time[i] > spawn_delay_seconds[i])
			{
				spawn_traffic(i);
			}
		}

		if (traffic_e::none != t.type)
		{
			t.position.z += TRAFFIC_SPEED * delta;
	
			if (t.position.z >= 5)
			{
				t.type = traffic_e::none;
				spawn_delay_seconds[i] = random_range(0, 3);
				despawn_time[i] = game_time;
			}
		}
	}
}

void katha::highway_t::spawn_traffic(const int index)
{
	traffic[index].position = get_random_traffic_spawn_position();
	traffic[index].type = get_random_traffic();
}

katha::vec3 katha::highway_t::get_bb(const traffic_e traffic)
{
	switch (traffic)
	{
		case traffic_e::none: return vec3(1);
		case traffic_e::car: return vec3(0.8, 0.5, 1);
		case traffic_e::truck: return vec3(0.8, 1.5, 2);
		case traffic_e::bike: return vec3(0.15, 0.4, 0.4);
	}

	return vec3(1);
}