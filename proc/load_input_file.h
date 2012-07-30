#ifndef LOAD_INPUT_FILE_H
#define LOAD_INPUT_FILE_H

#include <QString>
#include <QFileInfo>

class load_input_file
{

public:
  load_input_file (const QFileInfo &f);
  virtual ~load_input_file ();

  void parse_line (QString &line);
};

#endif // LOAD_INPUT_FILE_H
