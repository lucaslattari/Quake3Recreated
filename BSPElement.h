#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <vector>

#include "vector.h"
#include "lumps.h"

namespace BSP {
    template <typename T>
    class Element {
    public:
        Element() = default;
        virtual ~Element() = default;

        virtual void load(std::ifstream& file, LumpData& lumpData) {
            int numberOfElements = static_cast<int>(std::ceil(static_cast<float>(lumpData.length) / sizeof(T)));
            elements.resize(numberOfElements);

            file.seekg(lumpData.offset, std::ios::beg);
            file.read(reinterpret_cast<char*>(elements.data()), lumpData.length);

            //validate();
        }

        virtual void displayData() const = 0;
        virtual void validate() = 0;

        size_t size() const { return elements.size(); }
        const std::vector<T>& getData() const { return elements; }
    protected:
        std::vector<T> elements;
    };
}