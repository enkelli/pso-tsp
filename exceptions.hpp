///
/// FIT BUT, SFC, 1. project
/// Pavol Plaskon, xplask00
///

#pragma once

#include <stdexcept>

/// Exception thrown when argument parsing fails.
class ArgParserError: public std::runtime_error
{
    public:
        using std::runtime_error::runtime_error;
};
