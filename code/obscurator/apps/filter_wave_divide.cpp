#include <iostream>
#include <Utils.hpp>
#include <Image.hpp>
#include <externalLibs/colorm.hpp>

#include <ColorSpace.hpp>

int main(int argc, char *argv[])
{
    ARGCCHECK(2, "[image_input image_output]")

    Image in(argv[1]);

    {
        Image img = in;

        img.setRange(dvec2(-1, 1));

        dvec3 tmp = dvec3(0);
        for(auto &p : img.data())
        {
            dvec3 tmptmp = p;
            p = tmp/p;
            tmp = tmptmp;
        }

        img.save(argv[2]);
    }

    return EXIT_SUCCESS;
}