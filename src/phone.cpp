#include "phone.h"

void phone::check_phone_to_flags (phone_table_model::black_type black_list, int init_black_list)
{
  is_ok = 1;
  phone_internal_type phone_num_local = llabs (phone_num);
  phone_num_local /= 10000000L;
  switch (phone_num_local)
    {
      case 8495:
      case 8499:
        if (phone_num > 0)
          phone_num = -phone_num;
        is_gray = 0;
        is_black = 1;
        break;
    }

  if (phone_num_local > 8999 || init_black_list || phone_num_local <= 8899)
    {
      is_gray = 0;
      is_black = 1;
    }

  if (is_black)
    black_list.insert (phone_num);
}

phone::phone (const QString &phone_str, phone_table_model::black_type black_list, int init_black_list)
{
  QString phone_local = phone_str;
  phone_local.remove ("-");

  bool ok = 0;
  phone_num = phone_local.toLongLong (&ok);
  if (ok)
    {
      check_phone_to_flags (black_list, init_black_list);
    }
  else
    {
      phone_local.remove (" ");
      QString phone_black;
      QStringList phones_list = phone_local.split (",");
      is_black = 1;
      foreach (phone_black, phones_list)
        {
          phone_num = phone_black.toLongLong (&ok);
          if (ok)
            {
              check_phone_to_flags (black_list, init_black_list);
            }
        }
    }
}

phone::~phone ()
{

}
