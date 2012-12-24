/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) Copyright (C) 2010 Cosmin Mihai <yonutix.mc@gmail.>
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

#include "findtool.h"

FindToolUI::FindToolUI()
{
    find_w = new QWidget();

    /*Main find section constructor*/
    find_h_box = new QHBoxLayout(find_w);
    regex_bt = new QCheckBox();
    regex_lb = new QLabel("Regex");
    sensitive_bt = new QCheckBox();
    sensitive_lb = new QLabel("Case Sensitive");
    find_lb = new QLabel("Find:");
    srch = new QLineEdit();
    find_bt = new QPushButton();
    next_prev_bt = new QPushButton();

    find_bt->setText("Search");

    next_prev_bt->setText("Next");
    next_prev_bt->setCheckable(true);
    connect(next_prev_bt, SIGNAL(clicked()),
            this, SLOT(on_next_prev_button_toggle()));


    /*Search in all files section*/
    searchInFilesMainGUI();


    find_h_box->addWidget(all_files);
    find_h_box->addWidget(regex_lb);
    find_h_box->addWidget(regex_bt);
    find_h_box->addWidget(sensitive_lb);
    find_h_box->addWidget(sensitive_bt);
    find_h_box->addWidget(find_lb);
    find_h_box->addWidget(srch);
    find_h_box->addWidget(find_bt);
    find_h_box->addWidget(next_prev_bt);

    /*Optional GUI parts*/
    replace_w = NULL;
    replace_h_box = NULL;
    replace_lb = NULL;
    rplc = NULL;
    replace_bt = NULL;
    replace_all_bt = NULL;

    search_sol_w = NULL;
    results_list = NULL;

    ext = NULL;
    ext_lb = NULL;

}

void FindToolUI::searchInFilesMainGUI()
{
    all_files = new QWidget();
    v_box_layout = new QVBoxLayout(all_files);
    all_files_unchecked = new QWidget();
    h_box_layout = new QHBoxLayout(all_files_unchecked);
    search_solution = new QCheckBox();
    search_solution_lb = new QLabel("Search entire project");
    h_box_layout->addWidget(search_solution_lb);
    h_box_layout->addWidget(search_solution);
    v_box_layout->addWidget(all_files_unchecked);
    connect(search_solution, SIGNAL(stateChanged(int)),
            this, SLOT(on_switch_simple_complex_search()));
    find_h_box->insertWidget(0, all_files);
}

FindToolUI::~FindToolUI()
{
    removeSearchInFileUI();
    collapseReplace();

    /*Main find section*/
    delete next_prev_bt;
    delete find_bt;
    delete srch;
    delete find_lb;
    delete sensitive_bt;
    delete sensitive_lb;
    delete regex_bt;
    delete regex_lb;
    delete find_h_box;
}



void FindToolUI::on_switch_simple_complex_search()
{
    if(search_solution->isChecked()){
        expandSearchInFilesGUI();
    }
    else{
        collapseSearchInFileUI();
    }
}

void FindToolUI::expandSearchInFilesGUI()
{
    ext = new QLineEdit();
    ext_lb = new QLabel("File name filter regex:");
    v_box_layout->addWidget(ext_lb);
    v_box_layout->addWidget(ext);
}


QWidget *FindToolUI::getFindUI()
{
    return find_w;
}

QWidget *FindToolUI::getReplaceUI()
{
    return replace_w;
}

QString FindToolUI::searchedText()
{
    return srch->text();
}

void FindToolUI::on_next_prev_button_toggle()
{
    if(next_prev_bt->isChecked()){
        next_prev_bt->setText("Previous");
    }
    else{
        next_prev_bt->setText("Next");
    }
}



void FindToolUI::extendReplaceGUI()
{
    replace_w = new QWidget();

    replace_h_box = new QHBoxLayout(replace_w);
    replace_lb = new QLabel("Replace with:");
    rplc = new QLineEdit();
    replace_bt = new QPushButton("Replace");
    replace_all_bt = new QPushButton("Replace all");

    replace_h_box->addWidget(replace_lb);
    replace_h_box->addWidget(rplc);
    replace_h_box->addWidget(replace_bt);
    replace_h_box->addWidget(replace_all_bt);
}

void FindToolUI::makeSearchInFiles(QDir &dirName, QList<QString> &searchResult)
{
    QStringList lists =
            dirName.entryList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);

    foreach(QString fl, lists){
        QString filepath = dirName.absolutePath()+QString("/")+fl;

        if(QFileInfo(filepath).isDir()){
            QDir child = QDir(filepath);
            makeSearchInFiles(child, searchResult);
        }

        if(QFileInfo(filepath).isFile() ){
            bool file_ok  = true;
            if(ext->text() != ""){
                QRegExp expression(ext->text());
                if(!(expression.indexIn(QFileInfo(filepath).fileName()) > -1)){
                    file_ok = false;
                }
            }
            if(file_ok){
                QFile f(filepath);
                QString stringFound;
                if(!f.open(QIODevice::ReadOnly)){
                    stringFound = "File " + filepath + " Could not be read!";
                    searchResult.append(QString(stringFound));
                }
                else{
                    QTextStream textStream(&f);
                    QString toSearch = this->srch->text();
                    while(!textStream.atEnd()){
                        QString line = textStream.readLine();
                        if(!regex_bt->isChecked()){
                            if(sensitive_bt->isChecked()){
                                if((line.contains(toSearch) == true) &&
                                        (line.size() < 300)){
                                    line.replace(toSearch,
                                                 QString("<b><font color=\"#0000FF\">")+
                                                 toSearch+"</font></b>");
                                    stringFound =
                                            QString("<font color=\"#9966CC\">") +
                                            filepath + QString("</font>: ") +
                                            line;
                                    searchResult.append(stringFound);
                                }
                            }
                            else{
                                if(line.contains(toSearch, Qt::CaseInsensitive) ==
                                        true && line.size() < 300){
                                    line.replace(toSearch,
                                                 QString("<b><font color=\"#0000FF\">")+
                                                 toSearch+"</font></b>");
                                    stringFound = QString("<font color=\"#9966CC\">") +
                                            filepath + QString("</font>: ") +
                                            line;
                                    searchResult.append(stringFound);
                                }
                            }
                        }
                        else{
                            QRegExp searchedExp(toSearch);
                            if(searchedExp.indexIn(line) > -1){
                                int i = searchedExp.indexIn(line);
                                while(i > -1){
                                    QString replacer =
                                            QString("<b><font color=\"#0000FF\">")+
                                                    line.mid(i, searchedExp.matchedLength())+
                                                    "</font></b>";
                                    stringFound =
                                            line.replace(i,
                                                         searchedExp.matchedLength(),
                                                         replacer);
                                    i = searchedExp.indexIn(line,
                                                            i+
                                                            searchedExp.matchedLength());
                                }
                                searchResult.append(stringFound);
                            }
                        }
                    }
                }
                f.close();
            }
        }
    }
}

void FindToolUI::showSearchInFilesResults(QString toSearch)
{
    QDir dir(toSearch+QString("/"));
    QList<QString> list;
    makeSearchInFiles(dir, list);

    if(search_sol_w == NULL){
        search_sol_w = new QWidget();
        QVBoxLayout *qvbl = new QVBoxLayout(search_sol_w);
        results_list = new QListView();
        QStringListModel* localModel = new QStringListModel();
        localModel->setStringList(list);
        results_list->setModel(localModel);
        qvbl->addWidget(results_list);
        FindListViewDelegate* delegate = new FindListViewDelegate();
        results_list->setItemDelegate(delegate);
    }
    else{
        QStringListModel* localModel=
                qobject_cast<QStringListModel*>(results_list->model());
        localModel->setStringList(list);
    }
}

void FindToolUI::collapseSearchInFileUI()
{
    if(all_files != NULL){
        if(search_sol_w != NULL){
            delete results_list;
            delete search_sol_w;
            results_list = NULL;
            search_sol_w = NULL;
        }
        if(ext_lb != NULL){
            delete ext_lb;
            delete ext;
            ext_lb = NULL;
            ext = NULL;
        }
    }
}

void FindToolUI::removeSearchInFileUI()
{
    collapseSearchInFileUI();
    if(all_files != NULL){
        delete search_solution_lb;
        delete search_solution;
        delete h_box_layout;
        delete all_files_unchecked;
        delete v_box_layout;
        delete all_files;

        search_solution_lb = NULL;
        search_solution = NULL;
        all_files_unchecked = NULL;
        h_box_layout = NULL;
        v_box_layout = NULL;
        all_files = NULL;
    }
}

void FindToolUI::collapseReplace()
{
    if(replace_w != NULL){
        delete replace_bt;
        delete replace_all_bt;
        delete rplc;
        delete replace_lb;
        delete replace_h_box;
        delete replace_w;
        replace_bt = NULL;
        replace_all_bt = NULL;
        rplc = NULL;
        replace_lb = NULL;
        replace_h_box = NULL;
        replace_w = NULL;
    }
}


GoTo::GoTo()
{
    goTo_w = new QWidget();
    goto_box_h = new QHBoxLayout(goTo_w);
    goto_edit = new QLineEdit();
    goto_lb = new QLabel("Go to: ");
    goto_box_h->addWidget(goto_lb);
    goto_box_h->addWidget(goto_edit);

}

GoTo::~GoTo()
{
    delete goto_lb;
    delete goto_edit;
    delete goto_box_h;
    delete goTo_w;
}

QWidget *GoTo::getGotoObj()
{
    return goTo_w;
}

QString GoTo::getText()
{
    return goto_edit->text();
}

QLineEdit* GoTo::getLineEdit()
{
    return goto_edit;
}
