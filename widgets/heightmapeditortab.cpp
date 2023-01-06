#include "heightmapeditortab.h"
#include "nodes/noisesourcedatamodel.h"
#include "nodes/numbersourcedatamodel.h"

#include <QBoxLayout>

HeightmapEditorTab::HeightmapEditorTab(QWidget *parent)
    : QWidget{parent}
{
    model_registry_ = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
    model_registry_->registerModel<NoiseSourceDataModel>("Sources");
    model_registry_->registerModel<NumberSourceDataModel>("Sources");

    QVBoxLayout* l = new QVBoxLayout(this);

    graph_model_ = std::make_shared<QtNodes::DataFlowGraphModel>(model_registry_);
    node_scene_ = std::make_shared<QtNodes::DataFlowGraphicsScene>(*graph_model_);
    node_view_ = std::make_shared<QtNodes::GraphicsView>(node_scene_.get());

    l->addWidget(node_view_.get());

    QObject::connect(node_scene_.get(), &QtNodes::DataFlowGraphicsScene::sceneLoaded,
                     node_view_.get(), &QtNodes::GraphicsView::centerScene);

}
