#ifndef COMPLETEMODEL_H
#define COMPLETEMODEL_H

#include <QThread>
#include "mditexteditor.h"

class ModelFromScope;

class CompleteModel : public QObject
{
    Q_OBJECT
public:
    CompleteModel(MdiTextEditor *parent);
    ~CompleteModel();

public slots:
    void slotGetModel(int position);
    void slotObtainedModel(QStringListModel *model);

signals:
    void gotModel(QStringListModel *model);
    void requestModel(int position);

private:
    QThread *thread;
    MdiTextEditor *parent;
    ModelFromScope *worker;
};

class ModelFromScope : public QObject
{
    Q_OBJECT
public:
    ModelFromScope(CompleteModel *parent, MdiTextEditor *editor);

public slots:
    void slotGetModel(int position);

signals:
    void gotModel(QStringListModel *model);

private:
    CompleteModel *parent;
    MdiTextEditor *editor;

    QStringListModel *modelFromScope(int position);
    bool inScopeOf(int a, int b);
};

#endif
