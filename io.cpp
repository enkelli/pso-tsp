///
/// FIT BUT, SFC, 1. project
/// Pavol Plaskon, xplask00
///

#include <iostream>

#include "io.hpp"
#include "particle.hpp"

///
/// Reads distance matrix from standard input.
///
std::vector<std::vector<int>> get_input_matrix() {
	int cities;
	int distance;

	std::cerr << "Enter number of cities and input matrix:\n";

	std::cin >> cities;

	std::vector<std::vector<int>> distances(cities);
	for (int i = 0; i < cities; ++i) {
		for (int j = 0; j < cities; ++j) {
			std::cin >> distance;
			distances[i].push_back(distance);
		}
	}

	return distances;
}

///
/// Reads coordinates of cities from standard input.
///
std::vector<std::vector<int>> get_input_coords() {
	int cities;
	std::vector<std::vector<int>> xy(2);

	std::cerr << "Enter number of cities and their coordinates:\n";

	std::cin >> cities;

	int x, y;
	for (int i = 0; i < cities; ++i) {
		std::cin >> x;
		xy[0].push_back(x);
		std::cin >> y;
		xy[1].push_back(y);
	}

	return xy;
}

///
/// Prints coords of the shortest path.
///
void print_result_coords(const std::vector<std::vector<int>>& input_coords) {
	auto best_coords = Particle::unify_coords(Particle::get_best_coords());
	if (best_coords.empty()) {
		return;
	}

	for (const auto& c : best_coords) {
		std::cout << input_coords[0][c - 1] << " " << input_coords[1][c - 1] << "\n";
	}
	std::cout << input_coords[0][best_coords[0] - 1] << " " << input_coords[1][best_coords[0] - 1] << "\n";
}
