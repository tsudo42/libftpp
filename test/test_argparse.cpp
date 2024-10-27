#include "data_structures/argparse.hpp"
#include <cassert>
#include <iostream>

void test_basic_parsing()
{
    Argparse parser("test_prog", "This is a test program.");

    parser.add_argument({"help", "--help", false, "-h", "Show help message"});
    parser.add_argument({"output", "--output", true, "-o", "Output file", "default.txt"});

    std::vector<std::string> args = {"--help", "--output", "result.txt", "pos1", "pos2"};

    parser.parse(args);

    Argparse::Argument help_arg = parser["help"];
    assert(help_arg.is_set == true);
    assert(help_arg.value == "");

    Argparse::Argument output_arg = parser["output"];
    assert(output_arg.is_set == true);
    assert(output_arg.value == "result.txt");

    const auto &positional = parser.positional_args();
    assert(positional.size() == 2);
    assert(positional[0] == "pos1");
    assert(positional[1] == "pos2");

    std::cout << "Test1: basic tests - Passed" << std::endl;
}

void test_default_values()
{
    Argparse parser("test_prog");

    parser.add_argument({"verbose", "--verbose", false, "-v", "Enable verbose mode"});
    parser.add_argument({"config", "--config", true, "-c", "Config file", "config.yaml"});

    std::vector<std::string> args = {"--config", "custom_config.yaml"};

    parser.parse(args);

    Argparse::Argument verbose_arg = parser["verbose"];
    assert(verbose_arg.is_set == false);

    Argparse::Argument config_arg = parser["config"];
    assert(config_arg.is_set == true);
    assert(config_arg.value == "custom_config.yaml");

    std::cout << "Test2: default value - Passed" << std::endl;
}

void test_short_and_long_flags()
{
    Argparse parser("test_prog");

    parser.add_argument({"version", "--version", false, "-V", "Show version"});
    parser.add_argument({"output", "--output", true, "-o", "Output file"});

    std::vector<std::string> args = {"-V", "-o", "output.txt"};

    parser.parse(args);

    Argparse::Argument version_arg = parser["version"];
    assert(version_arg.is_set == true);
    assert(version_arg.value == "");

    Argparse::Argument output_arg = parser["output"];
    assert(output_arg.is_set == true);
    assert(output_arg.value == "output.txt");

    std::cout << "Test3: short long flags - Passed" << std::endl;
}

void test_positional_only()
{
    Argparse parser("test_prog");

    std::vector<std::string> args = {"input1.txt", "input2.txt", "input3.txt"};

    parser.parse(args);

    const auto &positional = parser.positional_args();
    assert(positional.size() == 3);
    assert(positional[0] == "input1.txt");
    assert(positional[1] == "input2.txt");
    assert(positional[2] == "input3.txt");

    std::cout << "Test4: positional arguments - Passed" << std::endl;
}

void test_usage()
{
    Argparse parser("test_prog", "A program to test Argparse.");

    parser.add_argument({"help", "--help", false, "-h", "Show help"});
    parser.add_argument({"output", "--output", true, "-o", "Output file", "default.txt"});

    std::string usage = parser.usage();
    assert(!usage.empty());

    std::cout << "Test5: show usage - Passed" << std::endl;
}

bool ok = false;
void invalid_argument_handler(const std::string &arg)
{
    assert(arg == "--unknown");
    ok = true;
}

void test_invalid_argument()
{
    Argparse parser("test_prog");

    std::vector<std::string> args = {"--unknown", "value"};

    parser.set_invalid_argument_handler(invalid_argument_handler);
    parser.parse(args);

    assert(ok);
    std::cout << "Test6: invalid argument - Passed" << std::endl;
}

int main()
{
    test_basic_parsing();
    test_default_values();
    test_short_and_long_flags();
    test_positional_only();
    test_usage();
    test_invalid_argument();

    std::cout << "Finished all tests." << std::endl;
    return 0;
}
