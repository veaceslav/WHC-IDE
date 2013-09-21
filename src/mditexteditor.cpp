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
#include "editor/completemodel.h"

MdiTextEditor::MdiTextEditor(const QString &fileName, QWidget *parent) :
    QPlainTextEdit(parent), c(NULL)
{
    QFile file(fileName);

    notBrace.first = QChar();
    notBrace.second = QChar();

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

    QFont font;
    font.setFamily(ide->editorSettings->fontFamily);
    font.setFixedPitch(true);
    font.setPointSize(ide->editorSettings->fontSize);

    setFont(font);

    this->setTabStopWidth(10 * tmp_ide->editorSettings->tabSize);

    this->setStyleSheet(QString("font: ") +
                        tmp_ide->editorSettings->fontSize +
                        "pt \"" +
                        tmp_ide->editorSettings->fontFamily +
                        "\";");

    c = new QCompleter(parent);
    c->setModel(modelFromScope(this->textCursor().position()));
    c->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    c->setWrapAround(false);
    this->setCompleter(c);
    prevCursorPos = this->textCursor().position();

    connect(this, SIGNAL(cursorPositionChanged()), this,
            SLOT(slotCursorChanged()));

    complModel = new CompleteModel(this);

    connect(this, SIGNAL(getModel(int)), complModel, SLOT(slotGetModel(int)));
    connect(complModel, SIGNAL(gotModel(QStringListModel*)), this,
            SLOT(slotGotModel(QStringListModel*)));
}

MdiTextEditor::~MdiTextEditor()
{
    complModel->deleteLater();
}

QStringListModel *MdiTextEditor::modelFromScope(int position)
{
    //Don't know what it does but it was here before
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif

    QString text = this->toPlainText();

    if(position < 0)
        position = text.size();
    if(position > text.size())
        return NULL;

    QSet<QString> foundWords;
    QRegExp regex("[a-zA-Z_][a-zA-Z0-9_]*");
    int currentPos = regex.indexIn(text);
    QStringList words;

    while (currentPos != -1 && currentPos < position)
    {
        if(inScopeOf(currentPos, position))
            foundWords.insert(regex.cap());
        currentPos = regex.indexIn(text, currentPos + regex.matchedLength());
    }
    foreach(QString word, foundWords)
    {
        if(word.size() > 3)
            words<<word;
    }
    words.sort();

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    return new QStringListModel(words, c);
}

bool MdiTextEditor::inScopeOf(int a, int b)
{
    if(a > b || a < 0 || b < 0)
        return false;

    QString text = this->toPlainText();

    if(a >= text.size() || b > text.size())
        return false;

    int levelDiff = 0;

    for(int i = a; i < b; i++)
        if(text.at(i) == '{')
            levelDiff++;
        else if(text.at(i) == '}')
            levelDiff--;

    if(levelDiff < 0)
        return false;
    else
        return true;
}

int MdiTextEditor::getIndentLevel(QTextCursor cr)
{
    int charPos = cr.position();
    QString text = this->toPlainText();
    QLinkedList<QChar> stack;

    for(int i = 0; i < charPos; i++)
        if(text.at(i) == '{')
            stack.append('{');
        else if(text.at(i) == '}')
            if(!stack.isEmpty())
                stack.pop_back();
            /**
             * Else the stack is broken. This means the brackets do not close
             * properly
             */
    return stack.size();
}

void MdiTextEditor::matchIndent(QTextCursor cr, int level)
{
    /**
     * Selects all the text from the beggining of the line to the '}' character
     */
    cr.movePosition(QTextCursor::PreviousCharacter);
    cr.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    QString line = cr.selectedText();

    for(int i = 0; i < line.size(); i++)
        if(line.at(i) != ' ' && line.at(i) != '\t')
            return;

    QString indent;
    if(ide->editorSettings->tabToSpaces)
        for(int i = 0; i < ide->editorSettings->tabSize; i++)
            indent += " ";
    else
        indent = "\t";

    /**
     * Remove old spaces and replace them with the desired indent
     */
    cr.removeSelectedText();
    for(int i = 0; i < level; i++)
        cr.insertText(indent);
}

QPair<QChar, QChar> MdiTextEditor::getBracePair(QChar brace)
{
    QVector<QPair<QChar, QChar> >braces;
    braces<<QPair<QChar, QChar>('[', ']')<<QPair<QChar, QChar>('{', '}')
            <<QPair<QChar, QChar>('(', ')');

    for(int i = 0; i < braces.size(); i++)
        if(braces[i].first == brace || braces[i].second == brace)
            return braces[i];
    return notBrace;
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
        }
    }
    else
    {
        /**
         * @brief indent - a string with the character(s) that compose the
         *                 indentation
         */
        QString indent;
        if(ide->editorSettings->tabToSpaces)
            for(int i = 0; i < ide->editorSettings->tabSize; i++)
                indent += " ";
        else
            indent = '\t';

        if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
        {
            this->insertPlainText("\n");
            for(int i = 0; i < getIndentLevel(this->textCursor()); i++)
                this->insertPlainText(indent);

            e->ignore();
            return;
        }
        if(e->key() == Qt::Key_Tab && ide->editorSettings->tabToSpaces)
        {
            this->insertPlainText(indent);
            e->ignore();
            return;
        }
    }
    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) &&
                       e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // dont process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);

    /**
     * If the right bracket is pressed, the indent level will be decreased by 1
     */
    if(e->key() == Qt::Key_BraceRight)
        matchIndent(this->textCursor(), getIndentLevel(this->textCursor()) - 1);

    /**
     * Highlight matching brackets
     */
    {
        QTextCursor crs;
        switch(e->key())
        {
            case Qt::Key_ParenLeft:
            case Qt::Key_ParenRight:
            case Qt::Key_BraceLeft:
            case Qt::Key_BraceRight:
            case Qt::Key_BracketLeft:
            case Qt::Key_BracketRight:
                crs = this->textCursor();
                crs.movePosition(QTextCursor::PreviousCharacter);
                bracketMatch(crs);
        }
    }

    const bool ctrlOrShift = e->modifiers() &
                                     (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    QString wordCharacters =
              "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";

    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut &&
        (hasModifier || e->text().isEmpty() ||
         completionPrefix.length() < 3 || completionPrefix.at(0).isDigit() ||
         !wordCharacters.contains(e->text().right(1))))
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

void MdiTextEditor::slotGotModel(QStringListModel *model)
{
    c->setModel(model);
}

void MdiTextEditor::slotCursorChanged()
{
    bracketMatch(this->textCursor());

    int currentPos = this->textCursor().position() - 1;
    QString text = this->toPlainText();

    /**
     * Moves the cursor to the left of the current word (the word under the
     * cursor). A "word" is a variable name or a number.
     */
    while(currentPos > 0 &&
          (text.at(currentPos).isLetter() || text.at(currentPos).isDigit() ||
           text.at(currentPos) == '_'))
        currentPos--;

    prevCursorPos--;

    /**
     * Same as above
     */
    while(prevCursorPos > 0 &&
          (text.at(prevCursorPos).isLetter() ||
           text.at(prevCursorPos).isDigit() || text.at(prevCursorPos) == '_'))
        prevCursorPos--;

    if(prevCursorPos != currentPos)
        emit getModel(currentPos);
    prevCursorPos = this->textCursor().position();
}

void MdiTextEditor::bracketMatch(QTextCursor cursorStart)
{
    /**
     * @brief direction - the direction in which to look for the other bracket
     *                    -1 means up, 1 means down, 0 means no direction
     *                    (char is not brace)
     */
    int direction = 0;
    /**
     * @brief stackSize - the size of the stack of brackets. Must be 0 to be
     *                    able to close (no other brackets left on bracket
     *                    stack)
     */
    int stackSize = 1;

    int charPos = cursorStart.position();
    QString text = this->toPlainText();
    QChar selectedChar = '\0';
    if(!cursorStart.atEnd())
        selectedChar = text.at(charPos);

    /**
     * @brief otherBracket - the corresponding bracket that must be found
     */
    QChar otherBracket;
    /**
     * @brief extraSel - the list that will contain the matching brackets
     *                   selected
     */
    QList<QTextEdit::ExtraSelection> extraSel;

    QPair<QChar, QChar> bracePair = getBracePair(selectedChar);
    if(bracePair != notBrace)
    {
        if(selectedChar == bracePair.first)
        {
            direction = 1;
            otherBracket = bracePair.second;
        }
        else
        {
            direction = -1;
            otherBracket = bracePair.first;
        }
    }

    int pos;
    /**
     * If direction is 0, this means we don't have to look for matching brackets
     */
    if(direction)
        for(pos = charPos + direction; pos >= 0 && pos < text.size() &&
            stackSize; pos += direction)
        {
            if(text.at(pos) == selectedChar)
                stackSize++;
            else if(text.at(pos) == otherBracket)
                stackSize--;
        }

    /**
    * Selects initial bracket
    */
    cursorStart.movePosition(QTextCursor::NextCharacter,
                             QTextCursor::KeepAnchor);

    /**
     * The stack must have size 0, else the stack is broken, or the bracket is
     * not closed yet.
     */
    if(!stackSize)
    {
        pos -= direction;
        QTextCursor cursorEnd(cursorStart);
        cursorEnd.setPosition(pos);

        /**
        * Selects the other bracket
        */
        cursorEnd.movePosition(QTextCursor::NextCharacter,
                               QTextCursor::KeepAnchor);

        QTextCharFormat fmt;
        fmt.setBackground(Qt::green);

        QTextEdit::ExtraSelection b1, b2;
        b1.cursor = cursorStart;
        b2.cursor = cursorEnd;
        b1.format = b2.format = fmt;
        extraSel<<b1<<b2;
    }
    else if(direction)
    {
        QTextCharFormat fmt;
        fmt.setBackground(Qt::red);

        QTextEdit::ExtraSelection b1;
        b1.cursor = cursorStart;
        b1.format = fmt;
        extraSel<<b1;
    }
    this->setExtraSelections(extraSel);
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
            painter.drawText(0, top, lineNumberArea->width(),
                             fontMetrics().height(), Qt::AlignRight, number);
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
