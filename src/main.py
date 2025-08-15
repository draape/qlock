from mapping import TimeMapper
from mapping import LedLayout
from clock import WordClock
from hardware.rpi_led_controller import LedController
import time
import datetime

if __name__ == "__main__":
  ROWS = 10
  COLS = 11
  MINUTE_LEDS = [110, 111, 112, 113]
  LOGO_LED = 114
  LED_COUNT = ROWS * COLS + len(MINUTE_LEDS) + 1

  WORDS_MAP = {
      "KLOKKEN": [(0, c) for c in range(7)],
      "HALV": [(4, c) for c in range(4)],
      "ÅTTE": [(6, c) for c in range(4, 8)],
      "FEM": [],
      "PÅ": [],
      "TI": [],
      "OVER": [],
      "KVART": [],
      "TOLV": [],
      "ETT": [],
      "TO": [],
      "TRE": [],
      "FIRE": [],
      "SEKS": [],
      "SYV": [],
      "NI": [],
      "ELLEVE": [],
  }

  controller = LedController(LED_COUNT)
  layout = LedLayout(ROWS, COLS, MINUTE_LEDS, LOGO_LED)
  clock = WordClock(controller, layout, WORDS_MAP)
  mapper = TimeMapper()

  while True:
      now = datetime.now()
      words_to_show, minute_leds = mapper.get_display(now.hour, now.minute)

      clock.clear()
      for word in words_to_show:
          clock.show_word(word)
      clock.show_minutes(minute_leds)
      clock.show_logo()
      clock.show()

      # Wait until start of next minute
      time.sleep(60 - now.second)