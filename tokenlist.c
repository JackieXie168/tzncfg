/*
 * tokenlist.c
 *
 * Created by Jackie Xie on 2011-07-17.
 * Copyright 2011 Jackie Xie. All rights reserved.
 *
 */

#include <xutils/xutils.h>

int main(int argc, char* argv[])
{
	char *str = NULL, *value = NULL, *delimiter = NULL, *tmp = NULL, *del = NULL;
	int position=0, c, i = 0, func = 0, val;
	char help_msg[1024] = "Usage: tokenlist [-s | -x <token list>] {[-a | -m <values> -p <position>] | [ -n <to get # of digits>] | [-i <position>] | [-v <value>] | [-e <value>] | [ -r <position>]} [-d delimiter]\n\n";
	
	strcat(help_msg, "tokenlist command summary\n");
	strcat(help_msg, "\ttokenlist is a function to insert/delete/modify/view token list by delimiter(s).\n");
	strcat(help_msg, "\t<token list>：input a token list.\n");
	strcat(help_msg, "\tdelimiter：a delimiter which is a seperator.\n");
	strcat(help_msg, "\t-s : to specify a token list.\n");
	strcat(help_msg, "\t-x : to get total counts of token by specified delimiter.\n");
	strcat(help_msg, "\t-d : to specify a delimiter.\n");
	strcat(help_msg, "\t-a：insert a value to the token list by delimiter.\n");
	strcat(help_msg, "\t-r : remove a value from a token list by specfied position.\n");
	strcat(help_msg, "\t-v : remove a value from a token list by specfied value.\n");
	strcat(help_msg, "\t-e : to check a value in a token list is exist or not.\n");
	strcat(help_msg, "\t-o : to replace delimiters in a token list by new delimiters.\n");
	strcat(help_msg, "\t-n : to get digits in a token list by speficied counts.\n");
	strcat(help_msg, "\t-m : to change a value by spcified position from a token list.\n");
	strcat(help_msg, "\t-i : to return the value which is at the specified position from a token list by delimiter.\n\n");

	if(argc <= 1 || ((isgraph(*argv[1]) || ispunct(*argv[1])) && *argv[1]!='-')){
		fprintf(stderr, "%s", help_msg);
		exit(0);
	}
	while ((c = getopt(argc, argv, "a:d:e:i:m:n:o:p:r:v:s:x:h")) != -1){
		switch (c) {
			case 'a':
				func = 1;
				value = optarg;
				break;
			case 'r':
				func = 2;
				position = atoi(optarg);
				break;
			case 'm':
				func = 3;
				value = optarg;
				break;
			case 's':
				str = optarg;
				break;
			case 'i':
				func = 4;
				position = atoi(optarg);
				break;
			case 'v':
				func = 5;
				value = optarg;
				position = 1;
				break;
			case 'x':
				func = 6;
				str = optarg;
				break;
			case 'e':
				func = 7;
				value = optarg;
				position = 1;
				break;
			case 'n':
				func = 8;
				val = atoi(optarg);
				position = 1;
				break;
			case 'o':
				func = 9;
				value = optarg;
				break;
			case 'p':
				position = atoi(optarg);
				break;
			case 'd':
				delimiter = optarg;
				break;
			case 'h':
				fprintf(stderr, "%s", help_msg);
				exit(0);
				break;
			default:
				fprintf(stderr, "%s", help_msg);
				exit(0);
				break;
		}
	}

	if(str == NULL && delimiter != NULL){
		if(func == 1 && value != NULL){
			tmp = StrDup(str);
			//printf("Inserting %s at position 1 ...\n", value);
			//tmp = StrDup(insert_str(nvram_get(nvram), value, delimiter, 1));
			str = insert_str(tmp, value, delimiter, 1);
			printf("%s\n", str);
		}
		else{
			fprintf(stderr, "%s", help_msg);
			exit(0);
		}
	}
	else if(delimiter != NULL && position != 0 && str != NULL){
		if(func == 1 && value != NULL){
			//printf("Inserting %s at position %d ...\n", value, position);
			tmp = StrDup(str);
			str = insert_str(str, value, delimiter, position);
			printf("%s\n", str);
		}
		else if(func == 2){
			//printf("Deleting position %d of %s ... \n", position, str);
			tmp = StrDup(str);
			str = delete_str(tmp, delimiter, position);
			printf("%s\n", str);
		}
		else if(func == 3 && value != NULL){
			//printf("Modifying position %d by %s ... \n", position, value);
			tmp = StrDup(str);
			str = modify_str(tmp, value, delimiter, position);
			printf("%s\n", str);
		}
		else if(func == 4){
			//printf("\nReading the values from token list %s ... \n", str);
			tmp = StrDup(str);
			//printf("%s[%d] = %s\n", str, position, index_str(tmp, delimiter, position));
			printf("%s\n", index_str(tmp, delimiter, position));
		}
		else if(func == 5){
			tmp = StrDup(str);
			str = delete_val(tmp, value, delimiter);
			printf("%s\n", str);
		}
		else if(func == 7){
			tmp = StrDup(str);
			if(val_exist(tmp, value, delimiter))
				printf("%s is exist\n", value);
			else
				printf("%s is not exist\n", value);
		}
		else if(func == 8){
			tmp = StrDup(str);
			printf("%s\n", str2digits(tmp, delimiter, val));
		}
		else{
			fprintf(stderr, "%s", help_msg);
			exit(0);
		}
	}
	else if(func == 6 && delimiter != NULL && str != NULL){
		tmp = StrDup(str);
		position = matchStrPosAt(delimiter, tmp, -1) + 1;
		printf("%d\n", position);
	}
	else if(func == 9 && delimiter != NULL && str != NULL && value != NULL){
		tmp = StrDup(str);
		if(strstr(value, delimiter) && strlen(value) > strlen(delimiter)){
		/* this code section is fix the old delimiter(s) is a subset of new delimiter(s) issue */
			char temp_deli[]="olddeliisasubsetofnewdeli";
			str = replaceall(tmp, delimiter, temp_deli, -2, matchStrPosAt(delimiter, tmp, -1));
			StrFree(tmp);
			tmp = StrDup(str);
			str = replaceall(tmp, temp_deli, value, -2, matchStrPosAt(temp_deli, tmp, -1));
		}
		else
			str = replaceall(tmp, delimiter, value, -2, matchStrPosAt(delimiter, tmp, -1));
		printf("%s\n", str);
	}
	else{
		fprintf(stderr, "%s", help_msg);
		exit(0);
	}

	StrFree(tmp); tmp = NULL;
	return 0;
}
