#include <QtGui>
#include <QLibraryInfo>
#include <QtXmlPatterns>
#include <QToolBar>

#include "mainwindow.h"
#include "settings.h"
#include <proc/load_input_file.h>
#include <proc/phone_table_model.h>
#include <proc/save_output_file.h>


void MainWindow::createToolBars ()
{
  fileToolBar = addToolBar (tr ("File"));
//   fileToolBar->addAction(newAct);
  fileToolBar->addAction (actionOpenDirectory);
  fileToolBar->addAction (actionRun);
  fileToolBar->addAction (actionSave);

//   editToolBar = addToolBar(tr("Edit"));
//   editToolBar->addAction (cutAct);
//   editToolBar->addAction (copyAct);
//   editToolBar->addAction (pasteAct);
}

void MainWindow::load_file (const QFileInfo &f, phone_table_model *model)
{


}

MainWindow::MainWindow () : m_fileTree (m_namePool)
{
  setupUi (this);

  createToolBars ();

  model = new phone_table_model (this);

  // Set up the font.
  {
    QFont font ("Courier", 6);
    font.setFixedPitch (true);

//     tableView->setFont (font);
    queryEdit->setFont (font);
    output->setFont (font);
  }

  {
    progress = new QProgressBar;
    statusbar->addPermanentWidget (progress);
  }

  QString path;
  QFile black_list_file;
// ----------------------------------------------------------------------------
  // init black list
  path = set_app.value ("save_dir", "").toString ();
  if (!path.isEmpty ())
    {
      const QDir dir_save (path);
      qDebug () << path;

      if (dir_save.exists ())
        {
          QString path_absolut_black (dir_save.absoluteFilePath (model->black_list_file_name ()));
          QFile file_info (path_absolut_black);
          if (file_info.exists ())
            {
#if 0
              QThread tread_load;
              load_input_file load_data (file_info, *model);
              model->init_black_list = 0;
#else

              thread = new load_thread (file_info, model, 1);
              qRegisterMetaType<QFileInfo> ("QFileInfo");
              qRegisterMetaType<phone_table_model::black_type> ("black_type");
              connect (thread, SIGNAL (load_input_file_data_signal (const QFileInfo &, phone_table_model *)),
                       this, SLOT (load_file (const QFileInfo &, phone_table_model *)));

              connect (thread, SIGNAL (progress_do (int)),
                       progress, SLOT (setValue (int)));

              black_list_file.setFileName (path_absolut_black);
#endif
            }
        }
      else
        statusbar->showMessage (tr ("Use the Open menu entry to select a directory."));
    }

// ----------------------------------------------------------------------------
  path = set_app.value ("data_dir", ""/*QDir::homePath ()*/).toString ();
  QString dir;
  if (!path.isEmpty ())
    {
      dir = path;
      qDebug () << dir;
    }

  if (!dir.isEmpty () && QDir (dir).exists ())
    {
      loadDirectory (dir);
    }
  else
    statusbar->showMessage (tr ("Use the Open menu entry to select a directory."));

  if (thread && black_list_file.exists ())
    thread->load_input_file_data (black_list_file, model, 1);

// ----------------------------------------------------------------------------
}

void MainWindow::on_actionRun_triggered ()
{
  QByteArray output_files;
  QBuffer buffer_files (&output_files);
  buffer_files.open (QIODevice::WriteOnly);
  QFileInfo file_info;
  QString path;

  foreach (file_info, m_fileTree.m_fileInfos)
    {
      path = file_info.fileName ();
      qDebug () << path;
      output_files.append (path.toUtf8 () + "\n");

      if (file_info.isFile () && file_info.suffix () == "txt")
        load_input_file load_data (0, file_info, model, 0);
      else
        qDebug () << QString::fromUtf8 ("Пропускаю файл: %1").arg (path);
    }

  // sort by price
  QSortFilterProxyModel *proxy_model = new QSortFilterProxyModel (this);
  proxy_model->setSourceModel (model);
  tableView->setModel (proxy_model);
  tableView->setSortingEnabled (1);
  tableView->resizeColumnsToContents ();
  tableView->sortByColumn (12, Qt::DescendingOrder);
//   qDebug () << QString::fromUtf8 ("Чёрный список") << model->black_list;
//   qDebug () << QString::fromUtf8 ("Белый список") << model->gray_list;
}

void MainWindow::evaluateResult ()
{
  QXmlQuery query (m_namePool);
  query.bindVariable ("fileTree", m_fileNode);
  query.setQuery (QUrl ("qrc:/filetree/queries/listTXTFiles.xq"));

  QByteArray formatterOutput;
  QBuffer buffer (&formatterOutput);
  buffer.open (QIODevice::WriteOnly);

  QXmlFormatter formatter (query, &buffer);
  query.evaluateTo (&formatter);

  output->setText (QString::fromUtf8 (formatterOutput.constData ()));
}



void MainWindow::on_actionOpenDirectory_triggered ()
{
  const QString directoryName = QFileDialog::getExistingDirectory (this);

  if (!directoryName.isEmpty ())
    loadDirectory (directoryName);
}




void MainWindow::loadDirectory (const QString &directory)
{
  Q_ASSERT (QDir (directory).exists ());

  set_app.setValue ("data_dir", directory.toUtf8 ());

  m_fileNode = m_fileTree.nodeFor (directory);

  QXmlQuery query (m_namePool);
  query.bindVariable ("fileTree", m_fileNode);
  query.setQuery (QUrl ("qrc:/filetree/queries/listTXTFiles.xq"));

  treeInfo->setText (tr ("Load input files from of %1.").arg (directory));
  evaluateResult ();
}


void MainWindow::on_actionSave_triggered ()
{
  QString path = set_app.value ("save_dir").toString ();

  if (path.isEmpty ())
    {
      const QString directoryName = QFileDialog::getExistingDirectory (this);
      if (!directoryName.isEmpty ())
        path = directoryName;
      else
        return;
    }

  saveToDirectory (path);
}

void MainWindow::saveToDirectory (const QString &directory)
{
  QDir out_dir = QDir (directory);
  Q_ASSERT (out_dir.exists () && model);

  set_app.setValue ("save_dir", directory.toUtf8 ());

  save_output_file out_data (out_dir, *model);
  statusbar->showMessage (tr ("File saved to dir %1. Black %2. Gray %3")
    .arg (directory).arg (model->black_list.size ()).arg (model->gray_list.size ()));
}

void MainWindow::on_actionAbout_triggered ()
{
  QMessageBox::about (this, tr ("About phone base"),
    tr ("<p>Select <b>File->Open Directory</b> and "
        "choose a directory. The directory is then "
        "loaded into the model.</p>"

        "<p>Then select Run, and save.</p>"));
}
