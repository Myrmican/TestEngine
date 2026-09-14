#pragma once

#include <QWidget>
#include <QTimer>
#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class EngineViewport : public QWidget {
    Q_OBJECT

public:
    

    explicit EngineViewport(QWidget* parent = nullptr);
    ~EngineViewport() override = default;
};