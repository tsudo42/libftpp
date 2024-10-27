#ifndef ARGPARSE_HPP
#define ARGPARSE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

class Argparse
{
public:
    struct Argument;
    struct ArgOption;

    Argparse() = default;
    explicit Argparse(const std::string &prog);
    explicit Argparse(const std::string &prog, const std::string &desc);

    virtual ~Argparse() = default;

    void add_argument(const ArgOption &option);

    void parse(int argc, const char *argv[]);
    void parse(const std::vector<std::string> &args);
    virtual void parse();

    Argument operator[](const std::string &name) const;

    std::vector<std::string> &positional_args();
    const std::vector<std::string> &positional_args() const;

    std::vector<std::string> &raw_args();
    const std::vector<std::string> &raw_args() const;

    virtual std::string usage() const;
    bool toggle_usage(bool show_usage);

    void set_invalid_argument_handler(std::function<void(const std::string &)> handler);
    void set_missing_parameter_handler(std::function<void(const std::string &)> handler);

private:
    std::string prog_;
    std::string desc_;

    std::unordered_map<std::string, ArgOption> options_;
    std::vector<std::string> raw_args_;

    std::unordered_map<std::string, Argument> parsed_args_;
    std::vector<std::string> positional_args_;

    bool show_usage_ = true;
    std::function<void(const std::string &)> invalid_argument_handler_ = nullptr;
    std::function<void(const std::string &)> missing_parameter_handler = nullptr;
};

struct Argparse::Argument
{
    std::string name;
    std::string value = "";
    bool is_set = false;
};

struct Argparse::ArgOption
{
    std::string name;
    std::string flag;
    bool has_param = false;
    std::string alt_flag = "";
    std::string help = "";
    std::string default_value = "";
    bool is_set = false;
};

#endif /* ARGPARSE_HPP */
