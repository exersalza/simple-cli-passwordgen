#include <ctime>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <getopt.h>


// define basic char that we'll use in our generator
const std::string LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
const std::string UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
const std::string SPECIAL = "$%&{[]}()?*_-#+.";
const std::string NUMBERS = "0123456789";

char getRandomChar(std::string& charset);


int main(int argc, char** argv) {
    int option;
    bool verbose = false;
    short length = 32;
    int option_index = 0;
    std::string cmd;
    std::string passwd;
    std::string generated_charset;
    std::stringstream characterSet;

    std::srand(std::time(nullptr));

    static struct option long_options[] = {
        {"verbose", no_argument, 0, 'v'},
        {"length", required_argument, 0, 'L'},
        {"lower", no_argument, 0, 'l'},
        {"upper", no_argument, 0, 'u'},
        {"special", no_argument, 0, 's'},
        {"numbers", no_argument, 0, 'n'},
        {0, 0, 0, 0},
    };

    while((option = getopt_long(argc, argv, "vLlusn:", 
                    long_options, &option_index)) != -1) {
        switch(option) {
            case 'v':
                verbose = true;
                break;
            case 'L':
                length = std::atoi(optarg);
                break;
            case 'l':
                characterSet << LOWERCASE;
                break;
            case 'u':
                characterSet << UPPERCASE;
                break;
            case 's':
                characterSet << SPECIAL;
                characterSet << SPECIAL;
                break;
            case 'n':
                characterSet << NUMBERS;
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " [-v] [-L length] [-l lower] [-u upper] [-n NUMBERS] [-s SPECIAL]" << std::endl;
                return 1;
        }
    }

    if(characterSet.rdbuf()->in_avail() == 0) { characterSet << LOWERCASE 
                     << UPPERCASE 
                     << NUMBERS 
                     << SPECIAL << ((std::time(nullptr) % 2) ? SPECIAL : ""); // higher special char thingi
    }

    generated_charset = characterSet.str();
   
    for (int i = 0; i < length; ++i)
        passwd += getRandomChar(generated_charset);
   
    if (verbose) std::cout << "Your password is: " << passwd << '\n';
    
    if (const char* ptrEnv = std::getenv("WAYLAND_DISPLAY")) {
        // for wayland based systems
        cmd = "wl-copy \"" + passwd + '"';
    } else {
        // for systems where xclip works, so no wayland
        cmd = "echo \"" + passwd + "\" | xclip -selection clipboard";
    }
    
    std::system(cmd.c_str());
    
    return 0;
}


char getRandomChar(std::string& char_set) {
    return char_set[std::rand() % char_set.length()];
};
