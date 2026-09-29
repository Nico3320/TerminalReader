#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

#include "fancytype.cpp"

#ifdef _WIN32
    const char* clearCode = "cls";
#else
    const char* clearCode = "clear";
#endif

std::string VERSION = "V1";

void clear() {
    system(clearCode);
}

int main(int argc, char *argv[]) {
    clear();
    std::vector<std::string> lines;

    if(argc != 2) {
        std::cerr << "usage: /TerminalReader.cpp myText.txt" << std::endl;
        return 0;

    } else {
        std::ifstream file(argv[1]);
        std::string line;

        if (file.is_open()) {
            while (std::getline(file, line)) lines.push_back(line);
            file.close();

        } else {
            std::cerr << "Unable to open file" << std::endl;
            return 0;

        }

    }
    header((" Terminal Reader " + VERSION + " "), '=', 5);
    print(" << Speed in ms >> ");
    
    int speed = 1000;
    std::cin >> speed;

    COLOR color(255,0,0);

    println("");
    print(" << Color >> ");

    std::cin >> color.r >> color.g >> color.b;
    
    for(std::string l : lines) {
        std::stringstream ss(l);

        std::string word;
        while(ss >> word) {
            clear();
            if(word.size() > 1) {
                print(char_to_string(word[0]));
                print(color.get());
                print(char_to_string(word[1]));
                print(COLOR(255,255,255).get());
                println(word.substr(2));

            } else println(word);
            std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        }

    }
    clear();

    header(" Ende ", '=', 5);

}