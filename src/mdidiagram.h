#ifndef MDIDIAGRAM_H
#define MDIDIAGRAM_H

#include <QMdiSubWindow>

class DiagramWindow;
/**
 * @brief The MdiDiagram class is designed to handle Diagrams window close and
 *                             and restore without destroying the intire object
 */
class MdiDiagram : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit MdiDiagram(DiagramWindow* diag, QMdiArea *parent,
                         Qt::WindowFlags flags);
    /**
     * @brief addtoMdi -add Diagram subwindow to QMdiArea
     *                  used by action Window->Show Diagram
     */
    void addtoMdi();

    /**
     * @brief isVisible - check is Diagram Window is not closed
     * @return true if is not closed
     */
    inline bool isVisible() const {return visible; }

protected:
    /**
     * @brief closeEvent -closing without removing from QMdiArea will leave
     *                    an empty tab bar, so close slot was reimplemented
     */
    virtual void closeEvent(QCloseEvent *closeEvent);
    
private:

    QMdiArea* mdiParent;
    bool visible;
};

#endif // MDIDIAGRAM_H
