#include <iostream>
#include <Utils.hpp>
#include <Image.hpp>

int main(int argc, char *argv[])
{
    ARGCCHECK(1, "[image]")

    Image i(argv[1]);

    i.setRange(dvec2(0, 1));
    i.setRange(dvec2(-1, 1));
    i.setRange(dvec2(1, -1));

    i.save("TEST_");

    return EXIT_SUCCESS;
}