#include "heightmapeditortab.h"
#include "qboxlayout.h"

HeightmapEditorTab::HeightmapEditorTab(QWidget *parent)
    : QWidget{parent}
{
    model_registry_ = std::make_shared<QtNodes::NodeDelegateModelRegistry>();

    QVBoxLayout* l = new QVBoxLayout(this);

    QtNodes::DataFlowGraphModel model(model_registry_);
    node_scene_ = std::make_shared<QtNodes::DataFlowGraphicsScene>(model, this);
    node_view_ = std::make_shared<QtNodes::GraphicsView>(node_scene_.get());

    l->addWidget(node_view_.get());

    QObject::connect(node_scene_.get(), &QtNodes::DataFlowGraphicsScene::sceneLoaded,
                     node_view_.get(), &QtNodes::GraphicsView::centerScene);
}
