//
// Created by victor on 09/01/2026.
//

#include "Column.h"

#include <iostream>

Column::Column(const std::string &columnTitle)
    : title(columnTitle) {
    data.reserve(256);
}

bool Column::insertValue(int value) {
    data.push_back(value);
    return true;
}

void Column::print() const {
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << "[" << i << "] " << data[i] << std::endl;
    }
}

int Column::valueCount(int value) const {
    int count = 0;
    for (int v : data) {
        if (v == value) {
            count++;
        }
    }
    return count;
}

int Column::getValueAt(int index) const {
    if (index < 0 || index >= static_cast<int>(data.size())) {
        throw std::out_of_range("Index out of range in Column::getValueAt");
    }
    return data[index];
}

int Column::countValuesGreaterThan(int value) const {
    int count = 0;
    for (int v : data) {
        if (v > value) {
            count++;
        }
    }
    return count;
}

int Column::countValuesLessThan(int value) const {
    int count = 0;
    for (int v : data) {
        if (v < value) {
            count++;
        }
    }
    return count;
}

int Column::countValuesEqualTo(int value) const {
    return valueCount(value);
}
