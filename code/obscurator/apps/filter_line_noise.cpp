#include <iostream>
#include <Utils.hpp>
#include <Image.hpp>
#include <externalLibs/colorm.hpp>

#include <ColorSpace.hpp>

int main(int argc, char *argv[])
{
    ARGCCHECK(1, "[image]")

    Image in(argv[1]);

    {
        Image img = in;

        // ColorSpaces::YCrCb::apply(img);

        dvec3 r(0);
        int cnt = 0;
        for(auto &p : img.data())
        {
            // double rmod = min(p.r+1, 32.);
            // double gmod = min(p.g+1, 32.);
            // double bmod = min(p.b+1, 32.);

            // dvec3 tmp = p;

            // p.r -= mod(p.r, (double)max(rmod, 1e-4));
            // p.g -= mod(p.g, (double)max(gmod, 1e-4));
            // p.b -= mod(p.b, (double)max(bmod, 1e-4));

            // p.r += rand()%(int)(rmod);
            // p.g += rand()%(int)(gmod);
            // p.b += rand()%(int)(bmod);

            if((cnt++)%128 == 0)
                r = dvec3(rand()%256, rand()%256, rand()%256);

            // p = mix(p, p + (r - 128.), 0.9);

            p = p + (r - 128.);
        
            // p = mix(tmp, p, 0.5);

            p = clamp(p, dvec3(0), dvec3(255));
        }

        // ColorSpaces::YCrCb::inverse(img);

        // system("mkdir ../out/NAIVE_LPB_CUT_[YCrCb]");
        img.save("result__");
    }

    return EXIT_SUCCESS;
}