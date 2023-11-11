#include "heightmapeditortab.h"
#include "nodes/multiplynoisedatamodel.h"
#include "nodes/noiseoutputdatamodel.h"
#include "nodes/noisesourcedatamodel.h"
#include "nodes/numbersourcedatamodel.h"

#include <QBoxLayout>

HeightmapEditorTab::HeightmapEditorTab(QWidget *parent)
    : QWidget{parent}
{
    model_registry_ = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
    model_registry_->registerModel<NoiseSourceDataModel>("Sources");
    model_registry_->registerModel<NumberSourceDataModel>("Sources");
    model_registry_->registerModel<MultiplyNoiseDataModel>("Operations");
    model_registry_->registerModel<NoiseOutputDataModel>("Output");

    QVBoxLayout* l = new QVBoxLayout(this);

    graph_model_ = std::make_shared<QtNodes::DataFlowGraphModel>(model_registry_);
    node_scene_ = std::make_shared<QtNodes::DataFlowGraphicsScene>(*graph_model_);
    node_view_ = std::make_shared<QtNodes::GraphicsView>(node_scene_.get());

    l->addWidget(node_view_.get());

    QObject::connect(node_scene_.get(), &QtNodes::DataFlowGraphicsScene::sceneLoaded,
                     node_view_.get(), &QtNodes::GraphicsView::centerScene);

    connect(graph_model_.get(), &QtNodes::DataFlowGraphModel::inPortDataWasSet,
            this, &HeightmapEditorTab::OnOutputUpdated);

    output_node_id_ = graph_model_->addNode("NoiseOutput");
}

void HeightmapEditorTab::OnOutputUpdated(
        QtNodes::NodeId const node_id,
        QtNodes::PortType const port_type,
        QtNodes::PortIndex const port_index)
{
    if(node_id == output_node_id_)
    {
        auto heightmap = GetHeightmap();
        if(heightmap != nullptr)
        {
            emit HeightmapChanged(heightmap);
        }
    }
}

std::shared_ptr<QPixmap> HeightmapEditorTab::GetHeightmap() const
{
    std::shared_ptr<QPixmap> heightmap = nullptr;

    QVariant vNoise = graph_model_->portData(output_node_id_,
                           QtNodes::PortType::Out,
                           QtNodes::PortIndex(0),
                           QtNodes::PortRole::Data);
    auto node_data = vNoise.value<std::shared_ptr<QtNodes::NodeData>>();
    auto noise_data = std::dynamic_pointer_cast<NoiseData>(node_data);
    if(noise_data != nullptr)
    {
        heightmap = std::make_shared<QPixmap>(*noise_data->AsBitmap(0, 0, 256, 256));
    }

    return heightmap;
}
