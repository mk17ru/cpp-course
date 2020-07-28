#include "Combinations.h"
#include "Component.h"

#include <iomanip>
#include <iostream>
#include <string>

template <typename... Args>
static void fail(Args&& ... args) noexcept
{
    ((std::cerr << args), ...);
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);
}

int main(int argc, char * argv[])
{
    if (argc != 2) {
        fail("Usage: combinations <combinations XML resource>");
    }

    Combinations combinations;

    const std::filesystem::path path{argv[1]};
    if (!combinations.load(path)) {
        fail("Failed to load combinations XML resource from ", path);
    }

    std::size_t num;
    std::cin >> num;
    if (std::cin.fail()) {
        fail("Unvalid number of legs");
    }

    std::vector<Component> components;
    components.reserve(num);
    while (num--) {
        components.emplace_back(Component::from_stream(std::cin));
        if (components.back().type == InstrumentType::Unknown) {
            fail("Failed to read component");
        }
    }

    std::vector<int> order;
    std::cout << combinations.classify(components, order) << std::endl;
    for (const auto i : order) {
        std::cout << i << std::endl;
    }

    return 0;
}
