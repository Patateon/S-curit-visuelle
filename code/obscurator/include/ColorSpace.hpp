#pragma once

#include <Image.hpp>
#include <functional>


#define ColorSpace(name, transform, inverseTransform) \
    namespace name{ \
        void apply(Image &img){for(auto &o : img.data()) {dvec3 i = o; transform}}; \
        void inverse(Image &img){for(auto &o : img.data()) {dvec3 i = o; inverseTransform}}; \
    }

namespace ColorSpaces 
{
    ColorSpace(
        YCrCb,
        {
            o.r = 0.2990*i.r + 0.5870*i.g + 0.1440*i.b;
            o.g = 0.5000*i.b - 0.1687*i.r - 0.3313*i.g;
            o.b = 0.5000*i.r - 0.4187*i.g - 0.0813*i.b;
        },
        {
            o.r = i.r + 1.402*i.b;
            o.g = i.r - 0.34414*i.g - 0.71414*i.b;
            o.b = i.r + 1.772*i.g;
        }
    )


}

