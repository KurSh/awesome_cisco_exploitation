/* $Id: xunpak.c,v 3.2 1995/11/17 07:42:06 hampton Exp $
 * $Source: /swtal/cherf.111/ios/boot/xunpak.c,v $
 *------------------------------------------------------------------
 * LZHUF.C English version 1.0
 *
 * Based on Japanese version 29-NOV-1988
 * LZSS coded by Haruhiko OKUMURA
 * Adaptive Huffman Coding coded by Haruyasu YOSHIZAKI
 * Edited and translated to English by Kenji RIKITAKE
 *------------------------------------------------------------------
 * $Log: xunpak.c,v $
 * Revision 3.2  1995/11/17  07:42:06  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  09:05:09  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.1  1995/06/07  19:13:04  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

#ifndef LIB
#  include <stdio.h>
#  include <string.h>
#  include <ctype.h>

  /* These values are Turbo-C dependent;
   EXIT_SUCCESS, EXIT_FAILURE
   renamed by Kenji */

#  define EXIT_OK 0
#  define EXIT_FAILED -1

   FILE  *infile, *outfile;
#endif

   unsigned char *inptr, *outptr, *instart, *outstart;
   long inlength;
#  define putc(c,file) *outptr++ = c

unsigned long int  textsize = 0, codesize = 0, printcount = 0;

void Error(char *message)
{
#ifndef LIB
	printf("\n%s\n", message);
	exit(EXIT_FAILED);
#endif
}

/* LZSS Parameters */

#define N		4096	/* Size of string buffer */
#define F		60	/* Size of look-ahead buffer */
#define THRESHOLD	2
#define NIL		N	/* End of tree's node  */

/* Huffman coding parameters */

#define N_CHAR  	(256 - THRESHOLD + F)
				/* character code (= 0..N_CHAR-1) */
#define T 		(N_CHAR * 2 - 1)	/* Size of table */
#define R 		(T - 1)			/* root position */
#define MAX_FREQ	0x8000
					/* update when cumulative frequency */
					/* reaches to this value */

typedef unsigned char uchar;

/*
 * Tables for encoding/decoding upper 6 bits of
 * sliding dictionary pointer
 */

/* decoder table */
uchar d_code[256] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
/*40*/	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
/*80*/	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
/*120*/	0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
	0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
	0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
	0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D,
	0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F,
/*160*/	0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11,
	0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13,
	0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15,
	0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
	0x18, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B,
	0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F,
	0x20, 0x20, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23,
	0x24, 0x24, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27,
	0x28, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B,
	0x2C, 0x2C, 0x2D, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
};
#ifdef V0
uchar d_len[256] = {
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
};
#endif
unsigned char text_buf[N + F - 1];
short match_position, match_length;

unsigned short freq[T + 1];	/* cumulative freq table */

/*
 * pointing parent nodes.
 * area [T..(T + N_CHAR - 1)] are pointers for leaves
 */
short prnt[T + N_CHAR];

/* pointing children nodes (son[], son[] + 1)*/
short son[T];

unsigned short getbuf = 0;
uchar getlen = 0;

unsigned short GetBit(void)	/* get one bit */
{
	short i;

	while (getlen <= 8) {
		if (inlength > 0) {
		    i = *inptr++; inlength--;
		} else i = 0;
		getbuf |= i << (8 - getlen);
		getlen += 8;
	}
	i = getbuf;
	getbuf <<= 1;
	getlen--;
	return (i < 0);
}


int GetByte(void)	/* get a byte */
{
	unsigned short i;

	while (getlen <= 8) {
		i = inlength > 0 ? inlength--, *inptr++ : 0;
		getbuf |= (unsigned short) i << (8 - getlen);
		getlen += 8;
	}
	(unsigned short) i = getbuf;
	getbuf <<= 8;
	getlen -= 8;
	return((unsigned short) i >> 8);
}


/* initialize freq tree */

void StartHuff()
{
	short i, j;

	for (i = 0; i < N_CHAR; i++) {
		freq[i] = 1;
		son[i] = i + T;
		prnt[i + T] = i;
	}
	i = 0; j = N_CHAR;
	while (j <= R) {
		freq[j] = freq[i] + freq[i + 1];
		son[j] = i;
		prnt[i] = prnt[i + 1] = j;
		i += 2; j++;
	}
	freq[T] = 0xffff;
	prnt[R] = 0;
}


/* reconstruct freq tree */

void reconst()
{
	short i, j, k;
	unsigned short f, l;

	/* halven cumulative freq for leaf nodes */
	j = 0;
	for (i = 0; i < T; i++) {
		if (son[i] >= T) {
			freq[j] = (freq[i] + 1) / 2;
			son[j] = son[i];
			j++;
		}
	}
	/* make a tree : first, connect children nodes */
	for (i = 0, j = N_CHAR; j < T; i += 2, j++) {
		k = i + 1;
		f = freq[j] = freq[i] + freq[k];
		for (k = j - 1; f < freq[k]; k--);
		k++;
		l = (j - k) * sizeof(short);
		
		(void)bcopy(&freq[k], &freq[k + 1], l);
		freq[k] = f;
		bcopy(&son[k], &son[k + 1], l);
		son[k] = i;
	}
	/* connect parent nodes */
	for (i = 0; i < T; i++) {
		if ((k = son[i]) >= T) {
			prnt[k] = i;
		} else {
			prnt[k] = prnt[k + 1] = i;
		}
	}
}


/* update freq tree */

void update(short c)
{
	short i, j, k, l;

	if (freq[R] == MAX_FREQ) {
		reconst();
	}
	c = prnt[c + T];
	do {
		k = ++freq[c];

		/* swap nodes to keep the tree freq-ordered */
		if (k > freq[l = c + 1]) {
			while (k > freq[++l]);
			l--;
			freq[c] = freq[l];
			freq[l] = k;

			i = son[c];
			prnt[i] = l;
			if (i < T) prnt[i + 1] = l;

			j = son[l];
			son[l] = i;

			prnt[j] = c;
			if (j < T) prnt[j + 1] = c;
			son[c] = j;

			c = l;
		}
	} while ((c = prnt[c]) != 0);	/* do it until reaching the root */
}

short DecodeChar()
{
	unsigned short c;

	c = son[R];

	/*
	 * start searching tree from the root to leaves.
	 * choose node #(son[]) if input bit == 0
	 * else choose #(son[]+1) (input bit == 1)
	 */
	while (c < T) {
		c += GetBit();
		c = son[c];
	}
	c -= T;
	update(c);
	return(c);
}

short DecodePosition()
{
	unsigned short i, j, c;

	/* decode upper 6 bits from given table */
	i = GetByte();
	c = (unsigned short)(d_code[i]) << 6;
#ifdef V0
	j = d_len[i];
#else
	if (i < 32) j = 3;
	else if (i < 80) j = 4;
	else if (i < 144) j = 5;
	else if (i < 192) j = 6;
	else if (i < 240) j = 7;
	else j = 8;
#endif
	/* input lower 6 bits directly */
	j -= 2;
	while (j--) {
		i = (i << 1) + GetBit();
	}
	return(c | i & 0x3f);
}

void Decode(void)  /* Decoding/Uncompressing */
{
	short  i, j, k, r, c;
	unsigned long int  count;

	bcopy(inptr, &textsize, sizeof textsize);
	inlength -= sizeof textsize;
	inptr += sizeof textsize;
#ifndef LIB
	fprintf(stderr, "\nOriginal file size %ld", textsize);
#endif

	if (textsize == 0)
		return;
	StartHuff();
	for (i = 0; i < N - F; i++)
		text_buf[i] = 0;
	r = N - F;
	for (count = 0; count < textsize; ) {
		c = DecodeChar();
		if (c < 256) {
			putc(c, outfile);
			text_buf[r++] = c;
			r &= (N - 1);
			count++;
		} else {
			i = (r - DecodePosition() - 1) & (N - 1);
			j = c - 255 + THRESHOLD;
			for (k = 0; k < j; k++) {
				c = text_buf[(i + k) & (N - 1)];
				putc(c, outfile);
				text_buf[r++] = c;
				r &= (N - 1);
				count++;
			}
		}
	}
	printf("%12ld\n", count);
}

#ifndef LIB
int main(int argc, char *argv[])
{
	char  *s;

	printf("\nSizes of data structures:");
	printf("\n  text_buf = %6d", sizeof text_buf);
	printf("\n  freq =     %6d", sizeof freq);
	printf("\n  prnt =     %6d", sizeof prnt);
	printf("\n  son  =     %6d\n\n", sizeof son);

	if (argc != 4) {
		printf("Usage:lzhuf e(compression)|d(uncompression)"
			" infile outfile\n");
		return EXIT_FAILED;
	}
	if ((s = argv[1], s[1] || strpbrk(s, "DEde") == NULL)
	 || (s = argv[2], (infile  = fopen(s, "rb")) == NULL)
	 || (s = argv[3], (outfile = fopen(s, "wb")) == NULL)) {
		printf("$@HHH(J %s\n", s);
		return EXIT_FAILED;
	}
	fseek(infile, 0L, 2);
	textsize = ftell(infile);
	rewind(infile);
	fprintf(stderr,"\nInput file size %ld", textsize);
	inptr = malloc(textsize + 1000);
	instart = inptr;
	if (fread(inptr, textsize, 1, infile) < 1)
		Error("Unable to read");  /* read size of original text */
	outstart = malloc(textsize * 2);
	outptr = outstart;
	inlength = textsize;
	Decode();
	fwrite(outstart, textsize, 1, outfile);
	fclose(infile);
	fclose(outfile);
	free(instart);
	free(outstart);
	return EXIT_OK;
}
#else
uncompress(in,out,len)
    unsigned char *in, *out;
    long len;
{
    inptr = in;
    outptr = out;
    inlength = len;
    Decode();
}
#endif
