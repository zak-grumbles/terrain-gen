#include "multiplyoperationdata.h"

MultiplyOperationData::MultiplyOperationData()
{

}

float MultiplyOperationData::GetValueAt(float x, float y) const
{
    float result = 0.0f;

    if(inputA_ != nullptr && inputB_ != nullptr)
    {
        float aResult = inputA_->GetValueAt(x, y);
        float bResult = inputB_->GetValueAt(x, y);
        result = aResult * bResult;
    }

    return result;
}

void MultiplyOperationData::SetInputA(std::shared_ptr<HeightData> newInput)
{
    this->inputA_ = newInput;
}

void MultiplyOperationData::SetInputB(std::shared_ptr<HeightData> newInput)
{
    this->inputB_ = newInput;
}
