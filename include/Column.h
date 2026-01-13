//
// Created by victor on 09/01/2026.
//

#ifndef PROJET_ALSI53_CDATAFRAME_COLUMN_H
#define PROJET_ALSI53_CDATAFRAME_COLUMN_H
#include <string>
#include <vector>


class Column {
private:
    std::string title;
    std::vector<int> data;

public :
    /**
    * Create a column
    * @param columnTitle : title of the column
    */
    Column(const std::string &columnTitle);

    /**
    * Add a new value to the column
    * @param value : value to add
    * @return : true if the value was added, false otherwise
    */
    bool insertValue(int value);

    /**
    * Print a column content show the index and the value
    */
    void print() const;

    /**
     * Return the number of occurrences of a value in the column
     * @param value : value to search
     * @return : number of occurrences
     */
    int valueCount(int value) const;

    /**
     * Return the value at a given index
     * @param index : index of the value
     * @return : value at the index
     */
    int getValueAt(int index) const;

    /**
     *Return the number of value superior to a given value
     * @param value : value to compare
     * @return : number of value superior to the given value
     */
    int countValuesGreaterThan(int value) const;

    /**
     *Return the number of value inferior to a given value
     * @param value : value to compare
     * @return : number of value inferior to the given value
     */
    int countValuesLessThan(int value) const;

    /**
     *Return the number of values equal to a given value
     * @param value : value to compare
     * @return : number of values equal to the given value
     */
    int countValuesEqualTo(int value) const;

    /**
    * Destructor : free the memory allocated for the column
    */
    ~Column() = default;
};


#endif //PROJET_ALSI53_CDATAFRAME_COLUMN_H
