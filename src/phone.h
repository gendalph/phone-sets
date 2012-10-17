#ifndef PHONE_H
#define PHONE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <proc/phone_table_model.h>

class phone : public QObject
{
  Q_OBJECT
public:

  // phone
  typedef long long int phone_internal_type;
  phone_internal_type phone_num;
  union
  {
    struct
    {
      unsigned int is_ok : 1;
      unsigned int is_gray : 1;
      unsigned int is_black : 1;
    };

    unsigned int flags;
  };

  void check_phone_to_flags (phone_table_model::black_type black_list, int init_black_list);

//   phone (phone_internal_type phone_ = 0) : phone_num (phone_), flags (0) { check_phone_to_flags (); };
  phone (const QString &phone_str, phone_table_model::black_type black_list, int init_black_list = 0);

  virtual ~phone ();
};

#endif // PHONE_H
