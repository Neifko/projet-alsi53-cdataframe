//
// Created by victor on 09/01/2026.
//

#include "CDataFrame.h"

#include <algorithm>

void CDataFrame::addColumn(std::string columnTitle) {
    columns.push_back(std::make_shared<Column>(columnTitle));
}


