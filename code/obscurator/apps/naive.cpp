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
        img.save("../out/NAIVE_LINE_NOISE/");
    }

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

        img.save("../out/NAIVE_WAVE_DIVIDE/");
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