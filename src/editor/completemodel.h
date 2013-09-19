#ifndef COMPLETEMODEL_H
#define COMPLETEMODEL_H

#include <QThread>
#include "mditexteditor.h"

class ModelFromScope;

class CompleteModel : public QObject
{
    Q_OBJECT
public:
    CompleteModel(MdiTextEditor *parent, QCompleter *completer);
    ~CompleteModel();

public slots:
    void slotGetModel(int position);
    void slotObtainedModel(QAbstractItemModel *model);

signals:
    void gotModel(QAbstractItemModel *model);
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
    ModelFromScope(CompleteModel *parent, MdiTextEditor *editor,
                   QCompleter *completer);

public slots:
    void slotGetModel(int position);

signals:
    void gotModel(QAbstractItemModel *model);

private:
    CompleteModel *parent;
    MdiTextEditor *editor;
    QCompleter *completer;

    QAbstractItemModel *modelFromScope(int position);
    bool inScopeOf(int a, int b);
};

#endif
