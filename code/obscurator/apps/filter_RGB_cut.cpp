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

        for(auto &p : img.data())
            p -= mod(p, dvec3(100));

        // system("mkdir ../out/NAIVE_LPB_CUT_[RGB]");
        img.save(argv[2]);
    }

    return EXIT_SUCCESS;
}