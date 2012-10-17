#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QMainWindow>
#include <QToolBar>
#include <QProgressBar>
#include <QXmlNamePool>

#include "ui_mainwindow.h"
#include <settings.h>
#include "filetree.h"
#include <proc/phone_table_model.h>
#include <proc/load_input_file.h>

class MainWindow : public QMainWindow, private Ui_MainWindow
{
  Q_OBJECT

public:
  MainWindow ();

  settings set_app;
  phone_table_model *model;

private slots:
  void on_actionOpenDirectory_triggered ();
  void on_actionAbout_triggered ();
  void on_actionRun_triggered ();
  void on_actionSave_triggered ();

  void load_file (const QFileInfo &f, phone_table_model *model);

private:
  void createToolBars ();
  void loadDirectory (const QString &directory);
  void saveToDirectory (const QString &directory);
  void evaluateResult ();

  const QXmlNamePool  m_namePool;
  const FileTree      m_fileTree;
  QXmlNodeModelIndex  m_fileNode;

  QToolBar *fileToolBar;
  QToolBar *editToolBar;
  QProgressBar *progress;

  load_thread *thread;
};

#endif // MAINWINDOW_H
