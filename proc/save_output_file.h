#ifndef SAVE_OUTPUT_FILE_H
#define SAVE_OUTPUT_FILE_H
#include <QDir>

class phone_table_model;

class save_output_file
{

public:
  save_output_file (QDir &out_dir, phone_table_model &model);
};

#endif // SAVE_OUTPUT_FILE_H
