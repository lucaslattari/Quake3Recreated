#pragma once

#include "BSPElement.h"

namespace BSP {
    class Lightmap {
    public:
        Lightmap() {};
        ~Lightmap() {};

        // The RGB data in a 128x128 image
        Vec3ui imageBits[128][128];
    };

    class Lightmaps : public BSP::Element<Lightmap> {
    public:
        void validate() override;
        void displayData() const override;
    };
}