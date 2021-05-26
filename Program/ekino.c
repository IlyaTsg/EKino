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
    char *genre;
    int release_date[3];
    float rating[2];
    int kinopoisk_star;
    struct films *prev;
    struct films *next;
} film;

film *FileEnter();
/* Forming list from file */

film *AppendKeyboard(film *first_el);
/* Append new el. from keyboard */

void SaveToFile(film *);
/* Save list to file */

film *MakeList();
/* Make new element of list */

void InsertAfter(film*, film*);
/* Insert new element after current */

void DemoOutput(film*);

int EnterString(char*);
/* Enter string from keyboard*/

int main()
{
    film *movies;
    movies = FileEnter();
    movies = AppendKeyboard(movies);
    SaveToFile(movies);
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
        if((new_list=MakeList()) && !feof(p_file)){ //if file is empty --> list = NULL
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
                            f_str = strtok(NULL, &sep);
                            if(new_list->genre=(char*)malloc(strlen(f_str))){
                                strcpy(new_list->genre, f_str);
                                f_str = strtok(NULL, &sep);
                                for(i=0; i<3; i++){ 
                                    new_list->release_date[i] = atoi(f_str);
                                    f_str = strtok(NULL, &sep);
                                }
                                for(i=0; i<2; i++){
                                    new_list->rating[i] = atof(f_str);
                                    f_str = strtok(NULL, &sep);
                                }
                                new_list->kinopoisk_star = atoi(f_str);
                                f_str = strtok(NULL, &sep);
                            }else puts("Memory allocation! Func: ListFromFile");
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

film *AppendKeyboard(film *list)
{
    char *tmp_str;
    char sep = '.';
    int slen, i;

    if(tmp_str=malloc(MaxStrLen)){
        if(list){
            while(list->next) list = list->next;
            list->next = MakeList();
            InsertAfter(list, list->next);
            list = list->next;
        }

        if(list){
            printf("Enter title of movie: ");
            slen = EnterString(tmp_str);
            if(list->title=malloc(sizeof(slen+1))){
                strcpy(list->title, tmp_str);
                printf("Enter director name of movie: ");
                slen = EnterString(tmp_str);
                if(list->director_name=malloc(sizeof(slen+1))){
                    strcpy(list->director_name, tmp_str);

                    printf("Enter genre of movie: ");
                    slen = EnterString(tmp_str);
                    if(list->genre=malloc(sizeof(slen+1))){
                        strcpy(list->genre, tmp_str);

                        printf("Enter release date of movie(by point): ");
                        EnterString(tmp_str);
                        tmp_str = strtok(tmp_str, &sep);
                        for(i=0; i<3; i++){
                            list->release_date[i] = atoi(tmp_str);
                            tmp_str = strtok(NULL, &sep);
                        }

                        if(tmp_str=malloc(MaxStrLen)){
                            printf("Enter IMDb rating: ");
                            EnterString(tmp_str);
                            list->rating[0] = atof(tmp_str);

                            printf("Enter Kinopoisk rating: ");
                            EnterString(tmp_str);
                            list->rating[1] = atof(tmp_str);
                            
                            printf("Enter kinopoisk star: ");
                            EnterString(tmp_str);
                            list->kinopoisk_star = atoi(tmp_str);
                        }else puts("Memory allocation! Func: ListFromKeyboard");
                    }else puts("Memory allocation! Func: ListFromKeyboard");
                }else puts("Memory allocation! Func: ListFromKeyboard");
            }else puts("Memory allocation! Func: ListFromKeyboard");
        }else puts("Memory allocation! Func: ListFromKeyboard");
    }else puts("Memory allocation! Func: ListFromKeyboard");

    while(list->prev) list = list->prev;

    return list;
}

void SaveToFile(film *list)
{
    FILE *p_file;
    int i;

    if(p_file=fopen("films.csv", "w")){
        while(list){
            fprintf(p_file, "\n%s;%s;%s;", list->title, list->director_name, list->genre);
            for(i=0; i<3; i++) fprintf(p_file, "%d;", list->release_date[i]);
            for(i=0; i<2; i++) fprintf(p_file, "%.2f;", list->rating[i]);
            fprintf(p_file, "%d", list->kinopoisk_star);
            list = list->next;
        }
    }else puts("File isn't open! Func: ListFromFile");
}

film *MakeList()
{
    film *first_el = NULL;
    if(first_el = malloc(sizeof(film))){
        first_el->director_name = NULL;
        first_el->title = NULL;
        first_el->genre = NULL;
        first_el->prev = NULL;
        first_el->next = NULL;
    }else puts("Memory allocation! Func: MakeList");
    return first_el;
}

void InsertAfter(film *cur_el, film *new_el)
{
    if(new_el){
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
}

void DemoOutput(film *list)
{
    while(list){
        printf("Title: %s\n", list->title);
        printf("Genre: %s\n", list->genre);
        printf("Adress of prev: %p\n", list->prev);
        printf("Adress of this: %p\n", list);
        printf("Adress of next: %p\n", list->next);
        list = list->next;
    }
}

int EnterString(char *str)
{
    int len;
    fgets(str, MaxStrLen, stdin);
    len = strlen(str);
    str[len-1] = '\0';
    return len;
}