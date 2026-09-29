#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

// ============================ Helpers

#ifdef _WIN32

#include <windows.h>

int getTerminalWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi{};

    if (!GetConsoleScreenBufferInfo(
            GetStdHandle(STD_OUTPUT_HANDLE),
            &csbi))
    {
        return 0;
    }

    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

#else

#include <sys/ioctl.h>
#include <unistd.h>

int getTerminalWidth() {
    struct winsize w{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        return 0; // failed
    }
    return w.ws_col;
}

#endif

std::string char_to_string(char c) {
    std::string str;
    str.push_back(c);
    return str;
}

bool contains_line_break(std::string str) {
    for(char c : str) if(c == '\n') return true;
    return false;
}

bool char_is_num(char c) {
    for(int i = 48; i < 58; i++) if(c == i) return true;
    return false;
}

// ============================ Ansi
std::unordered_map<std::string, std::string> ANSI = {
    {"BLACK",              "\033[30m"},
    {"RED",                "\033[31m"},
    {"GREEN",              "\033[32m"},
    {"YELLOW",             "\033[33m"},
    {"BLUE",               "\033[34m"},
    {"MAGENTA",            "\033[35m"},
    {"CYAN",               "\033[36m"},
    {"WHITE",              "\033[37m"},

    {"BRIGHT_BLACK",       "\033[90m"},
    {"BRIGHT_RED",         "\033[91m"},
    {"BRIGHT_GREEN",       "\033[92m"},
    {"BRIGHT_YELLOW",      "\033[93m"},
    {"BRIGHT_BLUE",        "\033[94m"},
    {"BRIGHT_MAGENTA",     "\033[95m"},
    {"BRIGHT_CYAN",        "\033[96m"},
    {"BRIGHT_WHITE",       "\033[97m"},

    {"RESET",               "\033[0m"},

    {"BOLD",                "\033[1m"},
    {"DIM",                 "\033[2m"},
    {"ITALIC",              "\033[3m"},
    {"UNDERLINE",           "\033[4m"},
    {"BLINK",               "\033[5m"},
    {"REVERSE",             "\033[7m"},
    {"HIDDEN",              "\033[8m"},
    {"STRIKETHROUGH",       "\033[9m"},

    {"BG_BLACK",           "\033[40m"},
    {"BG_RED",             "\033[41m"},
    {"BG_GREEN",           "\033[42m"},
    {"BG_YELLOW",          "\033[43m"},
    {"BG_BLUE",            "\033[44m"},
    {"BG_MAGENTA",         "\033[45m"},
    {"BG_CYAN",            "\033[46m"},
    {"BG_WHITE",           "\033[47m"},

    {"BG_BRIGHT_BLACK",   "\033[100m"},
    {"BG_BRIGHT_RED",     "\033[101m"},
    {"BG_BRIGHT_GREEN",   "\033[102m"},
    {"BG_BRIGHT_YELLOW",  "\033[103m"},
    {"BG_BRIGHT_BLUE",    "\033[104m"},
    {"BG_BRIGHT_MAGENTA", "\033[105m"},
    {"BG_BRIGHT_CYAN",    "\033[106m"},
    {"BG_BRIGHT_WHITE",   "\033[107m"}
};

// ============================ Classes
class COLOR {
    public:
    int r = 255;
    int g = 255;
    int b = 255;

    COLOR() {}
    COLOR(int pR, int pG, int pB) {
        r = pR;
        g = pG;
        b = pB;
    }

    void set_color(int pR, int pG, int pB) {
        r = pR;
        g = pG;
        b = pB;
    }

    std::string get() {
        std::string res = "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
        return res;
    }

    // Freundschaft ding mit cout, was dann automatisch den ansi string ausgibt
    // bsp. std::cout << myColor << "Text";

};

std::ostream& operator<<(std::ostream& os, const COLOR& c) {
    std::string str = "\033[38;2;" + std::to_string(c.r) + ";" + std::to_string(c.g) + ";" + std::to_string(c.b) + "m";
    os << str;
    return os;
}

class BORDER {
    public:
    char top    = '%';
    char bottom = '%';
    char left   = '%';
    char right  = '%';
    char corner = '%';
    bool strike =   1;
    bool bold   =   1;
    COLOR color = COLOR(255,255,255);

    public:
    BORDER() {}
    BORDER(char pTop, char pBottom, char pLeft, char pRight, char pCorner, bool pStrike, bool pBold, COLOR pColor) {
        top    = pTop;
        bottom = pBottom;
        left   = pLeft;
        right  = pRight;
        corner = pCorner;
        strike = pStrike;
        bold   = pBold;
        color  = pColor;
    }
    BORDER(std::vector<char> TBLRC, bool pStrike, bool pBold, COLOR pColor) {
        top    = TBLRC[0];
        bottom = TBLRC[1];
        left   = TBLRC[2];
        right  = TBLRC[3];
        corner = TBLRC[4];
        strike = pStrike;
        bold   = pBold;
        color  = pColor;
    }
    BORDER(std::vector<char> TBLRC) {
        top    = TBLRC[0];
        bottom = TBLRC[1];
        left   = TBLRC[2];
        right  = TBLRC[3];
        corner = TBLRC[4];
    }
};

// ============================ Funcs
std::string color_code(int r, int g, int b) {
    std::string res = "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
    return res;
}
std::string color_code(COLOR color) {
    std::string res = "\033[38;2;" + std::to_string(color.r) + ";" + std::to_string(color.g) + ";" + std::to_string(color.b) + "m";
    return res;
}

void mod(std::vector<std::string> mods) {
    for(std::string str : mods) std::cout << str;
}

void line(int size, COLOR color, std::string pieces) {
    std::cout << color_code(color.r, color.g, color.b);
    for(int i = 0; i < size; i++) std::cout << pieces;
    std::cout << ANSI["RESET"];
}
void line(int size, std::string pieces) {
    for(int i = 0; i < size; i++) std::cout << pieces;
    std::cout << ANSI["RESET"];
}

void headLine(COLOR color, char piece) {
    std::string str;
    str.push_back(piece);
    line(getTerminalWidth(), color, str);
}
void headLine(char piece) {
    std::string str;
    str.push_back(piece);
    line(getTerminalWidth(), str);
}

void header(std::string text, COLOR color, char piece, int margin) {
    line(margin, color, char_to_string(piece));
    std::cout << color << text;
    line(getTerminalWidth()-margin-text.size(), char_to_string(piece));
    std::cout << ANSI["RESET"] << std::endl;
}
void header(std::string text, char piece, int margin) {
    line(margin, char_to_string(piece));
    std::cout << text;
    line(getTerminalWidth()-margin-text.size(), char_to_string(piece));
    std::cout << std::endl;
}

std::string process(std::string str) {
    std::string current = "";
    COLOR col;
    int count = 0;
    std::string newStr  = "";
    bool record = false;
    bool record_color = false;

    for(char c : str) {
        if(c == '%') {
            if(!record) record = true;
        } else if(c == '$') {
            if(!record_color) record_color = true;
        } else {
            if(record) {
                if(c == ' ') {
                    newStr += ANSI[current];
                    record = false;
                    current = "";
                } else current.push_back(c);
            } else if(record_color) {
                if(!char_is_num(c)) {
                    switch(count) {
                        case 0:
                            col.r = stoi(current);
                            break;
                        case 1:
                            col.g = stoi(current);
                            break;
                        case 2:
                            col.b = stoi(current);
                            break;
                    }
                    current = "";
                    count++;
                    if(count == 3) {
                        count = 0;
                        newStr += color_code(col);
                        col = COLOR();
                        record_color = false;
                    }
                } else current.push_back(c);
            } else {
                newStr.push_back(c);
            }
        }
    }
    if(current.size() != 0) newStr += ANSI[current];
    return newStr;
}

void print(std::string str) {
    std::cout << process(str);
}
void println(std::string str) {
    std::cout << process(str) << std::endl;
}

void bordered_text(std::string text, BORDER border, int paddingVertical, int paddingHorizontal) {
    if(border.strike) std::cout << ANSI["STRIKETHROUGH"];
    if(border.bold)   std::cout << ANSI["BOLD"];
    std::cout << color_code(border.color);

    std::cout << border.corner;
    line(text.size()+(paddingHorizontal*2), char_to_string(border.top));
    if(border.strike) std::cout << ANSI["STRIKETHROUGH"];
    if(border.bold)   std::cout << ANSI["BOLD"];
    std::cout << color_code(border.color);

    std::cout << char_to_string(border.corner) << "\n";
    for(int i = 0; i < paddingVertical; i++) {
        std::cout << char_to_string(border.left);
        std::cout << ANSI["RESET"];
        line(text.size()+(paddingHorizontal*2), " ");

        if(border.strike) std::cout << ANSI["STRIKETHROUGH"];
        if(border.bold)   std::cout << ANSI["BOLD"];
        std::cout << color_code(border.color);
        std::cout << char_to_string(border.right);
        std::cout << "\n";
    }
    std::cout << char_to_string(border.left);
    std::cout << ANSI["RESET"];
    for(int i = 0; i < paddingHorizontal; i++) std::cout << " ";
    print(text);
    for(int i = 0; i < paddingHorizontal; i++) std::cout << " ";

    if(border.strike) std::cout << ANSI["STRIKETHROUGH"];
    if(border.bold)   std::cout << ANSI["BOLD"];
    std::cout << color_code(border.color);
    std::cout << char_to_string(border.right) << "\n";
    for(int i = 0; i < paddingVertical; i++) {
        std::cout << char_to_string(border.left);
        std::cout << ANSI["RESET"];
        line(text.size()+(paddingHorizontal*2), " ");

        if(border.strike) std::cout << ANSI["STRIKETHROUGH"];
        if(border.bold)   std::cout << ANSI["BOLD"];
        std::cout << color_code(border.color);
        std::cout << char_to_string(border.right);
        std::cout << "\n";
    }
    std::cout << border.corner;

    line(text.size()+(paddingHorizontal*2), char_to_string(border.bottom));
    if(border.strike) std::cout << ANSI["STRIKETHROUGH"];
    if(border.bold)   std::cout << ANSI["BOLD"];
    std::cout << color_code(border.color);

    std::cout << char_to_string(border.corner);
}