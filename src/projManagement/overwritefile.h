#ifndef OVERWRITEFILE_H
#define OVERWRITEFILE_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QDialogButtonBox>

class OverwriteFile : public QDialog
{
  Q_OBJECT

public:
    OverwriteFile(QDialog *parent, QString fileName);
    ~OverwriteFile();

signals:
    void signalOvewriteAccepted();
    void signalOvewriteRejected();

private slots:

    /**
     * @brief slotAddNewFile -add selected fiels to .whc
     */
    void slotCanOverwrite();
    void slotCannotOverwrite();
private:
    QDialogButtonBox *overrideFileChoice;
    QLabel *warnLabel;
};

#endif // OVERWRITEFILE_H
