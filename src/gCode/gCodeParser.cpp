//
// Created by Bartosz Jadczak on 15/10/2024.
//

#include "gCodeParser.h"

#include <fstream>
#include <iostream>
#include <regex>

std::vector<glm::vec3> GCodeParser::parse(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return {};
    }

    std::string line;
    std::regex expr("^N[0-9]+G01");
    std::regex exprX("X(-?[0-9]+\\.[0-9]+)");
    std::regex exprY("Y(-?[0-9]+\\.[0-9]+)");
    std::regex exprZ("Z(-?[0-9]+\\.[0-9]+)");
    std::smatch match;
    std::vector<glm::vec3> coordinates;

    std::string prev_loc = std::setlocale(LC_NUMERIC, nullptr);
    std::setlocale(LC_NUMERIC,"C");
    while (std::getline(file, line)) {
        if (std::regex_search(line, match, expr)) {
            float x = std::numeric_limits<float>::quiet_NaN();
            float y = std::numeric_limits<float>::quiet_NaN();
            float z = std::numeric_limits<float>::quiet_NaN();
            if(std::regex_search(line, match, exprX))
                x = std::stof(std::string(match[0].first+1, match[0].second));
            if(std::regex_search(line, match, exprY))
                y = std::stof(std::string(match[0].first+1, match[0].second));
            if(std::regex_search(line, match, exprZ))
                z = std::stof(std::string(match[0].first+1, match[0].second));

            coordinates.emplace_back(x, z, -y);
        }
    }
    std::setlocale(LC_NUMERIC, prev_loc.c_str());

    return coordinates;
}
