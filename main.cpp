#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

std::string reader(const std::string& filename)
{
    std::ifstream ifs (filename, std::ifstream::in);
    std::string line;
    std::vector<std::string> lines;

    while ( getline(ifs, line) )
    {
        lines.emplace_back(line);
    }
    ifs.close();

    std::string result;
    for (const auto& line: lines)
    {
        result.reserve(line.size());
        result.insert(result.end(), line.begin(), line.end());
    }
    return result;
}

void process_type(std::string_view content, std::unordered_map<std::string, std::set<std::string>>& map, std::set<std::string>& index)
{
    auto start_idx = content.find("<name>");
    auto end_idx = content.find("</name>");

    auto name = content.substr(start_idx+6, end_idx-start_idx-6);
    index.emplace(name);

    start_idx = content.find("<members>");
    while ( start_idx != std::string_view::npos )
    {
        content = content.substr(start_idx+9);
        end_idx = content.find("</members>");
        auto member = content.substr(0, end_idx);
        map[std::string(name)].emplace(member);
        content = content.substr(end_idx+10);
        start_idx = content.find("<members>");
    }
}

void parse(std::string content, 
        std::unordered_map<std::string, std::set<std::string>>& map, std::set<std::string>& index)
{
    auto start_idx = content.find("<types>");

    while ( start_idx != std::string::npos )
    {
        auto end_idx = content.find("</types>");
        process_type(content.substr(start_idx, end_idx-start_idx), map, index);
        content.erase(0, end_idx+8);
        start_idx = content.find("<types>");
    }
}

void print(const std::unordered_map<std::string, std::set<std::string>>& map, const std::set<std::string>& index)
{
    std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    std::cout << "<Package xmlns=\"http://soap.sforce.com/2006/04/metadata\">\n";
    for (const auto& name: index)
    {
        std::cout << "    <types>\n";
        for (const auto& member: map.at(name))
        {
            std::cout << "        <members>" << member << "</members>\n";
        }
        std::cout << "        <name>" << name << "</name>\n";
        std::cout << "    </types>\n";
    }

    std::cout << "    <version>53.0</version>\n";
    std::cout << "</Package>\n";
}

int main(int argc, char ** argv)
{
    if ( argc != 3 ) { std::cerr << "invalid number of arguments\n"; }

    auto file_content_1 = reader(argv[1]);
    auto file_content_2 = reader(argv[2]);

    std::unordered_map<std::string, std::set<std::string>> items_map;
    std::set<std::string> items_idx;

    parse(file_content_1, items_map, items_idx);
    parse(file_content_2, items_map, items_idx);

    print(items_map, items_idx);

    return 0;
}
