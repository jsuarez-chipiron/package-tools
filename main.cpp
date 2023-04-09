#include <fstream>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <algorithm>

struct type_element
{
    std::string name;
    std::vector<std::string> members;
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

void process_type(std::string_view content, std::unordered_map<std::string, type_element>& map, std::vector<std::string>& index)
{
    auto start_idx = content.find("<name>");
    auto end_idx = content.find("</name>");

    auto name = content.substr(start_idx+6, end_idx-start_idx-6);
    index.emplace_back(name);

    std::vector<std::string> members;

    start_idx = content.find("<members>");
    while ( start_idx != std::string_view::npos )
    {
        content = content.substr(start_idx+9);
        end_idx = content.find("</members>");
        auto member = content.substr(0, end_idx);
        members.emplace_back(member);
        content = content.substr(end_idx+10);
        start_idx = content.find("<members>");
    }
    type_element te;
    te.name = std::string(name);
    te.members = std::move(members);

    map.insert({te.name, te});
}

int main(int /*argc*/, char ** /*argv*/)
{
    // auto file_content = reader("/home/javier/Tech/c++/flattern/mini.xml");
    auto file_content = reader("/home/javier/Tech/c++/flattern/package.xml");

    std::unordered_map<std::string, type_element> items_map;
    std::vector<std::string> items_idx;

    auto start_idx = file_content.find("<types>");

    while ( start_idx != std::string::npos )
    {
        auto end_idx = file_content.find("</types>");
        process_type(file_content.substr(start_idx, end_idx-start_idx), items_map, items_idx);
        file_content.erase(0, end_idx+8);
        start_idx = file_content.find("<types>");
    }

    std::sort(items_idx.begin(), items_idx.end());

    std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    std::cout << "<Package xmlns=\"http://soap.sforce.com/2006/04/metadata\">\n";
    for (const auto& i: items_idx)
    {
        std::cout << "    <types>\n";
        type_element te = items_map.at(i);
        std::sort(te.members.begin(), te.members.end());
        for (const auto& j: te.members)
        {
            std::cout << "        <members>" << j << "</members>\n";
        }
        std::cout << "        <name>" << te.name << "</name>\n";
        std::cout << "    </types>\n";
    }

    std::cout << "    <version>53.0</version>\n";
    std::cout << "</Package>\n";

    return 0;
}
