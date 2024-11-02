#include "network/server.hpp"
#include "threading/thread_safe_iostream.hpp"

void handle1(long long &id, Message &msg)
{
    int value;
    msg >> value;
    threadSafeCout << "Received an integer: " << value << " from client " << id << std::endl;
}

void handle2(long long &id, Message &msg)
{
    size_t size;
    msg >> size;
    std::string str;
    for (size_t i = 0; i < size; ++i)
    {
        char c;
        msg >> c;
        str += c;
    }
    threadSafeCout << "Received a string: " << str << " from client " << id << std::endl;
}

void handle3(long long &id, Message &msg)
{
    double doubledValue;
    msg >> doubledValue;
    threadSafeCout << "Received a doubled value: " << doubledValue << " from client " << id << std::endl;
}

void main_()
{
    Server server;

    server.defineAction(1, handle1);
    server.defineAction(2, handle2);
    server.defineAction(3, handle3);

    server.start(8080);

    bool quit = false;
    while (!quit)
    {
        threadSafeCout << "Available operations :" << std::endl;
        threadSafeCout << " - 1 : send Message with type 1 (int)" << std::endl;
        threadSafeCout << " - 2 : send Message with type 2 (string)" << std::endl;
        threadSafeCout << " - 3 : send Message with type 3 (double)" << std::endl;
        threadSafeCout << " - u : update the client" << std::endl;
        threadSafeCout << " - q : close the program" << std::endl;

        std::string input;
        std::cin >> input;

        if (input.empty())
        {
            threadSafeCout << "Blank input, quitting." << std::endl;
            quit = true;
        }
        else if (input == "1")
        {
            threadSafeCout << "Enter an id: " << std::endl;
            long long id;
            std::cin >> id;
            Message message(1);
            int value;
            threadSafeCout << "Enter an integer: " << std::endl;
            std::cin >> value;
            message << value;
            server.sendTo(message, id);
        }
        else if (input == "2")
        {
            threadSafeCout << "Enter an id: " << std::endl;
            long long id;
            std::cin >> id;
            Message message(2);
            std::string str;
            threadSafeCout << "Enter a string: " << std::endl;
            std::cin >> str;
            message << str.size();
            for (char c : str)
            {
                message << c;
            }
            server.sendTo(message, id);
        }
        else if (input == "3")
        {
            threadSafeCout << "Enter an id: " << std::endl;
            long long id;
            std::cin >> id;
            Message message(3);
            double value;
            threadSafeCout << "Enter an double: " << std::endl;
            std::cin >> value;
            message << value;
            server.sendTo(message, id);
        }
        else if (input == "u")
        {
            server.update();
        }
        else if (input == "q")
        {
            quit = true;
        }
        else
        {
            threadSafeCout << "Invalid input." << std::endl;
        }
    }
}

int main()
{
    try
    {
        main_();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }
    return 0;
}
