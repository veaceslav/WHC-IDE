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

#include "mditexteditor.h"
#include "mdisubwindow.h"
MdiTextEditor::MdiTextEditor(const QString &fileName, QWidget *parent) :
    QPlainTextEdit(parent), c(NULL)
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    setFont(font);

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("WHC"),
                             tr("Cannot open file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
    }
    else
    {
        QTextStream in(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        setPlainText(in.readAll());
        QApplication::restoreOverrideCursor();
    }
    file.close();
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this,
            SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this,
            SLOT(updateLineNumberArea(QRect,int)));

    updateLineNumberAreaWidth(0);
    MdiSubWindow *tmp_subwin = qobject_cast<MdiSubWindow*>(parent);
    Ide *tmp_ide = qobject_cast<Ide*>(tmp_subwin->p);
    ide = tmp_ide;
    this->setTabStopWidth(10 * tmp_ide->editorSettings->tabSize);

    this->setStyleSheet(QString("font: ") +
                        tmp_ide->editorSettings->fontSize +
                        "pt \"" +
                        tmp_ide->editorSettings->fontFamily +
                        "\";");

    c = new QCompleter(parent);
    c->setModel(modelFromFile());
    c->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    c->setWrapAround(false);
    this->setCompleter(c);
}

QAbstractItemModel *MdiTextEditor::modelFromFile()
{
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QSet<QString> ls;
    QRegExp rx("[_]?[a-zA-Z]+[_]*[a-zA-Z0-9]*");
    QString str = this->toPlainText();
    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1)
    {
        ls.insert(rx.cap(0));
        pos += rx.matchedLength();
    }
    foreach(QString s, ls)
    {
        if(s.size() > 3)
            words<<s;
    }
    words.sort();

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    completionModel = new QStringListModel(words, c);
    return completionModel;
}

void MdiTextEditor::setCompleter(QCompleter *completer)
{
    if (c)
        QObject::disconnect(c, 0, this, 0);

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}

QCompleter *MdiTextEditor::completer() const
{
    return c;
}

void MdiTextEditor::insertCompletion(const QString &completion)
{
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QString MdiTextEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void MdiTextEditor::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

void MdiTextEditor::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible())
    {
        // The following keys are forwarded by the completer to the widget
        switch (e->key())
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
        default:
            break;
        }
    }
    else
    {
        if(e->key() == Qt::Key_Return)
        {
            QTextCursor cr = this->textCursor();
            cr.select(QTextCursor::LineUnderCursor);
            QString str = cr.selectedText();
            QRegExp rx("[_]?[a-zA-Z]+[_]*[a-zA-Z0-9]*");
            int pos = 0;
            while ((pos = rx.indexIn(str, pos)) != -1)
            {
                if(!words.contains(rx.cap(0)) && rx.cap(0).size() > 3)
                {
                    words.append(rx.cap(0));
                }
                pos += rx.matchedLength();
            }
            words.sort();
            completionModel->setStringList(words);
            if(ide->editorSettings->tabToSpaces)
            {
                pos = cr.selectedText().indexOf(QRegExp("[\\w|#]+"));
                cr = this->textCursor();
                cr.insertText(QString("\n"));
                for(int i = 0; i < ide->editorSettings->tabSize *
                                        (pos/ide->editorSettings->tabSize); i++)
                    cr.insertText(" ");
                e->ignore();
                return;
            }
            else
            {
                pos = cr.selectedText().indexOf(QRegExp("[\\w|#]+"));
                cr = this->textCursor();
                cr.insertText(QString("\n"));
                for(int i = 0; i < pos; i++)
                    cr.insertText(QString("\t"));
                e->ignore();
                return;
            }
        }
        if(e->key() == Qt::Key_Tab)
        {
            if(ide->editorSettings->tabToSpaces == true)
            {
                for(int i = 0; i < ide->editorSettings->tabSize; i++)
                    this->insertPlainText(" ");
                e->ignore();
                return;
            }
        }
    }
    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) &&
                       e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // dont process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);
    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut &&
        (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3 ||
                                              eow.contains(e->text().right(1))))
    {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix())
    {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0) +
                c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}


bool MdiTextEditor::save()
{
    //    QFile file(fileName);
    //    if (!file.open(QFile::WriteOnly | QFile::Text)) {
    //        QMessageBox::warning(this, tr("MDI"),
    //                             tr("Cannot write file %1:\n%2.")
    //                             .arg(fileName)
    //                             .arg(file.errorString()));
    //        return false;
    //    }

    //    QTextStream out(&file);
    //    QApplication::setOverrideCursor(Qt::WaitCursor);
    //    out << toPlainText();
    //    QApplication::restoreOverrideCursor();

    //    setCurrentFile(fileName);
    //    return true;
    return false;
}

void MdiTextEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }

}

int MdiTextEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
    return space;
}

void MdiTextEditor::updateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void MdiTextEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void MdiTextEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(),
                                lineNumberAreaWidth(), cr.height()));
}

void MdiTextEditor::replaceSpacesLineEnd()
{
    QTextDocument *doc = this->document();
    QRegExp exp("\\s+$");
    QTextCursor cursor = this->textCursor();
    cursor.setPosition(QTextCursor::Start);
    this->setTextCursor(cursor);
    cursor = doc->find(exp);
    while(!cursor.isNull())
    {
        cursor.insertText(QString(""));
        cursor = doc->find(exp);
    }
}

void MdiTextEditor::goTo(int to)
{
    if(to <= this->blockCount())
    {
        this->setFocus();
        QTextCursor c = this->textCursor();
        int currentLocation = this->textCursor().blockNumber();
        if(currentLocation < to)
        {
            c.movePosition(QTextCursor::NextBlock,
                           QTextCursor::MoveAnchor,
                           to - currentLocation-1);
        }
        else
        {
            c.movePosition(QTextCursor::PreviousBlock,
                           QTextCursor::MoveAnchor,
                           currentLocation - to + 1);
        }
        this->setTextCursor(c);
    }
    else
    {
        QMessageBox::warning(0,
                             tr("Warning!"),
                             tr("This line does not exists.\n"
                                "Check text field!"));
    }
}
