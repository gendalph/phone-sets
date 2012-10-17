#ifndef LOAD_INPUT_FILE_H
#define LOAD_INPUT_FILE_H

#include <QString>
#include <QFileInfo>
#include <QSet>
#include <QMap>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class phone_table_model;
class load_thread;

class load_input_file
{
public:

private:
  int agency_map_id; ///< index in list to find agency
  int phone_map_id; ///< index in list to find phone

public:
  void parse_line (QString &line, size_t line_num = 0);

  phone_table_model *model;

  load_input_file (load_thread *thread, const QFileInfo &f, phone_table_model *model, int init_black_list);
  virtual ~load_input_file ();

  int init_black_list;
};

class load_thread : public QThread
{
  Q_OBJECT

private:
  QMutex mutex;
  QWaitCondition condition;
  QFileInfo f;
  phone_table_model *model;

  int init_black_list;

  union
  {
    struct
    {
      unsigned int restart_flag;
      unsigned int abort_flag;
    };

    unsigned int flags;
  };

signals:
  void load_input_file_data_signal (const QFileInfo &f, phone_table_model *model);
  void progress_do (int progress);

protected:
  void run ();

public:
  load_thread (const QFileInfo &f_, phone_table_model *model_, int init_black_list, QObject *parent = 0);
  void load_input_file_data (const QFileInfo &f_, phone_table_model *model_, int init_black_list);
  void progress_func (int progress);

  ~load_thread ();
};

#endif // LOAD_INPUT_FILE_H
