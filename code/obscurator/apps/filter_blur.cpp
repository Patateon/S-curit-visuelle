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
        img.save(argv[2]);
    }


    return EXIT_SUCCESS;
}