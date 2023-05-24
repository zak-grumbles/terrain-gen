#ifndef TERRAINTAB_H
#define TERRAINTAB_H

#include <QObject>
#include <QWidget>

class TerrainTab : public QWidget
{
    Q_OBJECT
public:
    explicit TerrainTab(QWidget *parent = nullptr);

signals:

public slots:
    void OnHeightmapChanged(std::shared_ptr<QPixmap> heightmap);
};

#endif // TERRAINTAB_H
