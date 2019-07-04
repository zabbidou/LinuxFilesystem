#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "struct.h"

int main() {
    char* buffer = calloc(1000, sizeof(char));
    char* param = NULL;
    char cmd[10], dir_name[20], F[5], path[100];
    char object[10], obj_path[20];
    char absolute_path[100];
    int exit_code;
    int is_root;
    char* temp = NULL;
    char* end_ptr;

    Trie a = AllocTrie();

    Trie* current_dir = &a;
    Trie directory = NULL;

    FILE* in = fopen("in.txt", "r");
    if (!in) {
        fprintf(stdout, "Input file error\n");
        return -1;
    }

    FILE* out = fopen("output.out", "w");
    if (!out) {
        fprintf(stdout, "Output file error\n");
        fclose(in);
        return -1;
    }

    FILE* err = fopen("error.out", "w");
    if (!err) {
        fprintf(stdout, "Error file error\n");
        fclose(in);
        fclose(out);
        return -1;
    }

    int i = 0;
    int error_code;

    int counter = 0;
    int j = 0;

    scanf("%d ", &counter);
    printf("----%d----\n", counter);

    for (j = 0; j < counter; j++) {
        scanf("%[^\n]%*c", buffer);
        //printf("????\n");
        i++;
        
        //parse_shit(buffer, object, obj_path);
        //printf("buff: %s obj: %s path: %s\n", buffer, object, obj_path);

        if (!strncmp(buffer, "path", 4)) {
            char temp[20];
            sscanf(buffer, "%s %s", cmd, temp);
            is_path_valid(a, temp);
        }

        if (!strncmp(buffer, "mkdir ", 6)) {

            sscanf(buffer, "%s %[^\n]%*c", cmd, dir_name);

            printf("-------%d---%s\n", i, buffer);


            char* name = NULL;
            name = strdup(dir_name);

            param = strtok_r(dir_name, " ", &end_ptr);

            char* whatever = NULL;

            while (param != NULL) {
                //strcpy(obj_path, "\0");
                temp = strdup(param);
                memset(obj_path, 0, sizeof(obj_path));
                parse_shit(temp, object, obj_path);
                //char* pula = strtok(NULL, " ");

                printf("obj: %s obj path: %s\n",object, obj_path);
                
                //if (param[0] == '/') {

                // daca pot sa fac in pathu ala

                if (is_path_valid(a, obj_path) == 1) {
                
                    whatever = strdup(absolute_path);
                
                    printf("1: %s\n", absolute_path);
                
                    if (strlen(obj_path) != 0) {
                            // daca am obj path
                            //sprintf(buffer, "cd %s", obj_path);
                            
                        directory = *current_dir;

                        if (param[0] == '/') {
                            printf("absolut cu cd, %s\n", obj_path);
                            *current_dir = cd(a, *current_dir, obj_path, absolute_path, 1);
                            strcpy(absolute_path, whatever);
                            error_code = mkdir(*current_dir, object);
                            *current_dir = directory;
                        } else {
                            printf("relativ cu cd\n");
                            *current_dir = cd(a, *current_dir, obj_path, absolute_path, 0);
                            strcpy(absolute_path, whatever);
                            error_code = mkdir(*current_dir, object);
                            *current_dir = directory;
                        }
                    } else {
                        //daca n-am
                        if (param[0] == '/') {
                            printf("absolut fara cd\n");
                            error_code = mkdir(a, object);
                        } else {
                            printf("relativ fara cd\n");
                            error_code = mkdir(*current_dir, object);
                        }

                        if (error_code == -1) {
                            printf("NU AR TREBUI SA VEZI ASTA!!!\n");
                        }
                    }
                } else {
                    fprintf(err, "%s: No such file or directory\n", param);
                    printf("%s: No such file or directory\n", param);                        
                }

                param = strtok_r(NULL, " ", &end_ptr);
                printf("MAI AI DE FACUT ASTA: %s\n", param);
            }
        }

        if (!strncmp(buffer, "cd ", 3)) {

            sscanf(buffer, "%s %s", cmd, dir_name);

            Trie temp = *current_dir;
            Trie exit_value;
            char* name = NULL;
            name = strdup(dir_name);
            printf("before: %p\n", temp);

            exit_value = cd(a, temp, dir_name, absolute_path, 0);

            printf("after: %p %p\n", temp, *current_dir);

            if (exit_value != NULL) {
                current_dir = &exit_value;
            } else {
                current_dir = &temp;
                fprintf(err, "%s: No such file or directory\n", name);
                printf("%s: No such file or directory\n", name);

            }

            printf("afterv2: %p %p\n", temp, *current_dir);

            /*int i = 0;
            if (dir_name[0] == '/') {
                param = strtok(dir_name + 1, "/");
                is_root = 1;
            } else {
                param = strtok(dir_name, "/");
                is_root = 0;
            }
                printf("par:%s\n", param);

            if (param == NULL) {
                temp = cd(a, temp, param, absolute_path, is_root);
                current_dir = &temp;
            }

            while (param != NULL) {
                printf("%d %s\n", i, param);
                i++;

                printf("1: %p %p\n", temp, *current_dir);
                temp = *current_dir;
                temp = cd(a, temp, param, absolute_path, is_root);
                printf("2: %s \n", absolute_path);
                
                current_dir = &temp;
                printf("3: %p %p\n", temp, *current_dir);

                param = strtok(NULL, "/");

                if (is_root == 1) {
                    is_root = 0;
                }
            }*/
        }

        if (!strncmp(buffer, "ls", 2)) {
            exit_code = sscanf(buffer, "%s %s %s", cmd, F, dir_name);
            if (*current_dir == a) {
                is_root = 1;
            } else {
                is_root = 0;
            }
            if (exit_code == 3) {
                ////printf("ls cu -F si nume\n");
            }
            if (exit_code == 2) {
                ////printf("ls cu 2 param\n");
                if (!strcmp(F, "-F")) {
                    ////printf("are F, nu nume\n");
                    int lvl = (*current_dir)->file_level;
                    ls(*current_dir, is_root, 1, path, 0, lvl + 1, out);
                    fprintf(out, "\n");
                    printf("\n");
                    
                } else {
                    ////printf("are nume, nu F\n");
                }
            }
            if (exit_code == 1) {
                Trie temp = *current_dir;
                ls(temp, is_root, 0, path, 0, (*current_dir)->file_level + 1, out);
                fprintf(out, "\n");
                printf("\n");
                current_dir = &temp;
            }

        }

        if (!strcmp(buffer, "pwd")) {
            if (strlen(absolute_path) == 0) {
                printf("/\n");
                fprintf(out, "/\n");
            } else {
                printf("%s\n", absolute_path);
                fprintf(out, "%s\n", absolute_path);
            }
        }
    }

    return 0;
}