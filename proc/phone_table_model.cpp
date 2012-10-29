#include "phone_table_model.h"
#include <QDateTime>
#include <QDateTime>

QString phone_table_model::gray_list_file_name ()
{
  QString res = "gray_list.txt";
  if (1)
    {
      QDateTime current_date_time = QDateTime::currentDateTime ();
      res = current_date_time.toString (QString ("yyyy-MM-dd--hh-mm-ss")) + ".txt";
    }

  return res;
}

QVariant phone_table_model::data (const QModelIndex &index, int role) const
{
  if (!index.isValid ())
    return QVariant ();

  QList<gray_type::key_type> keys = gray_list.keys ();
  int i_row = index.row ();
  int j_col = index.column ();

  if (i_row >= keys.size ())
    return QVariant ();

  j_col--;
  if (j_col >= gray_list[keys[i_row]].size () || j_col < -1)
    return QVariant ();

  switch (role)
    {
    case Qt::TextAlignmentRole:
      if (j_col == -1 || j_col == 11)
        {
          return Qt::AlignRight + Qt::AlignVCenter;
        }
      break;
    case Qt::DisplayRole:
      if (j_col == -1)
        return keys[i_row];
      else
        {
          if (j_col != 11)
            return gray_list[keys[i_row]][j_col];
          else
            {
              QString price = gray_list[keys[i_row]][j_col];
              return price.remove (" ").toInt ();
            }
        }
      break;
    }

  return QVariant ();
}

int phone_table_model::columnCount (const QModelIndex &parent) const
{
  return title_map.size () + 1;
}

int phone_table_model::rowCount (const QModelIndex &parent) const
{
  return gray_list.size ();
}
