#pragma once

#include "BSPElement.h"

namespace BSP {
    // BSP texture class
    class Texture {
    public:
        char name[64];
        int flags;
        int textureType;

        Texture() : name{}, flags(0), textureType(0) {}
        ~Texture() {}
    };

    // BSP texture class
    class Textures : public BSP::Element<Texture> {
    public:
        void validate() override;
        void displayData() const override;
    };
}