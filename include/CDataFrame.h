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
    //void fillDataFrame();

    void fillDataFrame(const std::vector<std::vector<ColumnValue>>& data);
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
    bool insertRow(const std::vector<ColumnValue>& values);
    /**
     *Delete a row by index
     * @param rowIndex : index of the row to delete
     */
    void deleteRow(size_t rowIndex);

    /**
      * @brief Add a column with a specific type
      * @param columnTitle Title
      * @param type The type of data (INT, FLOAT, STRING...)
      */
    void addColumn(std::string columnTitle, ColumnType type);

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
    bool valueExists(const ColumnValue& value) const;
    /**
     *Get a value from the dataframe by indexes
     * @param rowIndex : row index of the value to get
     * @param colIndex : column index of the value to get
     * @return : value at the given indexes
     */
    const ColumnValue& getValue(size_t rowIndex, size_t colIndex) const;
    /**
     *Replace value in the dataframe, find the value by indexes
     * @param rowIndex : row index of the value to replace
     * @param colIndex : column index of the value to replace
     * @param newValue : new value to set
     */
    void replaceValue(size_t rowIndex, size_t colIndex, const ColumnValue& newValue);
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
    size_t countValuesEqualTo(const ColumnValue& value) const;

    /**
     *Count the number of cells greater than a given value in the dataframe
     * @param value : value to compare
     * @return : number of cells greater than the given value
     */
    size_t countValuesGreaterThan(const ColumnValue& value) const;

    /**
     *Count the number of cells less than a given value in the dataframe
     * @param value : value to compare
     * @return : number of cells less than the given value
     */
    size_t countValuesLessThan(const ColumnValue& value) const;

    /**
    * @brief Display dataframe information
    */
    void info() const;

    /**
    * @brief Sort the entire dataframe based on one column
    * @param colName Name of the column to sort by
    * @param ascending Sort direction
    */
    void sort(const std::string& colName, bool ascending = true);
};


#endif //PROJET_ALSI53_CDATAFRAME_CDATAFRAME_H
