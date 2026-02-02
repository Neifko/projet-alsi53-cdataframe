#include "Column.h"
#include <iostream>
#include <numeric>
#include <algorithm>


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

void Column::sort(bool ascending) {
	if (index.size() != data.size()) {
		index.resize(data.size());
		std::iota(index.begin(), index.end(), 0);
	}
	std::stable_sort(index.begin(), index.end(),
		[this, ascending](size_t a, size_t b) {
			const auto& valA = data[a];
			const auto& valB = data[b];

			if (valA.index() != valB.index()) {
				return valA.index() < valB.index();
			}
			return std::visit([&](auto&& argA, auto&& argB) -> bool {
					using T = std::decay_t<decltype(argA)>;
					if constexpr (std::is_same_v<std::decay_t<decltype(argB)>, T>) {
						 return ascending ? (argA < argB) : (argA > argB);
					}
					return false;
				}, valA, valB);
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
	if (!index.empty()) {
		sort(sortAscending);
	}
}

int Column::searchValue(const ColumnValue &val) const {
	if (!validIndex || index.empty()) {
		return -1;
	}

	auto it = std::lower_bound(index.begin(), index.end(), val,
		[this](size_t idx, const ColumnValue &searchVal) {
			const auto &element = data[idx];
			if (element.index() != searchVal.index()) {
				return false;
			}
			return sortAscending ? (element < searchVal) : (element > searchVal);
		});

	if (it != index.end()) {
		const auto &candidate = data[*it];
		if (candidate == val) {
			return 1;
		}
	}

	return 0;
}
