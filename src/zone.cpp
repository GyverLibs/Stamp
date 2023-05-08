#include "zone.h"

static int _dt_zone = 0;

void setStampZone(int zone) {
  _dt_zone = zone;
}

int getStampZone() {
  return _dt_zone;
}