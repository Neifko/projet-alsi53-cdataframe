//
// Created by victor on 09/01/2026.
//

#include "CDataFrame.h"
#include <iostream>
#include <algorithm>

void CDataFrame::addColumn(std::string columnTitle) {
    columns.push_back(std::make_shared<Column>(columnTitle));
}

void CDataFrame::deleteColumn(std::string columnTitle) {
    // Find and remove the column with the given title
    auto it = std::remove_if(columns.begin(), columns.end(),
                             [&columnTitle](const std::shared_ptr<Column>& col) {
                                 return col->getName() == columnTitle;
                             });

    if (it != columns.end()) {
        columns.erase(it, columns.end());
    }
}

void CDataFrame::setColumnNames(const std::vector<std::string> &names) {
    // Clear existing columns and add new ones
    columns.clear();
    for (const auto& name : names) {
        addColumn(name);
    }
}

size_t CDataFrame::getColumnsCount() const {
    return columns.size();
}

bool CDataFrame::insertRow(const std::vector<int> &values) {
    // Ensure the number of values matches the number of columns
    if (values.size() != columns.size()) {
        return false;
    }

    // Insert each value into the corresponding column
    for (size_t i = 0; i < columns.size(); ++i) {
        columns[i]->insertValue(values[i]);
    }
    return true;
}

size_t CDataFrame::getRowsCount() const {
    if (columns.empty()) {
        return 0;
    }
    return columns[0]->getSize();
}

void CDataFrame::print() const {
    if (columns.empty()) {
        std::cout << "Empty DataFrame" << std::endl;
        return;
    }

    // Print headers
    for (const auto& col : columns) {
        std::cout << col->getName() << "\t";
    }
    std::cout << std::endl;

    // Print rows
    size_t rows = getRowsCount();
    for (size_t i = 0; i < rows; ++i) {
        for (const auto& col : columns) {
            std::cout << col->getValueAt(static_cast<int>(i)) << "\t";
        }
        std::cout << std::endl;
    }
}

void CDataFrame::info() const {
    std::cout << "--- CDataFrame Info ---" << std::endl;
    std::cout << "Number of columns: " << getColumnsCount() << std::endl;
    std::cout << "Number of rows:    " << getRowsCount() << std::endl;

    std::cout << "Column headers:    ";
    for (const auto& col : columns) {
        std::cout << col->getName() << ",  ";
    }
    std::cout << std::endl;
}