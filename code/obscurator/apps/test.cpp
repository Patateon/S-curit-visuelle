#include <iostream>
#include <Utils.hpp>
#include <Image.hpp>

int main(int argc, char *argv[])
{
    ARGCCHECK(1, "[image]")

    Image in(argv[1]);

    in.setRange(dvec2(0, 1));
    in.setRange(dvec2(-1, 1));
    in.setRange(dvec2(1, -1));

    Image out = in;

    for(int i = 0; i < out.size().x; i++)
    for(int j = 0; j < out.size().y; j++)
    {
        dvec3 &o = out(j, i);

    }

    out.save("../images/");

    // i.save("TEST_");

    return EXIT_SUCCESS;
}