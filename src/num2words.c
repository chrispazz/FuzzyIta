#include "num2words.h"
#include "string.h"
#include "pebble.h"

static const char* const ONES[] = {
  "zero",
  "una",
  "due",
  "tre",
  "quattro",
  "cinque",
  "sei",
  "sette",
  "otto",
  "nove"
};

static const char* const TEENS[] ={
  "",
  "undici",
  "dodici",
  "tredici",
  "quattordici",
  "quindici",
  "sedici",
  "diciassette",
  "diciotto",
  "diciannove"
};

static const char* const TENS[] = {
  "",
  "dieci",
  "venti",
  "trenta",
  "quaranta",
  "cinquanta",
  "sessanta",
  "settanta",
  "ottanta",
  "novanta"
};

static const char* STR_OH_CLOCK = "";
static const char* STR_NOON = "mezzo giorno";
static const char* STR_MIDNIGHT = "mezza notte";
static const char* STR_QUARTER = "un quarto";
static const char* STR_TO = "meno";
static const char* STR_PAST = "e";
static const char* STR_HALF = "mezza";
static const char* STR_AFTER = "e";

static size_t append_number(char* words, int num) {
  int tens_val = num / 10 % 10;
  int ones_val = num % 10;

  size_t len = 0;

  if (tens_val > 0) {
    if (tens_val == 1 && num != 10) {
      strcat(words, TEENS[ones_val]);
      return strlen(TEENS[ones_val]);
    }
    strcat(words, TENS[tens_val]);
    len += strlen(TENS[tens_val]);
    if (ones_val > 0) {
      strcat(words, " ");
      len += 1;
    }
  }

  if (ones_val > 0 || num == 0) {
    strcat(words, ONES[ones_val]);
    len += strlen(ONES[ones_val]);
  }
  return len;
}

static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}

void fuzzy_time_to_words(int hours, int minutes, char* words, char* words2, size_t length) {
  int fuzzy_hours = hours;
  int fuzzy_minutes = ((minutes + 2) / 5) * 5;
	  bool invert = 0;

  // Handle hour & minute roll-over.
  if (fuzzy_minutes > 55) {
    fuzzy_minutes = 0;
    fuzzy_hours += 1;
    if (fuzzy_hours > 23) {
      fuzzy_hours = 0;
    }
  }

  size_t remaining = length;
  size_t remaining2 = length;
  memset(words, 0, length);
  memset(words2, 0, length);

  if (fuzzy_minutes > 37) {
    fuzzy_hours = (fuzzy_hours + 1) % 24;	
	 invert = 1;
  }
	
	
  if (invert == 0)
  {	
			if (fuzzy_hours == 0) {
				remaining -= append_string(words, remaining, STR_MIDNIGHT);
			  } else if (fuzzy_hours == 12) {
				remaining -= append_string(words, remaining, STR_NOON);
			  } else {
				remaining -= append_string(words, remaining, "le\n");
				remaining -= append_number(words, fuzzy_hours % 12);  
			  }
				
			  if (fuzzy_minutes != 0 && (fuzzy_minutes >= 10 || fuzzy_minutes == 5 || fuzzy_hours == 0 || fuzzy_hours == 12)) {
					if (fuzzy_minutes == 15) {
				  remaining2 -= append_string(words2, remaining2, STR_AFTER); //e
				  remaining2 -= append_string(words2, remaining2, " ");
				  remaining2 -= append_string(words2, remaining2, STR_QUARTER); //un quarto
				} else if (fuzzy_minutes == 45) {
				  remaining2 -= append_string(words2, remaining2, STR_TO); //meno
				  remaining2 -= append_string(words2, remaining2, " ");
				  remaining2 -= append_string(words2, remaining2, STR_QUARTER); //un quarto
				  
				} else if (fuzzy_minutes == 30) {
				  remaining2 -= append_string(words2, remaining2, STR_PAST); //e
				  remaining2 -= append_string(words2, remaining2, " ");
				  remaining2 -= append_string(words2, remaining2, STR_HALF); //mezza
				  
				} else if (fuzzy_minutes < 37) {
				  remaining2 -= append_string(words2, remaining2, STR_AFTER); //e
				  remaining2 -= append_string(words2, remaining2, " ");
				  remaining2 -= append_number(words2, fuzzy_minutes);
				} else {
				  remaining2 -= append_string(words2, remaining2, STR_TO); //meno
				  remaining2 -= append_string(words2, remaining2, " ");
				  remaining2 -= append_number(words2, 60 - fuzzy_minutes);
				  remaining2 -= append_string(words2, remaining2, " ");
				}
			  }
			
			  //if (fuzzy_minutes == 0 && !(fuzzy_hours == 0 || fuzzy_hours == 12)) {
			//	remaining2 -= append_string(words2, remaining2, " ");
			//	remaining2 -= append_string(words2, remaining2, STR_OH_CLOCK);
			 // }
	}  
	else
	{
	// Dopo i minuti 37 inverto la posizione, per dire "venti alle otto e simili"			
	
			if (fuzzy_minutes == 45) {
			  remaining -= append_string(words, remaining, "un quarto");	
			}	
			else {
			  remaining -= append_string(words, remaining, "\n");
			  remaining -= append_number(words, 60 - fuzzy_minutes);
			  //remaining -= append_string(words, remaining, " a");
			}

		  if (fuzzy_hours == 0) {
			remaining2 -= append_string(words2, remaining2, "a ");			  
			remaining2 -= append_string(words2, remaining2, STR_MIDNIGHT); //mezzanotte
		  } else if (fuzzy_hours == 12) {
			remaining2 -= append_string(words2, remaining2, "a ");				  
			remaining2 -= append_string(words2, remaining2, STR_NOON); //mezzogiorno
		  } else {
			remaining2 -= append_string(words2, remaining2, "alle\n");
			remaining2 -= append_number(words2, fuzzy_hours % 12);  
		  }
		
	}
  }
