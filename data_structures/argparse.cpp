#include "data_structures/argparse.hpp"
#include <iostream>
#include <cstdlib>

Argparse::Argparse(const std::string &prog) : prog_(prog) {}
Argparse::Argparse(const std::string &prog, const std::string &desc) : prog_(prog), desc_(desc) {}

void Argparse::add_argument(const ArgOption &option)
{
    options_[option.flag] = option;
    if (!option.alt_flag.empty())
    {
        options_[option.alt_flag] = option;
    }
}

void Argparse::parse(int argc, const char *argv[])
{
    raw_args_.clear();
    for (int i = 1; i < argc; ++i)
    {
        raw_args_.push_back(argv[i]);
    }
    parse();
}

void Argparse::parse(const std::vector<std::string> &args)
{
    raw_args_ = args;
    parse();
}

void Argparse::parse()
{
    bool no_more_options = false;

    parsed_args_.clear();
    positional_args_.clear();
    for (std::size_t i = 0; i < raw_args_.size(); ++i)
    {
        if (!no_more_options && raw_args_[i].size() > 1 && raw_args_[i][0] == '-')
        {
            std::string opt_input = raw_args_[i];
            auto iter = options_.find(opt_input);
            if (iter != options_.end())
            {
                if (iter->second.has_param)
                {
                    if (i + 1 < raw_args_.size())
                    {
                        parsed_args_[iter->second.name] = Argument{iter->second.name, raw_args_[i + 1], true};
                        ++i;
                    }
                    else
                    {
                        if (missing_parameter_handler)
                        {
                            missing_parameter_handler(opt_input);
                        }
                        else
                        {
                            std::cerr << "Missing argument for " << opt_input << std::endl;
                            std::cerr << "\n"
                                      << usage() << std::endl;
                            std::exit(1);
                        }
                    }
                }
                else
                {
                    parsed_args_[iter->second.name] = Argument{iter->second.name, "", true};
                }
            }
            else if (show_usage_ && (opt_input == "-h" || opt_input == "--help"))
            {
                std::cout << usage() << std::endl;
                std::exit(0);
            }
            else if (opt_input == "--")
            {
                no_more_options = true;
                continue;
            }
            else
            {
                if (invalid_argument_handler_)
                {
                    invalid_argument_handler_(opt_input);
                }
                else
                {
                    std::cerr << "Invalid argument: " << opt_input << std::endl;
                    std::cerr << "\n"
                              << usage() << std::endl;
                    std::exit(1);
                }
            }
        }
        else
        {
            positional_args_.push_back(raw_args_[i]);
        }
    }
}

Argparse::Argument Argparse::operator[](const std::string &name) const
{
    auto iter = parsed_args_.find(name);
    if (iter == parsed_args_.end())
    {
        auto iter_option = options_.find(name);
        if (iter_option != options_.end())
        {
            return Argument{name, iter_option->second.default_value, false};
        }
        return Argument{name, "", false};
    }
    return iter->second;
}

std::vector<std::string> &Argparse::positional_args()
{
    return positional_args_;
}
const std::vector<std::string> &Argparse::positional_args() const
{
    return positional_args_;
}

std::vector<std::string> &Argparse::raw_args()
{
    return raw_args_;
}
const std::vector<std::string> &Argparse::raw_args() const
{
    return raw_args_;
}

std::string Argparse::usage() const
{
    std::string usage = "Usage: " + prog_;
    if (!options_.empty())
    {
        usage += " [options]";
    }
    if (!desc_.empty())
    {
        usage += "\n\n" + desc_;
    }
    usage += "\n\nOptions:\n";
    for (const auto &option : options_)
    {
        usage += "  " + option.second.flag;
        if (!option.second.alt_flag.empty())
        {
            usage += ", " + option.second.alt_flag;
        }
        if (option.second.has_param)
        {
            usage += " <" + option.second.name + ">";
        }
        if (!option.second.help.empty())
        {
            usage += "\n    " + option.second.help;
        }
        if (!option.second.default_value.empty())
        {
            usage += " (default: " + option.second.default_value + ")";
        }
        usage += "\n";
    }
    return usage;
}

bool Argparse::toggle_usage(bool show_usage)
{
    show_usage_ = show_usage;
    return show_usage_;
}

void Argparse::set_invalid_argument_handler(std::function<void(const std::string &)> handler)
{
    invalid_argument_handler_ = handler;
}

void Argparse::set_missing_parameter_handler(std::function<void(const std::string &)> handler)
{
    missing_parameter_handler = handler;
}
