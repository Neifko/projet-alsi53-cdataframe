#include "Column.h"
#include <iostream>


Column::Column(ColumnType type) {
	data.reserve(256);
	columnType = type;
}


Column::Column(ColumnType type, const std::string& columnTitle)
	: title(columnTitle) {
	data.reserve(256);
	columnType = type;
}

bool Column::insertValue(const ColumnValue& value) {
	data.push_back(value);
	return true;
}

void Column::print() const {
	for (size_t i = 0; i < data.size(); ++i) {
		std::cout << "[" << i << "] " << data[i] << std::endl;
	}
}

int Column::valueCount(const ColumnValue& value) const {
	int count = 0;
	for (int v : data) {
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
	for (int v : data) {
		if (v > value) {
			count++;
		}
	}
	return count;
}

int Column::countValuesLessThan(const ColumnValue& value) const {
	int count = 0;
	for (int v : data) {
		if (v < value) {
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
	if (index < 0 || index >= data.size()) {
		throw std::out_of_range("Index out of range in Column::setValueAt");
	}
	data[index] = newValue;
}

void Column::deleteValueAt(size_t index) {
	if (index < 0 || index >= data.size()) {
		throw std::out_of_range("Index out of range in Column::deleteValueAt");
	}
	data.erase(data.begin() + index);
}

size_t Column::getSize() const {
	return data.size();
}