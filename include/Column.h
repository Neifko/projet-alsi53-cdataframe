#ifndef PROJET_ALSI53_CDATAFRAME_COLUMN_H
#define PROJET_ALSI53_CDATAFRAME_COLUMN_H
#include <string>
#include <vector>

#include "ColumnValue.h"

class Column {
private:
    std::string title;
    ColumnType columnType;
    std::vector<ColumnValue> data;
    std::vector<size_t> index;
    bool validIndex;
    bool sortAscending;

public :

    /**
    * Create a column with a given  type
    * @param type : type of the column
    */
    Column(ColumnType type);

    /**
    * Create a column with a given title and type
    * @param type : type of the column
    * @param columnTitle : title of the column
    */
    Column(ColumnType type, const std::string &columnTitle);

    /**
    * Add a new value to the column
    * @param value : value to add
    * @return : true if the value was added, false otherwise
    */
    bool insertValue(const ColumnValue& value);

    /**
    * Print a column content show the index and the value
    */
    void print() const;

    /**
     * Return the number of occurrences of a value in the column
     * @param value : value to search
     * @return : number of occurrences
     */
    int valueCount(const ColumnValue& value) const;

    /**
     * Return the value at a given index
     * @param index : index of the value
     * @return : value at the index
     */
    const ColumnValue& getValueAt(int index) const;

    /**
     *Return the number of value superior to a given value
     * @param value : value to compare
     * @return : number of value superior to the given value
     */
    int countValuesGreaterThan(const ColumnValue& value) const;

    /**
     *Return the number of value inferior to a given value
     * @param value : value to compare
     * @return : number of value inferior to the given value
     */
    int countValuesLessThan(const ColumnValue& value) const;

    /**
     *Return the number of values equal to a given value
     * @param value : value to compare
     * @return : number of values equal to the given value
     */
    int countValuesEqualTo(const ColumnValue& value) const;

    /**
     * Get the name of the column
     * @return : The column title
     */
    std::string getName() const;

    /**
     * Get the current size of the column
     * @return : size of the column
     */
    size_t getSize() const;

    /**
     * @brief Rename the column
     * @param newTitle : The new title
     */
    void setName(const std::string& newTitle);

    /**
     * @brief Replace value at a specific index
     * @param index : index to replace
     * @param newValue : the new value
     */
    void setValueAt(int index, const ColumnValue& newValue);

    /**
     * @brief Delete value at a specific index
     * @param index : index to delete
     */
    void deleteValueAt(size_t index);

    /**
    * Destructor : free the memory allocated for the column
    */
    ~Column() = default;
};

#endif //PROJET_ALSI53_CDATAFRAME_COLUMN_H
