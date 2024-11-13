#include <iostream>
#include <Utils.hpp>
#include <Image.hpp>
#include <externalLibs/colorm.hpp>
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
    char number = tmpname.back();
    tmpname.back() = '\0';

    std::string replacement;

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

    replacement = "../images/10_TEST/" + replacement + number + ".png";

    Image in2(replacement.c_str());

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

    {
        Image img = in;

        for(int i = 0; i < img.size().x; i++)
        for(int j = 0; j < img.size().y; j++)
        {
            dvec3 p1 = in(j, i);
            dvec3 p2 = in2(j, i);

            double cut = 16;

            p1 -= mod(p1, cut);
            // p1 += mod(p2, 64.);
            p1 += p2/(255./cut);

            img(j, i) = p1;

            // img(j, i) = mix(p1, p2, 0.25);
        }

        img.save("../out/test/");
    }


    return EXIT_SUCCESS;
}