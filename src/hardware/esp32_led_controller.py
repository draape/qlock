import sys

class LedController:
    def __init__(self, led_count, brightness=0.2, pin_num=18):
        self.led_count = led_count
        self.brightness = brightness
        self.pin_num = pin_num

        if sys.platform.startswith("linux"):  # Raspberry Pi
            import board
            import neopixel
            self.strip = neopixel.NeoPixel(
                getattr(board, f"D{pin_num}"),
                led_count,
                brightness=brightness,
                auto_write=False
            )
        elif sys.platform in ("esp32", "esp8266"):  # ESP
            import machine
            import neopixel
            self.strip = neopixel.NeoPixel(machine.Pin(pin_num), led_count)
        else:
            raise RuntimeError("Unsupported platform")

    def set_pixel(self, index, color):
        self.strip[index] = color

    def fill(self, color):
        for i in range(self.led_count):
            self.strip[i] = color

    def show(self):
        if hasattr(self.strip, "show"):
            self.strip.show()
        else:
            self.strip.write()