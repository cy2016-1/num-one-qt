﻿#ifndef QTNUMONE_H
#define QTNUMONE_H

#include <QMainWindow>
#include "MainGui.h"
#include "SingleDataDisplay.h"
#include "MPU6050Display.h"
#include "interactiveacts.h"
#include "PointCloudWidget.h"

class QtNumOne : public QMainWindow
{
    Q_OBJECT

public:
    MainGUi * aMainGui;
    QWidget * CurrentWgt = nullptr;

    QtNumOne(QWidget *parent = nullptr);
    void LinkSignalSlot();
    ~QtNumOne();
};
#endif // QTNUMONE_H
