#include "types.h"
#include "CHIP8.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>

#include <cstdio>
#include <unistd.h>
#include <signal.h>

std::vector<byte> readBinaryFile(const char *filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error(std::string("Failed to open ") + filename);
    }

    file.unsetf(std::ios::skipws);

    std::streampos bufSize;

    file.seekg(0, std::ios::end);
    bufSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<byte> vec(bufSize);

    file.read(reinterpret_cast<char *>(vec.data()), vec.size());

    return vec;
}

int hexLookup(unsigned char hex_digit)
{
    static const signed char hex_values[256] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,
            -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };
    int value = hex_values[hex_digit];
    if (value == -1) throw std::runtime_error("Invalid hex: Unknown digit");
    return value;
}

std::vector<byte> decodeHex(const std::string &hex)
{
    const auto len = hex.length();
    if (len & 1u) throw std::runtime_error("Invalid hex: Odd length");

    std::vector<byte> output;
    output.reserve(len / 2);
    for (auto it = hex.begin(); it != hex.end();)
    {
        unsigned int hi = hexLookup(*it++);
        unsigned int lo = hexLookup(*it++);
        output.push_back(hi << 4u | lo);
    }
    return output;
}

void slowGame(int signum)
{
    puts("The Pwn Lion has reached a verdict...\nIncredibly unoptimized, and not worth your time!\n - 2/10 Lions");
    exit(0);
}

int main(int argc, char **argv)
{

    //Alarm so people don't use too much resources
    struct sigaction act = {nullptr};
    act.sa_handler = &slowGame;
    act.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &act, nullptr);

    alarm(3);

    std::cout <<
              "---------------------------------------------------\n"
              "| The Pwn Lion got bored of the AAA games, and    |\n"
              "| decided to start reviewing community made       |\n"
              "| ones. He will accept your game in a hex format. |\n"
              "---------------------------------------------------\n" << std::endl;

    std::string input;
    std::getline(std::cin, input);

    std::vector<byte> decoded;

    try
    {
        decoded = decodeHex(input);
    } catch (const std::runtime_error &rt)
    {
        std::cout << "Runtime error: " << rt.what() << std::endl;
        return 0;
    }

    CHIP8 chip8(decoded);

    chip8.run();

    return 0;
}
