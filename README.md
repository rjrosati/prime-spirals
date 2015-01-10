# prime-spirals
Creates images of prime numbers in various spiral patterns.

The following types of spirals are currently supported:

    - Sacks
    - Ulam
    - Vogel


Additionally, random data with the same average density as the primes (scaled using the prime number theorem) can be plotted for comparison on each spiral type.

#Usage
    `spirals [image size] [spiral type]`

`[spiral type]` must be one of the following strings: Sacks, Ulam, Vogel, RandomSacks, RandomUlam, RandomVogel.

`[image size]` is the desired size of the generated image in pixels. Note that the algorithms as written can only generate odd-dimensioned square images. Even sizes will be incremented.

 The output image will be `spiral.png` in the executable directory. Polar spirals (Sacks and Vogel) will only generate prime numbers up to `image_size`, while Ulam spirals will show primes up to `image_size*image_size`. On my laptop (i5 M540 processor), a ~18,000 pixel image can be generated in just over a minute. RAM usage peaks at about 1.7GB for that example -- this could probably be optimized.

Please feel free to fork, branch, enhance, or otherwise modify this code.
Also feel free to contact me via my GitHub info with any comments, questions, or suggestions.

Thanks,
Robbie Rosati
