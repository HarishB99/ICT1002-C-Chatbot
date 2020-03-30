/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * knowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"

/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */
int knowledge_get(const char *intent, const char *entity, char *response, int n) {

	if (compare_token(intent, "who")){
    
  }
	return KB_NOTFOUND;

}


/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response) {
    NODE *next, *prev;

    if (compare_token(intent, "what") == 0) {
        NODE temp = { "", "", "", NULL };
        strcpy(temp.intent, intent);
        strcpy(temp.entity, entity);
        strcpy(temp.response, response);

        if (strlen(headWhat->intent) == 0) {
            *headWhat = temp;
        } else {
            prev = NULL;
            next = headWhat;

            while (next) {
                prev = next;
                next = next->next;
            }

            *(prev->next) = temp;
        }
    } else if (compare_token(intent, "where") == 0) {
        NODE temp = { "", "", "", NULL };
        strcpy(temp.intent, intent);
        strcpy(temp.entity, entity);
        strcpy(temp.response, response);

        if (strlen(headWhere->intent) == 0) {
            *headWhere = temp;
        } else {
            prev = NULL;
            next = headWhere;

            while (next) {
                prev = next;
                next = next->next;
            }

            *(prev->next) = temp;
        }
    } else if (compare_token(intent, "who") == 0) {
        NODE temp = { "", "", "", NULL };
        strcpy(temp.intent, intent);
        strcpy(temp.entity, entity);
        strcpy(temp.response, response);

        if (strlen(headWho->intent) == 0) {
            *headWho = temp;
        } else {
            prev = NULL;
            next = headWho;

            while (next) {
                prev = next;
                next = next->next;
            }

            *(prev->next) = temp;
        }
    } else {
        return KB_INVALID;
    }
    return KB_OK;
}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f) {

	int count = 0;

	return count;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {

	/* to be implemented */

}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {
    NODE *ptr_what = headWhat,
        *ptr_where = headWhere,
        *ptr_who = headWho;

    fprintf(f, "[what]\n");
    while (ptr_what != NULL) {
        fprintf(f, "%s=%s\n", ptr_what->entity, ptr_what->response);
        ptr_what = ptr_what->next;
    }
    fprintf(f, "\n");

    fprintf(f, "[where]\n");
    while (ptr_where != NULL) {
        fprintf(f, "%s=%s\n", ptr_where->entity, ptr_where->response);
        ptr_where = ptr_where->next;
    }
    fprintf(f, "\n");

    fprintf(f, "[who]\n");
    while (ptr_who != NULL) {
        fprintf(f, "%s=%s\n", ptr_who->entity, ptr_who->response);
        ptr_who = ptr_who->next;
    }
    fprintf(f, "\n");
}
