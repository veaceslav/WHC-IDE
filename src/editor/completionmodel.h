#ifndef COMPLETIONMODEL_H
#define COMPLETIONMODEL_H

#include <QThread>
#include "mditexteditor.h"

class ModelFromScope;

class CompletionModel : public QObject
{
    Q_OBJECT
public:
    CompletionModel(MdiTextEditor *parent);
    ~CompletionModel();

public slots:
    void slotGetModel(int position);

signals:
    void requestModel(int position);

private:
    QThread *thread;
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
