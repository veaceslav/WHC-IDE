/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) 2012 by Cosmin Mihai <yonutix.mc@gmail.com>
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

#include "editorSettings.h"

EditorSettings::EditorSettings(Ide* parent, Ui::settingsDialog* win)
{
    this->parent = parent;
    this->win = win;
    colors = win->tab_3;
    fonts = win->tab_4;
    setFontTab();
    setColorsTab();
    QObject::connect(win->checkBox, SIGNAL(stateChanged(int)),
                     this, SLOT(checkButtonChanged(int)));
    QObject::connect(win->pushButton_3, SIGNAL(clicked()),
                     this, SLOT(resetSettings()));

    loadSettings();
    QString tmp_currentText = win->comboBox->currentText();
    loadHighlight(tmp_currentText);

    QObject::connect(this->win->listWidget_2, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(listItemChanged(QListWidgetItem*)));
    QObject::connect(this->win->chooseColor, SIGNAL(clicked()),
                     this, SLOT(onChooseCOlorClicked()));
    QObject::connect(this->win->checkBox_2, SIGNAL(stateChanged(int)),
                     this, SLOT(endLineHighlightChange(int)));

}


void EditorSettings::applyButtonPressed()
{
    if(!checkSettings()){
#ifndef DEBUG
        QMessageBox::warning(0,
                             tr("Warning!"),
                             tr("The settings specified are not valid.\n"
                                "Those settings will not be saved!."));
#endif
        return;
    }

    saveSettings();

    applySettings();

    saveHighlightSettings();
}


void EditorSettings::setFontTab()
{
    QMdiSubWindow *subWin = parent->getUi()->mdiArea->currentSubWindow();
    if(subWin == NULL)
        return;

    QPlainTextEdit *currentDoc = qobject_cast<QPlainTextEdit*>(subWin->widget());
    if(currentDoc == NULL)
        return;

    QString num;
    win->lineEdit->setText(num.setNum(currentDoc->tabStopWidth()/10));

}

EditorSettings::~EditorSettings()
{
}

void EditorSettings::checkButtonChanged(int state)
{
    QString spaces;
    for(int i = 0; i < getTabSize(); i++){
        spaces += " ";
    }
    if(state == Qt::Unchecked){
        setTabToSpaces(false);
        foreach (QMdiSubWindow *window, parent->getUi()->mdiArea->subWindowList()) {
            QPlainTextEdit *doc =
                    qobject_cast<QPlainTextEdit*>(window->widget());
            if(doc != NULL){
                QTextCursor c = doc->textCursor();
                c.movePosition(QTextCursor::Start);
                doc->setTextCursor(c);
                while(doc->find(spaces)){
                    doc->textCursor().insertText("\t");
                }
            }
        }
    }
    else{
        setTabToSpaces(true);

        foreach (QMdiSubWindow *window, parent->getUi()->mdiArea->subWindowList()) {
            QPlainTextEdit *doc =
                    qobject_cast<QPlainTextEdit*>(window->widget());
            if(doc != NULL){
                QTextCursor c = doc->textCursor();
                c.movePosition(QTextCursor::Start);
                doc->setTextCursor(c);
                while(doc->find("\t")){
                    doc->textCursor().insertText(spaces);
                }
            }
        }
    }
}

void EditorSettings::loadSettings()
{
    QSettings sets("WHC","WHC IDE");
    QString line;
    QString set = sets.value("tabSize").toString();
    win->lineEdit->setText(QString::number(getTabSize()));
    line = sets.value("tabToSpaces").toString();
    if(line.toInt() == 0){
        setTabToSpaces(false);
        win->checkBox->setChecked(false);
    }
    else{
        setTabToSpaces(true);
        win->checkBox->setChecked(true);
    }
    line = sets.value("fontFamily").toString();
    setfontFamily(line);
    win->fontComboBox->setCurrentFont(getFontFamily());
    line = sets.value("fontSize").toString();
    setFontSize(line.toInt());
    win->font_size->setText(QString::number(getFontSize()));
    line = sets.value("endLineSpaces").toString();
    if(line.toInt() == 0){
        win->checkBox_2->setChecked(false);
    }
    else{
        win->checkBox_2->setChecked(true);
    }
}

void EditorSettings::saveSettings()
{

    QSettings sets("WHC","WHC IDE");
    sets.setValue("tabSize",win->lineEdit->text());
    parent->editorSettings->tabSize = win->lineEdit->text().toInt();
    if(win->checkBox->isChecked()){
        sets.setValue("tabToSpaces","1");
        parent->editorSettings->tabToSpaces = true;
    }
    else{
        sets.setValue("tabToSpaces","0");
        parent->editorSettings->tabToSpaces = false;
    }
    sets.setValue("fontFamily",win->fontComboBox->currentFont().family());
    parent->editorSettings->fontFamily = win->fontComboBox->currentFont().family();
    sets.setValue("fontSize",win->font_size->text());
    parent->editorSettings->fontSize = win->font_size->text().toInt();
    if(win->checkBox_2->isChecked()){
        sets.setValue("endLineSpaces","1");
        sets.setValue("fontSize",win->font_size->text());

        parent->editorSettings->endLineSpace = true;
    }
    else{
        sets.setValue("endLineSpaces","0");
        parent->editorSettings->endLineSpace = false;
    }
}

bool EditorSettings::checkSettings()
{
    bool ok;
    if(win->comboBox->currentText().isEmpty()){
#ifndef DEBUG
        QMessageBox::warning(0,
                             tr("Warning!"),
                             tr("The combo box is empty.\n"
                                "Those settings will not be saved!."));
#endif
        return false;
    }
    win->lineEdit->text().toInt(&ok, 10);
    if(!ok){
#ifndef DEBUG
        QMessageBox::warning(0,
                             tr("Warning!"),
                             tr("The settings specified are not valid.\n"
                                "Those settings will not be saved!."));
#endif
        return false;
    }
    win->font_size->text().toInt(&ok, 10);
    if(!ok){
#ifndef DEBUG
        QMessageBox::warning(0,
                             tr("Warning!"),
                             tr("The settings specified are not valid.\n"
                                "Those settings will not be saved!."));
#endif
        return false;
    }
    return true;
}

void EditorSettings::applySettings()
{
    foreach (QMdiSubWindow *window, parent->getUi()->mdiArea->subWindowList()) {
        QPlainTextEdit *doc =
                qobject_cast<QPlainTextEdit*>(window->widget());
        if(doc != NULL){
            doc->setTabStopWidth(10*win->lineEdit->text().toInt());
            doc->setStyleSheet(QString("font: ")+
                               win->font_size->text()+
                               "pt \""+
                               win->fontComboBox->currentFont().family()+
                               "\";");
        }
    }
}


void EditorSettings::resetSettings()
{
    setTabSize(8);
    win->lineEdit->setText(QString::number(8));
    setTabToSpaces(false);
    win->checkBox->setChecked(false);
    QString tmp_fontFamily = QString("Courier New");
    setfontFamily(tmp_fontFamily);
    win->fontComboBox->setCurrentFont(getFontFamily());
    setFontSize(10);
    win->font_size->setText(QString::number(getFontSize()));
    win->checkBox_2->setChecked(true);
}

void EditorSettings::setColorsTab()
{
    foreach(Highlighter* h, parent->langs){
        win->comboBox->addItem(h->getName());
    }
}

void EditorSettings::loadHighlight(QString &lang)
{
    bool ok = true;
    if(parent == NULL){
        return;
    }
    if(!parent->langs.contains(lang)){
        return;
    }
    Highlighter * h = parent->langs[lang];
    foreach(Type *t, h->getTypes()){
        win->listWidget_2->addItem(t->first);
        if(ok){
            loadKeyWords(t->second);
            ok = false;
        }
    }
    if(h->getTypes().size() > 0){
        win->listWidget_2->setCurrentRow(0);
    }

}

void EditorSettings::loadKeyWords(QVector<QRegExp> &w)
{
    QString kw;
    foreach(QRegExp r, w){
        kw += r.pattern()+QString(" ");
    }
    win->plainTextEdit->clear();
    win->plainTextEdit->insertPlainText(kw);
}


void EditorSettings::listItemChanged(QListWidgetItem * item)
{
    QString tmp_itemText = item->text();
    QVector<QRegExp> tmp_v =
            parent->langs[win->comboBox->currentText()]->getType(tmp_itemText);
    loadKeyWords(tmp_v);
}

void EditorSettings::onChooseCOlorClicked()
{
    QColor color = QColorDialog::getColor(Qt::green, win->tabWidget_2);
    QTextCharFormat cf;
    cf.setForeground(QBrush(color));
    Highlighter *h = getCurrentHighlighter();
    QString tmp_currentIndex;
    if (win->listWidget_2->currentItem()) {
        tmp_currentIndex = win->listWidget_2->currentItem()->text();
        h->sh->setStyle(tmp_currentIndex, cf);
     }
}

void EditorSettings::saveHighlightSettings()
{
    QFile output(QString("Global_settings/lang/") + win->comboBox->currentText() + QString(".lang"));
    output.open(QIODevice::WriteOnly);
    QXmlStreamWriter stream(&output);
    stream.setAutoFormatting(true);
    stream.writeComment(getCurrentHighlighter()->getLicence());
    stream.writeStartElement("language");
    stream.writeAttribute("id", win->comboBox->currentText());
    Highlighter * h = getCurrentHighlighter();
    foreach(Type *t, h->getTypes()){
        stream.writeStartElement("context");
        stream.writeAttribute("id", t->first);
        foreach(QRegExp ks, t->second){
            stream.writeStartElement("keyword");
            stream.writeCharacters(ks.pattern());
            stream.writeEndElement();
        }
        stream.writeEndElement();
    }
    stream.writeEndElement();
    output.close();

    QFile out(QString("Global_settings/lang/format_scheme/") + win->comboBox->currentText() + QString(".lang"));
    out.open(QIODevice::WriteOnly);
    QXmlStreamWriter streamc(&out);
    streamc.setAutoFormatting(true);
    streamc.writeStartElement("format");
    streamc.writeStartElement("format_scheme");
    streamc.writeAttribute("name", "scheme_1");
    foreach(Style *t, h->sh->styles){
        streamc.writeStartElement("color");
        streamc.writeAttribute("name", t->first);
        streamc.writeAttribute("format", "#");
        streamc.writeCharacters(t->second.foreground().color().name());
        streamc.writeEndElement();
    }
    streamc.writeEndElement();
    streamc.writeEndElement();
    output.close();
}

void EditorSettings::endLineHighlightChange(int state)
{
    parent->editorSettings->endLineSpace = (state == Qt::Checked);
    parent->langs[win->comboBox->currentText()]->sh->rehighlight();
}
