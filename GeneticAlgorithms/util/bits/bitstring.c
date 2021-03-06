#include "bits/bitstring.h"
#include "core/random.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define B bitstring_B

static unsigned char individual_bit_masks[] = {
	/* 0 */ //0x00,
	/* 1 */ 0x01,
	/* 2 */ 0x02,
	/* 3 */ 0x04,
	/* 4 */ 0x08,
	/* 5 */ 0x10,
	/* 6 */ 0x20,
	/* 7 */ 0x40,
	/* 8 */ 0x80
};

static unsigned char bit_masks[] = {
	/* 0000 0000 */ 0x00,
	/* 0000 0001 */ 0x01,
	/* 0000 0011 */ 0x03,
	/* 0000 0111 */ 0x07,
	/* 0000 1111 */ 0x0F,
	/* 0001 1111 */ 0x1F,
	/* 0011 1111 */ 0x3F,
	/* 0111 1111 */ 0x7F, /* 1111 1111 */ 0xFF
};

//static int seed = -1;

static unsigned char and(unsigned char a, unsigned char b) {
	return a & b;
}

static unsigned char or(unsigned char a, unsigned char b) {
	return a | b;
}


static unsigned char xor(unsigned char a, unsigned char b) {
	return a ^ b;
}


struct B {
	unsigned char *stream;
	unsigned int length;   /* byte length in bitstream */
	unsigned int bits;     /* number of bits usable in stream[length-1]; */
	/* i.e bitlength = (length-1)*8 + bits; */
};

B bitstring_new(unsigned int bitlength) {
	assert(bitlength);

	unsigned int bits = bitlength %8;
	unsigned int length = bits ? bitlength/8 + 1 : bitlength/8;
	bits = bits ? bits : 8;

	B result;
	result = malloc(sizeof(*result));
	assert(result);
	result->length = length;
	result->bits   = bits;
	result->stream = calloc(sizeof(*result->stream),length);
	assert(result->stream);
	return result;
}

void bitstring_delete(B string) {
	assert(string);
	free(string->stream);
	free(string);
}

B bitstring_copy(B string) {
	B copy = bitstring_new(bitstring_get_bitlength(string));
	unsigned int i;
	for(i = 0; i < string->length; ++i) {
		copy->stream[i] = string->stream[i];
	}

	return copy;
}

bitstring_B  bitstring_csv_fload(FILE *fp, unsigned int size) {

	bitstring_B string = bitstring_new(size * 8);
	unsigned int i,j;
	//char buf[10];
	//sprintf(buf, "%s,", MATRIX_TYPE_FMT_STR);

	for(i = 0; i < size; i++) {
		unsigned int value = 0;
		fscanf(fp, "%d ", &value);
		uint8_t result = value;

		for(j = 0; j < 8; ++j) {
			if(result & individual_bit_masks[j]) {
				bitstring_bitset(string, i * 8 + j);
			}
		}	
		

		char c = getc(fp);

		//assert((c == ',' || c == EOF || c == '\0' || c == '\n'));
	
		if(!(c == ',' || c == EOF || c == '\0' || c == '\n'))
			ungetc(c,fp);
	}
	return string;
}


int bitstring_eq(B stringA, B stringB) {
    if(bitstring_get_bitlength(stringA) != bitstring_get_bitlength(stringB))
	    return 0;
    unsigned int i;
    for(i = 0; i < stringA->length; ++i) {
	if(i != stringA->length - 1) {
		if(stringA->stream[i] != stringB->stream[i])
			return 0;
	} else {
		if((stringA->stream[i]&bit_masks[stringA->bits]) != (stringB->stream[i]&bit_masks[stringA->bits]))
			return 0;
	}
    }
    return 1;
}


B bitstring_random(unsigned int bitlength) {
	B result = bitstring_new(bitlength);

	unsigned int i = 0;
	for(i = 0; i < result->length; ++i){
		if(i != result->length-1)
			result->stream[i] = random_random();
		else
			result->stream[i] = (unsigned char)(((unsigned int)random_random())%256) & bit_masks[result->bits];
	}

	return result;
}


void bitstring_set(B string,unsigned char sequence, unsigned int startbit, unsigned int sequence_length) {

	unsigned int bitlength = bitstring_get_bitlength(string);

	assert(startbit < bitlength
	&& startbit + sequence_length <= bitlength
	&& sequence_length <= 8);

	unsigned int first_byte_offset = startbit %8;
	unsigned int first_byte_index  = startbit/8;
	string->stream[first_byte_index] |= ((sequence & bit_masks[sequence_length]) << first_byte_offset) & 0xFF;
	if(startbit + sequence_length > 8) {
string->stream[first_byte_index+1] |= ((sequence >> (8 - first_byte_offset))) & bit_masks[sequence_length + first_byte_offset -8];
	}
}

void bitstring_bitset(B string, unsigned int bitposition) {
	assert(bitposition < bitstring_get_bitlength(string));
	unsigned int byte_index = bitposition/8;
	unsigned int byte_offset = bitposition %8;
	string->stream[byte_index] |= 1<<byte_offset;
}

void bitstring_bitclear(B string, unsigned int bitposition){
	assert(bitposition < bitstring_get_bitlength(string));
	unsigned int byte_index = bitposition/8;
	unsigned int byte_offset = bitposition %8;
	// if 0 -> 1, if 1 -> 0
	(string->stream[byte_index] ^= (string->stream[byte_index] & individual_bit_masks[byte_offset]));
}

int bitstring_bittest(B string, unsigned int bitposition) {
	assert(bitposition < bitstring_get_bitlength(string));
	unsigned int byte_index = bitposition/8;
	unsigned int byte_offset = bitposition %8;
	if(string->stream[byte_index] & individual_bit_masks[byte_offset])
		return 1;
	else 
		return 0;
}

B bitstring_map(B stringA, B stringB,
	unsigned char (*function) (unsigned char , unsigned char )) {

	assert(bitstring_get_bitlength(stringA) == bitstring_get_bitlength(stringB));
	B result = bitstring_new(bitstring_get_bitlength(stringA));
		int i;
	for(i = 0; (unsigned int)i < stringA->length; i++) {
		unsigned char value = 0;
		value = function(stringA->stream[i], stringB->stream[i]);
		if((unsigned int)i == (unsigned int)stringA->length-1)
			value &= bit_masks[stringA->bits];
		result->stream[i] = value;
	}	
	return result;
}

B bitstring_and(B stringA, B stringB) {
	return bitstring_map(stringA, stringB, and);
}

B bitstring_or(B stringA, B stringB) {
	return bitstring_map(stringA, stringB, or);
}

B bitstring_xor(B stringA, B stringB) {
	return bitstring_map(stringA, stringB, xor);
}

B bitstring_crossover(B stringA, B stringB, unsigned int pivot){
		assert(bitstring_get_bitlength(stringA) == bitstring_get_bitlength(stringB));
		assert(pivot < bitstring_get_bitlength(stringA));
	B result = bitstring_new(bitstring_get_bitlength(stringA));


	unsigned int toA = pivot / 8;
	unsigned int toAbits = pivot %8;
	unsigned int i;
	
	for(i = 0; i < result->length; i++) {
		if(i < toA) {
			result->stream[i] = stringA->stream[i];
		}
		else if(i == toA && toAbits){
			result->stream[i] |= stringA->stream[i] &bit_masks[toAbits];
			result->stream[i] |= stringB->stream[i] & ~bit_masks[toAbits];	
		} else {
			result->stream[i] = stringB->stream[i];
		}

	}
	return result;
}

uint16_t bitstring_int(B string) {

	assert(string);
	uint16_t value = 0;
	
	unsigned int i;
	for(i = 0; i < string->length; i++) {
		value |= string->stream[i] << 8*i;
	}

	return value;

}

unsigned int bitstring_bitcount(B string) {
	unsigned int i,j;
	unsigned int count = 0;

	for(i = 0; i <string->length; ++i) {
		unsigned int max = 8;
		if(i == string->length-1)
			max = string->bits;
		unsigned char current_bytes = string->stream[i];
		for(j = 0; j < max; ++j) {
			if(1 & (current_bytes >> j))
				count++;
		}
//		if(i != string->length -1)
//			printf(",");
	}
	return count;

}


void bitstring_print(B string) {
	unsigned int i,j;

	for(i = 0; i <string->length; ++i) {
		unsigned int max = 8;
		if(i == string->length-1)
			max = string->bits;
		unsigned char current_bytes = string->stream[i];
		for(j = 0; j < max; ++j) {
			if(1 & (current_bytes >> j))
				printf("1");
			else printf("0");
		}
//		if(i != string->length -1)
//			printf(",");
	}
}

unsigned int bitstring_get_bitlength(B string) {
	assert(string);
	return (string->length - 1) * 8 + string->bits;
}

bitstring_B bitstring_generate(char *str) {
	assert(str);
	bitstring_B string;
	size_t size = strlen(str);
	string = bitstring_new(size);

	unsigned int i;
	for(i = 0; i < size; ++i){
		if(str[i] == '1') {
			bitstring_bitset(string,i);
		} else if(str[i] == '0') {
			bitstring_bitclear(string, i);
		}
	}
	return string;
}

void bitstring_mutate(B string, unsigned int mutation_count, double mutation_probability) {
	unsigned int i;
	unsigned int length = bitstring_get_bitlength(string);
	for(i = 0; i< mutation_count; ++i) {
		if(random_normal(0,1) < mutation_probability) {
			unsigned int pos = (unsigned int)random_range(0, length + 1);
			pos %= length;

			if(bitstring_bittest(string, pos)) bitstring_bitclear(string,pos);
			else bitstring_bitset(string, pos);
		}
	}

}


unsigned int bitstring_simmilarity(B stringA, B stringB) {
	assert(bitstring_get_bitlength(stringA) == bitstring_get_bitlength(stringB));
	
	unsigned int i,j;
	unsigned int result = 0;

	for(i = 0; i <stringA->length; ++i) {
		unsigned int max = 8;
		if(i == stringA->length-1)
			max = stringA->bits;
		for(j = 0; j < max; ++j) {
			if((bitstring_bittest(stringA, (i * 8 + j)) && bitstring_bittest(stringB, (i * 8 + j))) || (!bitstring_bittest(stringA, (i * 8 + j)) && !bitstring_bittest(stringB, (i * 8 + j)))) {
				result++;
			}
		}
//		if(i != (int)stream->length -1)
//			printf(",");
	}

	return result;
}


unsigned int bitstring_asbuckets(B stringA, unsigned int buckets) {
	assert(stringA);
	assert(buckets <= bitstring_get_bitlength(stringA));

	unsigned int total_size = bitstring_get_bitlength(stringA);

	unsigned int last_bucket_start = 0;
	unsigned int bucket_size = total_size / buckets;
	unsigned int current_bucket = 0;
	unsigned int current_bucket_score = 0;
	unsigned int best_bucket = 0;
	unsigned int best_bucket_score = 0;
	unsigned int i;

	for(i = 0; i < total_size; i++) {
		//printf("checking i(%d) - last_bucket_start(%d) = (%d) >= bucket_size (%d)\n", i, last_bucket_start, i - last_bucket_start, bucket_size);
		if(i - last_bucket_start >= bucket_size) {
			last_bucket_start = i;
			if(current_bucket_score > best_bucket_score) {
				best_bucket_score = current_bucket_score;
				best_bucket = current_bucket;
			}
			current_bucket_score = 0;
			current_bucket++;

		} 
		if(bitstring_bittest(stringA, i)) {
			current_bucket_score++;
		}

	}
	if(current_bucket_score > best_bucket_score) {
		best_bucket_score = current_bucket_score;
		best_bucket = current_bucket;
	}


	return best_bucket;
}

bitstring_B bitstring_newbucketstring(unsigned int size, unsigned int buckets, unsigned int index) {
	assert(index < buckets);
	assert(size >= buckets);
	assert(size >= 0);

	bitstring_B result = bitstring_new(size);
	unsigned int bits_per_bucket = size / buckets;
	unsigned int bits_of_last_bucket = 0;
	unsigned int current_bucket = 0;
	unsigned int i = 0;
	for(i = 0; i < size; ++i) {
		if(i - bits_of_last_bucket > bits_per_bucket) {
			bits_of_last_bucket = i;
			current_bucket++;	
		}
		if(current_bucket == index) {
			bitstring_bitset(result, i);
		}
	}

	return result;
}
