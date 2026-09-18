#include "portable-file-dialogs.h"
#include <ftxui/ftxui.hpp>

using namespace ftxui;

Element cell(Color color) {
    return text(" ") | bgcolor(color) | size(WIDTH, EQUAL, 4) | size(HEIGHT, EQUAL, 2);
};

int main() {
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::vector<Element>> rows = {
        {cell(Color::GrayLight)},
    };

    auto renderer = Renderer([] { return text("Select option: \n i: generate "); });

    auto component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Character('i')) {
            std::string file;
            auto selection =
                pfd::open_file("Select field .stp CAD file", ".", {"STEP files", "*.stp *.step"})
                    .result();
            if (!selection.empty()) {
                file = selection.front();
                return true;
            }

            return false;
        }
        return false;
    });

    screen.Loop(component);
    return 0;
};