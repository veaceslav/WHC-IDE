/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) 2012 by Veaceslav Munteanu <slavuttici at gmail dot com>
 * Copyright (C) 2012 by Cosmin Mihai <yonutix.mc@gmail.com>
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

#include <QFileDialog>
#include <QDebug>
#include "generalsettings.h"


GeneralSettings::GeneralSettings(Ide *parent, ProjectSettings *settings)
    : QDialog(parent), ui(new Ui::settingsDialog())
{

    ui->setupUi(this);
    this->settings = settings;

    QListWidgetItem *compiler = new QListWidgetItem("Compiler");
    compiler->setTextAlignment(Qt::AlignCenter);
    compiler->setIcon(QIcon(":/images/logoWHC.png"));
    ui->listWidget->addItem(compiler);

    connect(ui->listWidget, SIGNAL(itemSelectionChanged()),
            this, SLOT(slotUpdateWidget()));


    QListWidgetItem *editor = new QListWidgetItem("Editor");
    editor->setTextAlignment(Qt::AlignCenter);
    editor->setIcon(QIcon(":/images/logoWHC.png"));
    ui->listWidget->addItem(editor);

    connect(ui->clButton, SIGNAL(clicked()), this, SLOT(slotSetClDir()));

    connect(ui->libClButton, SIGNAL(clicked()), this, SLOT(slotSetLibClDir()));

    connect(ui->sdkButton, SIGNAL(clicked()), this, SLOT(slotSetSDKDir()));

    connect(ui->vsButton, SIGNAL(clicked()), this, SLOT(slotSetVSDir()));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveSettings()));

    QPushButton *applyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect(applyButton, SIGNAL(clicked()), this, SLOT(slotSaveSettings()));


    ui->stackedWidget->setCurrentIndex(0);
    editor_settings = new EditorSettings(parent, ui);
    loadSettings();
}

GeneralSettings::~GeneralSettings()
{
    delete editor_settings;
    delete ui;
}

void GeneralSettings::setDir(QLineEdit *line)
{
    QString dir =
            QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                              QDir::homePath(),
                                              QFileDialog::ShowDirsOnly |
                                              QFileDialog::DontResolveSymlinks);

    line->setText(dir);
}

void GeneralSettings::slotUpdateWidget()
{
    int index = ui->listWidget->currentRow();
    ui->stackedWidget->setCurrentIndex(index);
}

void GeneralSettings::slotSetVSDir()
{
    setDir(ui->vsLine);
}

void GeneralSettings::slotSetSDKDir()
{
    setDir(ui->sdkLine);
}

void GeneralSettings::slotSetClDir()
{
    setDir(ui->clLine);
}

void GeneralSettings::slotSetLibClDir()
{
    setDir(ui->libClLine);
}

void GeneralSettings::loadSettings()
{
    if(!settings->clPath.isEmpty())
        ui->clLine->setText(settings->clPath);

    if(!settings->libclPath.isEmpty())
        ui->libClLine->setText(settings->libclPath);

    if(!settings->MsSDKPath.isEmpty())
        ui->sdkLine->setText(settings->MsSDKPath);

    if(!settings->VStudioPath.isEmpty())
        ui->vsLine->setText(settings->VStudioPath);

    ui->saveFlow->setChecked(settings->saveFlow);

    ui->logging->setChecked(settings->enableLogging);

}

void GeneralSettings::slotSaveSettings()
{
    editor_settings->applyButtonPressed();

    settings->clPath = ui->clLine->text();

    settings->libclPath = ui->libClLine->text();

    settings->MsSDKPath = ui->sdkLine->text();

    settings->VStudioPath = ui->vsLine->text();

    settings->saveFlow = ui->saveFlow->isChecked();

    settings->enableLogging = ui->logging->isChecked();

    setSettingsToFile();
}

void GeneralSettings::setSettingsToFile()
{
    QSettings sets("WHC", "WHC IDE");

    sets.setValue("clPath", settings->clPath);
    sets.setValue("libclPath", settings->libclPath);
    sets.setValue("MsSDKPath", settings->MsSDKPath);
    sets.setValue("VStudioPath", settings->VStudioPath);
    sets.setValue("saveFlow", settings->saveFlow);
    sets.setValue("enableLogging", settings->enableLogging);

    sets.sync();
}
