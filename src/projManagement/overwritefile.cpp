#include "projManagement/overwritefile.h"

OverwriteFile::OverwriteFile(QDialog *parent, QString fileName)
    :QDialog(parent)
{
    this->resize(390, 148);
    this->setWindowTitle("Add New File");

    overrideFileChoice = new QDialogButtonBox(this);
    overrideFileChoice->setGeometry(QRect(40, 110, 341, 32));
    overrideFileChoice->setOrientation(Qt::Horizontal);
    overrideFileChoice->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    warnLabel = new QLabel(this);
    warnLabel->setGeometry(QRect(10, 20, 300, 42));
    warnLabel->setText("File \"" + fileName + "\" already exists.\n" +
                       "Proceeding will overwrite the old file!");

    QObject::connect(overrideFileChoice, SIGNAL(accepted()),
                     this, SLOT(slotCanOverwrite()));
    QObject::connect(overrideFileChoice, SIGNAL(rejected()),
                     this, SLOT(slotCannotOverwrite()));
}

OverwriteFile::~OverwriteFile()
{
}

void OverwriteFile::slotCannotOverwrite()
{
    emit signalOvewriteRejected();
    this->close();
}

void OverwriteFile::slotCanOverwrite()
{
    emit signalOvewriteAccepted();
    this->close();
}

