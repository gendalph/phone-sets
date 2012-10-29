#include "save_output_file.h"

#include <QTextStream>

#include "phone_table_model.h"

save_output_file::save_output_file (QDir &out_dir, phone_table_model &model)
{
  if (!model.black_list.isEmpty ())
    {
      QFile output_file (out_dir.absoluteFilePath (model.black_list_file_name ()));
      Q_ASSERT (output_file.open (QIODevice::WriteOnly | QIODevice::Text));
      QTextStream out_text (&output_file);

      static QString phone_id  = QString::fromUtf8 ("Телефоны");
      out_text << phone_id << "\n";

      QList<phone_table_model::black_type::key_type> list_to_print = model.black_list.toList ();
      qSort (list_to_print.begin (), list_to_print.end ());
      phone_table_model::black_type::key_type item;
      foreach (item, list_to_print)
        {
          out_text << item << "\n";
        }
    }

  if (!model.gray_list.isEmpty ())
    {
      QFile output_file (out_dir.absoluteFilePath (model.gray_list_file_name ()));
      Q_ASSERT_X (output_file.open (QIODevice::WriteOnly | QIODevice::Text), "save_output_file", output_file.errorString ().data ().c_str ());
      QTextStream out_text (&output_file);

      static QString phone_id  = QString::fromUtf8 ("Телефоны");
      out_text << phone_id << "\n";

      foreach (const QStringList &item, model.gray_list)
        {
          foreach (const QString &item_l, item)
            out_text << "\t" << item_l;
          out_text << "\n";
        }
    }
}

