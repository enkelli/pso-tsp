///
/// FIT BUT, SFC, 1. project
/// Pavol Plaskon, xplask00
///
/// Representation of swarm.
///

#pragma once

#include <vector>

#include "particle.hpp"


///
/// Representation of a swarm.
///
class Swarm {
public:
	static bool verbose;
	static bool to_stdout;

public:
	Swarm(int particle_count, int coords_count);

	void run(int steps);

private:
	void print_result(std::ostream& stream) const;

private:
	/// Number of iterations that PSO may perform.
	int steps;
	/// Number of particles that PSO may use.
	std::vector<Particle> particles;
};
