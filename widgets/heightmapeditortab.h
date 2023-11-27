#ifndef HEIGHTMAPEDITORTAB_H
#define HEIGHTMAPEDITORTAB_H

#include <QObject>
#include <QWidget>
#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeDelegateModelRegistry>

class HeightmapEditorTab : public QWidget {
    Q_OBJECT

public:
    explicit HeightmapEditorTab(QWidget *parent = nullptr);

    std::shared_ptr<QPixmap> GetHeightmap() const;

signals:
    void HeightmapChanged(std::shared_ptr<QPixmap> heightmap);

public slots:
    void OnOutputUpdated(
        QtNodes::NodeId const node_id, QtNodes::PortType const port_type,
        QtNodes::PortIndex const port_index
    );

protected:
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> model_registry_ =
        nullptr;
    std::shared_ptr<QtNodes::GraphicsView> node_view_           = nullptr;
    std::shared_ptr<QtNodes::DataFlowGraphicsScene> node_scene_ = nullptr;
    std::shared_ptr<QtNodes::DataFlowGraphModel> graph_model_   = nullptr;

    QtNodes::NodeId output_node_id_;
};

#endif  // HEIGHTMAPEDITORTAB_H
