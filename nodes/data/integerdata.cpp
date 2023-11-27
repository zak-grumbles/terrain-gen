#include "integerdata.h"

QtNodes::NodeDataType IntegerData::type() const {
    return QtNodes::NodeDataType{"integer", "Integer"};
}
