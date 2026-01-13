#include <iostream>

#include "Column.h"
#include "CDataFrame.h"

void test_column_class() {
    Column col("Test");

    col.insertValue(10);
    col.insertValue(20);
    col.insertValue(10);

    col.print();

    std::cout << "Occurrences de 10: " << col.valueCount(10) << std::endl;
    std::cout << "Superieurs a 15: " << col.countValuesGreaterThan(15) << std::endl;
    // int t = col.getValueAt(3); // to test out_of_range exception
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    test_column_class();

    CDataFrame myDataframe;

    myDataframe.addColumn("ID");
    myDataframe.addColumn("Age");
    myDataframe.addColumn("Score");

    myDataframe.insertRow({1, 20, 100});
    myDataframe.insertRow({2, 21, 200});
    myDataframe.insertRow({3, 22, 300});

    myDataframe.info();
    myDataframe.print();

    myDataframe.deleteColumn("Age");

    std::cout << "--------------------------------------" << std::endl;
    myDataframe.print();

    return 0;
}