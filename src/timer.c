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
  float elapsed;
  Sound sound;
} timer_s;

timer_s* timer_init(type_e type)
{
  timer_s* timer;

  timer = malloc(sizeof(*timer));
  timer->type = type;
  timer->val = 0;
  timer->update = timer_idle;
  timer->elapsed = 0.0f;
  timer->sound = timer->type == SOUND ? LoadSound("../resources/sound.wav") : (Sound){0};

  return timer;
}

void timer_free(timer_s* timer)
{
  if (timer->type == SOUND)
    UnloadSound(timer->sound);
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
  PlaySound(timer->sound);

  timer->elapsed += GetFrameTime() * 1000;
  if (timer->elapsed >= 16.67f)
  {
    timer->val--;
    timer->elapsed = 0.0f;
  }
  
  timer->update = timer->val ? timer_sound : timer_idle;
}

static void timer_delay(timer_s* timer)
{
  timer->elapsed += GetFrameTime() * 1000;
  if (timer->elapsed >= 16.67f)
  {
    timer->val--;
    timer->elapsed = 0.0f;
  }
    
  timer->update = timer->val ? timer_delay : timer_idle;
}
