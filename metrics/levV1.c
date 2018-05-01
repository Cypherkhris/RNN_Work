#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define HUGE (1000000000)

#define STACKSIZE (5)

#define LOCRATIO (1.5)

#define METRICSTHRESHOLD (1)

#define SIMILARITYTHRESHOLD (1)

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
	int numberOfNameOrVariables;
	int McCabeCyclomaticComplexity;

}myMethod_t;

typedef struct myPair_s{
  myMethod_t *first;
  myMethod_t *second;
  int dist;
  
}myPair_t;

void stack_print(myPair_t ** s){
  int i=0;
  for(i=0;i<=STACKSIZE;i++){
    printf("\t\ti %d dist %d\n", i, s[i]->dist);
  }
    
  }

void stack_init(myPair_t ** s){
  int i=0;
  for(i=0;i<=STACKSIZE;i++){
    myPair_t *p=(myPair_t*) malloc(sizeof(myPair_t));
    p->dist= HUGE;
    s[i]=p;
  }
    
  }

void insert(myPair_t *p, myPair_t ** s)
{
  int i;
  #ifdef DEBUG
  printf("Init stack for %d\n", p->dist);
  stack_print(s);
  #endif
  i=STACKSIZE;
  
  while (i>=0 && (p->dist < s[i]->dist)){
    #ifdef DEBUG
    printf("\ti %d dist %d stack %d test %d\n", i, p->dist,  s[i]->dist, p->dist < s[i]->dist );
    printf("\t\t\t copy back i %d into %d\n", i, i+1);
    #endif
    s[i+1] = s[i];
    i--;
  }

  
  i++;
  s[i]=p;
  #ifdef DEBUG
  printf("Final stack\n");
  stack_print(s);
  #endif
}



myMethod_t *parse_line(char *line){
	myMethod_t* m=(myMethod_t*)malloc(sizeof(myMethod_t));

	if(line != NULL){
	const char* tok;
	tok = strtok(line, "#"); //body
	if (!tok)
	return NULL;  

	m->body=strdup(tok);
	tok = strtok(NULL, "#");//name split
	m->nameSplit = strdup(tok);

	tok = strtok(NULL, "#");//name
	m->name= strdup(tok);

	tok = strtok(NULL, "#");//line_begin
	m->line_begin= atoi(tok);

	tok = strtok(NULL, "#");//line_end
	m->line_end= atoi(tok);

	tok = strtok(NULL, "#");//size
	m->size= atoi(tok);

	tok = strtok(NULL, "#");//numberOfParameters
	m->numberOfParameters= atoi(tok);

	tok = strtok(NULL, "#");//numberOfCalledMethodsOrFunctions
	m->numberOfCalledMethodsOrFunctions= atoi(tok);

	tok = strtok(NULL, "#");//numberOfDeclarations
	m->numberOfDeclarations= atoi(tok);

	tok = strtok(NULL, "#");//numberOfExpressionStatments
	m->numberOfExpressionStatments= atoi(tok);

	tok = strtok(NULL, "#");//numberOfExpressions
	m->numberOfExpressions= atoi(tok);

	tok = strtok(NULL, "#");//numberOfComments
	m->numberOfComments= atoi(tok);

	tok = strtok(NULL, "#");//numberOfNameOrVariables
	m->numberOfNameOrVariables= atoi(tok);

	tok = strtok(NULL, "#");//McCabeCyclomaticComplexity
	m->McCabeCyclomaticComplexity= atoi(tok);
}
else{
		printf("ERROR\n");
	}
	return m;  
}

int levenshtein(char *s1, char *s2, int thr) {
  unsigned int s1len, s2len, x, y, lastdiag, olddiag,lowest;
    s1len = strlen(s1);
    s2len = strlen(s2);
    
    unsigned int column[s1len+1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
	lowest=s1len+s1len;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 2));
            lastdiag = olddiag;
	    lowest=MIN(lowest,column[y]);
        }
	if (lowest > thr)
	 return HUGE;
    }
    return(column[s1len]);
}

int metrics(myMethod_t *s1, myMethod_t *s2){
   return(abs((s1->line_begin - s2->line_begin)+(s1->line_end - s2->line_end)+(s1->size - s2->size)+(s1->numberOfParameters - s2->numberOfParameters)+
  (s1->numberOfCalledMethodsOrFunctions - s2->numberOfCalledMethodsOrFunctions)+(s1->numberOfDeclarations - s2->numberOfDeclarations)+
  (s1->numberOfExpressionStatments - s2->numberOfExpressionStatments)+(s1->numberOfExpressions - s2->numberOfExpressions)+(s1->numberOfComments - s2->numberOfComments)+
  (s1->numberOfNameOrVariables - s2->numberOfNameOrVariables)+(s1->McCabeCyclomaticComplexity - s2->McCabeCyclomaticComplexity)));

}
int is_blank_line(const char *line) {
    const char accept[]=" \t\r\n"; /* white space characters (fgets stores \n) */
    return (strspn(line, accept) == strlen(line));
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
    myPair_t **stack = (myPair_t** ) malloc((STACKSIZE+1)*sizeof(myPair_t*));
    
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

     fclose(fp);
    
     fpres = fopen("result.csv","a");
     if (fpres == NULL)
       exit(1);
       
     unsigned int threshold= HUGE;
     int first=0, second=0, dist=0;
    for (first=0;first<cntr; first++){
        
          threshold= HUGE;
          myMethod_t* best = NULL;
          int skipped=0;
          myPair_t *m = NULL;
          
          stack_init(stack);
              for (second=first+1;second<cntr; second++){
              if(metrics(dict[first],dict[second])<=METRICSTHRESHOLD){
				  dist = levenshtein(dict[first]->body, dict[second]->body,threshold);
                    /*if(levenshtein(dict[first]->body, dict[second]->body,threshold)<SIMILARITYTHRESHOLD){
                      fprintf(fpres,"%s, %s, %i \n",dict[first]->name,dict[second]->name,metrics(dict[first],dict[second]));
                      }*/
				  if (threshold > dist){
					myPair_t *memo = (myPair_t*) malloc(sizeof(myPair_t));
					memo->first = dict[first];
					memo->second = dict[second];
					memo->dist=dist;
					insert(memo, stack);
					threshold = dist;
					best=dict[second];
					if (dist < SIMILARITYTHRESHOLD)
					fprintf(fpres,"%d %d %s %s ==> %d , %i\n",first, second,dict[first]->name, dict[second]->name, dist,metrics(dict[first],dict[second]));
				  }
              }
          }

       }
          
         /* for (second=first+1;second<cntr; second++){
               if (dict[first]->loc < LOCRATIO*dict[second]->loc || dict[first]->loc > LOCRATIO*dict[second]->loc){
                skipped++;
                continue;
              }
              //printf("%i\n",metri);
              if(metrics(dict[first],dict[second])<=METRICSTHRESHOLD){
                    if(levenshtein(dict[first]->body, dict[second]->body,threshold)<threshold)
                      printf("%s, %s, %i \n",dict[first]->name,dict[second]->name,metrics(dict[first],dict[second]));
              }
              // dist = levenshtein(dict[first]->body, dict[second]->body,threshold);
              if (threshold > dist){
                myPair_t *memo = (myPair_t*) malloc(sizeof(myPair_t));
                memo->first = dict[first];
                memo->second = dict[second];
                memo->dist=dist;
                insert(memo, stack);
                threshold = dist;
                best=dict[second];
                //printf("%d %d %s %s ==> %d\n",first, second,dict[first]->name, dict[second]->name, dist);
              }
          }

           printf("\t skipped %d ...\n",skipped);

          if (best != NULL){
        printf("\t\t Best dist %d \n",threshold);
        fprintf(fpres,"######, %d\n", threshold);
        //exit(0);
        for(i=0;i<STACKSIZE;i++){
        
        m=stack[i];
        if (m->first != NULL)
          fprintf(fpres,"%s, %s,  %d,  %d, %s, %s, %d, %d, %d\n",
          m->first->name,
        m->first->path,
        m->first->line_begin,
        m->first->line_end,
        m->second->name,
          m->second->path,
          m->second->line_begin,
          m->second->line_end,
          m->dist);
      }
      } else
      printf("\t\t No result all skipped %d ...\n",skipped);
  }*/
     
    
   if (line)
        free(line);
    
    fclose(fpres);
    exit(0);
}
