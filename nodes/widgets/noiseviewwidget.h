#ifndef NOISEVIEWWIDGET_H
#define NOISEVIEWWIDGET_H

#include <QBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QWidget>

#include "FastNoiseLite/FastNoiseLite.h"

#include "nodes/data/noisedata.h"

enum NoiseSourceType
{
    kPerlin = 0,
    kValue,
    kOpenSimplex2,
    kOpenSimplex2S
};

class NoiseViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NoiseViewWidget(std::shared_ptr<NoiseData> data, QWidget *parent = nullptr);

    void UpdateNoise();

signals:
    void NoiseTypeChanged(FastNoiseLite::NoiseType new_type);

protected slots:
    void OnNoiseTypeChanged_(int new_type);

protected:
    void GenerateBitmap_();

    FastNoiseLite::NoiseType ToFastNoiseEnum_(NoiseSourceType type) const;

    QComboBox* noise_picker_ = nullptr;
    QBitmap* noise_bitmap_ = nullptr;
    QLabel* noise_preview_ = nullptr;

    std::shared_ptr<NoiseData> noise_data_ = nullptr;
};

#endif // NOISEVIEWWIDGET_H
