#pragma once
/* indexio.h --- save and load index to a file 
 * 
 * 
 * Author: Mikaela A. O'Brien
 * Created: Wed Oct 27 13:27:17 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <hash.h>

/*indexsave() should save an index to a file named indexnm.
 * the index file shall contain one line for each word in the index
 * each line should have the following format:
 * <word> <docID1> <count1> <docID2> <count2>
 *
 * returns 0 for success, nonzero otherwise
 */

int32_t indexsave(hashtable_t *hashtable, char *filename, char *dirname);

/* indexload should load the file <filename> in directory <dirname>
 *
 * returns non-NULL for success, NULL otherwise
 *
 */

hashtable_t *indexload(char *filename, char *dirname);
