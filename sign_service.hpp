#pragma once
#include <unordered_map>

const int result_ok = 0; //All OK.
const int result_err = 1; //Some error. See result for details.

extern "C" char * key_create();

extern "C" char * block_create_c (
	char * type_text, 
	char * account_text,
	char * representative_text,
	char * link_text,
	char * balance_text,
	char * previous_text,
	char * work_text,
	char * key_text,
	int  * errorCode);