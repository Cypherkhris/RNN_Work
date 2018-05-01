#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define HUGE (1000000000)

#define STACKSIZE (5)

#define LOCRATIO (1.5)

#undef DEBUG

/*typedef struct myMethod_s{
int loc;
int line_begin;
int line_end;
char *body;
char  *class;
char *name;
char *path;
}myMethod_t;*/
typedef struct myMethod_s{
	char *body;
	char  *nameSplit;
	char  *name;
	int line_begin;
	int line_end;
	int size;
	int numberOfParameters;
	int numberOfCalledMethodsOrFunctions;
	int numberOfDeclarations;
	int numberOfExpressionStatments;
	int numberOfExpressions;
	int numberOfComments;
	int numberOfNmaeOrVariables;
	int McCabeCyclomaticComplexity;

}myMethod_t;

myMethod_t *parse_line(char *line){
	const char* tok;
	tok = strtok(line, "#"); //body
	if (!tok)
	return NULL;  

	myMethod_t* m=(myMethod_t*)malloc(sizeof(myMethod_t));
	m->body=strdup(tok);
	tok = strtok(NULL, "#");//name split
	m->nameSplit = strdup(tok);

	tok = strtok(NULL, "#");//name
	m->name= strdup(tok);

	tok = strtok(NULL, "#");//line_begin
	m->line_begin= strdup(tok);

	tok = strtok(NULL, "#");//line_end
	m->line_end= strdup(tok);

	tok = strtok(NULL, "#");//size
	m->size= strdup(tok);

	tok = strtok(NULL, "#");//numberOfParameters
	m->numberOfParameters= strdup(tok);

	tok = strtok(NULL, "#");//numberOfCalledMethodsOrFunctions
	m->numberOfCalledMethodsOrFunctions= strdup(tok);

	tok = strtok(NULL, "#");//numberOfDeclarations
	m->numberOfDeclarations= strdup(tok);

	tok = strtok(NULL, "#");//numberOfExpressionStatments
	m->numberOfExpressionStatments= strdup(tok);

	tok = strtok(NULL, "#");//numberOfExpressions
	m->numberOfExpressions= strdup(tok);

	tok = strtok(NULL, "#");//numberOfComments
	m->numberOfComments= strdup(tok);

	tok = strtok(NULL, "#");//numberOfNmaeOrVariables
	m->numberOfNmaeOrVariables= strdup(tok);

	tok = strtok(NULL, "#");//McCabeCyclomaticComplexity
	m->McCabeCyclomaticComplexity= strdup(tok);
	return m;  
}
int main(int argc, char** argv)
{
	FILE * fp;
	FILE * fpres=NULL;//results file pointer
	char * line = NULL;
	size_t len = 0;
	int cntr=0,i;
	ssize_t read;
	char **lines = NULL;
	// myPair_t **stack = (myPair_t** ) malloc((STACKSIZE+1)*sizeof(myPair_t*));

	fp = fopen(argv[1], "r"); // hard coded
	if (fp == NULL)
	exit(1);


	while ((read = getline(&line, &len, fp)) != -1) {
	cntr++;
	}

	//rewind(fp);
	fclose(fp);
	lines = (char **) malloc(cntr*sizeof(char*));
	myMethod_t** dict = (myMethod_t**)malloc(cntr*sizeof(myMethod_t*));

	cntr=0;
	fp = fopen(argv[1], "r"); // hard codes
	if (fp == NULL)
	exit(1);

	while ((read = getline(&line, &len, fp)) != -1) {

		line[strlen(line) - 1] = '\0';

		myMethod_t* m = parse_line(line);
		if (m != NULL){
			lines[cntr]=strdup(m->body);
			dict[cntr]=m;

			cntr++;
		}
	}
}
