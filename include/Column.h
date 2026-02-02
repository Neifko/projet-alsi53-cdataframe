#ifndef PROJET_ALSI53_CDATAFRAME_COLUMN_H
#define PROJET_ALSI53_CDATAFRAME_COLUMN_H
#include <string>
#include <vector>
#include <algorithm>

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
     *Convert a column value to string
     * @param i : index of the value to retrieve
     * @return : string representation of the value
     */
    std::string valueToString(size_t i) const;

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

    /**
    * @brief: Sort a column according to a given order
    * @param ascending : true for ascending, false for descending
    */
    void sort(bool ascending = true);

    /**
    * @brief: Display the contents of a column in sorted order
    * @param ascending: true for ascending, false for descending
    */
    void printSorted(bool ascending = true);

    /**
    * @brief: Remove the index of a column
    */
    void eraseIndex();

    /**
    * @brief: Check if an index is correct
    * @return: -1: index not existing,
    0: the index exists but invalid,
    1: the index is correct
    */
    int checkIndex() const;

    /**
    * @brief: Update the index
    */
    void updateIndex();

    /**
    * @brief: Test if a value exists in a column
    * @param val: The value to search for
    * @return: -1: column not sorted,
    0: value not found
    1: value found
    */
    template<typename T>
    int searchValue(const T& val) const;
};

/**
* @brief: Test if a value exists in a column
* @param val: The value to search for
* @return: -1: column not sorted,
0: value not found
1: value found
*/
template<typename T>
int Column::searchValue(const T& val) const {
    if (!validIndex || index.empty()) {
        return -1;
    }
    auto it = std::lower_bound(index.begin(), index.end(), val,
        [this](size_t idx, const T& searchVal) {
            const auto& element = data[idx];
            return std::visit([&searchVal, this](auto&& arg) -> bool {
                using U = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<U, T>) {
                    return sortAscending ? (arg < searchVal) : (arg > searchVal);
                }
                return false;
            }, element);
        });
    if (it != index.end()) {
        const auto& candidate = data[*it];
        bool found = std::visit([&val](auto&& arg) -> bool {
            using U = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<U, T>) {
                return arg == val;
            }
            return false;
        }, candidate);

        if (found) {
            return 1;
        }
    }

    return 0;
}

#endif //PROJET_ALSI53_CDATAFRAME_COLUMN_H
