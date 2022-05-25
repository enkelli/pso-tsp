///
/// FIT BUT, SFC, 1. project
/// Pavol Plaskon, xplask00
///
/// Representation of a particle.
///

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <numeric>

#include "particle.hpp"

/// Distance matrix of the cities.
std::vector<std::vector<int>> Particle::distances = {};

/// Current global best.
std::vector<int> Particle::global_best_coords = {};

/// Omega koecifient for current velocity.
int Particle::omega = 1;

/// Cognitivty coeficient for local best value.
int Particle::c1 = 2;

/// COgnitivity coeficient for global best value.
int Particle::c2 = 2;

///
/// Sets distance matrix value.
///
void Particle::set_distances(const std::vector<std::vector<int>> &distances) {
	Particle::distances = distances;
}

///
/// Sets new global best position (particle).
///
void Particle::set_best_particle(const Particle& particle) {
	global_best_coords = particle.coords;
}

///
/// Inits coeficients and needed values for PSO computation.
///
void Particle::init(int city_count) {
	// Let omega be 0.6 (recommended value is - 0.4 - 0.9). Then, we have to
	// keep 60% of previous velocity. Thus, 40% of citis may be swapped. 1 swap
	// swaps two cities, omega should be set to 20% of cities.
	omega = city_count * 0.2;
}

///
/// Returns coords of the shortest path found (globally).
///
std::vector<int> Particle::get_best_coords() {
	return global_best_coords;
}

///
/// Creates a new particle.
///
Particle::Particle(int coords_count) {
	coords.resize(coords_count);
	std::iota(coords.begin(), coords.end(), 1);
	std::random_shuffle(coords.begin(), coords.end());

	best_coords = coords;

	velocity = get_random_swap_coords(Particle::omega);
	unify_coords();
}

///
/// Rotates current coords to start with '1'.
///
void Particle::unify_coords() {
	coords = Particle::unify_coords(coords);
}

///
/// Rotates the given coords to start with '1'.
///
std::vector<int> Particle::unify_coords(const std::vector<int> & coords) {
	std::vector<int> result;
	result.reserve(coords.size());

	std::size_t one_pos = 0;
	for (std::size_t i = 0; i < coords.size(); ++i) {
		if (coords[i] == 1) {
			one_pos = i;
			break;
		}
	}
	for (std::size_t i = one_pos; i < coords.size(); ++i) {
		result.push_back(coords[i]);
	}
	for (std::size_t i = 0; i < one_pos; ++i) {
		result.push_back(coords[i]);
	}
	return result;
}

///
/// Computes distance for the current coords.
///
int Particle::fitness() const {
	return fitness(coords);
}

///
/// Computes distance for the given coords.
///
int Particle::fitness(const std::vector<int>& coords) {
	int sum = 0;
	for (std::size_t i = 0; i < coords.size() - 1; ++i) {
		sum += Particle::distances[coords[i] - 1][coords[i + 1] - 1];
	}
	if (coords.size() > 1) {
		sum += Particle::distances[coords.back() - 1][coords.front() - 1];
	}
	return sum;
}

///
/// Updates velocity for the next step.
///
void Particle::update_velocity() {
	velocity = get_random_swap_coords(Particle::omega);

	auto local_best_diff = diff_coords(best_coords);
	auto r1 = std::rand() % coords.size() + 1;
	// / 2 because one swap affects two cities
	local_best_diff = keep_only_n_coords(local_best_diff, r1 * Particle::c1);
	velocity.insert(local_best_diff.begin(), local_best_diff.end());

	auto global_best_diff = diff_coords(global_best_coords);
	auto r2 = std::rand() % coords.size() + 1;
	// / 2 because one swap affects two cities
	global_best_diff = keep_only_n_coords(global_best_diff, r2 * Particle::c2);
	velocity.insert(global_best_diff.begin(), global_best_diff.end());

	velocity = keep_only_n_coords(velocity, coords.size());
}

///
/// Updates position of the particle based on current velocity.
///
void Particle::update_position() {
	for (const auto& to_swap_coords : velocity) {
		swap_coords(to_swap_coords.first, to_swap_coords.second);
	}
	// has no effect, maybe a little bit worse
	//unify_coords();

	auto curr_distance = fitness();
	if (curr_distance < fitness(best_coords)) {
		best_coords = coords;
	}
	if (curr_distance < fitness(Particle::global_best_coords)) {
		Particle::set_best_particle(*this);
	}
}
///
/// Returns pairs with positions where current coords differ with other coords.
///
Particle::SwapCoords Particle::diff_coords(const std::vector<int> &other) const {
    return diff_coords(other, coords);
}

///
/// Returns pairs with positions where coords A differ with coords B.
///
Particle::SwapCoords Particle::diff_coords(
        const std::vector<int>& coords_a, std::vector<int> coords_b) const {
    SwapCoords coords_to_swap;

    for (std::size_t i = 0; i < coords_a.size(); ++i) {
        auto c1 = coords_a[i];
        auto c2 = coords_b[i];
        if (c1 == c2) {
            continue;
        }
        int to_swap1 = i;
        int to_swap2 = std::find(
            coords_b.begin(), coords_b.end(), c1
        ) - coords_b.begin();
        coords_to_swap.emplace(to_swap1, to_swap2);
        Particle::swap_coords(coords_b, to_swap1, to_swap2);
    }
    return coords_to_swap;
}


///
/// Swaps coords on positions i and j.
///
void Particle::swap_coords(int i, int j) {
	auto tmp = coords[i];
	coords[i] = coords[j];
	coords[j] = tmp;
}

///
/// Swaps coords on positions i and j.
///
void Particle::swap_coords(std::vector<int>& coords, int i, int j) {
	auto tmp = coords[i];
	coords[i] = coords[j];
	coords[j] = tmp;
}

///
/// Generates count random coords (cities) that may be swapped.
///
Particle::SwapCoords Particle::get_random_swap_coords(int count) const {
	SwapCoords coords_to_swap;
	for (int i = 0; i < count; ++i) {
		coords_to_swap.emplace(
			std::rand() % coords.size(),
			std::rand() % coords.size()
		);
	}
	return coords_to_swap;
}

///
/// Formats current coords path as string.
///
std::string Particle::format_coords() const {
	return format_coords(coords);
}

///
/// Formats the given coords path as string.
///
std::string Particle::format_coords(const std::vector<int>& coords) {
	std::string result;
	for (const auto& coord : coords) {
		result += std::to_string(coord) + " ";
	}
	return result;
}

///
/// Returns a new set with n coords of the given coords.
///
Particle::SwapCoords Particle::keep_only_n_coords(const SwapCoords& coords, int n) {
	SwapCoords n_coords;

	int i = 1;
	for (auto coord : coords) {
		n_coords.emplace(coord);
		i++;
		if (i == n) {
			break;
		}
	}
	return n_coords;
}
