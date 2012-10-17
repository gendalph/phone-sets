#include "phone_table_model.h"

QVariant phone_table_model::data (const QModelIndex &index, int role) const
{
  if (index.isValid () && role == Qt::DisplayRole)
    {
      QList<gray_type::key_type> keys = gray_list.keys ();
      int i_row = index.row ();
      int j_col = index.column ();

      if (i_row >= keys.size ())
        return QVariant ();

      if (j_col == 0)
        return keys[i_row];
      else
        {
          j_col--;
          if (j_col >= gray_list[keys[i_row]].size ())
            return QVariant ();
          return gray_list[keys[i_row]][j_col];
        }
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
