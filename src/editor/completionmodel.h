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
    void slotObtainedModel(QStringList words);

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
    ModelFromScope(MdiTextEditor *editor);

public slots:
    void slotGetModel(int position);

signals:
    void gotModel(QStringList words);

private:
    MdiTextEditor *editor;

    void modelFromScope(int position);
    bool inScopeOf(int a, int b);
};

#endif
