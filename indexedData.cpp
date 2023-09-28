#include "indexedData.h"

// Defina a fun��o de operador de sa�da aqui
std::ostream& operator<<(std::ostream& os, const IndexedData& indexedData) {
    const std::vector<int>& values = indexedData.getValues();

    os << "IndexedData Values:\n";

    for (int i = 0; i < values.size(); ++i) {
        os << "Index " << i << ": " << values[i] << "\n";
    }

    return os;
}
