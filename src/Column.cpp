#include "Column.h"
#include <iostream>


Column::Column(ColumnType type) : columnType(type) {
	data.reserve(256);
}


Column::Column(ColumnType type, const std::string& columnTitle)
	: title(columnTitle), columnType(type){
	data.reserve(256);
}

bool Column::insertValue(const ColumnValue& value) {
	data.push_back(value);
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
		if (v == value) {
			count++;
		}
	}
	return count;
}

const ColumnValue& Column::getValueAt(int index) const {
	if (index < 0 || index >= data.size()) {
		throw std::out_of_range("Index out of range in Column::getValueAt");
	}
	return data[index];
}

int Column::countValuesGreaterThan(const ColumnValue& value) const {
	int count = 0;
	for (const auto& v : data) {
		if (v.index() == value.index()) {
			if (v > value) {
				count++;
			}
		}
	}
	return count;
}

int Column::countValuesLessThan(const ColumnValue& value) const {
	int count = 0;
	for (const auto& v : data) {
		if (v.index() == value.index()) {
			if (v < value) {
				count++;
			}
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
	if (index < 0 || index >= data.size()) {
		throw std::out_of_range("Index out of range in Column::setValueAt");
	}
	data[index] = newValue;
}

void Column::deleteValueAt(size_t index) {
	if (index >= data.size()) {
		throw std::out_of_range("Index out of range in Column::deleteValueAt");
	}
	data.erase(data.begin() + index);
}

size_t Column::getSize() const {
	return data.size();
}