#include "highway.hpp"

#include "../katha/math/vector2.hpp"
#include "../katha/math/vector3.hpp"
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
	constexpr int MAX_LANES = 4;
	int lane = random_range<int>(0, MAX_LANES);
	while (abs(last_lane - lane) < 2)
	{
		lane = random_range<int>(0, MAX_LANES);
	}
	last_lane = lane;
	const int dist = random_range<int>(0, 5);
	const katha::vec3 position(-7.5 + lane * 5, 0, -300 + lane * 30);
	katha::log_line("spawn: {v3}", position.array());
	return position;
}

katha::highway_t::highway_t()
{
	for (int i = 0; i < traffic_count; i++)
	{
		spawn_delay_seconds[i] = random_range(2, 6);
	}
}

void katha::highway_t::update(const action_map_t& action_map, const float delta)
{
	game_time += delta;

	constexpr float PLAYER_SPEED = 10.0f;
	if (squared_length(action_map.movement))
	{
		player.x += action_map.movement.x * PLAYER_SPEED * delta;
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
				t.type = get_random_traffic();
				t.position = get_random_traffic_spawn_position();
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
	//
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