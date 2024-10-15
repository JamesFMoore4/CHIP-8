#include "timer.h"

static void timer_idle(timer_s* timer);
static void timer_delay(timer_s* timer);
static void timer_sound(timer_s* timer);

static update_f update_func[] = {timer_delay, timer_sound};

typedef struct timer_s
{
  byte val;
  type_e type;
  update_f update;
  Sound sound;
} timer_s;

timer_s* timer_init(type_e type)
{
  timer_s* timer;

  timer = malloc(sizeof(*timer));
  timer->type = type;
  timer->val = 0;
  timer->update = timer_idle;
  // load sound here

  return timer;
}

void timer_free(timer_s* timer)
{
  free(timer);
}

void timer_set(timer_s* timer, byte val)
{
  timer->val = val;
}

byte timer_get(timer_s* timer)
{
  return timer->val;
}

void timer_update(timer_s* timer)
{
  timer->update(timer);
}

static void timer_idle(timer_s* timer)
{
  timer->update = timer->val ? update_func[timer->type] : timer_idle;
}

static void timer_sound(timer_s* timer)
{
  // Play sound here
  timer->val--;
  timer->update = timer->val ? timer_sound : timer_idle;
}

static void timer_delay(timer_s* timer)
{
  timer->val--;
  timer->update = timer->val ? timer_delay : timer_idle;
}
