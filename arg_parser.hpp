///
/// FIT BUT, SFC, 1. project
/// Pavol Plaskon, xplask00
///

#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

///
/// Parser for input arguments.
///
class ArgParser
{
    public:
        /// Representation of the arguments that user wants to parse.
        struct ArgToParse
        {
            public:
                ArgToParse(
                    const std::string &shortForm,
                    const std::string &longForm,
                    const std::string &alias,
                    unsigned paramsCount
                );

            public:
                std::string shortForm;
                std::string longForm;
                /// Internal name used to uniquely identify argument.
                std::string alias;
                /// Count of the arguments that may be passed with this arg.
                unsigned paramsCount;
        };

        /// Representation of the parsed arguments.
        class ParsedArgs
        {
            public:
                /// Values for argument, if takes any.
                using ArgValues = std::vector<std::string>;

            public:
                ParsedArgs();

                bool hasArg(const std::string &name) const;
                std::pair<bool, ArgValues> getArg(const std::string &name) const;
                void addArg(const std::string &name, const ArgValues &values = ArgValues());
                bool addValueToArg(const std::string &name, const std::string &value);

            private:
                /// Maps argument alias to list of its values.
                using Args = std::map<std::string, ArgValues>;
                /// All arguments.
                Args args;
        };

    public:
        /// Definition of the arguments that should be parsed.
        using ArgsToParse = std::vector<ArgToParse>;

    public:
        ArgParser(
            const ArgsToParse &argsToParse,
            const std::string &positionalArgsAlias,
            unsigned positionalArgsCount = 0,
            bool positionalArgsAtTheEnd = false
        );

        ParsedArgs parseArgs(int argc, char **argv);

    private:
        void parseArg(const std::string &value);
        bool inputValueIsArg(const std::string &value) const;

    private:
        ArgsToParse argsToParse;
        /// We need alias even for positional arguments, should'nt
        /// collide with other arguments.
        std::string positionalArgsAlias;
        unsigned positionalArgsCount;
        /// Some tasks require positional args only at the end.
        /// So make the happy.
        bool positionalArgsAtTheEnd;

        /// C Convention.
        std::size_t firstArgumentIndex = 1;
        /// We want to remember last one, to add values to it.
        ArgToParse lastParsed = ArgToParse("", "", "", 0);
        /// Result of parsing.
        ParsedArgs parsedArgs = ParsedArgs();
};
