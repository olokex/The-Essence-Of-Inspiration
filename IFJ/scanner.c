/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: scanner.c
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


#include "scanner.h"
#include "utils.h"


//stavy automatu
#define ENUM_AUTOMATA_STATE(a)\
	a(state_start)\
	a(state_end)\
	a(state_string)\
	a(state_string_esc)\
	a(state_string_esc_hex)\
	a(state_string_esc_hex_1)\
	a(state_identifier)\
	a(state_identifier_check)\
	a(state_integer)\
	a(state_double)\
	a(state_double_e)\
	a(state_double_value)\
	a(state_double_e_mark)\
	a(state_double_e_value)\
	a(state_less)\
	a(state_more)\
	a(state_equal)\
	a(state_not_equal)\
	a(state_line_comment)\
	a(state_block_comment_check_b)\
	a(state_block_comment_check_e)\
	a(state_block_comment_check_g)\
	a(state_block_comment_check_i)\
	a(state_block_comment_check_n)\
	a(state_block_comment_start_check_space)\
	a(state_block_comment_value)\
	a(state_block_comment_end_check)\
	a(state_block_comment_end_check_e)\
	a(state_block_comment_end_check_n)\
	a(state_block_comment_end_check_d)\
	a(state_block_comment_end_check_space)\
	a(state_block_comment_end)\




enum tokenstate 
{
	ENUM_AUTOMATA_STATE(MAKE_ENUM)
};

const char *tokenstate_str[] = 
{
	ENUM_AUTOMATA_STATE(MAKE_STRING)
};

token_t token;

static int line_number;
static int char_number;

static FILE* file_handle;
static utils_string_t *line_us;
static char *line_buffer;
static bool hit_eof;

static int hexa_array[2];		//pole pro prevod HEXA znaku na cislo


void cleanup_behind_scanner()
{
	utils_string_destroy(line_us);
}

void init_scanner() 
{
    line_number = 0;
    char_number = 0;
    hit_eof = false;
    file_handle = stdin;
    

    line_us = utils_string_init();
    line_buffer = line_us->string;
    atexit(cleanup_behind_scanner);
}

void set_input_file(FILE *fh) 
{
    hit_eof = false;
    line_number = 0;
    char_number = 0;


    utils_string_clear(line_us);
    line_buffer = line_us->string;
    file_handle = fh;
}

int get_char_from_stdin()
{
	static int prev_char = 0;
	int ch = fgetc(file_handle);


	if (ch == EOF && prev_char != '\n' && prev_char != EOF)
	{
		ch = '\n';
	}
	prev_char = ch;
	return ch;
}

void load_next_line() 
{
    int c;
    utils_string_clear(line_us);


    while ((c = get_char_from_stdin()) != EOF) 
    {
        utils_string_add_char(line_us, c);
        if (c == '\n')
            break;
    }
    hit_eof = (c == EOF);
    line_buffer = line_us->string;
    line_number++;
    char_number = 0;
}

int get_next_char() 
{
    if (line_buffer[char_number] != '\0') 
    {
        return (unsigned char)line_buffer[char_number++];
    } 
    else if (hit_eof) 
    {
        return EOF;
    } 
    else 
    {
        load_next_line();
        return get_next_char();
    }
}

void unget_next_char(int c) 
{
    if (line_buffer[char_number] == '\0' && hit_eof)
        return;
    char_number--;
}


tokentype_t check_if_keyword(char *s) 
{
	//klicova slova
	if(strcmp(s,"def") == 0) return tkn_key_def;
	if(strcmp(s,"do") == 0) return tkn_key_do;
	if(strcmp(s,"else") == 0) return tkn_key_else;
	if(strcmp(s,"end") == 0) return tkn_key_end;
	if(strcmp(s,"if") == 0) return tkn_key_if;
	if(strcmp(s,"not") == 0) return tkn_key_not;
	if(strcmp(s,"nil") == 0) return tkn_key_nil;
	if(strcmp(s,"then") == 0) return tkn_key_then;
	if(strcmp(s,"while") == 0) return tkn_key_while;
	
	//nabizene funkce jako dalsi klicova slova
	if(strcmp(s,"inputs") == 0) return tkn_key_inputs;
	if(strcmp(s,"inputi") == 0) return tkn_key_inputi;
	if(strcmp(s,"inputf") == 0) return tkn_key_inputf;
	if(strcmp(s,"print") == 0) return tkn_key_print;
	if(strcmp(s,"length") == 0) return tkn_key_length;
	if(strcmp(s,"substr") == 0) return tkn_key_substr;
	if(strcmp(s,"ord") == 0) return tkn_key_ord;
	if(strcmp(s,"chr") == 0) return tkn_key_chr;

	return tkn_identifier;

}

int hexa_to_decimal(int letter)
{
	if (letter == '0')
		return 0;
	else if (letter == '1')
		return 1;
	else if (letter == '2')
		return 2;
	else if (letter == '3')
		return 3;
	else if (letter == '4')
		return 4;
	else if (letter == '5')
		return 5;
	else if (letter == '6')
		return 6;
	else if (letter == '7')
		return 7;
	else if (letter == '8')
		return 8;
	else if (letter == '9')
		return 9;
	else if (letter == 'A' || letter == 'a')
		return 10;
	else if (letter == 'B' || letter == 'b')
		return 11;
	else if (letter == 'C' || letter == 'c')
		return 12;
	else if (letter == 'D' || letter == 'd')
		return 13;
	else if (letter == 'E' || letter == 'e')
		return 14;
	else if (letter == 'F' || letter == 'f')
		return 15;
	else
		return 42;
		
}

int hexa_array_value()
{
	int tmp = 0;

	if((hexa_array[0] != 42) && (hexa_array[1] == 42))
	{
		tmp = hexa_array[0] * 1;
	}
	else if((hexa_array[0] != 42) && (hexa_array[1] != 42))
	{
		tmp = ((hexa_array[0] * 16) + (hexa_array[1] * 1));
	}
	//hexa_array[0] = 42;
	//hexa_array[1] = 42;


	return tmp;
}

token_t get_next_token()
{
	token_t tok;
	int c;
	int state = state_start;
	utils_string_t *us = utils_string_init();
	int idx;
	int escape_chr;
	

	while (state != state_end)
	{
		c = get_next_char();
		switch (state)
		{
			case state_start:
				switch(c)
				{
					case '_':
					case CASE_SMALL_ALPHA:
						tok.type = tkn_identifier;
						utils_string_add_char(us, c);
						state = state_identifier;
						break;
					case CASE_ALL_DIGIT:
						tok.type = tkn_integer;
						utils_string_add_char(us, c);			
						state = state_integer;
						break;
					case '-':
						tok.type = tkn_minus;
						state = state_end;
						break;
					case '+':
						tok.type = tkn_plus;
						state = state_end;
						break;
					case ',':
						tok.type = tkn_comma;
						state = state_end;
						break;
					case ')':
						tok.type = tkn_right_bracket;
						state = state_end;
						break;
					case '(':
						tok.type = tkn_left_bracket;
						state = state_end;
						break;
					case '*':
						tok.type = tkn_star;
						state = state_end;
						break;
					case '/':
						tok.type = tkn_slash;
						state = state_end;
						break;
					case '>':
						tok.type = tkn_more;
						state = state_more;
						break;
					case '<':
						tok.type = tkn_less;
						state = state_less;
						break;
					case '=':
						/*tok.type = tkn_equal;
						state = state_equal;
						break;*/
						if (char_number == 1)		//komentar muze zacit jen kdyz je '=' jako prvni znak na novem radku
						{
							state = state_block_comment_check_b;
						}
						else
						{
							tok.type = tkn_equal;
							state = state_equal;
						}
						break;
					case '!':
						tok.type = tkn_not_equal;
						state = state_not_equal;
						break;
					case '#':
						//tok.type = tkn_line_comment; //byl smazán
						state = state_line_comment;
						break;
					case '"':
						tok.type = tkn_string;
						state = state_string;
						break;
					case '\n':
						tok.type = tkn_new_line;
						state = state_end;
						break;
					case EOF:
						tok.type = tkn_end_of_file;
						state = state_end;
						break;
					case '\t':
					case ' ':
						continue;
					default:
						goto error;
				}
			break;
			




			case state_identifier:
				switch(c)
				{
					case '_':
					case CASE_ALL_DIGIT:
					case CASE_ALL_ALPHA:
						utils_string_add_char(us, c);
						break;
					case '?':
					case '!':
						utils_string_add_char(us, c);
						state = state_identifier_check;
						break;
					default:
						unget_next_char(c);
						state = state_end;
						break;
				}
				break;

			case state_identifier_check:
				switch(c)
				{
					case '_':
					case CASE_ALL_DIGIT:
					case CASE_ALL_ALPHA:
					case '?':				//vykricnik tu nebude protoze uz muze byt soucasti nerovna se !=
						goto error;
					default:
						unget_next_char(c);
						state = state_end;
						break;
				}
				break;

			case state_less:
				switch(c)
				{
					case '=':
						tok.type = tkn_less_or_equal;
						state = state_end;
						break;
					default:
						unget_next_char(c);
						state = state_end;
						break;
				}
				break;
			case state_more:
				switch(c)
				{
					case '=':
						tok.type = tkn_more_or_equal;
						state = state_end;
						break;
					default:
						unget_next_char(c);
						state = state_end;
						break;
				}
				break;
			case state_not_equal:
				switch(c)
				{
					case '=':
						tok.type = tkn_not_equal;		//zbytecne prenastavuju token type
						state = state_end;
						break;
					default:
						goto error;
				}
				break;
			case state_line_comment:
				switch(c)
				{
					case '\n':
						tok.type = tkn_new_line;
						state = state_end;
						break;
					case EOF:
						tok.type = tkn_end_of_file;
						state = state_end;
						break;
					default:
						continue;
				}
				break;

			



			case state_integer:
				switch(c)
				{
					case CASE_ALL_DIGIT:
						utils_string_add_char(us, c);
						break;
					case '.':
						tok.type = tkn_double;
						utils_string_add_char(us, c);
						state = state_double;
						break;
					case 'e':
					case 'E':
						tok.type = tkn_double;
						utils_string_add_char(us, tolower(c));	// ? to cecko tolower ??--dalsi casti uz nemuseji brat v potaz dve moznosti zapisu
						state = state_double_e;
						break;
					default:
						unget_next_char(c);
						state = state_end;
						break;
				}
				break;
			case state_double:
				switch(c)
				{
					case CASE_ALL_DIGIT:
						utils_string_add_char(us, c);
						state = state_double_value;
						break;
					default:
						goto error;
				}
				break;
			case state_double_value:
				switch(c)
				{
					case CASE_ALL_DIGIT:
						utils_string_add_char(us, c);
						break;
					case 'e':								//doplneni case e, E pro cisla napr. 25.5e+-5 
					case 'E':
						utils_string_add_char(us, tolower(c));
						state = state_double_e;
						break;
					default:
						unget_next_char(c);
						state = state_end;
						break;
				}
				break;
			case state_double_e:
				switch(c)
				{
					case CASE_ALL_DIGIT:
						utils_string_add_char(us, c);
						state = state_double_e_value;
						break;
					case '+':
					case '-':
						utils_string_add_char(us, c);
						state = state_double_e_mark;
						break;
					default:
						goto error;
				}
				break;
			case state_double_e_value:
				switch(c)
				{
					case CASE_ALL_DIGIT:
						utils_string_add_char(us, c);
						break;
					default:
						unget_next_char(c);
						state = state_end;
						break;
				}
				break;
			case state_double_e_mark:
				switch(c)
				{
					case CASE_ALL_DIGIT:
						utils_string_add_char(us, c);
						state = state_double_e_value;
						break;
					default:
						goto error;
				}
				break;
			



			case state_string:
				switch(c)
				{
					case '"':
						state = state_end;
						break;
					case '\\':
						state = state_string_esc;
						break;
					case '\n':
					case EOF:
						goto error;
					default:
						if (c <= 31)
							goto error;
						utils_string_add_char(us, c);
						break;
				}
				break;
			case state_string_esc:
				switch(c)
				{
					case '"':
						utils_string_add_char(us, '"');
						state = state_string;
						break;
					case 'n':
						utils_string_add_char(us, '\n');
						state = state_string;
						break;
					case 't':
						utils_string_add_char(us, '\t');
						state = state_string;
						break;
					case 's':
						utils_string_add_char(us, ' ');
						state = state_string;
						break;
					case '\\':
						utils_string_add_char(us, '\\');
						state = state_string;
						break;
					case 'x':
						//utils_string_add_char(us, c);
						hexa_array[0] = 42;	//vychozi hodnota -- odpoved na otazku zivota vesmiru a vubec
						hexa_array[1] = 42;	//vychozi hodnota -- odpoved na otazku zivota vesmiru a vubec
						state = state_string_esc_hex;
						break;
					default:
						goto error;
				}
				break;

			case state_string_esc_hex:
				switch(c)
				{
					case CASE_HEXA:
						//utils_string_add_char(us, c);
						hexa_array[0] = hexa_to_decimal(c);
						state = state_string_esc_hex_1;
						break;
					default:
						goto error;
				}
				break;
			case state_string_esc_hex_1:
				switch(c)
				{
					case CASE_HEXA:
						//utils_string_add_char(us, c);
						hexa_array[1] = hexa_to_decimal(c);
						utils_string_add_char(us, hexa_array_value());
						state = state_string;
						break;
					default:
						unget_next_char(c);
						utils_string_add_char(us, hexa_array_value());				
						state = state_string;
						break;
				}
				break;

			case state_equal:
				switch(c)
				{
					case '=':
						tok.type = tkn_equal_2;
						state = state_end;
						break;
					/*case 'b':
						state = state_block_comment_check_e;
						break;*/
					default:
						unget_next_char(c);
						state = state_end;
						break;


				}
				break;

			/*case state_block_comment_start_check:
				switch(c)
				{
					case '=':
						state = state_block_comment_check_b;
						break;
					default:
						unget_next_char(c);
						state = state_end;
						break;
				}
				break;*/


			case state_block_comment_check_b:
				switch(c)
				{
					case 'b':
						state = state_block_comment_check_e;
						break;
					default:
						goto error;
				}
				break;
			case state_block_comment_check_e:
				switch(c)
				{
					case 'e':
						state = state_block_comment_check_g;
						break;
					default:
						goto error;						
				}
				break;
			case state_block_comment_check_g:
				switch(c)
				{
					case 'g':
						state = state_block_comment_check_i;
						break;
					default:
						goto error;						
				}
				break;
			case state_block_comment_check_i:
				switch(c)
				{
					case 'i':
						state = state_block_comment_check_n;
						break;
					default:
						goto error;
				}
				break;
			case state_block_comment_check_n:
				switch(c)
				{
					case 'n': 						
						state = state_block_comment_start_check_space;
						break;
					default:
						goto error;
				}
				break;
			
			case state_block_comment_start_check_space:
				switch(c)
				{
					case ' ':			//alespon jedna mezera nebo tabulator
					case '\t':
						state = state_block_comment_value;
						break;
					case '\n':				
						state = state_block_comment_end_check;
						break;
					default:	//napr. EOF--tohle je error v komentari jelikoz neni validne ukonceny 
						goto error;

				}
				break;

			case state_block_comment_value:
				switch(c)
				{
					case EOF:				//muze nebo nemuze byt ukoncen znakem EOF ?? ---nemuze hodit error je spravne
						goto error;
					case '\n':
						state = state_block_comment_end_check;
						break;
					default:
						continue;
				}
				break;
			


			case state_block_comment_end_check:
				switch(c)
				{
					case '=':
						state = state_block_comment_end_check_e;
						break;
					default:
						unget_next_char(c);
						state = state_block_comment_value;
						break;

				}
				break;
			case state_block_comment_end_check_e:
				switch(c)
				{
					case 'e':
						state = state_block_comment_end_check_n;
						break;
					default:
						unget_next_char(c);
						state = state_block_comment_value;
						break;

				}
				break;
			case state_block_comment_end_check_n:
				switch(c)
				{
					case 'n':
						state = state_block_comment_end_check_d;
						break;
					default:
						unget_next_char(c);
						state = state_block_comment_value;
						break;

				}
				break;
			case state_block_comment_end_check_d:
				switch(c)
				{
					case 'd':
						state = state_block_comment_end_check_space;
						break;
					default:
						unget_next_char(c);
						state = state_block_comment_value;
						break;

				}
				break;
			case state_block_comment_end_check_space:
				switch(c)
				{
					case ' ':
					case '\t':
						state = state_block_comment_end;
						break;
					case '\n':			
						tok.type = tkn_new_line;
						state = state_end;
						break;
					case EOF:
						tok.type = tkn_end_of_file;
						state = state_end;
						break;
					default:				// misto erroru vratit do komentare
						state = state_block_comment_value;
						break;
				}
				break;
			case state_block_comment_end:
				switch(c)
				{
					case '\n':					
						tok.type = tkn_new_line;
						state = state_end;
						break;
					case EOF:
						tok.type = tkn_end_of_file;
						state = state_end;
						break;
					default:
						continue;
				}
				break;
		}
	}
    if (tok.type == tkn_identifier) {
        tok.type = check_if_keyword(us->string);
    }

    switch (tok.type) {
        case tkn_identifier:
        case tkn_string:
        case tkn_integer:
        case tkn_double:
            tok.value = utils_string_copy(us);
            break;
        default:
            tok.value = NULL;
            break;
    }
    utils_string_destroy(us);
    token = tok;
    return tok;

    error:
    	utils_string_destroy(us);
    	if (c != EOF)
	        fprintf(stderr, "Unexpected character '%c' (ASCII %d) found on line:position %d:%d\n", c, c, line_number, char_number);
	    else
	        fprintf(stderr, "Unexpected EOF found on line:position %d:%d\n", line_number, char_number);
	    exit(1);
}