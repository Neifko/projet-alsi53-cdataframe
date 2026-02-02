#include "CDataFrame.h"
#include <iostream>
#include <algorithm>

// Add constructor: create columns from vector of types with default names col0, col1, ...
CDataFrame::CDataFrame(const std::vector<ColumnType> &types) {
    columns.reserve(types.size());
    for (size_t i = 0; i < types.size(); ++i) {
        columns.push_back(std::make_shared<Column>(types[i], "col" + std::to_string(i)));
    }
}

void CDataFrame::addColumn(std::string columnTitle, ColumnType type) {
    columns.push_back(std::make_shared<Column>(type, columnTitle));
}

bool CDataFrame::deleteColumn(std::string columnTitle) {
    size_t initialSize = columns.size();
    auto it = std::remove_if(columns.begin(), columns.end(),
                             [&columnTitle](const std::shared_ptr<Column>& col) {
                                 return col->getName() == columnTitle;
                             });

    if (it != columns.end()) {
        columns.erase(it, columns.end());
        return true; // Colonne trouvee et supprimee
    }
    return false; // Colonne non trouvee
}

size_t CDataFrame::getColumnsCount() const {
    return columns.size();
}

bool CDataFrame::insertRow(const std::vector<ColumnValue>& values) {
    if (values.size() != columns.size()) {
        return false;
    }

    // Verify type compatibility for each value
    for (size_t i = 0; i < columns.size(); ++i) {
        if (!isValueCompatibleWithType(values[i], columns[i]->getType())) {
            // Allow numeric-to-numeric implicit conversion
            bool valueIsNumeric = std::visit([](auto&& arg) -> bool {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::monostate> || std::is_same_v<T, std::string>) {
                    return false;
                }
                return std::is_arithmetic_v<T>;
            }, values[i]);

            ColumnType colType = columns[i]->getType();
            bool typeIsNumeric = (colType >= ColumnType::UINT && colType <= ColumnType::DOUBLE);

            if (!(valueIsNumeric && typeIsNumeric)) {
                return false; // Type mismatch
            }
        }
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
    bool isSorted = (columns[0]->checkIndex() == 1);
    const std::vector<size_t>& idx = columns[0]->getIndex();

    for (size_t i = 0; i < rows; ++i) {
        size_t physicalRow = isSorted ? idx[i] : i;
        for (const auto& col : columns) {
            std::cout << col->valueToString(physicalRow) << "\t";
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
            std::cout << col->valueToString(i) << "\t";
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
            std::cout << columns[c]->valueToString(r) << "\t";
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

bool CDataFrame::renameColumn(const std::string& oldName, const std::string& newName) {
    for (auto& col : columns) {
        if (col->getName() == oldName) {
            col->setName(newName);
            return true; // Colonne trouvee et renommee
        }
    }
    return false; // Colonne non trouvee
}

bool CDataFrame::valueExists(const ColumnValue& value) const {
    for (const auto& col : columns) {
        if (col->valueCount(value) > 0) return true;
    }
    return false;
}

const ColumnValue& CDataFrame::getValue(size_t rowIndex, size_t colIndex) const {
    if (colIndex >= columns.size()) throw std::out_of_range("Column index out of range");
    return columns[colIndex]->getValueAt(static_cast<int>(rowIndex));
}

void CDataFrame::replaceValue(size_t rowIndex, size_t colIndex, const ColumnValue& newValue) {
    if (colIndex >= columns.size()) throw std::out_of_range("Column index out of range");
    columns[colIndex]->setValueAt(static_cast<int>(rowIndex), newValue);
}

size_t CDataFrame::countValuesEqualTo(const ColumnValue& value) const {
    size_t total = 0;
    for (const auto& col : columns) {
        total += col->countValuesEqualTo(value);
    }
    return total;
}

size_t CDataFrame::countValuesGreaterThan(const ColumnValue& value) const {
    size_t total = 0;
    for (const auto& col : columns) {
        total += col->countValuesGreaterThan(value);
    }
    return total;
}

size_t CDataFrame::countValuesLessThan(const ColumnValue& value) const {
    size_t total = 0;
    for (const auto& col : columns) {
        total += col->countValuesLessThan(value);
    }
    return total;
}

void CDataFrame::fillDataFrame(const std::vector<std::vector<ColumnValue>>& data) {
    for (const auto& row : data) {
        insertRow(row);
    }
}

void CDataFrame::sort(const std::string& colName, bool ascending) {
    std::shared_ptr<Column> refCol = nullptr;
    for (auto& col : columns) {
        if (col->getName() == colName) {
            refCol = col;
            break;
        }
    }

    if (!refCol) {
        std::cout << "Column " << colName << " not found" << std::endl;
        return;
    }

    if (!columns.empty() && columns[0]->checkIndex() == 1) {
        if (refCol != columns[0]) {
            refCol->setIndex(columns[0]->getIndex());
        }
    }

    refCol->sort(ascending);
    const std::vector<size_t>& sortedIndex = refCol->getIndex();

    for (auto& col : columns) {
        if (col != refCol) {
            col->setIndex(sortedIndex);
        }
    }
}


void CDataFrame::fillDataFrame() {
    int nbRows;
    std::cout << "How many rows do you want to add? ";
    std::cin >> nbRows;

    for (int i = 0; i < nbRows; ++i) {
        std::vector<ColumnValue> rowValues;
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

// Add setColumnNames: set titles if sizes match
void CDataFrame::setColumnNames(const std::vector<std::string>& names) {
    if (names.size() != columns.size()) {
        std::cout << "Warning: number of names (" << names.size()
                  << ") does not match number of columns (" << columns.size() << ")." << std::endl;
        return;
    }
    for (size_t i = 0; i < columns.size(); ++i) {
        columns[i]->setName(names[i]);
    }
}

// Add printHead: show up to first 10 rows
void CDataFrame::printHead() const {
    if (columns.empty()) {
        std::cout << "Empty DataFrame" << std::endl;
        return;
    }

    size_t rows = getRowsCount();
    size_t toShow = std::min<size_t>(10, rows);

    for (const auto& col : columns) {
        std::cout << col->getName() << "\t";
    }
    std::cout << std::endl;

    for (size_t r = 0; r < toShow; ++r) {
        for (const auto& col : columns) {
            std::cout << col->valueToString(r) << "\t";
        }
        std::cout << std::endl;
    }
}

// Add printTail: show up to last 10 rows
void CDataFrame::printTail() const {
    if (columns.empty()) {
        std::cout << "Empty DataFrame" << std::endl;
        return;
    }

    size_t rows = getRowsCount();
    if (rows == 0) {
        std::cout << "Empty DataFrame (no rows)" << std::endl;
        return;
    }

    size_t toShow = std::min<size_t>(10, rows);
    size_t start = rows - toShow;

    for (const auto& col : columns) {
        std::cout << col->getName() << "\t";
    }
    std::cout << std::endl;

    for (size_t r = start; r < rows; ++r) {
        for (const auto& col : columns) {
            std::cout << col->valueToString(r) << "\t";
        }
        std::cout << std::endl;
    }
}

void CDataFrame::printHeader() const {
    if (columns.empty()) {
        std::cout << "Empty DataFrame" << std::endl;
        return;
    }

    for (const auto& col : columns) {
        std::cout << col->getName() << "\t";
    }
    std::cout << std::endl;
}
