#ifndef PROJET_ALSI53_CDATAFRAME_COLUMN_H
#define PROJET_ALSI53_CDATAFRAME_COLUMN_H
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <stdexcept>

#include "ColumnValue.h"

/**
 * @brief Convert a ColumnType to its string representation
 * @param type The column type
 * @return String representation of the type
 */
std::string columnTypeToString(ColumnType type);

/**
 * @brief Compare two ColumnValue variants robustly
 * @param a First value
 * @param b Second value
 * @return -1 if a < b, 0 if a == b, 1 if a > b
 * @note monostate (NULL) is considered less than any other value
 * @note Numeric types are compared with implicit conversion
 * @note Strings are compared lexicographically only with strings
 * @note Mixed non-numeric types compare by variant index for stable ordering
 */
int compareValues(const ColumnValue& a, const ColumnValue& b);

/**
 * @brief Check if a ColumnValue matches the expected ColumnType
 * @param value The value to check
 * @param expectedType The expected column type
 * @return true if compatible, false otherwise
 */
bool isValueCompatibleWithType(const ColumnValue& value, ColumnType expectedType);

class Column {
private:
    std::string title;
    ColumnType columnType;
    std::vector<ColumnValue> data;
    std::vector<size_t> index;
    bool validIndex = false;      // Initialized
    bool sortAscending = true;    // Initialized

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
    bool insertValue(const ColumnValue &value);

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
    int valueCount(const ColumnValue &value) const;

    /**
     * Return the value at a given index
     * @param index : index of the value
     * @return : value at the index
     */
    const ColumnValue &getValueAt(int index) const;

    /**
     *Return the number of value superior to a given value
     * @param value : value to compare
     * @return : number of value superior to the given value
     */
    int countValuesGreaterThan(const ColumnValue &value) const;

    /**
     *Return the number of value inferior to a given value
     * @param value : value to compare
     * @return : number of value inferior to the given value
     */
    int countValuesLessThan(const ColumnValue &value) const;

    /**
     *Return the number of values equal to a given value
     * @param value : value to compare
     * @return : number of values equal to the given value
     */
    int countValuesEqualTo(const ColumnValue &value) const;

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
    void setName(const std::string &newTitle);

    /**
     * @brief Replace value at a specific index
     * @param index : index to replace
     * @param newValue : the new value
     */
    void setValueAt(int index, const ColumnValue &newValue);

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
    int searchValue(const ColumnValue &val) const;

    /**
    * @brief : Apply a function to all elements of a column
    * @param func : Function to apply (lambda or function pointer)
    * @return : Result of the operation
    */
    template<typename Func, typename ResultType>
    ResultType applyFunction(Func func) const;

    /**
    * @brief : Apply a function on 2 columns to create a 3rd
    * @param col1 : First column
    * @param col2 : Second column
    * @param func : Function to apply
    * @return : Pointer to the new column created
    */
    template<typename Func>
    static std::unique_ptr<Column> applyFunction(
        const Column &col1,
        const Column &col2,
        ColumnType resultType,
        Func func
    );

    /**
    * @brief Get the index vector
    */
    const std::vector<size_t>& getIndex() const;

    /**
    * @brief Set the index vector manually
    */
    void setIndex(const std::vector<size_t>& newIndex);

    /**
     * @brief Get the column type
     * @return The column type
     */
    ColumnType getType() const;
};

template<typename Func, typename ResultType>
ResultType Column::applyFunction(Func func) const {
    ResultType result{}; // valeur initiale par défaut

    for (const auto& v : data) {
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;

            // Ignorer les valeurs NULL
            if constexpr (std::is_same_v<T, std::monostate>) {
                return;
            }
            // Appliquer uniquement aux types arithmétiques
            else if constexpr (std::is_arithmetic_v<T>) {
                result = func(result, arg);
            }
        }, v);
    }

    return result;
}

template<typename Func>
std::unique_ptr<Column> Column::applyFunction(const Column &col1, const Column &col2, ColumnType resultType, Func func) {
    // Vérification de taille
    if (col1.getSize() != col2.getSize()) {
        return nullptr;
    }

    // Création de la colonne résultat
    auto result = std::make_unique<Column>(resultType, "result");

    for (size_t i = 0; i < col1.getSize(); ++i) {
        const auto &v1 = col1.getValueAt(i);
        const auto &v2 = col2.getValueAt(i);

        ColumnValue newValue = std::monostate{};

        std::visit([&](auto&& a, auto&& b) {
            using T1 = std::decay_t<decltype(a)>;
            using T2 = std::decay_t<decltype(b)>;

            // Si l'un des deux est NULL → NULL
            if constexpr (
                std::is_same_v<T1, std::monostate> ||
                std::is_same_v<T2, std::monostate>
            ) {
                newValue = std::monostate{};
            }
            // Si les deux sont arithmétiques
            else if constexpr (
                std::is_arithmetic_v<T1> &&
                std::is_arithmetic_v<T2>
            ) {
                newValue = func(a, b);
            }
        }, v1, v2);

        result->insertValue(newValue);
    }

    return result;
}

#endif //PROJET_ALSI53_CDATAFRAME_COLUMN_H
