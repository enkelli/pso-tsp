///
/// FIT BUT, SFC, 1. project
/// Pavol Plaskon, xplask00
///

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "arg_parser.hpp"
#include "io.hpp"
#include "math.hpp"
#include "particle.hpp"
#include "swarm.hpp"
#include "utils.hpp"


///
/// Prints help message.
///
void print_help() {
	std::cout << "TSP solver using PSO method\n";
	std::cout << "Optional arguments:\n";
	std::cout << "  -h, --help       Print this message.\n";
	std::cout << "  -i, --iterations Maximal number of iterations.\n";
	std::cout << "  -c, --coords Input has form of 2D coordinates.\n";
	std::cout << "  -p, --particles  Number of particles used.\n";
	std::cout << "  -v, --verbose    Print verbosely all iterations.\n";
	std::cout << "\n";
	std::cout << "Input format (see examples):\n";
	std::cout << "Number_of_cities N\n";
	std::cout << "N x N distance matrix\n";
	std::cout << "\n";
	std::cout << "Usage:\n";
	std::cout << "  $ ./pso-tsp\n";
	std::cout << "  $ 3\n";
	std::cout << "  $ 0 2 3\n";
	std::cout << "  $ 2 0 5\n";
	std::cout << "  $ 3 5 0\n";
	std::cout << "\n";
	std::cout << "  $ ./pso-tsp < file_with_input_matrix\n";
	std::cout << "\n";
	std::cout << "Notes:\n";
	std::cout << "  Distances from A to B and from B to A may differ.\n";
	std::cout << "  Diagonal values (from A to A) do not matter.\n";

}

///
/// Parses input arguments.
///
ArgParser::ParsedArgs parse_args(int argc, char **argv) {
    const ArgParser::ArgsToParse args_to_parse = {
		ArgParser::ArgToParse("-p", "--particles", "particles", 1),
		ArgParser::ArgToParse("-i", "--iterations", "iterations", 1),
		ArgParser::ArgToParse("-h", "--help", "help", 0),
		ArgParser::ArgToParse("-v", "--verbose", "verbose", 0),
		ArgParser::ArgToParse("-c", "--coords", "coords", 0),
	};
	auto arg_parser = ArgParser(args_to_parse, "positional");
	return arg_parser.parseArgs(argc, argv);
}

/// Default value for number of particles.
constexpr int PARTICLES_COUNT = 30;

/// Default value for number of iterations of PSO.
constexpr int ITERATIONS = 10000;

int main(int argc, char **argv) {
	const auto args = parse_args(argc, argv);

	if (args.hasArg("help")) {
		print_help();
		return 0;
	}

	int particles_count = PARTICLES_COUNT;
	if (args.hasArg("particles")) {
		if (args.getArg("particles").second.empty()) {
			print_help();
			std::cerr << "Invalid number of particles.\n";
			return -1;
		}

		particles_count = str_to_pint(args.getArg("particles").second[0]);
		if (particles_count < 1) {
			std::cerr << "Invalid number of particles.\n";
			print_help();
			return -1;
		}
	}
	int iterations = ITERATIONS;
	if (args.hasArg("iterations")) {
		if (args.getArg("iterations").second.empty()) {
			std::cerr << "Invalid number of iterations.\n";
			print_help();
			return -1;
		}

		iterations = str_to_pint(args.getArg("iterations").second[0]);
		if (iterations < 1) {
			std::cerr << "Invalid number of iterations.\n";
			print_help();
			return -1;
		}
	}

	if (args.hasArg("verbose")) {
		Swarm::to_stdout = false;
		Swarm::verbose = true;
	}

	std::vector<std::vector<int>> input_matrix;
	std::vector<std::vector<int>> input_coords;
	if (args.hasArg("coords")) {
		input_coords = get_input_coords();
		input_matrix = coords_to_distance_matrix(input_coords);
	} else {
		input_matrix = get_input_matrix();
	}

	std::cerr << "Starting PSO ...\n";

	std::cerr << "Particles: " << particles_count << "\n";
	std::cerr << "Iterations: " << iterations << "\n";

	std::srand(std::time(0));
	Particle::set_distances(input_matrix);
	Particle::init(input_matrix.size());

	Swarm swarm(particles_count, input_matrix.size());
	swarm.run(iterations);

	if (args.hasArg("coords")) {
		print_result_coords(input_coords);
	}

	return 0;
}
