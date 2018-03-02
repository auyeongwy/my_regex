/* Copyright 2018 Au Yeong Wing Yau

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
 */



/** @file my_regex.c
 *  Reference implementation of regex library. This example extracts key/value pairs from a string.
 *  Usage: 
 *  Just run my_regex
 */

#include <stdio.h>
#include <regex.h>
#include <string.h>

#define BUF_LEN 64


/** Main function of the application.
 *  @param argc Number of command line arguments.
 *  @param argv Array of command line arguments.
 *  @return 0 if success. Else -1.
 */
int main(int argc, char *argv[])
{
	regex_t re; /* The regex expression. */
	regmatch_t matches[3]; /* Regext matched strings. */
	int ret, len, max_len, offset; 
	char buf[BUF_LEN];
	char *txt_ptr, *char_ptr;
	char *const text = "option1=value1\noption2=value2"; /* The text to match against. */
	const char *const regexp = "([^=]+)=([^\n]+)"; /* The regular expression to match the text and extract the option/value pair. */
	
	
	if((ret = regcomp(&re, regexp, REG_EXTENDED)) != 0) { /* Compile the regex expression. */
		regerror(ret, &re, buf, BUF_LEN);
		buf[BUF_LEN - 1] = 0;
		printf("%s\n", buf);
		ret = -1;
		goto do_exit;
	} 
	
	
	max_len = strlen(text);
	offset = 0; 
	while(offset < max_len) { /* Need to iterate through the string to get the key/values. */
	
		txt_ptr = text + offset;
		if(regexec(&re, txt_ptr, 3, matches, 0) == 0) { /* Perform the match. */
			printf("Match OK\n");

			/* Print the matched string positions. */
			printf("%d %d\n", matches[0].rm_so, matches[0].rm_eo); 
			printf("%d %d\n", matches[1].rm_so, matches[1].rm_eo);
			printf("%d %d\n", matches[2].rm_so, matches[2].rm_eo);
		
			/* Get the option match. */
			char_ptr = txt_ptr + matches[1].rm_so;
			if((len = matches[1].rm_eo - matches[1].rm_so) > BUF_LEN)
				len = BUF_LEN-1;
			memcpy(buf, char_ptr, len);
			buf[len] = 0;
			printf("option:%s ", buf);
		
			/* Get the value match. */
			char_ptr = txt_ptr + matches[2].rm_so;
			if((len = matches[2].rm_eo - matches[2].rm_so) > BUF_LEN)
				len = BUF_LEN-1;
			memcpy(buf, char_ptr, len);
			buf[len] = 0;
			printf("value:%s\n", buf);
			
			offset += (matches[2].rm_eo + 1); /* After extracting a pair of values, increase the offset to the next part of the string. */
		}
		else {
			printf("Match NOK\n");
			break;
		}
	}
	
do_exit:
	regfree(&re);
	return ret;
}