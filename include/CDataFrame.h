//
// Created by victor on 09/01/2026.
//

#ifndef PROJET_ALSI53_CDATAFRAME_CDATAFRAME_H
#define PROJET_ALSI53_CDATAFRAME_CDATAFRAME_H
#include "../Column.h"
#include <vector>
#include <memory>

class CDataFrame {
    std::vector<std::shared_ptr<Column>> columns;

public:
    /**
    * Create empty Dataframe
    */
    CDataFrame() = default;

    /**
    * Create Dataframe with given column names
    * @param columnNames
    */
    void addColumn(std::string columnTitle);

    /**
    * @brief Delete a column by name
    * @param colName : Column name
    */
    void deleteColumn(std::string columnTitle);

    /**
    * @brief Set column names
    * @param names : Vector of column names
    */
    void setColumnNames(const std::vector<std::string>& names);

    /**
    * @brief Get the number of columns
    * @return : Number of columns
    */
    size_t getColumnsCount() const;

    /**
    * @brief Insert a new row
    * @param values : Vector of values to insert
    * @return : true if successful
    */
    bool insertRow(const std::vector<int>& values);

    /**
    * @brief Get the number of rows
    * @return : Number of rows
    */
    size_t getRowsCount() const;

    /**
    * @brief Display dataframe information
    */
    void info() const;


};


#endif //PROJET_ALSI53_CDATAFRAME_CDATAFRAME_H