#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef unsigned int uint;
typedef unsigned char uchar;

#include "radix_sort.h"

#define BITS_IN_BYTE 8
#define OPTIONS_IN_BYTE 256
#define HISTOGRAMS_ARR_SIZE OPTIONS_IN_BYTE

static void ByteSort(uint src[], uint dest[], size_t histograms[],
					 size_t num_of_elements, size_t byte_number);
static uchar IsolateByte(int byte_number, uint num);


/*******************************************************************/

int RadixSort(uint *arr, size_t arr_size)
{
	uint *new_arr = NULL;
	size_t *histograms = NULL;

	assert(arr != NULL);

	/* array to store the sorted elements */
	new_arr = malloc(arr_size * sizeof(uint));
	if (NULL == new_arr)
	{
		return (1);
	}

	/* array to count elements in each "bucket" during the mid-sorts */
	histograms = malloc(HISTOGRAMS_ARR_SIZE * sizeof(size_t));
	if (NULL == histograms)
	{
		free(new_arr);
		new_arr = NULL;

		return (1);
	}

	/* mid-sorts go from least significant byte to most significant byte,
	using both "arr" and "new_arr" to re-arrange elements*/
	ByteSort(arr, new_arr, histograms, arr_size, 0);
	ByteSort(new_arr, arr, histograms, arr_size, 1);
	ByteSort(arr, new_arr, histograms, arr_size, 2);
	ByteSort(new_arr, arr, histograms, arr_size, 3);

 	free(histograms);
	histograms = NULL; 
 	free(new_arr);
	new_arr = NULL; 

	return (0);	
}

/*********/
static void ByteSort(uint src[], uint dest[], size_t histograms[],
					 size_t num_of_elements, size_t byte_number)
{
	size_t i = 0;
	uint idx_in_histograms = 0;
	uint curr_element = 0;
	uchar isolated_byte = 0;

	/* sets array to all zeros */
	memset(histograms, 0, HISTOGRAMS_ARR_SIZE * sizeof(size_t));

	/* count how many elements match to each histogram */	
	for (i = 0; i < num_of_elements; ++i)
	{
		curr_element = src[i];
		isolated_byte = IsolateByte(byte_number, curr_element);
		++(histograms[isolated_byte]);
	}

	idx_in_histograms = 0;
	/* turn the counters in histograms array into indexes */
	for (i = 0; i < HISTOGRAMS_ARR_SIZE; ++i)
	{
		uint tmp = histograms[i];

		histograms[i] = idx_in_histograms;
		idx_in_histograms += tmp;
	}

	/* for each element, check the matching histogram, and place the element
	in the appropriate index in the sorted array */
	for (i = 0; i < num_of_elements; ++i)
	{
		static uint idx_in_dest = 0;

		curr_element = src[i];
		isolated_byte = IsolateByte(byte_number, curr_element);
		idx_in_histograms = isolated_byte;

		idx_in_dest = histograms[idx_in_histograms]++;
		dest[idx_in_dest] = curr_element;		
	}
}

/*********/
static uchar IsolateByte(int byte_number, uint num)
{
	/* implicitly converted to uchar */
	return ((num >> byte_number * BITS_IN_BYTE) & 0xFF);
}
