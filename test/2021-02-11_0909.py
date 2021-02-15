from bughuntingfrmod import Image
import numpy

im = Image(2, 3)

print("I make it succeed")
    # When this statememt is removed, the program segfaults.

print(numpy.asarray(im))
