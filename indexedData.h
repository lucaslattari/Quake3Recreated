#pragma once

#include <fstream>
#include <vector>
#include <iostream>

class IndexedData {
private:
    std::vector<int> values;

public:
    // Default constructor
    IndexedData() = default;

    // Constructor with parameters
    IndexedData(std::ifstream& file, int lumpOffset, int lumpLength) {
        load(file, lumpOffset, lumpLength);
    }

    // Destructor
    ~IndexedData() = default;

    // Getter methods
    const std::vector<int>& getValues() const { return values; }
    std::vector<int>& getValues() { return values; }

    // Setter methods
    void setValues(const std::vector<int>& newValues) { values = newValues; }

    friend std::ostream& operator<<(std::ostream& os, const IndexedData& indexedData);

    // Load method
    void load(std::ifstream& file, int lumpLength, int lumpOffset) {

        // Seek to the position in the file that stores the index information
        file.seekg(lumpOffset, std::ios::beg);

        // Calculate the number of indices from lump data
        int numberOfIndices = lumpLength / sizeof(int);

        values.resize(numberOfIndices);

        // Read in all the index information
        file.read(reinterpret_cast<char*>(values.data()), lumpLength);

        //std::cout << values.size() << std::endl;
        //std::cin.get();
    }

    void displayData() const {
        int i = 0;
        std::cout << "Indexed Data: [ ";
        for (const auto& value : values) {
            std::cout << i << ": " << value << " ";
            i++;
        }
        std::cout << "]" << std::endl;
    }
};