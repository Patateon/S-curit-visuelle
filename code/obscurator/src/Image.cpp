#include <Image.hpp>
#include <Utils.hpp>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <iostream>

Image::Image(const char *filename)
{
    stbi_uc *tmpData = stbi_load(filename, &_size.x, &_size.y, nullptr, 3);

    if(!tmpData)
    {
        FILE_ERROR_MESSAGE(filename, "Filen either don't exist or isn't supported by stb_image.");
        exit(EXIT_FAILURE); // I have no chill
    }

    _totalSize = _size.x*_size.y;
    _data.resize(_totalSize);
    
    for(uint i = 0; i < _totalSize; i++)
    {
        _data[i] = dvec3(((float)tmpData[i*3]), ((float)tmpData[i*3 + 1]), ((float)tmpData[i*3 + 2]));
    }

    _originalFilename = std::string(filename);
}

void Image::setRange(dvec2 newRange)
{
    for(uint i = 0; i < _totalSize; i++)
    {
        _data[i] = newRange.x + (newRange.y-newRange.x) * (_data[i] - _range.x)/(_range.y-_range.x);
    }

    _range = newRange;
}

void Image::save(std::string prefix)
{
    std::vector<stbi_uc> tmpData(_totalSize*3);

    setRange(dvec2(0, 255));

    for(uint i = 0; i < _totalSize*3; i+=3)
    {
        ivec3 out = _data[i/3];
        tmpData[i+0] = out.r;
        tmpData[i+1] = out.g;
        tmpData[i+2] = out.b;
    }

    std::string filename = "../out/" + prefix + getNameOnlyFromPath(_originalFilename.c_str()) + ".png";
    stbi_write_png(filename.c_str(), _size.x, _size.y, 3, tmpData.data(), 0);

    std::cout << TERMINAL_TIMER << "Saved image : " << TERMINAL_FILENAME << filename << TERMINAL_RESET << "\n";
}

dvec3 & Image::operator()(int x, int y)
{
    return _data[x*_size.y + y];
}