#ifndef PROJET_ALSI53_CDATAFRAME_COLUMNVALUE_H
#define PROJET_ALSI53_CDATAFRAME_COLUMNVALUE_H
#include <any>
#include <cstdint>
#include <string>
#include <variant>

enum class ColumnType {
    NULLVAL = 1,
    UINT,
    INT,
    USHORT,
    SHORT,
    ULONG,
    LONG,
    UCHAR,
    CHAR,
    FLOAT,
    DOUBLE,
    STRING
};

using ColumnValue = std::variant<
    std::monostate,
    uint32_t,
    int32_t,
    uint16_t,
    int16_t,
    uint64_t,
    int64_t,
    uint8_t,
    int8_t,
    float,
    double,
    std::string
>;

#endif //PROJET_ALSI53_CDATAFRAME_COLUMNVALUE_H
