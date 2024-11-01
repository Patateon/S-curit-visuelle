#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace glm;

#define Member(type, name, initval) \
    protected : type _##name = initval; \
    public : const type& name(){return _##name;};


class Image
{

    Member(ivec2, size, vec2(0))
    Member(uint, totalSize, 0)
    Member(dvec2, range, vec2(0.f, 255.f))
    Member(std::vector<dvec3>, data, {})
    Member(std::string, originalFilename, "")

    public : 

        Image(){};
        Image(const char *filename);

        void save(std::string prefix);

        double &operator()(int x, int y);

        void setRange(dvec2 newRange);
};
