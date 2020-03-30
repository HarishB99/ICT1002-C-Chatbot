/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "chat1002.h"

typedef struct Dictionary {
    char *key;
    char *value;
    struct Dictionary *next;
} Dictionary;

typedef struct {
    Dictionary **smalltalks;
} Hashtable;

unsigned int hash(const char *key) {
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_length = strlen(key);

    for (; i < key_length; ++i) {
        value = value * 37 + key[i];
    }

    value = value % MAX_SMALLTALK_SIZE;

    return value;
}

Dictionary *hashtable_pair(const char *key, const char *value) {
    // allocate the entry
    Dictionary *smalltalks = malloc(sizeof(Dictionary) * 1);
    smalltalks->key = malloc(strlen(key) * 1);
    smalltalks->value = malloc(strlen(value) + 1);

    // copy the key & value in place
    strcpy(smalltalks->key, key);
    strcpy(smalltalks->value, value);

    // next starts out null but may be set later on
    smalltalks->next = NULL;

    return smalltalks;
}

Hashtable *hashtable_create(void) {
    // allocate table
    Hashtable *hashtable = malloc(sizeof(Hashtable) * 1);

    // allocate table entries
    hashtable->smalltalks = malloc(sizeof(Dictionary *) * MAX_SMALLTALK_SIZE);

    // set each to null (needed for proper operation)
    for (int i = 0; i < MAX_SMALLTALK_SIZE; ++i) {
        hashtable->smalltalks[i] = NULL;
    }

    return hashtable;
}

void hashtable_set(Hashtable *hashtable, const char *key, const char *value) {
    unsigned int slot = hash(key);

    // try to look up an entry set
    Dictionary *smalltalk = hashtable->smalltalks[slot];

    // no entry = empty slot, insert immediately
    if (smalltalk == NULL) {
        hashtable->smalltalks[slot] = hashtable_pair(key, value);
        return;
    }

    Dictionary *prev;

    /* Walk through each smalltalk until either the   * end is reached or a matching key is found
     */
    while (smalltalk != NULL) {
        // check key
        if (strcmp(smalltalk->key, key) == 0) {
            // match found, replace value
            free(smalltalk->value);
            smalltalk->value = malloc(strlen(value) + 1);
            strcpy(smalltalk->value, value);
            return;
        }

        // walk to next
        prev = smalltalk;
        smalltalk = prev->next;
    }

    // end of chain reached without a match, add new
    prev->next = hashtable_pair(key, value);
}

int print_smalltalk(Hashtable *hashtable, char *full_input, char *response, int n) {
    for (int i = 0; i < MAX_SMALLTALK_SIZE; ++i) {
        Dictionary *smalltalk = hashtable->smalltalks[i];

        if (smalltalk == NULL) {
            continue;
        }
        if (strcmp(smalltalk->key, full_input) == 0) {
            snprintf(response, n, "%s", smalltalk->value);
            if (strcmp(full_input, "bye") == 0) {
                return 1;
            }
        }
    }
    return 0;
}


/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {

    return "Pepehands";

}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {

    return "Anon";

}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n) {
    /* check for empty input */
    if (inc < 1) {
        snprintf(response, n, "");
        return 0;
    }

    char *full_input;
    full_input = malloc(sizeof(char) * MAX_RESPONSE);
    memset(full_input, 0, sizeof(char) * MAX_RESPONSE);
    for (int i = 0; i < inc; ++i) {
        strcat(full_input, inv[i]);
        if (i != inc - 1) {
            strcat(full_input, " ");
        }
    }

    /* look for an intent and invoke the corresponding do_* function */
    if (chatbot_is_exit(inv[0]))
        return chatbot_do_exit(inc, inv, response, n);
    else if (chatbot_is_smalltalk(full_input))
        return chatbot_do_smalltalk(inc, full_input, response, n);
    else if (chatbot_is_load(inv[0]))
        return chatbot_do_load(inc, inv, response, n);
    else if (chatbot_is_question(inv[0]))
        return chatbot_do_question(inc, inv, response, n);
    else if (chatbot_is_reset(inv[0]))
        return chatbot_do_reset(inc, inv, response, n);
    else if (chatbot_is_save(inv[0]))
        return chatbot_do_save(inc, inv, response, n);
    else {
        snprintf(response, n, "I don't understand \"%s\".", full_input);
        free(full_input);
        return 0;
    }
}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent) {

    return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;

}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {

    snprintf(response, n, "Goodbye!");

    return 1;

}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent) {

    return compare_token(intent, "load") == 0;  

}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n) {
    FILE *f;   
    int count = 0; 
    if(inv[1] == NULL){
      strcpy(response, "No input detected");
    }else{
      f = fopen(inv[1],"r");
      if(f){
        count = knowledge_read(f);
        snprintf(response, n, "Read %d responses from %s", count, inv[1]);
        fclose(f);
      }else{
        strcpy(response, "Error loading file");
      }
    }

    return 0;

}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent) {
    return compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent,"who") == 0 ;

    return 0;

}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n) {

  int i;
  char userintent[MAX_INTENT];
  strncpy(userintent, inv[0], sizeof(userintent)/sizeof(userintent[0]));
  snprintf(response,n,"%s",userintent);
  char userentity[MAX_ENTITY];
  char chatbotresponse[MAX_RESPONSE];
  char usernoun[MAX_INPUT];
  int get_reply_code;
  int put_reply_code;


  if (compare_token(inv[1], "is") == 0 || compare_token(inv[1], "are") == 0){
    
    strncpy(usernoun, inv[1], sizeof(usernoun)/sizeof(usernoun[0]));
    for (i=1;i < inc ; i++){
      inv[i] = inv[i+1];
      // snprintf(response, n, "%s", usernoun);
  }
    inv--;
    // for (i=0;i<inc;i++){
    //   if (inv[i][inc-1] = "/n"){
    //     inv[i][inc--] = "/0";

    //   }
    // }
     inv[-1] = "\0";
    for (i=2;i <inc;i++){
      strcat(strcat(userentity, " "), inv[i]);
      

    }
    
   
    
  }
else{
    // snprintf(response, n, "%s", usernoun);
}
  // snprintf(response, n, "%s", userentity);

  get_reply_code = knowledge_get(userintent, userentity, chatbotresponse, n);
  printf("%d",get_reply_code);
  return 0;
  }


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */

// 8 errors monkaS
 
int chatbot_is_reset(const char *intent) {
      
	if (compare_token(intent,"reset")==0){
	  printf("chatbot resetting is starting... \n");  
	    return 1;
	}else{
      return 0;  
  }



 }


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {
  /*
    strcpy(response,"chatbot_do_reset is started...");
    if (smalltalks!=NULL) {
        Dictionary *pointer = smalltalks;
        while (smalltalks != NULL){
          smalltalks -> next;
          free(pointer);
          *pointer = smalltalks;
        }
        

        

    }  
    */
    return 0;

}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "save"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent) {
    return compare_token(intent, "save") == 0;
}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n) {
    char *filename;
      if (inc < 2) {
          snprintf(response, n, "Please provide a name for the file to save my knowledge to.");
      return 0;
      } else if (inc == 2) {
      strcpy(filename, inv[1]);
      } else {
          int second_word_is_as = compare_token(inv[1], "as") == 0;
          int second_word_is_to = compare_token(inv[1], "to") == 0;
          
      if (second_word_is_as || second_word_is_to) {
        strcpy(filename, inv[2]);
        if (inc > 3) {
          for (int k = 3; k < inc; k++) {
            strcat(filename, " ");
            strcat(filename, inv[k]);
          }
        }
          } else {
              snprintf(response, n, "I do not understand what do you mean by %s %s.", inv[0], inv[1]);
        return 0;
          }
      }

    FILE *file = fopen(filename, "w");

    if (filename == NULL) {
      snprintf(response, n, "Unable to open '%s' for writing.", filename);
      return 0;
    }

    knowledge_write(file);
    fclose(file);

    return 0;
}


/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the one of the smalltalks
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char *intent) {
    /* Create an array containing the trigger words
     * MAX_TRIGGER_SIZE is used in the event the trigger ever increases,
     * we just need to change the value of MAX_TRIGGER_SIZE
     * Remember to null terminate your trigger array!*/

    char *trigger[MAX_TRIGGER_SIZE] = {"hi", "hello", "regressing", "shrek", "bee", "error", "bye", NULL};

    /* Lowercase user input (intent)
     * Use tolower() to convert any uppercase to lowercase
     * tolower() only works with one character, thus, to lower a string,
     * we need to use a for loop.
     * Before we lower the user input, we need to first create a copy using
     * strcpy(). This is because we can't change a const value. If you tried
     * to, you will get the error "Read-only variable is not assignable" */
    char input[MAX_INTENT];
    strcpy(input, intent);

    for (int i = 0; i < strlen(input); i++) {
        input[i] = tolower(input[i]);
    }
    /* Check if user input matches any element in the trigger array with
     * strcmp(array_element, input). If strcmp return 0, return 1.
     * Else, return 0. We will be using a for loop which loops through the
     * array, checking whether the input matches an element in the trigger
     * array. If it does, return 1. Else, check if the element is the last
     * element in the trigger array. Else, carry on looping. */
    int size = sizeof(trigger) / sizeof(trigger[0]);
    for (int j = 0; trigger[j] != '\0'; ++j) {
        if (strcmp(trigger[j], input) == 0) {
            return 1;
        } else if (strcmp(trigger[j], input) != 0 && j == (size - 1)) {
            return 0;
        }
    }
    return 0;
}


/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char *full_input, char *response, int n) {
    Hashtable *hashtable = hashtable_create();

    hashtable_set(hashtable, "hello", "darkness my old friend");
    hashtable_set(hashtable, "regressing", "I often wonder if we are "
                                           "growing as a people... or in fact, regressing.");
    hashtable_set(hashtable, "shrek", "Somebody once told me the world is "
                                      "gonna roll me");
    hashtable_set(hashtable, "bee",
                  "According to all known laws of aviation, there is no "
                  "way that a bee should be able to fly. Its wings are "
                  "too small to get its fat little body off the ground. "
                  "The bee, of course, flies anyways. Because bees don't "
                  "care what humans think is impossible.");
    hashtable_set(hashtable, "error", "I AM ERROR");
    hashtable_set(hashtable, "bye", "Goodbye!");

    int k = print_smalltalk(hashtable, full_input, response, n);
    if (k == 1) {
        free(full_input);
        return 1;
    }
    free(full_input);
    return 0;
}
