///
/// FIT BUT, SFC, 1. project
/// Pavol Plaskon, xplask00
///
/// Representation of a particle.
///

#pragma once

#include <set>
#include <string>
#include <utility>
#include <vector>

///
/// Representation of a particle.
///
class Particle {
public:
	static void set_distances(const std::vector<std::vector<int>> &distances);
	static void set_best_particle(const Particle& particle);
	static void init(int city_count);
	static std::vector<int> get_best_coords();

private:
	static std::vector<std::vector<int>> distances;
	static std::vector<int> global_best_coords;

	static int omega;
	static int c1;
	static int c2;

public:
	Particle(int coords_count);

	int fitness() const;
	static int fitness(const std::vector<int>& coords);
	void update_velocity();
	void update_position();

	std::string format_coords() const;
	static std::string format_coords(const std::vector<int>& coords);

	static std::vector<int> unify_coords(const std::vector<int>& coords);

private:
	using SwapCoords = std::set<std::pair<int, int>>;

	SwapCoords diff_coords(const std::vector<int> &other) const;
	SwapCoords diff_coords(
        const std::vector<int>& coords_a, std::vector<int> coords_b) const;
	void swap_coords(int i, int j);
	static void swap_coords(std::vector<int>& coords, int i, int j);
	SwapCoords get_random_swap_coords(int count) const;
	void unify_coords();
	SwapCoords keep_only_n_coords(const SwapCoords& coords, int n);


private:
	/// Current coords of a particle.
	std::vector<int> coords;
	/// Best position of a particle.
	std::vector<int> best_coords;
	/// Velocity of a particle.
	SwapCoords velocity;
};
