#include "load_input_file.h"
#include "enc/encoding_utils.h"

#include <QtDebug>
#include <QTextCodec>
#include <QStringList>

#include "phone.h"
#include "phone_table_model.h"

void load_input_file::parse_line (QString &line, size_t line_num)
{
  static QString agency_id = QString::fromUtf8 ("Агентство");
  static QString phone_id  = QString::fromUtf8 ("Телефоны");

  if (line.isEmpty ())
    return;

  line.remove ("\r\n");
  QStringList list = line.split ("\t");

  if (line_num)
    {
      if (!init_black_list)
        {
          // Q_ASSERT ((init_black_list) && phone_map_id >= 0);
          if (init_black_list && phone_map_id >= 0)
            {
               // error
            }
          else
            {

            }
        }
      else
        {
          if (phone_map_id < 0)
            phone_map_id = 0;
        }

      if (list.size () < phone_map_id)
        {
          printf ("Error line size %d < phone id %d\n", list.size (), phone_map_id);
          return; //< skip line
        }

      phone p_num (list[phone_map_id], model->black_list, init_black_list);
      if (!p_num.is_ok)
        return; //< skip line

      if ((p_num.is_gray || !p_num.is_black) && !init_black_list)
        {
          // test grey list ??
          if (model->black_list.contains (p_num.phone_num))
            return; //< skip black line

          if (model->gray_list.contains (p_num.phone_num))
            {
              // in gray list - skip ??
            }
          else
            {
              model->gray_list.insert (p_num.phone_num, list);
            }
        }
      else
        {
          // black list
          model->black_list.insert (p_num.phone_num);
        }

//       qDebug () << phone_id << list[phone_map_id];
    }
  else
    {
      // parce title
      ;
      QString title_node;
      size_t num = 0;
      foreach (title_node, list)
        {
          model->title_map[title_node] = num++;
        }

//       ("К", "Метро", "ОтМ", "Улица", "№ дома", "Дом", "Площадь", "Б", "Т", "С", "П", "Цена,руб", "Дата", "Агентство", "Телефоны", "

// новая версия - формат поменялся 2013 04 24
//      К	Метро	От М	Улица	№ дома	Дом	Площадь	Б	Цена,руб	Цена,$	Дата	Источник	Телефоны

      phone_map_id = model->title_map.value (phone_id, -1);

//      qDebug () << phone_id << phone_map_id;
    }

//   qDebug () << list;
}

load_input_file::load_input_file (load_thread *thread, const QFileInfo &f, phone_table_model *model_, int init_black_list_, QProgressBar *progress)
: phone_map_id (-1), model (model_), init_black_list (init_black_list_)
{
  QFile input_file (f.absoluteFilePath ());

  input_file.open (QIODevice::ReadOnly);

  // parse line
  qint64 line_len = 0;
  const size_t line_len_max = 1024, line_len_utf8_max = line_len_max * 4;
  char line_str[line_len_max] = {};
  char line_str_utf8[line_len_utf8_max] = {};

  qint64 file_size = f.size (), read_size = 0;
  qint64 line_num = 0;
  int progress_cur, progress_prev = 0;
  if (thread)
    thread->progress_func (0);
  else
    progress->setValue (0);

  do
    {
      memset (line_str, 0, line_len_max * sizeof (char));
      line_len = input_file.readLine (line_str, line_len_max);
      read_size += line_len;

      if (line_len > 1)
        {
          convert_unknown_string_to_utf8 (line_str, line_str_utf8, line_len_utf8_max);
          QString local_line = QString::fromUtf8 (line_str_utf8);
          parse_line (local_line, line_num++);

          progress_cur = (int) ((read_size / (double) file_size) * 100.);
          if (progress_cur != progress_prev)
            {
              progress_prev = progress_cur;
              if (thread)
                thread->progress_func (progress_cur);
              else
                progress->setValue (progress_cur);
            }
        }
    } while (line_len > 0);

  if (thread)
    thread->progress_func (100);
  else
    progress->setValue (progress_cur);
  input_file.close ();
}

load_input_file::~load_input_file ()
{
}

void load_thread::progress_func (int progress)
{
  emit progress_do (progress);
}

load_thread::load_thread (const QFileInfo &f_, phone_table_model *model_, int init_black_list_, QObject *parent)
: QThread (parent), flags (0), f (f_), model (model_), init_black_list (init_black_list_)
{
  restart_flag = 0;
  abort_flag = 0;
}

load_thread::~load_thread ()
{
  mutex.lock ();
  abort_flag = 1;
  condition.wakeOne ();
  mutex.unlock ();

  wait ();
}

void load_thread::load_input_file_data (const QFileInfo &f_, phone_table_model *model_, int init_black_list_)
{
  QMutexLocker locker (&mutex);

  f = f_;
  model = model_;
  init_black_list = init_black_list_;

  if (!isRunning ())
    {
      start (LowPriority);
    }
  else
    {
      restart_flag = 1;
      condition.wakeOne ();
    }
}

void load_thread::run ()
{
  forever
    {
      mutex.lock ();
      /// get class data to local vars
      const QFileInfo &f_ = f;
      mutex.unlock ();

      if (restart_flag)
        break;
      if (abort_flag)
        return;

      load_input_file loader_for_file (this, f_, model, init_black_list);
      if (!restart_flag)
        emit load_input_file_data_signal (f, model);

      mutex.lock ();

      if (!restart_flag)
        condition.wait (&mutex);

      restart_flag = 0;
      mutex.unlock ();
    }
}
