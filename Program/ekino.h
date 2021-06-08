#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int EnterString(char *str)
{
    int len;
    fgets(str, MaxStrLen, stdin);
    len = strlen(str);
    str[len-1] = '\0';
    return len;
}

film *MakeList()
{
    film *first_el = NULL;
    if(first_el = (film*)malloc(sizeof(film))){
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
    if(new_el&&cur_el){
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
    film *tmp_el;
    char *tmp_str;
    char sep = '.';
    int slen, i;

    tmp_el = list;

    if(tmp_str=(char*)malloc(MaxStrLen)){
        while(list){
            tmp_el = list;
            list = list->next;
        }
        list = MakeList();
        InsertAfter(tmp_el, list);
        
        if(list){
            printf("Enter title of movie: ");
            slen = EnterString(tmp_str);
            if(list->title=(char*)malloc(sizeof(slen+1))){
                strcpy(list->title, tmp_str);
                printf("Enter director name of movie: ");
                slen = EnterString(tmp_str);
                if(list->director_name=(char*)malloc(sizeof(slen+1))){
                    strcpy(list->director_name, tmp_str);

                    printf("Enter genre of movie: ");
                    slen = EnterString(tmp_str);
                    if(list->genre=(char*)malloc(sizeof(slen+1))){
                        strcpy(list->genre, tmp_str);

                        printf("Enter release date of movie(by point): ");
                        EnterString(tmp_str);
                        tmp_str = strtok(tmp_str, &sep);
                        for(i=0; i<3; i++){
                            list->release_date[i] = atoi(tmp_str);
                            tmp_str = strtok(NULL, &sep);
                        }

                        if(tmp_str=(char*)malloc(MaxStrLen)){
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

film *ClearMem(film *list)
{
    if(list){
        ClearMem(list->next);
        free(list->director_name);
        free(list->genre);
        free(list->title);
        list->director_name = NULL;
        list->genre = NULL;
        list->title = NULL;
        free(list);
        list = NULL;
    }
    return list;
}

int OutputBy(film *movies, char *user_par, char *(*FuncName)(film*)){
    int flag; // is here film with whis param
    flag = 1;
    printf("\n|Title|Director name|Genre|Date of release|Imdb r|Kinopoisk r|Kinopoisk star|\n");
    while(movies){
        if(!strcmp(FuncName(movies), user_par)){
            puts("----------------------------------------------------------------------------");
            printf("|%s", movies->title);
            printf("|%s", movies->director_name);
            printf("|%s", movies->genre);
            printf("|%d.%d.%d", movies->release_date[0], movies->release_date[1], movies->release_date[2]);
            printf("|%.2f", movies->rating[0]);
            printf("|%.2f", movies->rating[1]);
            printf("|%d|\n", movies->kinopoisk_star);
            flag = 0;
        }
        movies = movies->next;
    }
    puts("----------------------------------------------------------------------------");
    return flag;
}

char *GetDirecName(film *elem){ return elem->director_name; }
char *GetTitle(film *elem){ return elem->title; }
char *GetGenre(film *elem){ return elem->genre; }

film *Swap(film *el_1, film *el_2)
{
    film *tmp_el;
    if(el_1 && el_2){
        if(el_1->next == el_2){
            el_1->next = el_2->next;
            el_2->prev = el_1->prev;
            el_1->prev = el_2;
            el_2->next = el_1;
        }
        else{
            tmp_el = el_1->next;
            el_1->next = el_2->next;
            el_2->next = tmp_el;
            tmp_el = el_1->prev;
            el_1->prev = el_2->prev;
            el_2->prev = tmp_el;
            if(el_1->prev) el_1->prev->next = el_1;
            if(el_2->next) el_2->next->prev = el_2;
        }
        if(el_1->next) el_1->next->prev = el_1;
        if(el_2->prev) el_2->prev->next = el_2;
        while(el_1->prev) el_1 = el_1->prev;
    }
    return el_1;
}

film *SortBy(film *movies, int (*GetParam)(film*))
{
    if(movies){
        film *tmp_el1, *tmp_el2;
        int i, j;
        while(movies->next){
            tmp_el1 = movies->next;
            while(tmp_el1){
                if(GetParam(movies) > GetParam(tmp_el1)){ 
                    Swap(movies, tmp_el1);
                    tmp_el2 = tmp_el1;
                    tmp_el1 = movies;
                    movies = tmp_el2;
                }
                tmp_el1 = tmp_el1->next;
            }
            movies = movies->next;
        }
        while(movies->prev) movies = movies->prev;
    }
    return movies;
}

int GetYear(film *elem){ return elem->release_date[2]; }
int GetMounth(film *elem){ return elem->release_date[1]; }
int GetDay(film *elem){ return elem->release_date[0]; }
int GetRating(film *elem){ return elem->rating[0]; }

void OutputList(film *list)
{
    printf("\n|Title|Director name|Genre|Date of release|Imdb r|Kinopoisk r|Kinopoisk star|\n");
    while(list){
        puts("----------------------------------------------------------------------------");
        printf("|%s", list->title);
        printf("|%s", list->director_name);
        printf("|%s", list->genre);
        printf("|%d.%d.%d", list->release_date[0], list->release_date[1], list->release_date[2]);
        printf("|%.2f", list->rating[0]);
        printf("|%.2f", list->rating[1]);
        printf("|%d|\n", list->kinopoisk_star);
        list = list->next;
    }
    puts("----------------------------------------------------------------------------");
}