#define blueking_width		64
#define blueking_height		41
#define blueking_colors		17
#define blueking_back		0
int	blueking_reds[] = {8192, 18176, 20736, 35328, 23040, 21248, 23040, 
34304, 38912, 20992, 21504, 39424, 53248, 52480, 
7168, 48640, 50944};
int	blueking_greens[] = {45568, 31744, 43776, 41472, 25600, 19456, 33280, 
37632, 44800, 35840, 41216, 52480, 55552, 51456, 
10240, 45056, 47872};
int	blueking_blues[] = {43520, 42240, 52992, 48640, 39424, 11264, 17920, 
17408, 32256, 29440, 40960, 50688, 50944, 35328, 
5376, 13312, 25600};
unsigned char	blueking_rasterA[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,
0x02,0x02,0x02,0x02,0x02,0x01,0x02,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x02,0x02,
0x02,0x03,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x04,0x05,0x06,0x05,0x05,0x06,0x07,0x08,
0x09,0x09,0x09,0x01,0x0a,0x0a,0x0a,0x01,0x08,0x03,0x03,0x0a,0x03,0x03,0x03,0x03,
0x01,0x01,0x02,0x02,0x03,0x02,0x03,0x03,0x03,0x02,0x02,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x07,0x06,0x07,0x06,0x09,0x02,0x02,0x02,0x04,0x07,0x04,0x04,0x04,0x09,
0x09,0x01,0x0a,0x0a,0x09,0x0a,0x08,0x08,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x01,
0x0b,0x03,0x03,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x02,0x05,0x01,0x01,0x04,0x09,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x01,0x08,0x09,0x08,0x08,0x0b,0x03,0x02,
0x02,0x02,0x02,0x03,0x03,0x02,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x0c,0x08,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x02,0x05,0x01,0x01,0x04,0x02,
0x06,0x09,0x02,0x09,0x07,0x07,0x08,0x0b,0x01,0x02,0x02,0x08,0x0c,0x0b,0x0b,0x0c,
0x0b,0x03,0x02,0x02,0x03,0x03,0x02,0x03,0x03,0x03,0x03,0x0b,0x0b,0x0c,0x0d,0x0d,
0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x04,0x0e,0x04,0x05,0x05,
0x02,0x07,0x08,0x02,0x09,0x0d,0x08,0x0c,0x08,0x0b,0x02,0x02,0x02,0x08,0x0c,0x0c,
0x0d,0x03,0x0c,0x02,0x02,0x0b,0x03,0x03,0x01,0x03,0x03,0x08,0x0b,0x0d,0x0f,0x10,
0x0f,0x0d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x05,0x05,0x04,0x02,0x05,
0x02,0x07,0x07,0x08,0x02,0x08,0x0d,0x08,0x0d,0x0d,0x0c,0x0c,0x02,0x02,0x02,0x0c,
0x0d,0x0c,0x0c,0x0c,0x02,0x02,0x0b,0x03,0x01,0x03,0x03,0x0b,0x08,0x0d,0x10,0x0f,
0x0f,0x0f,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x05,0x05,0x05,0x02,0x05,
0x05,0x02,0x07,0x0d,0x0d,0x02,0x08,0x0d,0x0d,0x08,0x10,0x0d,0x0d,0x0c,0x02,0x02,
0x03,0x0c,0x0c,0x0c,0x0c,0x01,0x02,0x03,0x03,0x03,0x03,0x08,0x0d,0x0d,0x0f,0x0f,
0x0d,0x0f,0x0d,0x10,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x0e,0x0e,0x05,0x02,
0x05,0x02,0x10,0x10,0x0d,0x08,0x02,0x0d,0x0f,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x02,
0x02,0x03,0x0c,0x0c,0x0c,0x0c,0x01,0x02,0x03,0x03,0x03,0x08,0x08,0x0d,0x0f,0x0f,
0x0f,0x0f,0x0d,0x0d,0x07,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x05,0x0e,0x0e,0x05,0x02,
0x05,0x08,0x02,0x0f,0x0d,0x0d,0x0c,0x02,0x0d,0x0d,0x0d,0x0c,0x0d,0x0d,0x0d,0x0d,
0x03,0x02,0x02,0x0c,0x0c,0x08,0x0d,0x01,0x02,0x03,0x03,0x08,0x08,0x08,0x08,0x0f,
0x0f,0x0f,0x10,0x10,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x0e,0x05,0x05,0x05,
0x02,0x09,0x02,0x07,0x10,0x0d,0x0d,0x03,0x02,0x03,0x0d,0x0d,0x10,0x0d,0x0c,0x0d,
0x0d,0x0d,0x02,0x02,0x0c,0x0d,0x0d,0x0d,0x01,0x02,0x03,0x03,0x08,0x01,0x01,0x07,
0x08,0x10,0x07,0x08,0x08,0x0a,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x0e,0x0e,0x05,0x05,
0x02,0x06,0x10,0x02,0x10,0x10,0x0d,0x0d,0x0b,0x02,0x02,0x0d,0x0d,0x0d,0x0d,0x0d,
0x0d,0x0d,0x0d,0x01,0x02,0x08,0x08,0x10,0x08,0x02,0x03,0x03,0x01,0x02,0x02,0x01,
0x08,0x08,0x08,0x09,0x0a,0x01,0x0a,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x0e,0x0e,0x02,0x0e,
0x02,0x06,0x08,0x08,0x02,0x0d,0x0d,0x10,0x0f,0x0d,0x0c,0x02,0x02,0x0d,0x0d,0x0d,
0x0d,0x0d,0x0d,0x0d,0x0b,0x02,0x01,0x0d,0x08,0x03,0x03,0x08,0x02,0x0d,0x0f,0x02,
0x08,0x09,0x08,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x0e,0x02,0x05,
0x05,0x02,0x0d,0x10,0x02,0x08,0x0d,0x0f,0x0d,0x0f,0x10,0x0d,0x03,0x02,0x03,0x0d,
0x10,0x0d,0x0d,0x0d,0x0f,0x0d,0x02,0x02,0x01,0x01,0x03,0x01,0x02,0x0f,0x0d,0x02,
0x01,0x08,0x01,0x01,0x01,0x01,0x01,0x01,0x09,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x03,0x03,0x03,0x0b,0x03,0x03,0x00,0x00,0x00,0x00,0x02,0x0e,0x0e,0x0e,0x02,
0x05,0x05,0x02,0x10,0x08,0x02,0x0f,0x10,0x10,0x0d,0x0d,0x10,0x0f,0x0d,0x02,0x02,
0x03,0x0d,0x0d,0x10,0x0d,0x0d,0x0d,0x01,0x01,0x01,0x01,0x03,0x01,0x02,0x02,0x01,
0x08,0x01,0x01,0x01,0x01,0x01,0x01,0x0d,0x0d,0x06,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x03,0x0b,0x0b,0x03,0x0b,0x0b,0x0b,0x0b,0x00,0x00,0x02,0x05,0x0e,0x05,0x02,
0x05,0x05,0x02,0x10,0x10,0x08,0x02,0x0f,0x0f,0x0d,0x0d,0x0d,0x0d,0x10,0x0d,0x0c,
0x02,0x02,0x0d,0x0d,0x0d,0x0d,0x0d,0x08,0x0f,0x0f,0x01,0x01,0x03,0x01,0x01,0x08,
0x01,0x01,0x01,0x01,0x01,0x05,0x0d,0x0e,0x0e,0x0d,0x06,0x05,0x00,0x00,0x00,0x00,
0x00,0x03,0x03,0x03,0x0b,0x03,0x0b,0x0c,0x0c,0x0c,0x00,0x00,0x0e,0x05,0x02,0x05,
0x02,0x05,0x02,0x10,0x08,0x10,0x0d,0x02,0x0d,0x0f,0x10,0x0d,0x0d,0x0d,0x10,0x0d,
0x10,0x03,0x02,0x02,0x08,0x0f,0x0d,0x0f,0x0f,0x06,0x0f,0x0f,0x01,0x03,0x03,0x06,
0x01,0x01,0x09,0x06,0x09,0x02,0x0d,0x0e,0x0e,0x0d,0x05,0x02,0x01,0x01,0x00,0x00,
0x00,0x03,0x0b,0x0b,0x0b,0x0b,0x0b,0x0c,0x0c,0x0c,0x0c,0x05,0x05,0x05,0x02,0x05,
0x02,0x08,0x09,0x02,0x0d,0x0d,0x10,0x0b,0x02,0x02,0x10,0x0f,0x10,0x0d,0x10,0x10,
0x0d,0x0d,0x10,0x02,0x02,0x02,0x01,0x0f,0x0f,0x0f,0x06,0x0f,0x0f,0x01,0x06,0x07,
0x02,0x06,0x04,0x02,0x02,0x01,0x06,0x0d,0x0d,0x05,0x02,0x07,0x01,0x09,0x09,0x00,
0x00,0x03,0x03,0x03,0x03,0x0b,0x0b,0x0b,0x0c,0x0c,0x0c,0x05,0x05,0x05,0x02,0x05,
0x02,0x07,0x07,0x02,0x08,0x10,0x0d,0x0d,0x0f,0x0b,0x02,0x02,0x10,0x0d,0x0d,0x0f,
0x10,0x0d,0x10,0x0f,0x10,0x0f,0x0b,0x0f,0x06,0x0f,0x0f,0x06,0x0f,0x0f,0x07,0x05,
0x02,0x02,0x02,0x06,0x06,0x07,0x09,0x06,0x05,0x02,0x06,0x09,0x01,0x0e,0x0e,0x00,
0x00,0x03,0x0b,0x0b,0x0b,0x0b,0x0b,0x0c,0x0c,0x0c,0x0c,0x07,0x05,0x0e,0x02,0x05,
0x05,0x02,0x07,0x0f,0x02,0x0f,0x10,0x0f,0x0f,0x10,0x0f,0x0b,0x02,0x02,0x0f,0x0f,
0x10,0x10,0x0f,0x0f,0x10,0x08,0x0d,0x0f,0x0f,0x06,0x0f,0x0f,0x06,0x0f,0x0f,0x05,
0x02,0x07,0x07,0x09,0x04,0x01,0x09,0x02,0x02,0x09,0x01,0x01,0x0e,0x0c,0x0c,0x00,
0x00,0x07,0x03,0x03,0x03,0x0b,0x0b,0x0b,0x0c,0x0c,0x0c,0x0c,0x05,0x05,0x02,0x0e,
0x05,0x02,0x0f,0x10,0x10,0x02,0x0f,0x10,0x0f,0x10,0x0f,0x0f,0x0d,0x02,0x02,0x02,
0x08,0x0f,0x10,0x0f,0x0d,0x0d,0x0f,0x06,0x06,0x0f,0x06,0x06,0x0f,0x0f,0x0f,0x05,
0x05,0x02,0x04,0x01,0x02,0x02,0x02,0x01,0x01,0x01,0x01,0x0e,0x0c,0x09,0x09,0x00,
0x00,0x07,0x0b,0x0b,0x0b,0x0b,0x03,0x0b,0x0b,0x0b,0x0b,0x0c,0x05,0x05,0x0e,0x02,
0x05,0x02,0x06,0x07,0x0d,0x01,0x02,0x0f,0x0f,0x10,0x08,0x10,0x10,0x0d,0x0d,0x0b,
0x02,0x02,0x02,0x02,0x0a,0x0d,0x0d,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x02,
0x0f,0x02,0x02,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x09,0x0c,0x09,0x06,0x00,0x00,
0x00,0x06,0x07,0x02,0x03,0x03,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0e,0x0e,0x0e,0x02,
0x05,0x05,0x02,0x07,0x0f,0x0f,0x03,0x02,0x0f,0x0f,0x10,0x0d,0x0f,0x0d,0x0f,0x0f,
0x07,0x0f,0x08,0x01,0x02,0x02,0x01,0x01,0x01,0x06,0x0f,0x0f,0x0f,0x0f,0x02,0x05,
0x0f,0x02,0x06,0x04,0x01,0x09,0x01,0x01,0x01,0x01,0x04,0x04,0x04,0x00,0x00,0x00,
0x00,0x00,0x07,0x0a,0x0b,0x03,0x0a,0x03,0x03,0x03,0x03,0x0a,0x0e,0x06,0x0e,0x0e,
0x0e,0x05,0x05,0x02,0x07,0x07,0x0f,0x0a,0x02,0x02,0x0f,0x0f,0x0d,0x10,0x0d,0x0d,
0x10,0x0f,0x10,0x10,0x07,0x07,0x08,0x0d,0x06,0x06,0x07,0x07,0x07,0x02,0x05,0x05,
0x05,0x02,0x09,0x04,0x01,0x04,0x01,0x09,0x01,0x04,0x05,0x0e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x00,0x00,0x01,0x01,0x0e,
0x0e,0x05,0x05,0x05,0x02,0x06,0x07,0x07,0x0f,0x0d,0x02,0x02,0x02,0x0a,0x10,0x0f,
0x0f,0x0d,0x07,0x07,0x0d,0x10,0x07,0x07,0x07,0x07,0x07,0x07,0x05,0x02,0x05,0x05,
0x02,0x04,0x05,0x04,0x04,0x01,0x04,0x05,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x06,0x06,0x09,0x06,0x05,0x00,0x00,0x00,0x00,0x02,0x01,0x05,
0x05,0x05,0x02,0x05,0x06,0x02,0x09,0x07,0x07,0x07,0x0d,0x0d,0x10,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x07,0x05,0x05,0x05,0x02,
0x09,0x04,0x04,0x04,0x04,0x0e,0x0e,0x05,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x0e,
0x0e,0x05,0x05,0x02,0x05,0x05,0x02,0x0a,0x07,0x07,0x06,0x07,0x0f,0x10,0x10,0x07,
0x07,0x08,0x09,0x07,0x07,0x07,0x08,0x07,0x07,0x06,0x07,0x06,0x05,0x05,0x02,0x04,
0x04,0x05,0x05,0x05,0x0e,0x05,0x0e,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x05,0x04,
0x05,0x05,0x05,0x05,0x02,0x06,0x06,0x02,0x02,0x01,0x07,0x07,0x07,0x07,0x10,0x10,
0x07,0x07,0x07,0x06,0x07,0x06,0x06,0x06,0x09,0x02,0x02,0x02,0x09,0x07,0x06,0x07,
0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x02,
0x05,0x05,0x05,0x06,0x09,0x02,0x06,0x06,0x07,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x09,0x05,0x06,0x06,0x06,0x05,0x05,
0x05,0x05,0x05,0x05,0x0e,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x05,0x02,
0x05,0x02,0x05,0x09,0x05,0x06,0x02,0x06,0x06,0x06,0x06,0x07,0x07,0x07,0x09,0x09,
0x09,0x09,0x04,0x06,0x06,0x06,0x06,0x09,0x05,0x06,0x02,0x05,0x05,0x05,0x05,0x05,
0x05,0x05,0x05,0x0e,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x01,
0x02,0x04,0x02,0x05,0x04,0x06,0x04,0x02,0x02,0x02,0x04,0x05,0x06,0x05,0x06,0x06,
0x06,0x05,0x06,0x05,0x02,0x02,0x02,0x02,0x02,0x02,0x05,0x05,0x05,0x05,0x05,0x05,
0x04,0x05,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,
0x02,0x05,0x02,0x02,0x04,0x06,0x06,0x06,0x06,0x06,0x02,0x02,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x0e,0x05,0x0e,0x0e,0x05,0x05,0x05,0x05,0x05,0x05,0x0e,0x06,
0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,
0x02,0x04,0x09,0x02,0x06,0x06,0x06,0x06,0x09,0x09,0x06,0x06,0x06,0x05,0x05,0x05,
0x05,0x0e,0x05,0x0e,0x0e,0x0e,0x00,0x00,0x00,0x00,0x0f,0x02,0x0f,0x07,0x0f,0x02,
0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,
0x04,0x02,0x06,0x07,0x02,0x01,0x04,0x05,0x05,0x06,0x06,0x06,0x06,0x07,0x06,0x06,
0x05,0x05,0x05,0x0e,0x00,0x00,0x00,0x00,0x00,0x09,0x0f,0x01,0x0f,0x02,0x02,0x04,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x01,0x07,0x02,0x02,0x04,0x02,0x02,0x02,0x02,0x06,0x06,0x07,0x06,0x06,0x01,0x02,
0x05,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x02,0x10,0x02,0x02,0x04,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x01,0x02,0x07,0x07,0x07,0x06,0x08,0x09,0x02,0x01,0x02,0x01,0x02,0x01,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x07,0x0f,0x02,0x04,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x02,0x01,0x02,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x07,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x0f,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
unsigned char	blueking_rasterB[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x06,0x06,0x06,0x06,0x06,0x01,0x02,
0x02,0x01,0x01,0x02,0x01,0x01,0x02,0x03,0x02,0x03,0x03,0x02,0x03,0x03,0x02,0x02,
0x02,0x03,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x06,0x06,0x01,0x04,0x04,0x05,0x06,0x05,0x05,0x06,0x07,0x08,
0x09,0x09,0x09,0x01,0x0a,0x0a,0x0a,0x01,0x08,0x03,0x03,0x0a,0x03,0x03,0x03,0x03,
0x01,0x01,0x02,0x02,0x03,0x02,0x03,0x03,0x03,0x02,0x02,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x07,0x06,0x07,0x06,0x09,0x02,0x02,0x02,0x04,0x07,0x04,0x04,0x04,0x09,
0x09,0x01,0x0a,0x0a,0x09,0x0a,0x08,0x08,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x01,
0x0b,0x03,0x03,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x02,0x05,0x01,0x01,0x04,0x09,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x01,0x08,0x09,0x08,0x08,0x0b,0x03,0x02,
0x02,0x02,0x02,0x03,0x03,0x02,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x0c,0x08,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x02,0x05,0x01,0x01,0x04,0x02,
0x06,0x09,0x02,0x09,0x07,0x07,0x08,0x0b,0x01,0x02,0x02,0x08,0x0c,0x0b,0x0b,0x0c,
0x0b,0x03,0x02,0x02,0x03,0x03,0x02,0x03,0x03,0x03,0x03,0x0b,0x0b,0x0c,0x0d,0x0d,
0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x04,0x0e,0x04,0x05,0x05,
0x02,0x07,0x08,0x02,0x09,0x0d,0x08,0x0c,0x08,0x0b,0x02,0x02,0x02,0x08,0x0c,0x0c,
0x0d,0x03,0x0c,0x02,0x02,0x0b,0x03,0x03,0x01,0x03,0x03,0x08,0x0b,0x0d,0x0f,0x10,
0x0f,0x0d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x05,0x05,0x04,0x02,0x05,
0x02,0x07,0x07,0x08,0x02,0x08,0x0d,0x08,0x0d,0x0d,0x0c,0x0c,0x02,0x02,0x02,0x0c,
0x0d,0x0c,0x0c,0x0c,0x02,0x02,0x0b,0x03,0x01,0x03,0x03,0x0b,0x08,0x0d,0x10,0x0f,
0x0f,0x0f,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x05,0x05,0x05,0x02,0x05,
0x05,0x02,0x07,0x0d,0x0d,0x02,0x08,0x0d,0x0d,0x08,0x10,0x0d,0x0d,0x0c,0x02,0x02,
0x03,0x0c,0x0c,0x0c,0x0c,0x01,0x02,0x03,0x03,0x03,0x03,0x08,0x0d,0x0d,0x0f,0x0f,
0x0d,0x0f,0x0d,0x10,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x0e,0x0e,0x05,0x02,
0x05,0x02,0x10,0x10,0x0d,0x08,0x02,0x0d,0x0f,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x02,
0x02,0x03,0x0c,0x0c,0x0c,0x0c,0x01,0x02,0x03,0x03,0x03,0x08,0x08,0x0d,0x0f,0x0f,
0x0f,0x0f,0x0d,0x0d,0x07,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x05,0x0e,0x0e,0x05,0x02,
0x05,0x08,0x02,0x0f,0x0d,0x0d,0x0c,0x02,0x0d,0x0d,0x0d,0x0c,0x0d,0x0d,0x0d,0x0d,
0x03,0x02,0x02,0x0c,0x0c,0x08,0x0d,0x01,0x02,0x03,0x03,0x08,0x08,0x08,0x08,0x0f,
0x0f,0x0f,0x10,0x10,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x0e,0x05,0x05,0x05,
0x02,0x09,0x02,0x07,0x10,0x0d,0x0d,0x03,0x02,0x03,0x0d,0x0d,0x10,0x0d,0x0c,0x0d,
0x0d,0x0d,0x02,0x02,0x0c,0x0d,0x0d,0x0d,0x01,0x02,0x03,0x03,0x08,0x01,0x01,0x07,
0x08,0x10,0x07,0x08,0x08,0x0a,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x0e,0x0e,0x05,0x05,
0x02,0x06,0x10,0x02,0x10,0x10,0x0d,0x0d,0x0b,0x02,0x02,0x0d,0x0d,0x0d,0x0d,0x0d,
0x0d,0x0d,0x0d,0x01,0x02,0x08,0x08,0x10,0x08,0x02,0x03,0x03,0x01,0x02,0x02,0x01,
0x08,0x08,0x08,0x09,0x0a,0x01,0x0a,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x0e,0x0e,0x02,0x0e,
0x02,0x06,0x08,0x08,0x02,0x0d,0x0d,0x10,0x0f,0x0d,0x0c,0x02,0x02,0x0d,0x0d,0x0d,
0x0d,0x0d,0x0d,0x0d,0x0b,0x02,0x01,0x0d,0x08,0x03,0x03,0x08,0x02,0x0d,0x0f,0x02,
0x08,0x09,0x08,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x0e,0x02,0x05,
0x05,0x02,0x0d,0x10,0x02,0x08,0x0d,0x0f,0x0d,0x0f,0x10,0x0d,0x03,0x02,0x03,0x0d,
0x10,0x0d,0x0d,0x0d,0x0f,0x0d,0x02,0x02,0x01,0x01,0x03,0x01,0x02,0x0f,0x0d,0x02,
0x01,0x08,0x01,0x01,0x01,0x01,0x01,0x01,0x09,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x03,0x03,0x0b,0x03,0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x0e,0x0e,0x02,
0x05,0x05,0x02,0x10,0x08,0x02,0x0f,0x10,0x10,0x0d,0x0d,0x10,0x0f,0x0d,0x02,0x02,
0x03,0x0d,0x0d,0x10,0x0d,0x0d,0x0d,0x02,0x01,0x01,0x01,0x03,0x01,0x02,0x02,0x01,
0x08,0x01,0x01,0x01,0x01,0x01,0x01,0x0d,0x0d,0x06,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x0b,0x0b,0x03,0x0b,0x0b,0x0b,0x0b,0x00,0x00,0x02,0x05,0x0e,0x05,0x02,
0x05,0x05,0x02,0x10,0x10,0x08,0x02,0x0f,0x0f,0x0d,0x0d,0x0d,0x0d,0x10,0x0d,0x0c,
0x02,0x02,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x02,0x0d,0x01,0x01,0x03,0x01,0x01,0x08,
0x01,0x01,0x01,0x01,0x01,0x05,0x0d,0x0e,0x0e,0x0d,0x06,0x05,0x00,0x00,0x00,0x00,
0x00,0x00,0x03,0x03,0x0b,0x03,0x0b,0x0c,0x0c,0x0c,0x00,0x00,0x0e,0x05,0x02,0x05,
0x02,0x05,0x02,0x10,0x08,0x10,0x0d,0x02,0x0d,0x0f,0x10,0x0d,0x0d,0x0d,0x10,0x0d,
0x10,0x03,0x02,0x02,0x08,0x0f,0x0d,0x0d,0x0d,0x02,0x0d,0x0d,0x01,0x03,0x03,0x06,
0x01,0x01,0x09,0x06,0x09,0x02,0x0d,0x0e,0x0e,0x0d,0x05,0x02,0x01,0x01,0x00,0x00,
0x00,0x03,0x0b,0x0b,0x0b,0x0b,0x0b,0x0c,0x0c,0x0c,0x0c,0x05,0x05,0x05,0x02,0x05,
0x02,0x08,0x09,0x02,0x0d,0x0d,0x10,0x0b,0x02,0x02,0x10,0x0f,0x10,0x0d,0x10,0x10,
0x0d,0x0d,0x10,0x02,0x02,0x02,0x01,0x0d,0x0d,0x0f,0x06,0x0f,0x0f,0x01,0x06,0x07,
0x02,0x06,0x04,0x02,0x02,0x01,0x06,0x0d,0x0d,0x05,0x02,0x07,0x01,0x09,0x09,0x00,
0x00,0x03,0x03,0x03,0x03,0x0b,0x0b,0x0b,0x0c,0x0c,0x0c,0x05,0x05,0x05,0x02,0x05,
0x02,0x07,0x07,0x02,0x08,0x10,0x0d,0x0d,0x0f,0x0b,0x02,0x02,0x10,0x0d,0x0d,0x0f,
0x10,0x0d,0x10,0x0f,0x10,0x0f,0x02,0x02,0x06,0x0f,0x0f,0x06,0x06,0x0f,0x07,0x05,
0x02,0x02,0x02,0x06,0x06,0x07,0x09,0x06,0x05,0x02,0x06,0x09,0x01,0x0e,0x0e,0x00,
0x00,0x03,0x0b,0x0b,0x0b,0x0b,0x0b,0x0c,0x0c,0x0c,0x0c,0x07,0x05,0x0e,0x02,0x05,
0x05,0x02,0x07,0x0f,0x02,0x0f,0x10,0x0f,0x0f,0x10,0x0f,0x0b,0x02,0x02,0x0f,0x0f,
0x10,0x10,0x0f,0x0f,0x10,0x08,0x0d,0x0d,0x02,0x06,0x06,0x0f,0x0f,0x0f,0x0f,0x05,
0x02,0x07,0x07,0x09,0x04,0x01,0x09,0x02,0x02,0x09,0x01,0x01,0x0e,0x0c,0x0c,0x00,
0x00,0x03,0x03,0x03,0x03,0x0b,0x0b,0x0b,0x0c,0x0c,0x0c,0x0c,0x05,0x05,0x02,0x0e,
0x05,0x02,0x0f,0x10,0x10,0x02,0x0f,0x10,0x0f,0x10,0x0f,0x0f,0x0d,0x02,0x02,0x02,
0x08,0x0f,0x10,0x0f,0x0d,0x0d,0x0d,0x0d,0x06,0x0f,0x0f,0x06,0x06,0x06,0x0f,0x05,
0x05,0x02,0x04,0x01,0x02,0x02,0x02,0x01,0x01,0x01,0x01,0x0e,0x0c,0x09,0x09,0x00,
0x00,0x07,0x0b,0x0b,0x0b,0x0b,0x03,0x0b,0x0b,0x0b,0x0b,0x0c,0x05,0x05,0x0e,0x02,
0x05,0x02,0x06,0x07,0x0d,0x01,0x02,0x0f,0x0f,0x10,0x08,0x10,0x10,0x0d,0x0d,0x0b,
0x02,0x02,0x02,0x02,0x0a,0x0d,0x0d,0x0d,0x0d,0x0f,0x06,0x0f,0x0f,0x0f,0x0f,0x02,
0x0f,0x02,0x02,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x09,0x0c,0x09,0x06,0x00,0x00,
0x00,0x06,0x03,0x03,0x03,0x03,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0e,0x0e,0x0e,0x02,
0x05,0x05,0x02,0x07,0x0f,0x0f,0x03,0x02,0x0f,0x0f,0x10,0x0d,0x0f,0x0d,0x0f,0x0f,
0x07,0x0f,0x08,0x01,0x02,0x02,0x02,0x01,0x01,0x0f,0x0f,0x0f,0x0f,0x0f,0x02,0x05,
0x0f,0x02,0x06,0x04,0x01,0x09,0x01,0x01,0x01,0x01,0x04,0x04,0x04,0x00,0x00,0x00,
0x00,0x00,0x07,0x09,0x09,0x03,0x0b,0x0b,0x03,0x03,0x03,0x0a,0x0e,0x06,0x0e,0x0e,
0x0e,0x05,0x05,0x02,0x07,0x07,0x0f,0x0a,0x02,0x02,0x0f,0x0f,0x0d,0x10,0x0d,0x0d,
0x10,0x0f,0x10,0x10,0x07,0x07,0x08,0x0d,0x06,0x06,0x07,0x07,0x07,0x02,0x05,0x05,
0x05,0x02,0x09,0x04,0x01,0x04,0x01,0x09,0x01,0x04,0x05,0x0e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x00,0x00,0x01,0x01,0x0e,
0x0e,0x05,0x05,0x05,0x02,0x06,0x07,0x07,0x0f,0x0d,0x02,0x02,0x02,0x0a,0x10,0x0f,
0x0f,0x0d,0x07,0x07,0x0d,0x10,0x07,0x07,0x07,0x07,0x07,0x07,0x05,0x02,0x05,0x05,
0x02,0x04,0x05,0x04,0x04,0x01,0x04,0x05,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x06,0x06,0x09,0x06,0x05,0x00,0x00,0x00,0x00,0x02,0x01,0x05,
0x05,0x05,0x02,0x05,0x06,0x02,0x09,0x07,0x07,0x07,0x0d,0x0d,0x10,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x07,0x05,0x05,0x05,0x02,
0x09,0x04,0x04,0x04,0x04,0x0e,0x0e,0x05,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x0e,
0x0e,0x05,0x05,0x02,0x05,0x05,0x02,0x0a,0x07,0x07,0x06,0x07,0x0f,0x10,0x10,0x07,
0x07,0x08,0x09,0x07,0x07,0x07,0x08,0x07,0x07,0x06,0x07,0x06,0x05,0x05,0x02,0x04,
0x04,0x05,0x05,0x05,0x0e,0x05,0x0e,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x05,0x04,
0x05,0x05,0x05,0x05,0x02,0x06,0x06,0x02,0x02,0x01,0x07,0x07,0x07,0x07,0x10,0x10,
0x07,0x07,0x07,0x06,0x07,0x06,0x06,0x06,0x09,0x02,0x02,0x02,0x09,0x07,0x06,0x07,
0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x02,
0x05,0x05,0x05,0x06,0x09,0x02,0x06,0x06,0x07,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x09,0x05,0x06,0x06,0x06,0x05,0x05,
0x05,0x05,0x05,0x05,0x0e,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x05,0x02,
0x05,0x02,0x05,0x09,0x05,0x06,0x02,0x06,0x06,0x06,0x06,0x07,0x07,0x07,0x09,0x09,
0x09,0x09,0x04,0x06,0x06,0x06,0x06,0x09,0x05,0x06,0x02,0x05,0x05,0x05,0x05,0x05,
0x05,0x05,0x05,0x0e,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,
0x02,0x04,0x02,0x05,0x04,0x06,0x04,0x02,0x02,0x02,0x04,0x05,0x06,0x05,0x06,0x06,
0x06,0x05,0x06,0x05,0x02,0x02,0x02,0x02,0x02,0x02,0x05,0x05,0x05,0x05,0x05,0x05,
0x04,0x05,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x06,
0x02,0x05,0x02,0x02,0x04,0x06,0x06,0x06,0x06,0x06,0x02,0x02,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x0e,0x05,0x0e,0x0e,0x05,0x05,0x05,0x05,0x05,0x05,0x0e,0x06,
0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,
0x02,0x04,0x09,0x02,0x06,0x06,0x06,0x06,0x09,0x09,0x06,0x06,0x06,0x05,0x05,0x05,
0x05,0x0e,0x05,0x0e,0x0e,0x0e,0x00,0x00,0x00,0x00,0x0f,0x02,0x0f,0x07,0x0f,0x02,
0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,
0x04,0x02,0x06,0x07,0x02,0x01,0x04,0x05,0x05,0x06,0x06,0x06,0x06,0x07,0x06,0x06,
0x05,0x05,0x05,0x0e,0x00,0x00,0x00,0x00,0x00,0x09,0x0f,0x01,0x0f,0x02,0x02,0x04,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x01,0x07,0x02,0x02,0x04,0x02,0x02,0x02,0x02,0x02,0x01,0x02,0x01,0x02,0x01,0x02,
0x05,0x07,0x00,0x00,0x00,0x00,0x00,0x02,0x08,0x02,0x10,0x02,0x02,0x04,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x02,0x01,0x02,0x01,0x02,0x01,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x07,0x07,0x0f,0x02,0x04,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x06,0x0f,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};