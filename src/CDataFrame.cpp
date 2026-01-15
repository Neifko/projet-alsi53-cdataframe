#include "CDataFrame.h"
#include <iostream>
#include <algorithm>

void CDataFrame::addColumn(std::string columnTitle) {
    columns.push_back(std::make_shared<Column>(columnTitle));
}

void CDataFrame::deleteColumn(std::string columnTitle) {
    auto it = std::remove_if(columns.begin(), columns.end(),
                             [&columnTitle](const std::shared_ptr<Column>& col) {
                                 return col->getName() == columnTitle;
                             });

    if (it != columns.end()) {
        columns.erase(it, columns.end());
    }
}

size_t CDataFrame::getColumnsCount() const {
    return columns.size();
}

bool CDataFrame::insertRow(const std::vector<int> &values) {
    if (values.size() != columns.size()) {
        return false;
    }

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

    for (const auto& col : columns) {
        std::cout << col->getName() << "\t";
    }
    std::cout << std::endl;

    size_t rows = getRowsCount();
    for (size_t i = 0; i < rows; ++i) {
        for (const auto& col : columns) {
            std::cout << col->getValueAt(i) << "\t";
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

void CDataFrame::printRows(size_t startRow, size_t endRow) const {
    if (columns.empty() || startRow > endRow || endRow > getRowsCount()) {
        std::cout << "Invalid range or empty dataframe" << std::endl;
        return;
    }

    for (const auto& col : columns) {
        std::cout << col->getName() << "\t";
    }
    std::cout << std::endl;

    for (size_t i = startRow; i < endRow; ++i) {
        for (const auto& col : columns) {
            std::cout << col->getValueAt(i) << "\t";
        }
        std::cout << std::endl;
    }
}

void CDataFrame::printColumns(size_t startCol, size_t endCol) const {
    if (startCol > endCol || endCol > columns.size()) {
        std::cout << "Invalid column range" << std::endl;
        return;
    }

    for (size_t i = startCol; i < endCol; ++i) {
        std::cout << columns[i]->getName() << "\t";
    }
    std::cout << std::endl;

    size_t rows = getRowsCount();
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = startCol; c < endCol; ++c) {
            std::cout << columns[c]->getValueAt(r) << "\t";
        }
        std::cout << std::endl;
    }
}

void CDataFrame::deleteRow(size_t rowIndex) {
    if (rowIndex >= getRowsCount()) {
        std::cout << "Index out of bounds" << std::endl;
        return;
    }
    for (auto& col : columns) {
        col->deleteValueAt(rowIndex);
    }
}

void CDataFrame::renameColumn(const std::string& oldName, const std::string& newName) {
    for (auto& col : columns) {
        if (col->getName() == oldName) {
            col->setName(newName);
            return;
        }
    }
    std::cout << "Column " << oldName << " not found." << std::endl;
}

bool CDataFrame::valueExists(int value) const {
    for (const auto& col : columns) {
        if (col->valueCount(value) > 0) return true;
    }
    return false;
}

int CDataFrame::getValue(size_t rowIndex, size_t colIndex) const {
    if (colIndex >= columns.size()) throw std::out_of_range("Column index out of range");
    return columns[colIndex]->getValueAt(rowIndex);
}

void CDataFrame::replaceValue(size_t rowIndex, size_t colIndex, int newValue) {
    if (colIndex >= columns.size()) throw std::out_of_range("Column index out of range");
    columns[colIndex]->setValueAt(rowIndex, newValue);
}

size_t CDataFrame::countValuesEqualTo(int value) const {
    size_t total = 0;
    for (const auto& col : columns) {
        total += col->countValuesEqualTo(value);
    }
    return total;
}

size_t CDataFrame::countValuesGreaterThan(int value) const {
    size_t total = 0;
    for (const auto& col : columns) {
        total += col->countValuesGreaterThan(value);
    }
    return total;
}

size_t CDataFrame::countValuesLessThan(int value) const {
    size_t total = 0;
    for (const auto& col : columns) {
        total += col->countValuesLessThan(value);
    }
    return total;
}

void CDataFrame::fillDataFrame(const std::vector<std::vector<int>>& data) {
    for (const auto& row : data) {
        insertRow(row);
    }
}

void CDataFrame::fillDataFrame() {
    int nbRows;
    std::cout << "How many rows do you want to add? ";
    std::cin >> nbRows;

    for (int i = 0; i < nbRows; ++i) {
        std::vector<int> rowValues;
        std::cout << "Row " << i << " : " << std::endl;
        for (const auto& col : columns) {
            int val;
            std::cout << "Value for " << col->getName() << ": ";
            std::cin >> val;
            rowValues.push_back(val);
        }
        insertRow(rowValues);
    }
}