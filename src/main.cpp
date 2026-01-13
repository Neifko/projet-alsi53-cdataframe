#include <iostream>

#include "Column.h"

void test_column_class() {
    Column col("Test");

    col.insertValue(10);
    col.insertValue(20);
    col.insertValue(10);

    col.print();

    std::cout << "Occurrences de 10: " << col.valueCount(10) << std::endl;
    std::cout << "Superieurs a 15: " << col.countValuesGreaterThan(15) << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    test_column_class();

    return 0;
}