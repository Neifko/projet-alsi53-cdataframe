//
// Created by victor on 09/01/2026.
//

#ifndef PROJET_ALSI53_CDATAFRAME_CDATAFRAME_H
#define PROJET_ALSI53_CDATAFRAME_CDATAFRAME_H
#include "Column.h"
#include <vector>
#include <memory>

class CDataFrame {
    std::vector<std::shared_ptr<Column> > columns;

public:
    /**
    * Create empty Dataframe
    */
    CDataFrame() = default;

    /**
     *Fill the dataframe with user typed data
     */
    void fillDataFrame();

    /**
     *Fill the dataframe with hardcoded data in array of arrays
     *like {{1,2,3},{4,5,6},{7,8,9}}
     * @param data : array of arrays of integers
     */
    void fillDataFrame(const std::vector<std::vector<int> > &data);

    /**
    * @brief Display the entire dataframe content
    */
    void print() const;

    /**
     *Display a certain number of rows of the dataframe between startRow and endRow
     * @param startRow : starting row index
     * @param endRow : ending row index
     */
    void printRows(size_t startRow, size_t endRow) const;

    /**
     *Display a certain number of columns of the dataframe between startCol and endCol
     * @param startCol : starting column index
     * @param endCol : ending column index
     */
    void printColumns(size_t startCol, size_t endCol) const;

    /**
    * @brief Insert a new row
    * @param values : Vector of values to insert
    * @return : true if successful
    */
    bool insertRow(const std::vector<int> &values);

    /**
     *Delete a row by index
     * @param rowIndex : index of the row to delete
     */
    void deleteRow(size_t rowIndex);

    /**
    * Create Dataframe with given column names
    * @param columnTitle
    */
    void addColumn(std::string columnTitle);

    /**
    * @brief Delete a column by name
    * @param columnTitle : Column name
    */
    void deleteColumn(std::string columnTitle);

    /**
     *Rename the title of a column
     * @param oldName : current name of the column
     * @param newName : new name of the column
     */
    void renameColumn(const std::string &oldName, const std::string &newName);

    /**
     *Check if a value exists in the dataframe
     * @param value : value to search
     * @return : true if the value exists, false otherwise
     */
    bool valueExists(int value) const;

    /**
     *Get a value from the dataframe by indexes
     * @param rowIndex : row index of the value to get
     * @param colIndex : column index of the value to get
     * @return : value at the given indexes
     */
    int getValue(size_t rowIndex, size_t colIndex) const;

    /**
     *Replace value in the dataframe, find the value by indexes
     * @param rowIndex : row index of the value to replace
     * @param colIndex : column index of the value to replace
     * @param newValue : new value to set
     */
    void replaceValue(size_t rowIndex, size_t colIndex, int newValue);

    /**
    * @brief Get the number of columns
    * @return : Number of columns
    */
    size_t getColumnsCount() const;

    /**
    * @brief Get the number of rows
    * @return : Number of rows
    */
    size_t getRowsCount() const;

    /**
     *Count the number of cells equals to a given value in the dataframe
     * @param value : value to count
     * @return : number of cells equals to the given value
     */
    size_t countValuesEqualTo(int value) const;

    /**
     *Count the number of cells greater than a given value in the dataframe
     * @param value : value to compare
     * @return : number of cells greater than the given value
     */
    size_t countValuesGreaterThan(int value) const;

    /**
     *Count the number of cells less than a given value in the dataframe
     * @param value : value to compare
     * @return : number of cells less than the given value
     */
    size_t countValuesLessThan(int value) const;

    /**
    * @brief Display dataframe information
    */
    void info() const;

    /**
    * @brief Set column names
    * @param names : Vector of column names
    * @deprecated should not exist
    */
    void setColumnNames(const std::vector<std::string> &names);
};


#endif //PROJET_ALSI53_CDATAFRAME_CDATAFRAME_H
