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

    std::string tmpname = getNameOnlyFromPath(argv[1]); 
    char number = glm::clamp(tmpname.back(), '0', '9');
    tmpname.back() = '\0';

    std::string replacement = "cat_";

    if(!strcmp(tmpname.c_str(), "airplaine_"))
        replacement = "dog_";
    else
    if(!strcmp(tmpname.c_str(), "automobile_"))
        replacement = "horse_";
    else
    if(!strcmp(tmpname.c_str(), "bird_"))
        replacement = "cat_";
    else
    if(!strcmp(tmpname.c_str(), "cat_"))
        replacement = "frog_";
    else
    if(!strcmp(tmpname.c_str(), "deer_"))
        replacement = "truck_";
    else
    if(!strcmp(tmpname.c_str(), "dog_"))
        replacement = "automobile_";
    else
    if(!strcmp(tmpname.c_str(), "frog_"))
        replacement = "ship_";
    else
    if(!strcmp(tmpname.c_str(), "horse_"))
        replacement = "bird_";
    else
    if(!strcmp(tmpname.c_str(), "ship_"))
        replacement = "deer_";
    else
    if(!strcmp(tmpname.c_str(), "truck_"))
        replacement = "airplaine_";

    std::string blurPlacement2 = "../out/NAIVE_HEAVY_BLUR_[RGB]/"
        + replacement + number + ".png";

    Image inBlur2(blurPlacement2.c_str());
    
    replacement = 
    "../images/10_TEST/" 
    // "../out/NAIVE_HEAVY_BLUR_[RGB]/"
    + replacement + number + ".png";

    Image in2(replacement.c_str());

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
        Image img2 = in2;

        Image imgHSL = in;
        Image imgHSL2 = in2;
        Image imgBlurHSL = inBlur; 

        ColorSpaces::HSL::apply(imgHSL);
        ColorSpaces::HSL::apply(imgHSL2);
        ColorSpaces::HSL::apply(imgBlurHSL);


        for(int i = 0; i < img.size().x; i++)
        for(int j = 0; j < img.size().y; j++)
        {
            dvec3 &o = img(j, i);

            dvec3 p1 = in(j, i);
            dvec3 b1 = inBlur(j, i);

            dvec3 p2 = in2(j, i);

            dvec3 p1HSL = imgHSL(j, i);
            dvec3 b1HSL = imgBlurHSL(j, i);

            dvec3 p2HSL = imgHSL2(j, i);



            float freq1 = clamp(length(p1-b1)/255., 0., 1.);

            // dvec3 rcol = dvec3(rand()%255, rand()%255, rand()%255);

            /**
             * Accuracy : 0.15
            double a = (1.- pow(freq2, 1.0)) * (1. - pow(freq1, 0.5));
            o = mix(p1, p2, a);
             */

            /**
             * Accuracy 0.06
            double noiseAlpha = 1. - pow(freq1, 0.5);
            p1 = mix(p1, rcol, noiseAlpha*0.25);

            if(rand()%4)
                o = mix(p1, b2, dvec3(1, 0.75, 1));
            else
                o = p1;
             */


            double noiseAlpha = 1. - pow(freq1, 0.5);
            // p1 = mix(p1, rcol, noiseAlpha*0.25);

            dvec3 rcol = dvec3(rand()%360, rand()%100, rand()%100);
            p1HSL = mix(p1HSL, rcol, noiseAlpha*0.4);

            // double noiseAlpha2 = 1. - pow(freq2, 0.5); 


            // o = p1HSL;
            // o.g = 0;

            o = p2HSL;
            o.b = p1HSL.b;

            // o = mix(p1HSL, p2HSL, 0.5);

            // o.b = 50.;
            // o.g = 50.;
        }

        ColorSpaces::HSL::inverse(img);
        img.save(argv[2]);
    }
    return EXIT_SUCCESS;
}