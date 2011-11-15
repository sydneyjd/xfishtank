
struct ebmhdr {
	char magic[12];
	unsigned int width, height;
	int num_colors;
};
struct rgb_color {
	unsigned int red, green, blue;
	int pix_len;
};

