#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define MAX_STR_LEN 69
#define PROFILES 10000
#define MAX_LINE_LEN 1024


struct date{
  int y;
  int m;
  int d;
};

struct profile{
  int id;
  char name[MAX_STR_LEN+1];
  struct date day;
  char place[MAX_STR_LEN+1];
  char *data;
};

struct profile profile_data_store[PROFILES];
int profile_data_nitems = 0;

void subst(char *str, char c1, char c2)
{
  int i=0;

  for(;*(str+i);i++){
  
    if(*(str+i)==c1){
      *(str+i)=c2;
    }
    
  }
  

}


int split(char *str, char *ret[], char sep, int max)
{ 
  int t=0;
  ret[t]= str;
  t=t+1;
  while(*str && t<max){
    if(*str==sep){
      *str='\0';
      ret[t++]=str+1;
      
      
    }
    str++;
      }

  
  return t;

}





int get_line(FILE *fp,char *line)
{
  if (fgets(line, MAX_LINE_LEN + 1, fp) == NULL)
    return 0;
   
  subst(line, '\n', '\0');
  return 1;
}



struct date *ddate(struct date *a,char *str)
{  
  char *ret[3];

  if(split(str,ret,'-',3)!=3)
    return 0;

  a->y =atoi(ret[0]);
  a->m = atoi(ret[1]);
  a->d = atoi(ret[2]);
  return a;
}
struct profile *new_profile(struct profile *t, char *line)
{
  
  char *ret[5];
 
  if (split(line, ret, ',', 5) !=5){
    --profile_data_nitems;
    return '\0';
  }
  t->id=atoi(ret[0]);
  strncpy(t->name,ret[1],MAX_STR_LEN);
  t->name[MAX_STR_LEN]='\0';

  if (ddate(&t->day,ret[2])=='\0'){
    --profile_data_nitems;
    return '\0';
  } 
  strncpy(t->place,ret[3],MAX_STR_LEN);
  t->place[MAX_STR_LEN]='\0';

  t->data= (char *)malloc(sizeof(char) * (strlen(ret[4])+1));	       
  strcpy(t->data,ret[4]);
 
  profile_data_nitems++;
  return t;
}





void cmd_quit()
{
  exit(0);
}

void cmd_check()
{
  printf("%d profile(s)\n", profile_data_nitems);
}

char *date_to_string(char buf[], struct date *date)
{
  sprintf(buf,"%04d-%02d-%02d", date->y, date->m, date->d);
  return buf;
}



void print_profile(struct profile *t)
{
  char date[11];

  printf("Id    : %d\n" ,t->id);
  printf("Name  : %s\n", t->name);
  printf("Birth : %s\n",date_to_string(date, &t->day));
  printf("Addr  : %s\n",t->place);
  printf("Com.  : %s\n",t->data);
}
 


  void cmd_printf(int nitems)
  {
    
    int s;
   
    if(nitems>0||nitems==0){
      if(nitems>profile_data_nitems||nitems==0)nitems=profile_data_nitems; 
      for(s=0;s<nitems;s++){
      print_profile(&profile_data_store[s]);
      printf("\n");
      }
    }
     
    else if(nitems<0){
      if(profile_data_nitems + nitems > 0)s = profile_data_nitems + nitems;
      else s=0;
      for(;s<profile_data_nitems;s++){
      print_profile(&profile_data_store[s]);
      printf("\n");

      }
    }
  }

int cmd_read(char *filename)
{
  char line[MAX_LINE_LEN + 1];
  FILE *fp;
  
  
 
  if((fp= fopen(filename, "r")) == NULL){
    fprintf(stderr, "Could not open %s\n",fp);
      return -1;
  }
  
  while(get_line(fp, line)) {
    parse_line(line);
  }

  fclose(fp);
 

  return 0;
}

void fprint_profile_csv(FILE *fp,struct profile *t)
{
  char date[11];
  fprintf(fp,"%d,%s,%s,%s,%s\n",t->id,t->name,date_to_string(date, &t->day),t->place,t->data);


}
int cmd_write(char *filename)
{
  int i;
  FILE *fp; 
  
  if ((fp = fopen(filename, "w")) == NULL){
    fprintf(stderr, "Could not open %s\n",fp);
    return -1;
  }
 
  for(i = 0; i < profile_data_nitems; i++){
    fprint_profile_csv(fp,&profile_data_store[i]);
  }
  
  fclose(fp);
  return 0;
}

void cmd_find(char *word)
{
  int i,a;
  char day_str[11],s[9];
  struct profile *t;
  sprintf(s, "%d", t -> id);
  for(i = 0; i < profile_data_nitems; i++){
   t = &profile_data_store[i];
    date_to_string(day_str, &(t+i)->day);
    sprintf(s, "%d", t -> id);
    
    if(strcmp( s , word) == 0|| strcmp( t ->name, word) == 0 || strcmp(day_str, word) == 0 || strcmp(t-> place, word) == 0 || strcmp(t->data, word) == 0){
      print_profile(&profile_data_store[i]);
      printf("\n");
    }
  }
}


void swap(struct profile *p1, struct profile *p2)
{
  struct profile tmp;

  *(&tmp) = *p1;
  *p1 = *p2;
  *p2 = *(&tmp);
}





int compare_date(struct date *d1, struct date *d2)
{
  if (d1->y != d2->y) return d1->y - d2->y;
  if (d1->m != d2->m) return d1->m - d2->m;
  return d1->d - d2->d;
}



int compare_profile(struct profile *p1, struct profile *p2, int column)
{
  switch (column) {
  case 1:
    return p1->id - p2->id;
  case 2:
    return strcmp(p1->name, p2->name);
  case 3:
    return compare_date(&p1->day, &p2->day);
  case 4:
    return strcmp(p1->place, p2->place);
  case 5: 
    return strcmp(p1->data, p2->data);
 default:
    fprintf(stderr, "Please enter a number from 1 to 5");
  }
}


int cmd_sort(int low, int high,struct profile *p ,int n)
{
 

  
 int i = low; int j = high;
  if(i < j){
    while(i < j){
      while(compare_profile(&p[i],&p[high],n) < 0 && i<j) ++i;
      while(compare_profile(&p[j],&p[high],n) >= 0 && i<j)--j;
      swap(&p[i],&p[j]);
    }
    
    
    swap(&p[i],&p[high]);
 if(low != high){
   --j;++i;
    cmd_sort(low, j,p,n);
    cmd_sort(i, high,p,n);
  }
  }

}


void cmd_delete(struct profile *p, int n)
{
  int i ;
  if(n > profile_data_nitems || n==0 || n < -profile_data_nitems){
    fprintf(stderr, "The number of registrations is small");
 }
  else {
    if(n<0) n+=profile_data_nitems;
    else --n;
    for(i=0;n+i<profile_data_nitems;i++){
      swap(&profile_data_store[n+i],&profile_data_store[n+1+i]);
    }
  }
 

    profile_data_nitems--;
}
   


 
void exec_command(char cmd, char *param)
{
  switch(cmd){
  case 'Q': cmd_quit();    break;
  case 'C': cmd_check();   break;
  case 'P': cmd_printf(atoi(param));  break;
  case 'R': cmd_read(param);    break;
  case 'W': cmd_write(param);   break;
  case 'F': cmd_find(param);    break;
  case 'S': cmd_sort(0,profile_data_nitems -1,profile_data_store, atoi(param));     break;
  case 'D': cmd_delete(profile_data_store,atoi(param));  break;
  default:
    fprintf(stderr, "%%%c command is invoked with arg: '%s'\n", cmd,param);
  break;
}
}

void parse_line(char *line)
{
  if (line[0] == '%'){
    exec_command(line[1], &line[3]);
  } else {
    new_profile(&profile_data_store[profile_data_nitems], line);
  }
}





int main()
{
  char line[MAX_LINE_LEN+1];
  
  while (get_line(stdin,line)){
    parse_line(line);
}
  return 0;
}
