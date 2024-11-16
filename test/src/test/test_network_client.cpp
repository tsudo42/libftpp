#include "network/client.hpp"
#include "threading/thread_safe_iostream.hpp"

void handle1(Message &msg)
{
    int value;
    msg >> value;
    threadSafeCout << "Received an integer: " << value << std::endl;
}

void handle2(Message &msg)
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
    threadSafeCout << "Received a string: " << str << std::endl;
}

void handle3(Message &msg)
{
    double doubledValue;
    msg >> doubledValue;
    threadSafeCout << "Received a doubled value: " << doubledValue << std::endl;
}

void main_()
{
    Client client;

    client.defineAction(1, handle1);
    client.defineAction(2, handle2);
    client.defineAction(3, handle3);

    client.connect("localhost", 8080);

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
            Message message(1);
            int value;
            threadSafeCout << "Enter an integer: " << std::endl;
            std::cin >> value;
            message << value;
            client.send(message);
        }
        else if (input == "2")
        {
            Message message(2);
            std::string str;
            threadSafeCout << "Enter a string: " << std::endl;
            std::cin >> str;
            message << str.size();
            for (char c : str)
            {
                message << c;
            }
            client.send(message);
        }
        else if (input == "3")
        {
            Message message(3);
            double value;
            threadSafeCout << "Enter an double: " << std::endl;
            std::cin >> value;
            message << value;
            client.send(message);
        }
        else if (input == "u")
        {
            client.update();
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
