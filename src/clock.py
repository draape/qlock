class WordClock:
  def __init__(self, controller, layout, words_map):
    self.controller = controller
    self.layout = layout
    self.words_map = words_map

  def show_word(self, word, color=(255, 255, 255)):
    if word not in self.words_map:
        return
    for row, col in self.words_map[word]:
        idx = self.layout.get_index(row, col)
        self.controller.set_pixel(idx, color)

  def show_minutes(self, minute_count, color=(255, 255, 255)):
    for i in range(minute_count):
        self.controller.set_pixel(self.layout.minute_leds[i], color)

  def show_logo(self, color=(255, 255, 255)):
    self.controller.set_pixel(self.layout.logo_led, color)

  def clear(self):
    self.controller.fill((0, 0, 0))

  def show(self):
    self.controller.show()