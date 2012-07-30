#ifndef LOAD_INPUT_FILE_H
#define LOAD_INPUT_FILE_H

#include <QString>
#include <QFileInfo>

class load_input_file
{
private:
  int agency_map_id; ///< index in list to find agency
  int phone_map_id; ///< index in list to find phone

public:
  load_input_file (const QFileInfo &f);
  virtual ~load_input_file ();

  void parse_line (QString &line, size_t line_num = 0);
};

#endif // LOAD_INPUT_FILE_H
