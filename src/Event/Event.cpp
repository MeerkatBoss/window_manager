#include "Event/Event.h"

namespace event
{

size_t registerEventType(void)
{
  static size_t availableId = event::UserEventTypeMin;

  return ++availableId;
}

} // namespace event
