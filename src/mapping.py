class TimeMapper:
  def __init__(self):
    self.hour_words = {
        0: "TOLV", 1: "ETT", 2: "TO", 3: "TRE", 4: "FIRE",
        5: "FEM", 6: "SEKS", 7: "SYV", 8: "ÅTTE",
        9: "NI", 10: "TI", 11: "ELLEVE", 12: "TOLV"
    }

  def get_display(self, hour, minute):
    words = ["KLOKKEN"]

    # Minute LEDs
    extra_minutes = minute % 5

    if minute >= 55:
        words += ["FEM", "PÅ"]
        hour = (hour + 1) % 12
    elif minute >= 50:
        words += ["TI", "PÅ"]
        hour = (hour + 1) % 12
    elif minute >= 45:
        words += ["KVART", "PÅ"]
        hour = (hour + 1) % 12
    elif minute >= 40:
        words += ["TI", "OVER", "HALV"]
        hour = (hour + 1) % 12
    elif minute >= 35:
        words += ["FEM", "OVER", "HALV"]
        hour = (hour + 1) % 12
    elif minute >= 30:
        words.append("HALV")
        hour = (hour + 1) % 12
    elif minute >= 25:
        words += ["FEM", "PÅ", "HALV"]
        hour = (hour + 1) % 12
    elif minute >= 20:
        words += ["TI", "PÅ", "HALV"]
        hour = (hour + 1) % 12
    elif minute >= 15:
        words += ["KVART", "OVER"]
    elif minute >= 10:
        words += ["TI", "OVER"]
    elif minute >= 5:
        words += ["FEM", "OVER"]

    words.append(self.hour_words[hour % 12])
    return words, extra_minutes
  
class LedLayout:
  def __init__(self, rows, cols, minute_leds, logo_led):
    self.rows = rows
    self.cols = cols
    self.minute_leds = minute_leds
    self.logo_led = logo_led
    self.index_map = self._generate_snake_map()

  def _generate_snake_map(self):
    """Generate mapping from (row, col) → physical LED index"""
    mapping = {}
    index = 0
    for col in range(self.cols):
        if col % 2 == 0:
            # Up direction
            for row in reversed(range(self.rows)):
                mapping[(row, col)] = index
                index += 1
        else:
            # Down direction
            for row in range(self.rows):
                mapping[(row, col)] = index
                index += 1
    return mapping

  def get_index(self, row, col):
    return self.index_map[(row, col)]