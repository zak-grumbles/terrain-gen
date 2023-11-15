#ifndef MULTIPLYOPERATIONDATA_H
#define MULTIPLYOPERATIONDATA_H

#include "nodes/data/heightdata.h"

class MultiplyOperationData : public HeightData
{
public:
    MultiplyOperationData();
    virtual ~MultiplyOperationData() {}

    float GetValueAt(float x, float y) const override;

    void SetInputA(std::shared_ptr<HeightData> newInput);
    void SetInputB(std::shared_ptr<HeightData> newInput);

protected:
    HeightDataType type_ = HeightDataType::kOperation;

    std::shared_ptr<HeightData> inputA_ = nullptr;
    std::shared_ptr<HeightData> inputB_ = nullptr;
};

#endif // MULTIPLYOPERATIONDATA_H
