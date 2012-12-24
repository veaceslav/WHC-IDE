#ifndef MDIDIAGRAMVIEW_H
#define MDIDIAGRAMVIEW_H

#include <QWidget>

#include "model/projecttreemodel.h"
#include "diagram/diagramwindow.h"

/**
namespace Ui {
    class MdiDiagramView;
}
*/
class Ide;

class MdiDiagramView : public QWidget
{
    Q_OBJECT

public:
    explicit MdiDiagramView(QDomDocument *proj, Ide *parent);
    ~MdiDiagramView();

    DiagramWindow* DiagWindow() const
                              { return window;}


private:
    DiagramWindow* window;

};

#endif // MDIDIAGRAMVIEW_H
