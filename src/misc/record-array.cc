#include "record-array.hh"

using namespace record_array;

template <> std::string record_array::Type<int8_t  >::name = "char";
template <> std::string record_array::Type<uint8_t >::name = "uchar";
template <> std::string record_array::Type<int16_t >::name = "short";
template <> std::string record_array::Type<uint16_t>::name = "ushort";    
template <> std::string record_array::Type<int32_t >::name = "int";
template <> std::string record_array::Type<uint32_t>::name = "uint";
template <> std::string record_array::Type<float   >::name = "float";
template <> std::string record_array::Type<double  >::name = "double";

template <> record_array::TYPE_ID record_array::Type<int8_t  >::id = record_array::T_CHAR;
template <> record_array::TYPE_ID record_array::Type<uint8_t >::id = record_array::T_UCHAR;
template <> record_array::TYPE_ID record_array::Type<int16_t >::id = record_array::T_SHORT;
template <> record_array::TYPE_ID record_array::Type<uint16_t>::id = record_array::T_USHORT;
template <> record_array::TYPE_ID record_array::Type<int32_t >::id = record_array::T_INT;
template <> record_array::TYPE_ID record_array::Type<uint32_t>::id = record_array::T_UINT;
template <> record_array::TYPE_ID record_array::Type<float   >::id = record_array::T_FLOAT;
template <> record_array::TYPE_ID record_array::Type<double  >::id = record_array::T_DOUBLE;

std::map<record_array::TYPE_ID, record_array::Type_info> record_array::Type_map = {
    { record_array::T_CHAR,      { "char",   1 } },
    { record_array::T_UCHAR,     { "uchar",  1 } },
    { record_array::T_SHORT,     { "short",  2 } },
    { record_array::T_USHORT,    { "ushort", 2 } },
    { record_array::T_INT,       { "int",    4 } },
    { record_array::T_UINT,      { "uint",   4 } },
    { record_array::T_FLOAT,     { "float",  4 } },
    { record_array::T_DOUBLE,    { "double", 8 } } };

