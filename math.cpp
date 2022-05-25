///
/// FIT BUT, SFC, 1. project
/// Pavol Plaskon, xplask00
///

#include <cmath>
#include <cstddef>
#include <iostream>

#include "math.hpp"


///
/// Computes distance matrix for citis with the given coords.
///
std::vector<std::vector<int>> coords_to_distance_matrix(const std::vector<std::vector<int>>& coords) {
	if (coords.empty()) {
		return {};
	}

	auto coords_count = coords[0].size();
	std::vector<std::vector<int>> distances(coords_count);

	for (std::size_t i = 0; i < coords_count; ++i) {
		for (std::size_t j = 0; j < coords_count; ++j) {
			int dist = std::sqrt(
				std::pow(coords[0][i] - coords[0][j], 2) +
				std::pow(coords[1][i] - coords[1][j], 2)
			);
			distances[i].push_back(dist);
		}
	}
	return distances;
}
