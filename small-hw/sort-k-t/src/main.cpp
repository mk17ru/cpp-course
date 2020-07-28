#include<iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <tuple>

struct splited_input {
    std::string s;
    std::pair<int, int> sorted_range;
    explicit splited_input(std::string & cur) : s(cur) {}
};

bool comp(splited_input & a, splited_input & b) {
    return a.s.compare(a.sorted_range.first, a.sorted_range.second, b.s, b.sorted_range.first, b.sorted_range.second) <= 0;
}

std::vector< splited_input > read_input(std::istream &input) {
    std::vector< splited_input > lines;
    std::string str;
    while (getline(input, str)) {
        lines.emplace_back(str);
    }
    return lines;
}

void split_lines( std::vector<splited_input> & lines, std::string_view sep, int field1, int field2) {

    for (auto & line : lines) {
        int start = 0, len = 0;
        size_t last = 0;
        int number_of_str = 0;
        size_t found =  line.s.find_first_of(sep);
        while (found != std::string::npos)
        {
            if (found - last > 0) {
                number_of_str++;
                if (field1 == 0 ||(number_of_str >=field1 && (field2 == 0 || number_of_str <=field2))) {
                    if (len == 0) {
                        start = last;
                    }
                    len += found - last;
                }
            }
            int new_found = line.s.find_first_of(sep,found + 1);
            if (field1 == 0 ||(number_of_str >=field1 && (field2 == 0 || number_of_str <=field2))) {
                len += found - last;
            }
            last = found + 1;
            found = new_found;
        }
        if (line.s.size() - last > 0) {
            number_of_str++;
            if (field1 == 0 ||(number_of_str >= field1 && (field2 == 0 || number_of_str <= field2))) {
                if (len == 0) {
                    start = last;
                }
                len += line.s.size() - last;
            }
        }
        line.sorted_range = {start, len};
    }
}

std::tuple<std::string, const char *, int, int> parse_commands( int argc, char ** argv) {
    int field1 = 0, field2 = 0;
    const char * input = nullptr;
    const std::string sep_mode = "-t";
    const std::string col_mode = "-k";
    const std::string minus = "-";
    std::string sep= " ";
    int i = 1;
    while(i < argc) {
        if (argv[i] == sep_mode)  {
            i++;
            sep = argv[i];
            i++;
            continue;
        }
        if (argv[i] == col_mode)  {
            i++;
           field1 = std::stoi(argv[i]);
            i++;
            if (i < argc) {
                std::string str = argv[i];
                bool is_number = true;
                for (auto p : str) {
                    if (p < '0' || p > '9') {
                        is_number = false;
                    }
                }
                if (is_number) {
                    field2 = std::stoi(argv[i]);
                    ++i;
                }
            }
            continue;
        }
        if (argv[i] == minus) {
        } else {
            input = argv[i];
        }
        ++i;
    }

    return std::make_tuple(sep, input, field1, field2);
}


int main(int argc, char ** argv)
{
    std::string column;
    std::vector<splited_input> lines;

    auto res_commands = parse_commands(argc, argv);
    const auto [sep, input, field1, field2] = parse_commands(argc, argv);

    if (input != nullptr) {
        std::ifstream f(input);
        lines = read_input(f);
    }
    else {
        lines = read_input(std::cin);
    }

    split_lines(lines, sep, field1, field2);
    sort(lines.begin(), lines.end(), comp);
    for (size_t j = 0; j < lines.size(); ++j) {
        std::cout << lines[j].s << std::endl;
    }

    return 0;
}