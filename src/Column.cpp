#include "Column.h"
#include <iostream>
#include <numeric>
#include <algorithm>
#include <cmath>

std::string columnTypeToString(ColumnType type) {
    switch (type) {
        case ColumnType::NULLVAL: return "NULL";
        case ColumnType::UINT: return "UINT";
        case ColumnType::INT: return "INT";
        case ColumnType::USHORT: return "USHORT";
        case ColumnType::SHORT: return "SHORT";
        case ColumnType::ULONG: return "ULONG";
        case ColumnType::LONG: return "LONG";
        case ColumnType::UCHAR: return "UCHAR";
        case ColumnType::CHAR: return "CHAR";
        case ColumnType::FLOAT: return "FLOAT";
        case ColumnType::DOUBLE: return "DOUBLE";
        case ColumnType::STRING: return "STRING";
        default: return "UNKNOWN";
    }
}

// Helper: check if variant holds a numeric type
static bool isNumericValue(const ColumnValue& v) {
	return std::visit([](auto&& arg) -> bool {
		using T = std::decay_t<decltype(arg)>;
		if constexpr (std::is_same_v<T, std::monostate> || std::is_same_v<T, std::string>) {
			return false;
		} else {
			return std::is_arithmetic_v<T>;
		}
	}, v);
}

// Helper: convert numeric ColumnValue to double for comparison
static double toDouble(const ColumnValue& v) {
	return std::visit([](auto&& arg) -> double {
		using T = std::decay_t<decltype(arg)>;
		if constexpr (std::is_arithmetic_v<T> && !std::is_same_v<T, std::monostate>) {
			return static_cast<double>(arg);
		}
		return 0.0; // Should not be called for non-numeric
	}, v);
}

int compareValues(const ColumnValue& a, const ColumnValue& b) {
	bool aIsNull = std::holds_alternative<std::monostate>(a);
	bool bIsNull = std::holds_alternative<std::monostate>(b);

	// NULL handling: NULL < any non-NULL
	if (aIsNull && bIsNull) return 0;
	if (aIsNull) return -1;
	if (bIsNull) return 1;

	bool aIsString = std::holds_alternative<std::string>(a);
	bool bIsString = std::holds_alternative<std::string>(b);

	// String vs String
	if (aIsString && bIsString) {
		const auto& sa = std::get<std::string>(a);
		const auto& sb = std::get<std::string>(b);
		if (sa < sb) return -1;
		if (sa > sb) return 1;
		return 0;
	}

	// Numeric vs Numeric
	bool aIsNumeric = isNumericValue(a);
	bool bIsNumeric = isNumericValue(b);

	if (aIsNumeric && bIsNumeric) {
		double da = toDouble(a);
		double db = toDouble(b);
		if (da < db) return -1;
		if (da > db) return 1;
		return 0;
	}

	// Mixed types: use variant index for stable ordering
	if (a.index() < b.index()) return -1;
	if (a.index() > b.index()) return 1;
	return 0;
}

bool isValueCompatibleWithType(const ColumnValue& value, ColumnType expectedType) {
	return std::visit([expectedType](auto&& arg) -> bool {
		using T = std::decay_t<decltype(arg)>;

		// monostate (NULL) is always compatible
		if constexpr (std::is_same_v<T, std::monostate>) {
			return true;
		}

		switch (expectedType) {
			case ColumnType::NULLVAL:
				return std::is_same_v<T, std::monostate>;
			case ColumnType::UINT:
				return std::is_same_v<T, uint32_t>;
			case ColumnType::INT:
				return std::is_same_v<T, int32_t> || std::is_same_v<T, int>;
			case ColumnType::USHORT:
				return std::is_same_v<T, uint16_t>;
			case ColumnType::SHORT:
				return std::is_same_v<T, int16_t>;
			case ColumnType::ULONG:
				return std::is_same_v<T, uint64_t>;
			case ColumnType::LONG:
				return std::is_same_v<T, int64_t>;
			case ColumnType::UCHAR:
				return std::is_same_v<T, uint8_t>;
			case ColumnType::CHAR:
				return std::is_same_v<T, int8_t>;
			case ColumnType::FLOAT:
				return std::is_same_v<T, float>;
			case ColumnType::DOUBLE:
				return std::is_same_v<T, double>;
			case ColumnType::STRING:
				return std::is_same_v<T, std::string>;
			default:
				return false;
		}
	}, value);
}

Column::Column(ColumnType type)
	: columnType(type), validIndex(false), sortAscending(true) {
	data.reserve(256);
}

Column::Column(ColumnType type, const std::string& columnTitle)
	: title(columnTitle), columnType(type), validIndex(false), sortAscending(true) {
	data.reserve(256);
}

ColumnType Column::getType() const {
	return columnType;
}

bool Column::insertValue(const ColumnValue& value) {
	// Verify type compatibility (allow NULL for any type)
	if (!isValueCompatibleWithType(value, columnType)) {
		// For numeric types, allow implicit conversions between numeric types
		bool valueIsNumeric = isNumericValue(value);
		bool typeIsNumeric = (columnType >= ColumnType::UINT && columnType <= ColumnType::DOUBLE);

		if (!(valueIsNumeric && typeIsNumeric)) {
			return false; // Type mismatch
		}
	}

	data.push_back(value);

	// Invalidate index after insertion
	if (!index.empty()) {
		validIndex = false;
	}

	return true;
}

void Column::print() const {
	for (size_t i = 0; i < data.size(); ++i) {
		std::cout << "[" << i << "] " << valueToString(i) << std::endl;
	}
}

std::string Column::valueToString(size_t i) const {
	return std::visit([](auto&& arg) -> std::string {
		using T = std::decay_t<decltype(arg)>;
		if constexpr (std::is_same_v<T, std::monostate>) {
			return "NULL";
		}
		else if constexpr (std::is_same_v<T, std::string>) {
			return arg;
		}
		else {
			return std::to_string(arg);
		}
		}, data[i]);
}

int Column::valueCount(const ColumnValue& value) const {
	int count = 0;
	for (const auto& v : data) {
		if (compareValues(v, value) == 0) {
			count++;
		}
	}
	return count;
}

const ColumnValue& Column::getValueAt(int index) const {
	if (index < 0 || static_cast<size_t>(index) >= data.size()) {
		throw std::out_of_range("Index out of range in Column::getValueAt");
	}
	return data[index];
}

int Column::countValuesGreaterThan(const ColumnValue& value) const {
	int count = 0;
	for (const auto& v : data) {
		if (compareValues(v, value) > 0) {
			count++;
		}
	}
	return count;
}

int Column::countValuesLessThan(const ColumnValue& value) const {
	int count = 0;
	for (const auto& v : data) {
		if (compareValues(v, value) < 0) {
			count++;
		}
	}
	return count;
}

int Column::countValuesEqualTo(const ColumnValue& value) const {
	return valueCount(value);
}

std::string Column::getName() const {
	return title;
}

void Column::setName(const std::string& newTitle) {
	title = newTitle;
}

void Column::setValueAt(int index, const ColumnValue& newValue) {
	if (index < 0 || static_cast<size_t>(index) >= data.size()) {
		throw std::out_of_range("Index out of range in Column::setValueAt");
	}
	data[index] = newValue;

	// Invalidate index after modification
	if (!this->index.empty()) {
		validIndex = false;
	}
}

void Column::deleteValueAt(size_t index) {
	if (index >= data.size()) {
		throw std::out_of_range("Index out of range in Column::deleteValueAt");
	}
	data.erase(data.begin() + index);

	// Invalidate index after deletion
	if (!this->index.empty()) {
		validIndex = false;
	}
}

size_t Column::getSize() const {
	return data.size();
}

void Column::sort(bool ascending) {
	// Initialize or resize index
	index.resize(data.size());
	std::iota(index.begin(), index.end(), 0);

	std::stable_sort(index.begin(), index.end(),
		[this, ascending](size_t a, size_t b) {
			int cmp = compareValues(data[a], data[b]);
			return ascending ? (cmp < 0) : (cmp > 0);
		});

	validIndex = true;
	sortAscending = ascending;
}

const std::vector<size_t>& Column::getIndex() const {
	return index;
}

void Column::setIndex(const std::vector<size_t>& newIndex) {
	index = newIndex;
	validIndex = true;
}

void Column::printSorted(bool ascending) {
	if (!validIndex || sortAscending != ascending) {
		sort(ascending);
	}
	for (size_t i = 0; i < index.size(); ++i) {
		std::cout << "[" << index[i] << "] " << valueToString(index[i]) << std::endl;
	}
}

void Column::eraseIndex() {
	index.clear();
	validIndex = false;
}

int Column::checkIndex() const {
	if (index.empty()) {
		return -1;
	}
	if (!validIndex) {
		return 0;
	}
	return 1;
}

void Column::updateIndex() {
	if (index.empty()) {
		return; // No index to update
	}
	// Re-sort with current direction to rebuild valid index
	sort(sortAscending);
}

int Column::searchValue(const ColumnValue &val) const {
	if (!validIndex || index.empty()) {
		return -1; // Column not sorted
	}

	// Binary search using compareValues
	size_t left = 0;
	size_t right = index.size();

	while (left < right) {
		size_t mid = left + (right - left) / 2;
		const auto& midVal = data[index[mid]];
		int cmp = compareValues(midVal, val);

		if (sortAscending) {
			if (cmp < 0) {
				left = mid + 1;
			} else if (cmp > 0) {
				right = mid;
			} else {
				return 1; // Found
			}
		} else {
			// Descending order: comparison is reversed
			if (cmp > 0) {
				left = mid + 1;
			} else if (cmp < 0) {
				right = mid;
			} else {
				return 1; // Found
			}
		}
	}

	return 0; // Not found
}
