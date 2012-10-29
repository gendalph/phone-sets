#include "save_output_file.h"

#include <QTextStream>
#include <QDebug>

#include "phone_table_model.h"

save_output_file::save_output_file (QDir &out_dir, phone_table_model &model, QSortFilterProxyModel &proxy_model)
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
      QString output_file_str = out_dir.absoluteFilePath (model.gray_list_file_name ());
      QFile output_file (output_file_str);
      qDebug () << output_file_str;
      Q_ASSERT_X (output_file.open (QIODevice::WriteOnly | QIODevice::Text), "save_output_file", output_file.errorString ().toLocal8Bit ());
      QTextStream out_text (&output_file);

      static QString phone_id  = QString::fromUtf8 ("Телефоны");
      out_text << phone_id << "\n";

      QList<phone_table_model::gray_type::key_type> keys = model.gray_list.keys ();
      for (int proxy_row = 0; proxy_row < proxy_model.rowCount (); proxy_row++)
        {
          int i_row = proxy_model.mapToSource (proxy_model.index (proxy_row, 0)).row ();
          qDebug () << i_row << keys[i_row] << model.gray_list[keys[i_row]];
          foreach (const QString &item_l, model.gray_list[keys[i_row]])
            out_text << "\t" << item_l;
          out_text << "\n";
        }
    }
}

