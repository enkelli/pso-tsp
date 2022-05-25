///
/// FIT BUT, SFC, 1. project
/// Pavol Plaskon, xplask00
///

#include <cctype>

#include "arg_parser.hpp"
#include "exceptions.hpp"

///
/// Creates new argument that is expected in input args.
///
/// @c Alias is used as key to access arguments in result. It's better than
/// using one of short/long form when one of them is missing.
/// This should exactly define arguments name.
///
ArgParser::ArgToParse::ArgToParse(
    const std::string &shortForm,
    const std::string &longForm,
    const std::string &alias,
    unsigned paramsCount):
    shortForm(shortForm), longForm(longForm), alias(alias), paramsCount(paramsCount) {}


///
/// Creates new representation for parsed arguments.
///
ArgParser::ParsedArgs::ParsedArgs() = default;

///
/// Checks whether argument with name @c name is among parsed ones.
///
bool ArgParser::ParsedArgs::hasArg(const std::string &name) const
{
    return args.find(name) != args.end();
}

///
/// Return @c pair < @c bool, @c argument_values>.
///
/// Where @c bool says whether argument is among parsed and when it's @c true,
/// second part contains it values (if has any).
///
/// @c argument_values are undefined when first part of pair is @c false.
///
std::pair<bool, ArgParser::ParsedArgs::ArgValues> ArgParser::ParsedArgs::getArg(
    const std::string &name
    ) const
{
    auto arg = args.find(name);
    if (arg != args.end())
    {
        return std::make_pair(true, arg->second);
    }
    return std::make_pair(false, ArgValues());
}

///
/// Adds new argument to parsed arguments.
///
/// Values may be added later with @c addValueToArg.
///
void ArgParser::ParsedArgs::addArg(const std::string &name, const ArgValues &values)
{
    args[name] = values;
}

///
/// Adds another value for argument with name @c name.
///
/// Does nothing when argument is not in parsed arguments.
///
bool ArgParser::ParsedArgs::addValueToArg(const std::string &name, const std::string &value)
{
    auto arg = args.find(name);
    if (arg != args.end())
    {
        arg->second.push_back(value);
        return true;
    }
    return false;
}

///
/// Creates new argument parser.
///
ArgParser::ArgParser(
    const ArgsToParse &argsToParse,
    const std::string &positionalArgsAlias,
    unsigned positionalArgsCount,
    bool positionalArgsAtTheEnd):
    argsToParse(argsToParse), positionalArgsAlias(positionalArgsAlias),
    positionalArgsCount(positionalArgsCount), positionalArgsAtTheEnd(positionalArgsAtTheEnd) {}

///
/// Parses input arguments.
///
/// @return @c Args.Vector of arguments.
///
/// @throw ArgParserError
///
ArgParser::ParsedArgs ArgParser::parseArgs(int argc, char **argv)
{
    ArgParser::ParsedArgs::ArgValues positionalArgs;

    for (int i = firstArgumentIndex; i < argc; ++i)
    {
        const std::string inputValue(argv[i]);

        if (inputValueIsArg(inputValue))
        {
            parseArg(inputValue);
            // It's error when there were some positional arguments
            // and they are permitted only at the end.
            if (positionalArgsAtTheEnd && (!positionalArgs.empty()))
            {
                throw ArgParserError("unexpected argument " + positionalArgs[0]);
            }
        }
        else
        {
            // Store value to the previous parsed arg.
            // When it has already all values that we store the value to positional args.
            auto previousParsedArg = parsedArgs.getArg(lastParsed.alias);
            if (previousParsedArg.first)
            {
                if (previousParsedArg.second.size() < lastParsed.paramsCount)
                {
                    parsedArgs.addValueToArg(lastParsed.alias, inputValue);
                }
                else
                {
                    positionalArgs.push_back(inputValue);
                }
            }
            else
            {
                positionalArgs.push_back(inputValue);
            }

        }
    }
    // At the end we add positional args to result.
    parsedArgs.addArg(positionalArgsAlias, positionalArgs);
    return parsedArgs;
}

///
/// Parses one argument and stores it in parsed arguments.
///
/// It does nothing with argument's values.
///
void ArgParser::parseArg(const std::string &value)
{
    for (const auto &toParse: argsToParse)
    {
        if ((value == toParse.shortForm) || (value == toParse.longForm))
        {
            lastParsed = toParse;
            // Here maybe we should check also values count.
            // It's not always error when argument is present several times.
            if (parsedArgs.hasArg(toParse.alias))
            {
                throw ArgParserError("duplicit argument: " + toParse.alias);
            }
            parsedArgs.addArg(toParse.alias);
            return;
        }
    }
    throw ArgParserError("unknown argument " + value);
}

///
/// Checks whether @c value is input argument.
///
bool ArgParser::inputValueIsArg(const std::string &value) const
{
    return value.size() > 1 && value[0] == '-' && (value[1] == '-' || isalpha(value[1]));
}
