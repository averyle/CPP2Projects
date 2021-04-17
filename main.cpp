#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <memory>

struct Color {
    float red;
    float green;
    float blue;

    static Color random() {
        static std::mt19937 engine{ std::random_device{}() };
        static std::uniform_real_distribution<float> dist{ 0, 1 };

        return Color{ dist(engine), dist(engine), dist(engine) };
    }
};

void colorSink(std::unique_ptr<Color> color) {
    // ... do nothing ... do not change the signature or implementation of this function
    std::cout << "Success!\n";
}

std::ostream& operator<<(std::ostream& out, Color& color) {
    out << "COLOR(RED=" << color.red << ", GREEN=" << color.green << ", BLUE=" << color.blue << ")";
    return out;
}

int main() {
    // 1. overload the output stream operator so that you can print this color directly to the output stream
    auto c = Color::random();
    // TODO: uncomment the line below, the color should be printed
    std::cout << "First color is: " << c << "\n";

    // 2. here is a list of ten colors, print them out in order, and then in reverse order
    std::vector<Color> colors;
    for (int i = 0; i < 10; i++) {
        colors.push_back(Color::random());
    }

    std::cout << "COLORS: [\n";
    for (auto i = colors.begin(); i != colors.end(); ++i) {
        std::cout << *i << "\n";
    }
    std::cout << "]\n";

    std::cout << "REVERSED COLORS: [\n";
    std::reverse(colors.begin(), colors.end());
    for (auto i = colors.begin(); i != colors.end(); ++i) {
        std::cout << *i << "\n";
    }
    std::cout << "]\n";

    // 3. find the color in the list that is darkest and print it out
    // the darkest color is defined as the color where the sum of the red, green, and blue components is the smallest
    auto darkestColor = colors[0];
    float min = darkestColor.red + darkestColor.green + darkestColor.blue;
    for (int i = 1; i < colors.size(); i++) {
            if ((colors[i].red + colors[i].green + colors[i].blue) < min) {
                darkestColor = colors[i];
                min = darkestColor.red + darkestColor.green + darkestColor.blue;
            }
    }
    std::cout << "The darkest color is " << darkestColor << "\n";

    // 4. order the colors from smallest 'blue' value to largest 'blue' value and print them out
    std::vector<Color> blueSorted;
    auto comp = [](const Color& c1, const Color& c2) { return (c1.blue < c2.blue); };
    std::sort(colors.begin(), colors.end(), comp);
    std::cout << "COLORS SORTED BY BLUE: [ \n";
    for (auto i = colors.begin(); i != colors.end(); ++i) {
        std::cout << *i << "\n";
    }
    std::cout << "]\n";

    // 5. create a std::unique_ptr to a color on the free-store and call it 'colorPtr', pass this color to the
    // 'colorSink' function.
    // Uncomment the below two lines, and replace the ... with code
    //std::unique_ptr<Color> colorPtr = std::make_unique<Color>();
    //colorSink(colorPtr);
}