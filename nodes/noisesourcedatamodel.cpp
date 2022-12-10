#include "noisesourcedatamodel.h"

NoiseSourceDataModel::NoiseSourceDataModel() :
    combo_box_{nullptr},
    noise_data_(std::make_shared<NoiseData>())
{
}

QJsonObject NoiseSourceDataModel::save() const
{
    QJsonObject modelJson = QtNodes::NodeDelegateModel::save();

    // TODO: Serialization logic

    return modelJson;
}

void NoiseSourceDataModel::load(QJsonObject const& model)
{
    // TODO: Deserialization logic
}

