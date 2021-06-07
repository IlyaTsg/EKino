#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ekino.h"

#define OS linux
#ifdef OS 
#define CLS system("clear")
#else
#define CLS system("cls")
#endif

int main()
{
    enum Bool {False, True};
    film *movies;
    char *(*FindParam[3])(film*);
    char del_str[MaxStrLen], edit_str[MaxStrLen];
    int mode, mode_1, mode_2, mode_3, e_mode;
    int edit_flag;

    FindParam[0] = GetDirecName;
    FindParam[1] = GetTitle;
    FindParam[2] = GetGenre;

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
                            puts("Delete is OK!");
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
                                    else puts("Edit is OK!");
                                    getchar();
                                }
                            }while(e_mode);
                        }else puts("Catalog is empty!");
                        getchar();
                        break;
                    case 4:
                        if(movies){
                            movies = ClearMem(movies);
                            puts("Clearing is OK!");
                        }else puts("Catalog is empty!");
                        getchar();
                        break;
                    case 5:
                        SaveToFile(movies);
                        puts("Save is OK!");
                        getchar();
                    case 0:
                        break;
                    default:
                        puts("Enter correct mode!");
                        getchar();
                    }
                }while(mode_1);
                break;
            case 2:
                if(movies){
                    do{
                        CLS;
                        puts("-----Menu-----");
                        puts("1 - By director name");
                        puts("2 - By title");
                        puts("3 - By genre");
                        puts("0 - Exit");
                        do{
                            printf("Enter your choice: ");
                            scanf("%d", &mode_2);
                            getchar();
                        }while(mode_2<0 || mode_2>3);
                        if(mode_2){
                            printf("Enter parameter: ");
                            EnterString(edit_str);
                            if(OutputBy(movies, edit_str, FindParam[mode_2-1])) puts("No films with this parameter!");
                            getchar();
                        }
                    }while(mode_2);
                }
                else{ 
                    puts("Catalog is empty!");
                    getchar();
                }
                break;
            case 3:
                if(movies){
                    do{
                        CLS;
                        puts("-----Menu-----");
                        puts("1 - By date of release");
                        puts("2 - By Imdb rating");
                        puts("0 - Exit");
                        printf("Enter your choice: ");
                        scanf("%d", &mode_3);
                        getchar();
                        switch(mode_3){
                            case 1:
                                movies = SortBy(movies, GetDay);
                                movies = SortBy(movies, GetMounth);
                                movies = SortBy(movies, GetYear);
                                puts("Sort is OK!");
                                getchar();
                                break;
                            case 2:
                                movies = SortBy(movies, GetRating);
                                puts("Sort is OK!");
                                getchar();
                                break;
                            case 0:
                                break;
                            default: 
                                puts("Enter correct mode!");
                                getchar();
                        }
                    }while(mode_3);
                }
                else{ 
                    puts("Catalog is empty!");
                    getchar();
                }
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