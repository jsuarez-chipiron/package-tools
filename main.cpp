#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

struct type_element
{
    std::string name;
    std::set<std::string> members;
};

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

void process_type(std::string_view content, std::unordered_map<std::string, type_element>& map, std::set<std::string>& index)
{
    auto start_idx = content.find("<name>");
    auto end_idx = content.find("</name>");

    auto name = content.substr(start_idx+6, end_idx-start_idx-6);
    index.emplace(name);

    std::set<std::string> members;

    start_idx = content.find("<members>");
    while ( start_idx != std::string_view::npos )
    {
        content = content.substr(start_idx+9);
        end_idx = content.find("</members>");
        auto member = content.substr(0, end_idx);
        members.emplace(member);
        content = content.substr(end_idx+10);
        start_idx = content.find("<members>");
    }
    type_element te;
    te.name = std::string(name);
    te.members = std::move(members);

    if ( map.contains(te.name) )
    {
        te.members.insert(map.at(te.name).members.begin(), map.at(te.name).members.end());
    }
    map[te.name] = te;
}

void parse(std::string content, 
        std::unordered_map<std::string, type_element>& map, std::set<std::string>& index)
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

void print(const std::unordered_map<std::string, type_element>& map, const std::set<std::string>& index)
{
    std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    std::cout << "<Package xmlns=\"http://soap.sforce.com/2006/04/metadata\">\n";
    for (const auto& i: index)
    {
        std::cout << "    <types>\n";
        type_element te = map.at(i);
        for (const auto& j: te.members)
        {
            std::cout << "        <members>" << j << "</members>\n";
        }
        std::cout << "        <name>" << te.name << "</name>\n";
        std::cout << "    </types>\n";
    }

    std::cout << "    <version>53.0</version>\n";
    std::cout << "</Package>\n";
}

int main(int /*argc*/, char ** /*argv*/)
{
    auto file_content_1 = reader("/home/javier/Tech/c++/flattern/package.xml");
    auto file_content_2 = reader("/home/javier/Tech/c++/flattern/mini.xml");

    std::unordered_map<std::string, type_element> items_map;
    std::set<std::string> items_idx;

    parse(file_content_1, items_map, items_idx);
    parse(file_content_2, items_map, items_idx);

    print(items_map, items_idx);

    return 0;
}
