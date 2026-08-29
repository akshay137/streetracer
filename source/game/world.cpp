#include "world.hpp"
#include "../graphics/graphics.hpp"
#include "../math/vector2.hpp"
#include "../math/vector3.hpp"
#include "../math/quaternion.hpp"
#include "../type/vertex.hpp"
#include "../time/time.hpp"
#include "../utility.hpp"

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

katha::world_t::traffic_e get_random_traffic()
{
	const int traffic_type = random_range<int>(1, 4);
	return static_cast<katha::world_t::traffic_e>(traffic_type);
}

katha::vec3 get_random_traffic_spawn_position()
{
	const float RANGE = 100;
	katha::vec3 pos(
		random_range<float>(-RANGE, RANGE),
		0,
		random_range<float>(-RANGE, RANGE)
	);
	return pos;

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

katha::world_t::world_t()
{
	camera.position = player.transform.local_to_world(vec3(0, 3, -9));
	camera = camera.look_at(player.transform.local_to_world(vec3(0, 3, 0)));

	for (int i = 0; i < traffic_count; i++)
	{
		spawn_traffic(i);
	}
}

katha::result_e katha::world_t::load(graphics_i* gfx)
{
	const uint64_t start = now();

	log_line("world::load()");
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
	result_e result = gfx->create_mesh(
		&vehicle_mesh,
		vertices,
		nullptr,
		sizeof(vertices) / sizeof(vertices[0]),
		0
	);

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

	const uint64_t end = now() - start;
	log_line("world::load() -> time: {td}", end);
	return result_e::success;
}

void katha::world_t::clear(graphics_i* gfx)
{
	log_line("world::clear()");
	gfx->delete_texture(&checker_board_texture);
	gfx->delete_mesh(&vehicle_mesh);
}

void katha::world_t::update(const action_map_t& action_map, const float delta)
{
	game_time += delta;

	player.apply(
		action_map.throttle,
		action_map.brake,
		action_map.steering_angle,
		delta
	);

	camera.position = player.transform.local_to_world(vec3(0, 3, -10));
	const vec3 target = player.transform.local_to_world(vec3(0, 1.5, 0));
	camera = camera.look_at(target);

	// log_line("player: {v3}, {q} | s: {f}",
	// 	player.transform.position.array(),
	// 	player.transform.orientation.array(),
	// 	action_map.steering_angle
	// );
	// log_line("camera: {v3}, {q}",
	// 	camera.position.array(),
	// 	camera.orientation.array()
	// );

	// vec3 rot_right = rotate(camera.orientation, transform_t::RIGHT);
	// log_line("right: {v3}, {f}", rot_right.array(), dot(rot_right, transform_t::RIGHT));

	for (int i = 0; i < traffic_count; i++)
	{
		traffic_t& t = traffic[i];
	}
}

void katha::world_t::spawn_traffic(const int index)
{
	traffic[index].position = get_random_traffic_spawn_position();
	traffic[index].type = get_random_traffic();
}

katha::vec3 katha::world_t::get_bb(const traffic_e traffic)
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