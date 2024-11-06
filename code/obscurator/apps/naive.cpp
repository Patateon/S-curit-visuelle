#include <iostream>
#include <Utils.hpp>
#include <Image.hpp>
#include <externalLibs/colorm.hpp>

#include <ColorSpace.hpp>

int main(int argc, char *argv[])
{
    ARGCCHECK(1, "[image]")

    Image in(argv[1]);

    // i.setRange(dvec2(0, 1));
    // i.setRange(dvec2(-1, 1));
    // i.setRange(dvec2(1, -1));

    {
        Image img = in;

        for(auto &p : img.data())
            p -= mod(p, dvec3(100));

        // system("mkdir ../out/NAIVE_LPB_CUT_[RGB]");
        img.save("../out/NAIVE_LPB_CUT_[RGB]/");
    }
    {
        Image img = in;

        ColorSpaces::YCrCb::apply(img);

        for(auto &p : img.data())
            p -= mod(p, dvec3(100));

        ColorSpaces::YCrCb::inverse(img);

        // system("mkdir ../out/NAIVE_LPB_CUT_[YCrCb]");
        img.save("../out/NAIVE_LPB_CUT_[YCrCb]/");
    }

    {
        Image img = in;

        // ColorSpaces::sRGB::apply(img);

        for(auto &p : img.data())
        {
            int rmod = 64;
            int gmod = 8;
            int bmod = 32;

            p.r -= mod(p.r, (double)rmod);
            p.g -= mod(p.g, (double)gmod);
            p.b -= mod(p.b, (double)bmod);

            p.r += rand()%rmod;
            p.g += rand()%gmod;
            p.b += rand()%bmod;
        }

        // ColorSpaces::sRGB::inverse(img);

        // system("mkdir ../out/NAIVE_LPB_CUT_[YCrCb]");
        img.save("../out/test/");
    }

    {
        Image img = in;

        const int kernelSize = img.size().x/32;

        for(int i = 0; i < img.size().x; i++)
        for(int j = 0; j < img.size().y; j++)
        {
            
            ivec2 beg = max(ivec2(0)  , ivec2(i, j) - kernelSize);
            ivec2 end = min(img.size(), ivec2(i, j) + kernelSize);

            dvec3 sum(0);

            for(int k = beg.x; k < end.x; k++)
            for(int l = beg.y; l < end.y; l++)
                if((k+j)%4 == 1)
                    sum += in(l, k);
            
            sum /= 0.25*(float)((end.x-beg.x)*(end.y-beg.y));

            img(j, i) = sum;
        }

        // system("mkdir ../out/NAIVE_HEAVY_BLUR_[RGB]");
        img.save("../out/NAIVE_HEAVY_BLUR_[RGB]/");
    }


    return EXIT_SUCCESS;
}