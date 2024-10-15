//
// Created by Bartosz Jadczak on 15/10/2024.
//

#ifndef GCODEPARSER_H
#define GCODEPARSER_H

#include <string>
#include <vector>
#include <glm/vec3.hpp>


class GCodeParser {
public:
    static std::vector<glm::vec3> parse(const std::string &filePath);
};



#endif //GCODEPARSER_H
