#include "OutTime.h"

std::string outtime(double total_seconds, char *buffer)
{
  unsigned hours = unsigned(total_seconds) / 60 / 60;
  unsigned minutes = unsigned(total_seconds) / 60 - 60 * hours;
  double seconds = total_seconds - 60*60*hours - 60*minutes;

  std::sprintf(buffer, "%02u:%02u:%05.2f", hours, minutes, seconds);
  return buffer;
}

