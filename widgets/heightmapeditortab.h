#ifndef HEIGHTMAPEDITORTAB_H
#define HEIGHTMAPEDITORTAB_H

#include <QObject>
#include <QWidget>

#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeDelegateModelRegistry>

class HeightmapEditorTab : public QWidget
{
    Q_OBJECT

public:
    explicit HeightmapEditorTab(QWidget *parent = nullptr);

signals:

protected:
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> model_registry_ = nullptr;
    std::shared_ptr<QtNodes::GraphicsView> node_view_ = nullptr;
    std::shared_ptr<QtNodes::DataFlowGraphicsScene> node_scene_ = nullptr;
};

#endif // HEIGHTMAPEDITORTAB_H
