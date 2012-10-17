#ifndef PHONE_TABLE_MODEL_H
#define PHONE_TABLE_MODEL_H

/// \file proc/phone_table_model.h

#include <QtCore/QAbstractTableModel>
#include <QStringList>
#include <QSet>

class phone_table_model : public QAbstractTableModel
{
  Q_OBJECT

public:
  phone_table_model (QObject *parent = 0) : QAbstractTableModel (parent) {};
  ~phone_table_model () {};
  virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual int columnCount (const QModelIndex &parent = QModelIndex()) const;
  virtual int rowCount (const QModelIndex &parent = QModelIndex()) const;

  static QString black_list_file_name () { return "black_list.txt"; }
  static QString gray_list_file_name () { return "gray_list.txt"; }

  // members types
  typedef QSet<long long int> black_type;
  typedef QMap<unsigned long long int, QStringList> gray_type;
  typedef QMap<QString, size_t> title_type;

  // members data
  black_type black_list;
  gray_type gray_list;
  title_type title_map;
};

#endif // PHONE_TABLE_MODEL_H
