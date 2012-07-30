#include "load_input_file.h"
#include "enc/encoding_utils.h"

#include <QtDebug>
#include <QTextCodec>
#include <QStringList>

void load_input_file::parse_line (QString &line)
{
  if (line.isEmpty ())
    return;

  QStringList list = line.split ("\t");

  qDebug () << list;
}

load_input_file::load_input_file (const QFileInfo &f)
{
  QFile input_file (f.absoluteFilePath ());

  input_file.open (QIODevice::ReadOnly);

  // parse line
  ssize_t line_len = 0;
  const size_t line_len_max = 1024 * 4;
  char line_str[line_len_max] = {};
  char line_str_utf8[line_len_max * 8] = {};

//   QTextCodec *codec = QTextCodec::codecForName ("Windows-1250");
//   QTextCodec::setCodecForCStrings (codec);
//   QTextCodec::ConverterState state_enc;

  do
    {
      memset (line_str, 0, line_len_max * sizeof (char));
      line_len = input_file.readLine (line_str, line_len_max);

      if (line_len > 1)
        {
          convert_unknown_string_to_utf8 (line_str, line_str_utf8, line_len_max * 8);
          QString local_line = QString::fromUtf8 (line_str_utf8);
//           QString local_line = codec->convertToUnicode (line_str, line_len - 1, &state_enc);
          parse_line (local_line);
        }
    } while (line_len > 0);

  input_file.close ();
}

load_input_file::~load_input_file ()
{

}

