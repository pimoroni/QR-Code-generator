QR Code generator library - MicroPython Bindings
================================================


Introduction
------------

These MicroPython Bindings wrap the QR Code generator C library for use with MicroPython.


Examples
--------

```python
import qrcode

code = qrcode.QRCode(ecc=qrcode.ECC_MEDIUM)
code.set_text("Hello World!")

width, height = code.get_size()

for y in range(height):
    for x in range(width):
        (... paint code.get_module(x, y) ...)
```
