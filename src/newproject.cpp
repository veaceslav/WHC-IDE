/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) 2012 by Veaceslav Munteanu <slavuttici at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "newproject.h"

NewProject::NewProject(Ide* parent)
    :QDialog(parent)

{
    this->parent=parent;
    this->resize(527, 369);
    this->setMinimumSize(QSize(331, 175));
    this->setMaximumSize(QSize(999, 999));
    this->setSizeIncrement(QSize(0, 0));
    this->setBaseSize(QSize(0, 0));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    this->setWindowTitle("New WHC Project");
    layoutWidget = new QWidget(this);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(320, 330, 201, 29));

    /** Cancel and Finish Buttons set-up **/
    buttonLayout = new QHBoxLayout(layoutWidget);
    buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    cancelButton = new QPushButton(layoutWidget);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
    cancelButton->setText("Cancel");


    buttonLayout->addWidget(cancelButton);

    finishButton = new QPushButton(layoutWidget);
    finishButton->setText("Finish");
    finishButton->setObjectName(QString::fromUtf8("finishButton"));
    finishButton->setAutoDefault(true);
    finishButton->setDefault(true);
    finishButton->setShortcut(QApplication::translate("this", "Return", 0, QApplication::UnicodeUTF8));

    buttonLayout->addWidget(finishButton);

    /** Project name , Location & Template Text Description info set-up **/
    layoutWidget_2 = new QWidget(this);
    layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
    layoutWidget_2->setGeometry(QRect(160, 20, 351, 72));
    verticalLayout = new QVBoxLayout(layoutWidget_2);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout = new QHBoxLayout();
    projNameLabel = new QLabel(layoutWidget_2);
    projNameLabel->setObjectName(QString::fromUtf8("projNameLabel"));
    projNameLabel->setMinimumSize(QSize(90, 0));
    projNameLabel->setText(QApplication::translate("this", "Project name", 0, QApplication::UnicodeUTF8));

    horizontalLayout->addWidget(projNameLabel);

    projectName = new QLineEdit(layoutWidget_2);
    projectName->setObjectName(QString::fromUtf8("projectName"));



    horizontalLayout->addWidget(projectName);


    verticalLayout->addLayout(horizontalLayout);

    line = new QFrame(layoutWidget_2);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShadow(QFrame::Raised);
    line->setFrameShape(QFrame::VLine);

    verticalLayout->addWidget(line);

    horizontalLayout_5 = new QHBoxLayout();
    horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
    horizontalLayout_5->setSizeConstraint(QLayout::SetDefaultConstraint);
    locationLabel = new QLabel(layoutWidget_2);
    locationLabel->setObjectName(QString::fromUtf8("locationLabel"));
    locationLabel->setMinimumSize(QSize(90, 0));
    locationLabel->setText(QApplication::translate("this", "Location", 0, QApplication::UnicodeUTF8));

    horizontalLayout_5->addWidget(locationLabel);

    projectLocation = new QLineEdit(layoutWidget_2);
    projectLocation->setObjectName(QString::fromUtf8("projectLocation"));
    projectLocation->setReadOnly(true);
    projectLocation->setText(QDir::homePath());

    horizontalLayout_5->addWidget(projectLocation);

    setprojectLocation = new QPushButton(layoutWidget_2);
    setprojectLocation->setObjectName(QString::fromUtf8("setprojectLocation"));
    setprojectLocation->setMaximumSize(QSize(20, 16777215));
    setprojectLocation->setText(QApplication::translate("this", "...", 0, QApplication::UnicodeUTF8));

    horizontalLayout_5->addWidget(setprojectLocation);


    verticalLayout->addLayout(horizontalLayout_5);

    templateInfo = new QTextEdit(this);
    templateInfo->setObjectName(QString::fromUtf8("templateInfo"));
    templateInfo->setEnabled(true);
    templateInfo->setGeometry(QRect(160, 140, 351, 131));
    templateInfo->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);"));
    templateInfo->setFrameShape(QFrame::NoFrame);
    templateInfo->setFrameShadow(QFrame::Plain);
    templateInfo->setReadOnly(true);
    templateInfo->setHtml(QApplication::translate("this", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">Template for processing raw PGM image files. </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Ubuntu'; font-size:11pt; color:#000000;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">-single input</span></p>\n"
"<"
                    "p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">-single output</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">-single pass</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">-single kernel</span></p></body></html>", 0, QApplication::UnicodeUTF8));

    /** WHC image logo **/

    label_3 = new QLabel(this);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(0, 0, 151, 111));
    label_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
    label_3->setFrameShape(QFrame::Box);
    label_3->setLineWidth(0);
    label_3->setScaledContents(false);
    label_3->setMargin(2);
    label_3->setIndent(-1);

    label_4 = new QLabel(this);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(20, 10, 111, 81));
    label_4->setStyleSheet(QString::fromUtf8("background-color: rgb(159, 159, 159);"));
    label_4->setFrameShape(QFrame::NoFrame);
    label_4->setPixmap(QPixmap(QString::fromUtf8(":/images/icon.png")));
    label_4->setScaledContents(true);

    /** Stacked Widget - 4 Pages **/
    templateImage = new QStackedWidget(this);
    templateImage->setObjectName(QString::fromUtf8("templateImage"));
    templateImage->setEnabled(true);
    templateImage->setGeometry(QRect(0, 110, 151, 261));
    templateImage->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
    templateImage->setFrameShape(QFrame::NoFrame);

    /** Page 1 **/
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    label_6 = new QLabel(page);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setEnabled(true);
    label_6->setGeometry(QRect(0, 0, 151, 271));
    label_6->setPixmap(QPixmap(QString::fromUtf8(":/images/SISD.jpg")));
    label_6->setScaledContents(true);
    templateImage->addWidget(page);

    /** Page 2 **/
    page_3 = new QWidget();
    page_3->setObjectName(QString::fromUtf8("page_3"));
    label_8 = new QLabel(page_3);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setGeometry(QRect(0, 0, 151, 261));
    label_8->setPixmap(QPixmap(QString::fromUtf8(":/images/whcMIMD0.jpg")));
    label_8->setScaledContents(true);
    templateImage->addWidget(page_3);

    /** Page 3 **/
    page_4 = new QWidget();
    page_4->setObjectName(QString::fromUtf8("page_4"));
    label_9 = new QLabel(page_4);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(0, 0, 151, 261));
    label_9->setPixmap(QPixmap(QString::fromUtf8(":/images/whcMIMD.jpg")));
    label_9->setScaledContents(true);
    templateImage->addWidget(page_4);

    /** Page 4 - default empty page **/
    page_5 = new QWidget();
    page_5->setObjectName(QString::fromUtf8("page_5"));
    templateImage->addWidget(page_5);

    /** end of stacked widget **/

    /** Template Project Combo Box **/
    templateProj = new QComboBox(this);
    templateProj->setObjectName(QString::fromUtf8("templateProj"));
    templateProj->setGeometry(QRect(160, 110, 349, 31));
    templateProj->clear();
    templateProj->insertItems(0, QStringList()
     << QApplication::translate("this", "Multiple Program Nodes (WHC)", 0, QApplication::UnicodeUTF8)
                              /** This options are not yet implemented **/
     /**
     << QApplication::translate("this", "Image Processing (Simple)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("this", "Single Program Node (WHC)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("this", "Empty Project", 0, QApplication::UnicodeUTF8)
     **/
    );
    line_2 = new QFrame(this);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setGeometry(QRect(140, 0, 21, 381));
    line_2->setFrameShape(QFrame::VLine);
    line_2->setFrameShadow(QFrame::Sunken);

    templateImage->setCurrentIndex(2);

    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(finishButton,SIGNAL(clicked()),this,SLOT(slotCreateProject()));
    connect(templateProj,SIGNAL(currentIndexChanged(int)),this,SLOT(slotUpdateImage()));
    connect(templateProj,SIGNAL(currentIndexChanged(int)),this,SLOT(slotUpdateTemplateInfo()));
    connect(setprojectLocation,SIGNAL(clicked()),this,SLOT(slotSelectLocation()));
}

void NewProject::slotUpdateImage()
{
    templateImage->setCurrentIndex(templateProj->currentIndex());
}

void NewProject::slotUpdateTemplateInfo()
{
    int index = templateProj->currentIndex();

    switch(index)
    {
        case 0:
        {
            templateInfo->setHtml(QApplication::translate("this", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
        "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
        "p, li { white-space: pre-wrap; }\n"
        "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">Template for processing raw PGM image files. </span></p>\n"
        "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Ubuntu'; font-size:11pt; color:#000000;\"><br /></p>\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">-single input</span></p>\n"
        "<"
         "p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; \"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">-single output</span></p>\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; \"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">-single pass</span></p>\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; \"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">-single kernel</span></p></body></html>", 0, QApplication::UnicodeUTF8));
            break;
        }
        case 1:
        {
            templateInfo->setHtml(QApplication::translate("this", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
        "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
        "p, li { white-space: pre-wrap; }\n"
        "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">Template for WHC platform. </span></p>\n"
        "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Ubuntu'; font-size:11pt; color:#000000;\"><br /></p>\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">- single program (single node)</span></p>\n"
        "<"
         "p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; \"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">- multiple inputs</span></p>\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; \"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">- multiple outputs</span></p>\n", 0, QApplication::UnicodeUTF8));
            break;
        }
        case 2:
        {
            templateInfo->setHtml(QApplication::translate("this", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
        "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
        "p, li { white-space: pre-wrap; }\n"
        "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">Template for WHC platform. </span></p>\n"
        "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Ubuntu'; font-size:11pt; color:#000000;\"><br /></p>\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">- multiple programs (multiple pipelined nodes)</span></p>\n"
        "<"
         "p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">- multiple inputs</span></p>\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\">- mutiple outputs</span></p>\n", 0, QApplication::UnicodeUTF8));
            break;
        }
        case 3:
        {
            templateInfo->setHtml(QApplication::translate("this", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
        "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
        "p, li { white-space: pre-wrap; }\n"
        "</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; color:#000000;\"> Empty Project. </span></p>\n", 0, QApplication::UnicodeUTF8));
        }
    }
}
void NewProject::slotSelectLocation()
{

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 QDir::homePath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    projectLocation->setText(dir);

}

void NewProject::slotCreateProject()
{

    QString path = projectLocation->text();
    QString projName = projectName->text();
    if(path.isEmpty())
    {
        QMessageBox msg;
        msg.setText("No project location specified.");
        msg.setInformativeText("Please select project location.");
        msg.exec();
        return;
    }
    if(projName.isEmpty())
    {
        QMessageBox msg;
        msg.setText("Project Name is Empty.");
        msg.setInformativeText("Please choose a name for your project");
        msg.exec();
        return;
    }
    QDir dir;
    /** Building file path: dir + "/" + filename **/
    path.append("/");
    path.append(projName);
    /** project Directory **/
    dir.mkdir(path);
    /** source and data subDirs **/
    dir.mkdir(path + "/src");
    dir.mkdir(path + "/data");

    /** Project .whc file **/
    path.append("/");
    path.append(projName);
    path.append(".whc");
    QFile file(path);

    file.open(QFile::WriteOnly);

    /** Creating xml Object **/
    QDomElement  projN;
    QDomDocument xmlDoc("WHC");

    projN = xmlDoc.createElement("project");
    xmlDoc.appendChild(projN);
    projN.setAttribute(QString("name"),projName);

    QDomElement task = xmlDoc.createElement("tasks");
    QDomElement files = xmlDoc.createElement("data");
    QDomElement diag  = xmlDoc.createElement("diagram");

    projN.appendChild(task);
    projN.appendChild(files);
    projN.appendChild(diag);


    /** Write data to file **/
    QByteArray data(xmlDoc.toByteArray());
    QDataStream stream( &file );
    stream.writeRawData(data.data(), data.size());
    file.close();

    parent->startNewProject(path);
    this->close();
}
NewProject::~NewProject()
{
    /*
    Ide::destroyObj(&buttonLayout);
    Ide::destroyObj(&finishButton);
    Ide::destroyObj(&cancelButton);
    //Ide::destroyObj(&layoutWidget);
    //Ide::destroyObj(&verticalLayout);
    //Ide::destroyObj(&layoutWidget_2);
    Ide::destroyObj(&horizontalLayout);
    Ide::destroyObj(&projNameLabel);
    Ide::destroyObj(&line);
    Ide::destroyObj(&horizontalLayout_5);
    Ide::destroyObj(&projectName);
    Ide::destroyObj(&projectLocation);
    Ide::destroyObj(&setprojectLocation);
    Ide::destroyObj(&templateInfo);
    Ide::destroyObj(&label_3);
    Ide::destroyObj(&label_4);
    Ide::destroyObj(&label_6);
    Ide::destroyObj(&label_8);
    Ide::destroyObj(&label_9);
    Ide::destroyObj(&page);
    Ide::destroyObj(&line_2);
    Ide::destroyObj(&templateProj);
    */
}
