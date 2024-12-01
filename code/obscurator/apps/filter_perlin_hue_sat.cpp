#include <iostream>
#include <Utils.hpp>
#include <Image.hpp>
#include <externalLibs/colorm.hpp>
#include <externalLibs/PerlinNoise.hpp>
#include <string>

#include <ColorSpace.hpp>


int main(int argc, char *argv[])
{
    ARGCCHECK(2, "[image_input image_output]")

    Image in(argv[1]);

    Image inBlur;

	const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{ seed };

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
        // img.save("result__");

        inBlur = img;
    }

    {
        Image img = in;

        Image imgHSL = in;
        Image imgBlurHSL = inBlur; 

        ColorSpaces::HSL::apply(imgHSL);
        ColorSpaces::HSL::apply(imgBlurHSL);

        for(int i = 0; i < img.size().x; i++)
        for(int j = 0; j < img.size().y; j++)
        {
            dvec3 &o = img(j, i);

            dvec3 p1 = in(j, i);
            dvec3 b1 = inBlur(j, i);

            dvec3 p1HSL = imgHSL(j, i);
            dvec3 b1HSL = imgBlurHSL(j, i);

            float freq1 = clamp(length(p1-b1)/255., 0., 1.);

            double scale = 1e-2;
            double scale2 = 5e-3;
            double r = perlin.octave2D_01(i*scale, j*scale, 4);

            dvec3 rcol = dvec3(
                perlin.octave2D_01(-i*scale2, j*scale2, 4), 
                perlin.octave2D_01(i*scale2, -j*scale2, 4), 
                perlin.octave2D_01(-i*scale2, -j*scale2, 4));

            double noiseAlpha = 1. - pow(freq1, 0.5);
            p1HSL = mix(p1HSL, dvec3(rand()%360, rand()%100, rand()%100), 0.8*noiseAlpha);

            o = mix(p1HSL, rcol*dvec3(360*5, 100, 100), 0.5*pow(r, 0.5)*dvec3(1, 1, 0.25));
            // o = rcol*dvec3(360*5, 100, 100);
            // o = 128.*pow(r, 0.5)*dvec3(1, 1, 0.25);
        }

        ColorSpaces::HSL::inverse(img);
        img.save(argv[2]);
    }

    return EXIT_SUCCESS;
}