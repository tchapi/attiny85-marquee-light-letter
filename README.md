An ATTiny85 (w/ Micronucleus) sketch controlling a 60-LED RGB strip used in a marquee light letter.


![outside](https://raw.githubusercontent.com/tchapi/attiny85-marquee-light-letter/master/pictures/outside.png "Outside picture")

![inside](https://raw.githubusercontent.com/tchapi/attiny85-marquee-light-letter/master/pictures/inside.png "Inside picture")


Uses the [Light WS2812](https://github.com/cpldcpu/light_ws2812) library from [cpldcpu](https://github.com/cpldcpu).

**BOM :**

  * An Olimexino 85
  * A WS2812 Led Strip like [this one](https://www.sparkfun.com/products/12027)
  * Two buttons to change modes and variants like [these](https://www.sparkfun.com/products/97)
  * And two corresponding [10KΩ resistors](https://www.sparkfun.com/products/8374) to pull them down

The LED strip has 3 modes ("_mode_" button to choose them) :
	
  * Normal  — plain color
  * Breathing light — plain color
  * Rainbow mode — multicolor
	
For the two first modes, you can choose the color with the "_variant_" button.

See [this article on foobarflies.io](http://www.foobarflies.io/programming-an-olimexino-85/) for how to program the **ATTiny85** with a **Micronucleus** bootloader.