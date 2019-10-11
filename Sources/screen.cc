#include "screen.hpp"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string_view>

#ifdef _WIN32
#include <conio.h>

void Screen::ClearScreen()
{
    system("cls");
}

int Screen::GetKey()
{
    return _getch();
}

#else

#include <termios.h>
#include <unistd.h>

void Screen::ClearScreen()
{
    system("clear");
}

int Screen::GetKey(void)
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

#endif

namespace Screen::Color
{
Modifier::Modifier(Code pCode) : code(pCode)
{
    // Do nothing.
}

std::ostream& operator<<(std::ostream& os, const Modifier& mod)
{
    return os << "\033[" << static_cast<int>(mod.code) << "m";
}
}  // namespace Screen::Color

namespace Screen::Image
{
std::string LogoImage()
{
    // 9 x 35
    constexpr std::size_t height = 9;
    constexpr std::size_t width = 35;
    const auto logoText = {
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
        "BB                                 BB",
        "BB  BBBBB   BBBBB   BB BB   BBBBB  BB",
        "BB     BB   BB BB   BB BB   BB BB  BB",
        "BB  BBBBB   BB BB   BBBBB   BBBBB  BB",
        "BB  BB      BB BB      BB   BB BB  BB",
        "BB  BBBBB   BBBBB      BB   BBBBB  BB",
        "BB                                 BB",
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    };
    const auto logo = std::begin(logoText);

    std::ostringstream logoImage;
    logoImage << Color::fgCyan << Color::bgCyan << logo[0];

    for (std::size_t i = 1; i < height - 1; ++i)
    {
        logoImage << '\n';
        std::string_view str = logo[i];

        if (i == 1 || i == height - 2)
        {
            str.remove_prefix(2);
            str.remove_suffix(2);
            logoImage << Color::bgCyan << "  " << Color::bgWhite << str
                      << Color::bgCyan << "  ";
            continue;
        }

        // logoImage << Color::bgCyan << "  ";

        bool alreadyWhite = true;

        for (std::size_t j = 0; j < width; ++j)
        {
            if (!alreadyWhite && str[j] == ' ')
            {
                logoImage << Color::bgWhite;
                alreadyWhite = true;
            }
            else if (alreadyWhite && str[j] == 'B')
            {
                if (j < 3)
                {
                    logoImage << Color::bgCyan;
                }
                else if (j < 9)
                {
                    logoImage << Color::bgRed;
                }
                else if (j < 17)
                {
                    logoImage << Color::bgYellow;
                }
                else if (j < 25)
                {
                    logoImage << Color::bgGreen;
                }
                else if (j < 32)
                {
                    logoImage << Color::bgBlue;
                }
                else
                {
                    logoImage << Color::bgCyan;
                }
                alreadyWhite = false;
            }
            logoImage << " ";
        }
        logoImage << Color::bgCyan << "  ";
    }

    logoImage << '\n' << Color::fgCyan << Color::bgCyan << logo[height - 1];

    return logoImage.str();
}
}  // namespace Screen::Image