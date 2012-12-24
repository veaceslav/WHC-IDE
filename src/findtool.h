/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) Copyright (C) 2012 Cosmin Mihai <yonutix.mc@gmail.com>
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

#include <QMainWindow>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QPushButton>
#include <QLayout>
#include <QSyntaxHighlighter>
#include <QTextEdit>
#include <QTextCursor>
#include <QPlainTextEdit>
#include <QKeySequence>
#include <QDockWidget>
#include <QWidget>
#include <QCheckBox>
#include <QListView>
#include <QStyledItemDelegate>
#include <QStringList>
#include <QStringListModel>
#include <QPainter>
#include <QDir>
#include <QTextStream>

#include "mdisubwindow.h"
#include "mditexteditor.h"



class FindListViewDelegate:public QStyledItemDelegate{
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QStyleOptionViewItemV4 options = option;
        initStyleOption(&options, index);

        painter->save();

        QTextDocument doc;
        doc.setHtml(options.text);

        options.text = "";
        options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

        painter->translate(options.rect.left(), options.rect.top());
        QRect clip(0, 0, options.rect.width(), options.rect.height());
        doc.drawContents(painter, clip);

        painter->restore();

    }
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const
    {
        QSize result = QStyledItemDelegate::sizeHint(option, index);
        return result;
    }

};


/**
  * \class Class for find tool interface and core
  * Basically in GUI are three main components: The find component,
  * the replace component and the output list for search in entire
  * project results.
  */
class FindToolUI:QObject{
    Q_OBJECT
public:
    /*Main find component - is always initialized*/
    QWidget *find_w;
    QHBoxLayout *find_h_box;
    QCheckBox *regex_bt;
    QLabel *regex_lb;
    QCheckBox *sensitive_bt;
    QLabel *sensitive_lb;
    QLabel *find_lb;
    QLineEdit *srch;
    QPushButton * next_prev_bt;
    QPushButton *find_bt;

    /*Replace component - initialized on request*/
    QWidget *replace_w;
    QHBoxLayout *replace_h_box;
    QLabel *replace_lb;
    QLineEdit *rplc;
    QPushButton *replace_bt, *replace_all_bt;


    QWidget *all_files;
    QVBoxLayout *v_box_layout;
    QWidget *all_files_unchecked;
    QHBoxLayout *h_box_layout;
    QCheckBox *search_solution;
    QLabel *search_solution_lb;

    QLabel *ext_lb;
    QLineEdit *ext;

    /*Search output component - initialized on request*/
    QWidget *search_sol_w;
    QListView *results_list;



    FindToolUI();
    ~FindToolUI();

    QWidget* getFindUI();
    QWidget* getReplaceUI();

    /**
      \return Returns the text from the main text field
      */
    QString searchedText();

    /*Extends tool also for replace*/
    void extendReplaceGUI();

    /**
      * \brief Removes the replace GUI features
      */
    void collapseReplace();

    /**
      * \brief Put the results of the search in a list and shows it
      */
    void showSearchInFilesResults(QString toSearch);

    /**
      * \brief Make a recursive search in the project directory
      * the function yelds results trough \param searchResults
      */
    void makeSearchInFiles(QDir &dirName, QList<QString> &searchResult);

    /**
      * \brief Removes only the field for files extensions filter
      * and eventually the output list
      */
    void collapseSearchInFileUI();

    /**
      * \brief A new text field appear for searching the entire project
      * filtering the files
      */
    void expandSearchInFilesGUI();

    /**
      * \brief Removes the checkbox for "Search entire project" and eventualy
      * its extension (when replace form appear)
      */
    void removeSearchInFileUI();

    /**
      * \brief Constructs the Checkbox and frame for "search in files" GUI
      */
    void searchInFilesMainGUI();


public slots:
    /**
      * \brief When switching the next/previous direction
      */
    void on_next_prev_button_toggle();

    /**
      * \brief It activates when the checkbutton for
      * "Search entire project" is pressed
      */
    void on_switch_simple_complex_search();
};

class GoTo:QObject{
    Q_OBJECT

    QWidget *goTo_w;
    QHBoxLayout *goto_box_h;
    QLineEdit *goto_edit;
    QLabel * goto_lb;

public:
    GoTo();
    ~GoTo();
    QWidget *getGotoObj();
    QString getText();
    QLineEdit *getLineEdit();
};
