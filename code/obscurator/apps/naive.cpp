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
            p -= mod(p, dvec3(32));

        img.save("NAIVE_LPB_CUT_[RGB]_");
    }
    {
        Image img = in;

        ColorSpaces::YCrCb::apply(img);

        for(auto &p : img.data())
            p -= mod(p, dvec3(32));

        ColorSpaces::YCrCb::inverse(img);

        img.save("NAIVE_LPB_CUT_[YCrCb]_");
    }

    {
        Image img = in;

        for(int i = 0; i < img.size().x; i++)
        for(int j = 0; j < img.size().y; j++)
        {
            ivec2 beg = max(ivec2(0)  , ivec2(i, j) - img.size()/32);
            ivec2 end = min(img.size(), ivec2(i, j) + img.size()/32);

            dvec3 sum(0);

            for(int k = beg.x; k < end.x; k++)
            for(int l = beg.y; l < end.y; l++)
                sum += img(k, j);
            
            sum /= (float)((end.x-beg.x)*(end.y-beg.y));

            img(i, j) = sum;
        }

        img.save("NAIVE_HEAVY_BLUR_[RGB]_");
    }

    return EXIT_SUCCESS;
}