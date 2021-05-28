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

film *DelElByTit(film*, char*);
/* Delete element by title */

film *DelEl(film*, film*);
/* Delete element of list */

int EditFilm(film*, int, char*, char*);
/* Edit film */

void DemoOutput(film*);

int EnterString(char*);
/* Enter string from keyboard*/

int main()
{
    enum Bool {False, True};
    film *movies;
    char del_str[MaxStrLen], edit_str[MaxStrLen];
    int mode, mode_1, mode_2, mode_3, e_mode;
    int edit_flag;

    movies = FileEnter();
    do{
        CLS;
        puts("-----Menu-----");
        puts("1 - Edit catalog");
        puts("2 - Find film");
        puts("3 - Sort catalog");
        puts("4 - Output all films");
        puts("0 - Exit");
        printf("Enter your choice: ");
        scanf("%d", &mode);
        getchar();
        switch(mode){
            case 1:
                do{
                    CLS;
                    puts("-----Menu-----");
                    puts("1 - Add film");
                    puts("2 - Delete film");
                    puts("3 - Edit film");
                    puts("4 - Clear catalog");
                    puts("5 - Save changes to file");
                    puts("0 - Exit");
                    printf("Enter your choice: ");
                    scanf("%d", &mode_1);
                    getchar();
                    switch (mode_1){
                    case 1:
                        movies = AppendKeyboard(movies);
                        break;
                    case 2:
                        CLS;
                        if(movies){
                            printf("Enter title of film: ");
                            EnterString(del_str);
                            movies = DelElByTit(movies, del_str);
                            puts("Delete is OK");
                        }else puts("Catalog is empty!");
                        getchar();
                        break;
                    case 3:
                        if(movies){
                            edit_flag = False;
                            do{
                                CLS;
                                puts("-----Menu-----");
                                puts("1 - Edit title");
                                puts("2 - Edit director name");
                                puts("3 - Edit genre");
                                puts("4 - Edit date of release");
                                puts("5 - Edit IMdb rating");
                                puts("6 - Edit Kinopoisk rating");
                                puts("7 - Edit kinopisk stars");
                                puts("0 - Exit");
                                printf("Enter title of editing film: ");
                                EnterString(del_str);
                                do{
                                    printf("Enter your choice: ");
                                    scanf("%d", &e_mode);
                                    getchar();
                                }while(e_mode<0 || e_mode>7);
                                if(e_mode){
                                    printf("Enter new parameter(that you've chosen) of film: ");
                                    EnterString(edit_str);
                                    if(!EditFilm(movies, e_mode, edit_str, del_str)) puts("This film was not founded!");
                                    else puts("Edit is OK");
                                    getchar();
                                }
                            }while(e_mode);
                        }else puts("Catalog is empty!");
                        getchar();
                        break;
                    case 4:
                        break;
                    case 5:
                        SaveToFile(movies);
                    case 0:
                        break;
                    default:
                        puts("Enter correct mode!");
                        getchar();
                    }
                }while(mode_1);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                DemoOutput(movies);
                getchar();
                break;
            case 0:
                break;
            default:
                puts("Enter correct mode!");
                getchar();
        }
    }while(mode);
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
        fclose(p_file);
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
            fprintf(p_file, "%s;%s;%s;", list->title, list->director_name, list->genre);
            for(i=0; i<3; i++) fprintf(p_file, "%d;", list->release_date[i]);
            for(i=0; i<2; i++) fprintf(p_file, "%.2f;", list->rating[i]);
            if(list->next) fprintf(p_file, "%d\n", list->kinopoisk_star);
            else fprintf(p_file, "%d", list->kinopoisk_star);
            list = list->next;
        }
        fclose(p_file);
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

film *DelElByTit(film *list, char *name)
{
    film *tmp_el;
    tmp_el = list;
    while(tmp_el){
        if(!strcmp(tmp_el->title, name)) list = DelEl(list, tmp_el);
        tmp_el = tmp_el->next;
    }
    return list;
}

film *DelEl(film *fir_el, film *cur_el)
{
    film *ret_el;
    if(cur_el&&fir_el){
        if(fir_el == cur_el){
            ret_el = fir_el->next;
            if(ret_el) ret_el->prev = NULL;
        }
        else{
            ret_el = fir_el;
            cur_el->prev->next = cur_el->next;
            if(cur_el->next) cur_el->next->prev = cur_el->prev;
        }
        free(cur_el->director_name);
        cur_el->director_name = NULL;
        free(cur_el->genre);
        cur_el->genre = NULL;
        free(cur_el->title);
        cur_el->title = NULL;
        free(cur_el);
        cur_el = NULL;
    }
    return ret_el;
}

int EditFilm(film *list, int mode, char *par, char *name)
{
    char *tmp_str;
    char sep = '.';
    int i, is_film;

    is_film = 0;

    while(list){
        if(!strcmp(list->title, name)){
            switch(mode){
                case 1:
                    strcpy(list->title, par);
                    break;
                case 2:
                    strcpy(list->director_name, par);
                    break;
                case 3:
                    strcpy(list->genre, par);
                    break;
                case 4:
                    tmp_str = (char*)malloc(MaxStrLen);
                    strcpy(tmp_str, par);
                    tmp_str = strtok(tmp_str, &sep);
                    for(i=0; i<3; i++){
                        list->release_date[i] = atoi(tmp_str);
                        tmp_str = strtok(NULL, &sep);
                    }
                    break;
                case 5:
                    list->rating[0] = atof(par);
                    break;
                case 6:
                    list->rating[1] = atof(par);
                    break;
                case 7:
                    list->kinopoisk_star = atoi(par);
            }
            is_film = 1;
        }
        list = list->next;
    }
    return is_film;
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