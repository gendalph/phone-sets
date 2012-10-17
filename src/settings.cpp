#include "settings.h"

settings::settings (QObject *parent) : QSettings ("Bross", "Phone base", parent)
{
}

