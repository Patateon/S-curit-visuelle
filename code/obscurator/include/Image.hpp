#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace glm;

#define Member(type, name, initval) \
    protected : type _##name = initval; \
    public : const type& name(){return _##name;};

#define UnprotectedMember(type, name, initval) \
    protected : type _##name = initval; \
    public : type& name(){return _##name;};

class Image
{

    Member(ivec2, size, vec2(0))
    Member(uint, totalSize, 0)
    Member(dvec2, range, vec2(0.f, 255.f))
    Member(std::string, originalFilename, "")
    
    UnprotectedMember(std::vector<dvec3>, data, {})

    public : 

        Image(){};
        Image(const char *filename);

        void save(std::string prefix);

        dvec3 &operator()(int x, int y);

        void setRange(dvec2 newRange);
};
