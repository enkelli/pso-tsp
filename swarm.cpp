///
/// FIT BUT, SFC, 1. project
/// Pavol Plaskon, xplask00
///
/// Representation of swarm.
///

#include <cstddef>
#include <iostream>
#include <string>

#include "particle.hpp"
#include "swarm.hpp"


/// Should print position in each iteration?
bool Swarm::verbose = false;

/// Should print result to standard output?
bool Swarm::to_stdout = true;

///
/// Creates a new swarm.
///
Swarm::Swarm(int particle_count, int coords_count) {
	for (int i = 0; i < particle_count; ++i) {
		particles.push_back(Particle(coords_count));
	}
	if (particles.empty()) {
		return;
	}

	auto best_particle = particles[0];
	auto best_fitness = best_particle.fitness();
	for (std::size_t i = 1; i < particles.size(); ++i) {
		auto curr_fitness = particles[i].fitness();
		if (curr_fitness < best_fitness) {
			best_fitness = curr_fitness;
			best_particle = particles[i];
		}
	}
	Particle::set_best_particle(best_particle);
}

///
/// Runs PSO.
///
void Swarm::run(int steps) {
	for (auto i = 0; i < steps; ++ i) {
		for (auto& particle : particles) {
			particle.update_velocity();
			particle.update_position();
		}
		if (verbose) {
			std::cerr << "Iteration: " << i << "\n";
			for (const auto& particle : particles) {
				std::cerr << particle.format_coords();
				std::cerr << " distance: " << std::to_string(particle.fitness()) << "\n";
			}
		}
	}

	print_result(to_stdout ? std::cout : std::cerr);
}

///
/// Prints the result of PSO.
///
void Swarm::print_result(std::ostream& stream) const {
	stream << "Result:\n";
	stream << Particle::format_coords(
		Particle::unify_coords(Particle::get_best_coords())
	);
	stream << "distance: " << std::to_string(Particle::fitness(Particle::get_best_coords())) << "\n";
}
