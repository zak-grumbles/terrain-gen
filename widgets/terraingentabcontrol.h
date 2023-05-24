#ifndef TERRAINGENTABCONTROL_H
#define TERRAINGENTABCONTROL_H

#include <QObject>
#include <QTabWidget>

class TerrainGenTabControl : public QTabWidget
{
    Q_OBJECT
public:
    explicit TerrainGenTabControl(QWidget* parent = nullptr);

public slots:
    void OnHeightmapChanged(std::shared_ptr<QPixmap> heightmap);
};

#endif // TERRAINGENTABCONTROL_H
