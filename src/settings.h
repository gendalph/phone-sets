#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class settings : public QSettings
{

public:
  settings (QObject *parent = 0);
};

#endif // SETTINGS_H
