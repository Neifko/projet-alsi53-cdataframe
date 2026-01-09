//
// Created by victor on 09/01/2026.
//

#ifndef PROJET_ALSI53_CDATAFRAME_CDATAFRAME_H
#define PROJET_ALSI53_CDATAFRAME_CDATAFRAME_H
#include "Column.h"
#include <vector>
#include <memory>

class CDataFrame {
    std::vector<std::shared_ptr<Column>> columns;

public:



};


#endif //PROJET_ALSI53_CDATAFRAME_CDATAFRAME_H