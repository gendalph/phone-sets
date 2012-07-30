#include "load_input_file.h"
#include "enc/encoding_utils.h"

#include <QtDebug>
#include <QTextCodec>
#include <QStringList>

void load_input_file::parse_line (QString &line, size_t line_num)
{
  static QString agency_id = QString::fromUtf8 ("Агентство");
  static QString phone_id  = QString::fromUtf8 ("Телефоны");

  if (line.isEmpty ())
    return;

  QStringList list = line.split ("\t");

  if (line_num)
    {
      Q_ASSERT (agency_map_id >= 0 && phone_map_id >= 0);

      qDebug () << agency_id << list[agency_map_id];
      qDebug () << phone_id << list[phone_map_id];
    }
  else
    {
      // parce title
      QMap<QString, size_t> title_map;
      QString title_node;
      size_t num = 0;
      foreach (title_node, list)
        {
          title_map[title_node] = num++;
        }

//       ("К", "Метро", "ОтМ", "Улица", "№ дома", "Дом", "Площадь", "Б", "Т", "С", "П", "Цена,руб", "Дата", "Агентство", "Телефоны", "

      agency_map_id = title_map[agency_id];
      phone_map_id = title_map[phone_id];

      qDebug () << agency_id << agency_map_id;
      qDebug () << phone_id << phone_map_id;
    }

//   qDebug () << list;
}

load_input_file::load_input_file (const QFileInfo &f) : agency_map_id (-1), phone_map_id (-1)
{
  QFile input_file (f.absoluteFilePath ());

  input_file.open (QIODevice::ReadOnly);

  // parse line
  ssize_t line_len = 0;
  const size_t line_len_max = 1024 * 4;
  char line_str[line_len_max] = {};
  char line_str_utf8[line_len_max * 8] = {};

  size_t line_num = 0;
  do
    {
      memset (line_str, 0, line_len_max * sizeof (char));
      line_len = input_file.readLine (line_str, line_len_max);

      if (line_len > 1)
        {
          convert_unknown_string_to_utf8 (line_str, line_str_utf8, line_len_max * 8);
          QString local_line = QString::fromUtf8 (line_str_utf8);
//           QString local_line = codec->convertToUnicode (line_str, line_len - 1, &state_enc);
          parse_line (local_line, line_num++);
        }
    } while (line_len > 0);

  input_file.close ();
}

load_input_file::~load_input_file ()
{

}

