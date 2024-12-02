#include <iostream>
#include <Utils.hpp>
#include <Image.hpp>
#include <externalLibs/colorm.hpp>
#include <externalLibs/PerlinNoise.hpp>
#include <string>

#include <ColorSpace.hpp>


int main(int argc, char *argv[])
{
    ARGCCHECK(1, "[image]")

    Image in(argv[1]);

    // i.setRange(dvec2(0, 1));
    // i.setRange(dvec2(-1, 1));
    // i.setRange(dvec2(1, -1));

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



    std::string blurPlacement = "../out/NAIVE_HEAVY_BLUR_[RGB]/"
        + getNameOnlyFromPath(argv[1]) + ".png";

    Image inBlur(blurPlacement.c_str());


    // {
    //     Image img = in;

    //     for(int i = 0; i < img.size().x; i++)
    //     for(int j = 0; j < img.size().y; j++)
    //     {
    //         dvec3 p1 = in(j, i);
    //         dvec3 p2 = in2(j, i);

    //         img(j, i) = mix(p1, p2, 0.25);
    //     }

    //     img.save("../out/test/");
    // }


	const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{ seed };

    /****** 
     * 
     * RESULT : 0.11
     * 
     * WITHIN THE MARGIN OF ERROR FOR PERFECTLY RANDOM
     * 
     * VISUAL RESULT : MEH BUT VERY RECOGNIZABLE
     * 
     * ******/
    // if(false)
    {
        Image img = in;
        Image img2 = in2;

        double r = 0;

        dvec3 rcol(0);

        for(int i = 0; i < img.size().x; i++)
        for(int j = 0; j < img.size().y; j++)
        {
            dvec3 p1 = img(j, i);
            dvec3 &o = img(j, i);

            float d1 = (length(inBlur(j, i)-in(j, i)) )/255.;

            r = 1. - pow(d1, 0.5);

            rcol = dvec3(rand()%255, rand()%255, rand()%255);
            o = mix(p1, rcol, 0.25*r*dvec3(1, 1, 1));
        }

        img.save("../out/FREQUENCY_ATTACK/");
    }

    // if(false)
    {
        Image img = in;
        Image img2 = in2;

        Image imgHSL = in;
        Image imgHSL2 = in2;
        Image imgBlurHSL = inBlur; 
        Image imgBlurHSL2 = inBlur2; 

        ColorSpaces::HSL::apply(imgHSL);
        ColorSpaces::HSL::apply(imgHSL2);
        ColorSpaces::HSL::apply(imgBlurHSL);
        ColorSpaces::HSL::apply(imgBlurHSL2);


        for(int i = 0; i < img.size().x; i++)
        for(int j = 0; j < img.size().y; j++)
        {
            dvec3 &o = img(j, i);

            dvec3 p1 = in(j, i);
            dvec3 b1 = inBlur(j, i);

            dvec3 p2 = in2(j, i);
            dvec3 b2 = inBlur2(j, i);

            dvec3 p1HSL = imgHSL(j, i);
            dvec3 b1HSL = imgBlurHSL(j, i);

            dvec3 p2HSL = imgHSL2(j, i);



            float freq1 = clamp(length(p1-b1)/255., 0., 1.);
            float freq2 = clamp(length(p2-b2)/255., 0., 1.);

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
        img.save("../out/HUE_SATURATION_ATTACK/");
    }


    // if(false)
    {
        Image img = in;
        Image img2 = in2;

        Image imgHSL = in;
        Image imgHSL2 = in2;
        Image imgBlurHSL = inBlur; 
        Image imgBlurHSL2 = inBlur2; 

        ColorSpaces::HSL::apply(imgHSL);
        ColorSpaces::HSL::apply(imgHSL2);
        ColorSpaces::HSL::apply(imgBlurHSL);
        ColorSpaces::HSL::apply(imgBlurHSL2);

        for(int i = 0; i < img.size().x; i++)
        for(int j = 0; j < img.size().y; j++)
        {
            dvec3 &o = img(j, i);

            dvec3 p1 = in(j, i);
            dvec3 b1 = inBlur(j, i);

            dvec3 p2 = in2(j, i);
            dvec3 b2 = inBlur2(j, i);

            dvec3 p1HSL = imgHSL(j, i);
            dvec3 b1HSL = imgBlurHSL(j, i);

            dvec3 p2HSL = imgHSL2(j, i);
            dvec3 b2HSL = imgBlurHSL2(j, i);

            float freq1 = clamp(length(p1-b1)/255., 0., 1.);
            float freq2 = clamp(length(p2-b2)/255., 0., 1.);

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
        img.save("../out/PERLIN_HUE_SATURATION_ATTACK/");
    }


    /*
        15%
    */
//    if(false)
    {
        Image img = in;
        Image img2 = in2;

        Image imgHSL = in;
        Image imgHSL2 = in2;
        Image imgBlurHSL = inBlur; 
        Image imgBlurHSL2 = inBlur2; 

        ColorSpaces::HSL::apply(imgHSL);
        ColorSpaces::HSL::apply(imgHSL2);
        ColorSpaces::HSL::apply(imgBlurHSL);
        ColorSpaces::HSL::apply(imgBlurHSL2);

        for(int i = 0; i < img.size().x; i++)
        for(int j = 0; j < img.size().y; j++)
        {
            dvec3 &o = img(j, i);

            dvec3 p1 = in(j, i);
            dvec3 b1 = inBlur(j, i);

            dvec3 p2 = in2(j, i);
            dvec3 b2 = inBlur2(j, i);

            dvec3 p1HSL = imgHSL(j, i);
            dvec3 b1HSL = imgBlurHSL(j, i);

            dvec3 p2HSL = imgHSL2(j, i);
            dvec3 b2HSL = imgBlurHSL2(j, i);

            float freq1 = clamp(length(p1-b1)/255., 0., 1.);
            float freq2 = clamp(length(p2-b2)/255., 0., 1.);

            double scale = 1e-1;
            double scale2 = 5e-3;
            double r = perlin.octave2D_01(i*scale, j*scale, 4);

            dvec3 rcol = dvec3(
                perlin.octave2D_01(-i*scale2, j*scale2, 4), 
                perlin.octave2D_01(i*scale2, -j*scale2, 4), 
                perlin.octave2D_01(-i*scale2, -j*scale2, 4));

            double noiseAlpha = 1. - pow(freq1, 0.5);
            p1 = mix(p1, dvec3(rand()%255, rand()%255, rand()%255), 1*noiseAlpha);
            o = mix(p1, rcol*255.,
                clamp(
                    pow(r, 0.5)*dvec3(1, 0.5, 1) 
                    , dvec3(0), dvec3(1)
                )
             );
            
        }

        // ColorSpaces::HSL::inverse(img);
        img.save("../out/NON_LINEAR_RGB_PERLIN_ATTACK/");
    }

    if(false)
    {
        Image img = in;
        Image img2 = in2;

        Image imgHSL = in;
        Image imgHSL2 = in2;
        Image imgBlurHSL = inBlur; 
        Image imgBlurHSL2 = inBlur2; 

        ColorSpaces::HSL::apply(imgHSL);
        ColorSpaces::HSL::apply(imgHSL2);
        ColorSpaces::HSL::apply(imgBlurHSL);
        ColorSpaces::HSL::apply(imgBlurHSL2);

        for(int i = 0; i < img.size().x; i++)
        for(int j = 0; j < img.size().y; j++)
        {
            dvec3 &o = img(j, i);

            dvec3 p1 = in(j, i);
            dvec3 b1 = inBlur(j, i);

            dvec3 p2 = in2(j, i);
            dvec3 b2 = inBlur2(j, i);

            dvec3 p1HSL = imgHSL(j, i);
            dvec3 b1HSL = imgBlurHSL(j, i);

            dvec3 p2HSL = imgHSL2(j, i);
            dvec3 b2HSL = imgBlurHSL2(j, i);

            float freq1 = clamp(length(p1-b1)/255., 0., 1.);
            float freq2 = clamp(length(p2-b2)/255., 0., 1.);

            double scale = 1e-1;
            double scale2 = 5e-3;
            double r = perlin.octave2D_01(i*scale, j*scale, 4);

            dvec3 rcol = dvec3(
                perlin.octave2D_01(-i*scale2, j*scale2, 4), 
                perlin.octave2D_01(i*scale2, -j*scale2, 4), 
                perlin.octave2D_01(-i*scale2, -j*scale2, 4));

            double noiseAlpha = 1. - pow(freq1, 0.5);
            p1 = mix(p1, dvec3(rand()%255, rand()%255, rand()%255), 1*noiseAlpha);

            o = p1;
            o.r = p2.r;
            o.b = p2.b;
        }

        // ColorSpaces::HSL::inverse(img);
        img.save("../out/test/");
    }

    // {
    //     Image img = in;
    //     Image img2 = in2;

    //     double r = 0;

    //     dvec3 rcol(0);

    //     // ColorSpaces::HSL::apply(img);
    //     // ColorSpaces::HSL::apply(img2);

    //     for(int i = 0; i < img.size().x; i++)
    //     for(int j = 0; j < img.size().y; j++)
    //     {
    //         dvec3 p1 = img(j, i);
    //         dvec3 p2 = img2(j, i);
    //         dvec3 &o = img(j, i);

    //         dvec3 lp1 = in(j, i-1);

    //         // if(i%2 == 0 && j == 0)
    //         //     r = rand()%255;

    //         float d1 = (
    //             + length(inBlur(j, i)-in(j, i)) 
    //             + length(inBlur(j, i)-in(j, i-1)) 
    //             + length(inBlur(j, i)-in(j-1, i)) 
    //             + length(inBlur(j, i)-in(j, i-1)) 
    //             + length(inBlur(j, i)-in(j, i+1)) 
    //             + length(inBlur(j, i)-in(j+1, i)) 
    //             + length(inBlur(j, i)-in(j+1, i+1)) 
    //         )/(7.* 441.67295593) 
    //         ;

    //         // o = dvec3(d1)*255.;

    //         // continue;


    //         r = perlin.octave2D_01(i*1e-1, j*1e-1, 4);


    //         // r *= pow(d1, 0.5);

    //         r = 1. - pow(d1, 0.5);

    //         // std::cout << r << "\n";

    //         // r /= length(p1-lp1);

    //         double a = r;

    //         // a = pow(a, 5.);
    //         a = pow(a, 2.0);
    //         // a = smoothstep(0., 1., a);
    //         // a = clamp(a*5., 0.0, 1.0);

    //         o = mix(p1, p2, a*dvec3(1, 1, 1));

    //         if(true)
    //         {
    //             rcol = dvec3(rand()%255, rand()%255, rand()%255);
    //         }
    //         o = mix(p1, rcol, 0.75*a*dvec3(1, 1, 1));

    //         // o = dvec3(r*255.);

    //         // o = mix(o, p2, 0.1);

    //         // o = dvec3(d1*255.f);

    //         // o.g = p1.g;
    //     }

        // ColorSpaces::HSL::inverse(img);
        // ColorSpaces::HSL::inverse(img2);
    // }

    // {

    //     Image img = in;
    //     Image img2 = in2;

    //     ColorSpaces::HSL::apply(img);
    //     ColorSpaces::HSL::apply(img2);

    //     for(int i = 0; i < img.size().x; i++)
    //     for(int j = 0; j < img.size().y; j++)
    //     {
    //         dvec3 p1 = img(j, i);
    //         dvec3 p2 = img2(j, i);
    //         dvec3 &o = img(j, i);

    //         // o = p1-p2;

    //         o *= dvec3(0, 0, 1);
    //         o += dvec3(0, 0, 0);

    //         o.r = p2.r;
    //         o.g = p2.g;

    //         // double cut = 16;

    //         // p1 -= mod(p1, cut);
    //         // // p1 += mod(p2, 64.);
    //         // p1 += p2/(255./cut);

    //         // img(j, i) = p1;

    //         // o = mix(p1, p2, 0.25);

    //         // o = mix(p1, p2, 
    //         //     dvec3(1, 0.25, 1) * 
    //         //     (double)(rand()%128)/128.);

    //         // o = p1;
    //         // o.r = p2.r;
    //         // o.g = p2.g;

    //         // img(j, i) = o;

    //         // if(rand()%4 == 0)
    //         // {
    //         //     o = p2; 
    //         // }
    //         // else
    //         // {
    //         //     o = p1;
    //         // }
    //     }

    //     ColorSpaces::HSL::inverse(img);
    //     ColorSpaces::HSL::inverse(img2);

    //     img.save("../out/test/");
    // }


    return EXIT_SUCCESS;
}