#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OS linux
#ifdef OS 
#define CLS system("clear")
#else
#define CLS system("cls")
#endif

#define MaxStrLen 128

typedef struct films{
    char *title;
    char *director_name;
    int release_date[3];
    float rating[2];
    int kinopoisk_star;
    struct films *prev;
    struct films *next;
} film;

film *FileEnter();
/* Forming list from file */

film *MakeList();
/* Make new element of list */

void InsertAfter(film*, film*);
/* Insert new element after current */

void DemoOutput(film*);

int main()
{
    film *movies;
    movies = FileEnter();
    DemoOutput(movies);
    return 0;
}

film *FileEnter()
{
    FILE *p_file;
    film *new_list = NULL;
    char *f_str = NULL;
    char sep = ';';
    int slen, i;
    
    if(p_file=fopen("films.csv", "r")){
        if(new_list=MakeList()){
            while(!feof(p_file)){
                if(f_str=(char*)malloc(MaxStrLen)){
                    fgets(f_str, MaxStrLen, p_file);
                    slen = strlen(f_str);
                    if(f_str[slen-1]=='\n') f_str[slen-1] = '\0';
                    else f_str[slen] = '\0';

                    f_str = strtok(f_str, &sep);
                    if(new_list->title=(char*)malloc(strlen(f_str))){
                        strcpy(new_list->title, f_str);
                        f_str = strtok(NULL, &sep);
                        if(new_list->director_name=(char*)malloc(strlen(f_str))){ 
                            strcpy(new_list->director_name, f_str);
                            for(i=0; i<3; i++){ 
                                f_str = strtok(NULL, &sep);
                                new_list->release_date[i] = atoi(f_str);
                            }
                            for(i=0; i<2; i++){ 
                                f_str = strtok(NULL, &sep);
                                new_list->rating[i] = atof(f_str);
                            }
                            f_str = strtok(NULL, &sep);
                            new_list->kinopoisk_star = atoi(f_str);
                        }else puts("Memory allocation! Func: ListFromFile");
                    }else puts("Memory allocation! Func: ListFromFile");
                }else puts("Memory allocation! Func: ListFromFile");
                if((!feof(p_file))){
                    if(new_list->next=MakeList()){
                        InsertAfter(new_list, new_list->next);
                        new_list = new_list->next;
                    }else puts("Memory allocation! Func: ListFromFile");
                }
            }
            while(new_list->prev) new_list = new_list->prev;
        }else puts("Memory allocation! Func: ListFromFile");
    }else puts("File isn't open! Func: ListFromFile");
    return new_list;
}

film *MakeList()
{
    film *first_el = NULL;
    if(first_el = malloc(sizeof(film))){
        first_el->director_name = NULL;
        first_el->title = NULL;
        first_el->prev = NULL;
        first_el->next = NULL;
    }else puts("Memory allocation! Func: MakeList");
    return first_el;
}

void InsertAfter(film *cur_el, film *new_el)
{
    if(!cur_el->next || cur_el->next==new_el){
        cur_el->next = new_el;
        new_el->next = NULL;
    } 
    else{
        new_el->next = cur_el->next;
        new_el->next->prev = new_el; 
        cur_el->next = new_el;
    }
    new_el->prev = cur_el;
}

void DemoOutput(film *list)
{
    while(list){
        printf("Title: %s\n", list->title);
        printf("Adress of prev: %p\n", list->prev);
        printf("Adress of this: %p\n", list);
        printf("Adress of next: %p\n", list->next);
        list = list->next;
    }
}