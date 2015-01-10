// Creates an image of an Ulam spiral (http://en.wikipedia.org/wiki/Ulam_spiral)
// or a Sacks spiral (http://www.numberspiral.com/)

#include "lodepng/lodepng.h"
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <ctime>

// makes a square. Don't want a square? Crop the image.

// #define PRINT_PRIMES
#define PRIME_COLOR_R  0xFF
#define PRIME_COLOR_G  0xFF
#define PRIME_COLOR_B  0xFF
#define PRIME_COLOR_A  0xFF

#define BG_COLOR_R  0x00
#define BG_COLOR_G  0x00
#define BG_COLOR_B  0x00
#define BG_COLOR_A  0xFF

typedef enum {
	SACKS,
	ULAM,
	VOGEL,
	RANDOM_SACKS,
	RANDOM_ULAM,
	RANDOM_VOGEL
} spiral_t;


// standard square spiral (CCW, start @ +x)
//
//   5 <- 4 <- 3
//   |         ^
//   v         |
//   6    1 -> 2    ...
//   |              ^
//   v              |
//   7 -> 8 -> 9 -> 10
//
void make_ulam(unsigned int size, std::vector<unsigned char>& img_spiral, std::vector<unsigned char>& nums) {

	int cen = size / 2;
	unsigned int size2 = size*size;
	int x = cen, y = cen;
	int dir = 0, square_len = 1;
	for (unsigned int i = 0; i<size2; i++) {
		if (nums[i]) {
			img_spiral[4 * (y*size + x) + 0] = PRIME_COLOR_R;
			img_spiral[4 * (y*size + x) + 1] = PRIME_COLOR_G;
			img_spiral[4 * (y*size + x) + 2] = PRIME_COLOR_B;
			img_spiral[4 * (y*size + x) + 3] = PRIME_COLOR_A;
		}
		else {
			img_spiral[4 * (y*size + x) + 0] = BG_COLOR_R;
			img_spiral[4 * (y*size + x) + 1] = BG_COLOR_G;
			img_spiral[4 * (y*size + x) + 2] = BG_COLOR_B;
			img_spiral[4 * (y*size + x) + 3] = BG_COLOR_A;
		}
		switch (dir) {
		case 0:
			x++;
			if (x - cen >= square_len)
				dir++;
			break;
		case 1:
			y--;
			if (cen - y >= square_len)
				dir++;
			break;
		case 2:
			x--;
			if (cen - x >= square_len)
				dir++;
			break;
		case 3:
			y++;
			if (y - cen >= square_len) {
				dir = 0;
				square_len++;
			}
			break;
		}
	}
}

// polar spiral
// 
//  
//  6 2   5
//       1  4  9 ... 
// 7   3   
//        8
//
// perfect squares all fall on theta=0 line
//
// See http://www.dcs.gla.ac.uk/~jhw/spirals/
// Parametric polar plot:
// theta = sqrt(i)*2*pi;
// r = sqrt(i);
// x = round(r *  cos(theta)) + cen
// y = round(r * -sin(theta)) + cen
unsigned make_sacks(unsigned int size, std::vector<unsigned char>& img_spiral, std::vector<unsigned char>& nums) {
	unsigned int x, y;
	unsigned int top_num = size*size;
	double r, theta;
	// probably need to resize the image
	// circle with same area as a square with side length s needs a square with s' = 2/sqrt(pi) * s to contain it
	unsigned size_tmp = size * 2 + 1;// but this is a very sparse circle. try 2x? (+1 to force odd) -- seems to work
	std::cout << "Polar spirals require a bigger image to display all primes, resizing to " << size_tmp << std::endl;
	img_spiral.resize(4 * size_tmp*size_tmp);
	// init to black (same as composite #s)
	for (unsigned int i = 0; i < size_tmp*size_tmp; i++) {
		img_spiral[4 * i + 0] = BG_COLOR_R;
		img_spiral[4 * i + 1] = BG_COLOR_G;
		img_spiral[4 * i + 2] = BG_COLOR_B;
		img_spiral[4 * i + 3] = BG_COLOR_A;
	}

	unsigned int cen = size_tmp / 2;
	for (unsigned int i = 0; i < top_num; i++) {
		r = sqrt(i + 1);
		theta = r * 2 * M_PI;
		x = (unsigned)round(r* cos(theta)) + cen;
		y = (unsigned)round(r*-sin(theta)) + cen;

		if (nums[i]) {
			img_spiral[4 * (y*size_tmp + x) + 0] = PRIME_COLOR_R;
			img_spiral[4 * (y*size_tmp + x) + 1] = PRIME_COLOR_G;
			img_spiral[4 * (y*size_tmp + x) + 2] = PRIME_COLOR_B;
			img_spiral[4 * (y*size_tmp + x) + 3] = PRIME_COLOR_A;
		} // else already black
	}
	return size_tmp;
}

// similar to Sacks, but with
// r = sqrt(i);
// theta = i * ((2 * pi) / (phi*phi));
unsigned make_vogel(unsigned int size, std::vector<unsigned char>& img_spiral, std::vector<unsigned char>& nums) {
	unsigned int x, y;
	unsigned int top_num = size*size;
	const double phi = (1 + sqrt(5)) / 2; // golden ratio
	double r, theta;
	// probably need to resize the image
	// circle with same area as a square with side length s needs a square with s' = 2/sqrt(pi) * s to contain it
	unsigned size_tmp = size * 2 + 1;// but this is a very sparse circle. try 2x? (+1 to force odd) -- seems to work
	std::cout << "Polar spirals require a bigger image to display all primes, resizing to " << size_tmp << std::endl;
	img_spiral.resize(4 * size_tmp*size_tmp);
	// init to black (same as composite #s)
	for (unsigned int i = 0; i < size_tmp*size_tmp; i++) {
		img_spiral[4 * i + 0] = BG_COLOR_R;
		img_spiral[4 * i + 1] = BG_COLOR_G;
		img_spiral[4 * i + 2] = BG_COLOR_B;
		img_spiral[4 * i + 3] = BG_COLOR_A;
	}

	unsigned int cen = size_tmp / 2;
	for (unsigned int i = 0; i < top_num; i++) {
		r = sqrt(i + 1);
		theta = (i+1) * (2 * M_PI) / (phi*phi);
		x = (unsigned)round(r* cos(theta)) + cen;
		y = (unsigned)round(r*-sin(theta)) + cen;

		if (nums[i]) {
			img_spiral[4 * (y*size_tmp + x) + 0] = PRIME_COLOR_R;
			img_spiral[4 * (y*size_tmp + x) + 1] = PRIME_COLOR_G;
			img_spiral[4 * (y*size_tmp + x) + 2] = PRIME_COLOR_B;
			img_spiral[4 * (y*size_tmp + x) + 3] = PRIME_COLOR_A;
		} // else already black
	}
	return size_tmp;
}



int main(int argc, char** argv) {
    unsigned int size = 18000, outsize = size;
	spiral_t spiral_type = RANDOM_ULAM;
	const char *spiral_strings[] = { "a Sacks", "an Ulam", "a Vogel", "random junk in the shape of a Sacks", "random junk in the shape of an Ulam", "random junk in the shape of a Vogel" };

	if (argc >= 3) {
		std::stringstream(argv[1]) >> size;
		if (argv[2][0] == 'S') spiral_type = SACKS;
		else if (argv[2][0] == 'U') spiral_type = ULAM;
		else if (argv[2][0] == 'V') spiral_type = VOGEL;
		else if (argv[2][0] == 'R') {
			if (argv[2][6] == 'S') spiral_type = RANDOM_SACKS;
			else if (argv[2][6] == 'U') spiral_type = RANDOM_ULAM;
			else if (argv[2][6] == 'V') spiral_type = RANDOM_VOGEL;
			else {
				std::cout << "Unrecognized option: " << argv[2] << std::endl; exit(-1);
			}
		}
		else {
			std::cout << "Unrecognized option: " << argv[2] << std::endl; exit(-1);
		}
	}

	std::cout << "Generating " << spiral_strings[spiral_type] << " spiral." << std::endl;
    if (!(size%2)) {
        std::cout<<"WARNING: only odd image sizes supported, generating image of size "<<++size<<std::endl;
		++outsize;
    }
    unsigned int size2 = size*size;

    std::vector<unsigned char> img_spiral; img_spiral.resize(size2*4); // RGBARGBA...
    std::vector<unsigned char> nums; nums.resize(size2); // not bool because vector<bool> packs the bits
    
    nums[0] = false;
    for (unsigned int i=1;i<size2;i++) {
        nums[i] = true;
    }
    std::cout<<"Prime array initialized."<<std::endl;

	if (spiral_type == SACKS || spiral_type == ULAM || spiral_type == VOGEL) {
		std::cout << "Sieving primes..." << std::endl;
		// sieve of Eratosthenes
		#ifdef PRINT_PRIMES
		std::cout<<"Removing all multiples of ";
		#endif
		for (unsigned int i = 1; i < size2; i++) {
			if (!nums[i]) continue;
			#ifdef PRINT_PRIMES
			std::cout<<i+1<<",";
			#endif
			for (unsigned int j = 2 * i + 1; j < size2; j += (i + 1)) {
				nums[j] = false;
			}
		}
		#ifdef PRINT_PRIMES
		std::cout<<std::endl;
		#endif
	}
	else {
		std::cout << "Filling prime array with random junk (scaled to have ~ same density as the primes)..." << std::endl;
		srand(time(0));
		double prob = 0.0;
		for (unsigned int i = 0; i < size2; i++) {
			if (i<5) prob = 1.0 / log(i + 1);
			else prob = (i + 1) / log(i + 1) - i / log(i);
			nums[i] = rand() < RAND_MAX * prob; // gives a random integer, use PNT and John's calculated instantaneous probability to give a decent distribution
		}

	}
    
    std::cout<<"Making and spiraling image..."<<std::endl;

	switch (spiral_type) {
	case SACKS:
	case RANDOM_SACKS:
		outsize = make_sacks(size, img_spiral, nums);
		break;
	case ULAM:
	case RANDOM_ULAM:
		make_ulam(size, img_spiral, nums);
        outsize = size;
		break;
	case VOGEL:
	case RANDOM_VOGEL:
		outsize = make_vogel(size, img_spiral, nums);
		break;
	}
    
    std::cout<<"Writing file..."<<std::endl;
    unsigned error = lodepng::encode("./spiral.png",img_spiral,outsize,outsize);
    if (error) std::cout << "encoder error " << error << ":" << lodepng_error_text(error) << std::endl;


    return 0;
}
