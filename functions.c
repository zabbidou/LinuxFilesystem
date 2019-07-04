#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"

int ceva() {
    printf("functions.c\n");
    return 0;
}

Trie AllocTrie() {
    Trie a = calloc(1, sizeof(TNode));
    a->is_final = 0;
    a->file_level = 0;
    
    if (!a) {
        return NULL;
    }

    return a;
}

void parse_shit(char* string, char* object, char* path) {
    int poz = -1;
    unsigned int i;
    for (i = 0; i < strlen(string); i++) {
        if (string[i] == '/') {
            poz = i;
        }
    }

    if (poz == -1) {
        printf("------ai belit cariciul-------\n");
        strcpy(object, string);
        path = NULL;
    } else {
        strncpy(path, string, poz);
        strcpy(object, string + poz + 1);
    }

    //printf("pula: %s\n", path);
    //printf("pulaaaa: %s\n", object);
}

int is_path_valid (Trie a, char* path) {
    unsigned int i = 0;
    for (i = 0; i < strlen(path); i++) {
        if (path[i] == '/') {
            continue;
        }

        if (a->children[path[i] - 'a'] != NULL) {
            //printf("e bun, exista\n");
            a = a->children[path[i] - 'a'];
        } else {
            //printf("NO.\n");
            return -1;
        }
    }
    return 1;
}

int mkdir(Trie a, char* name) {
    printf("----------------------cur dir: %p\n\n", a);
    Trie p = a;
    int file_level;
    unsigned int i;
    //printf("what\n");
    if (name[0] == '/') {
        //printf("mkdir in root\n");
        //p = a;
        file_level = 1;
        i = 1;
    } else {
        //printf("mkdir in current_dir\n");
            //printf("fin~~~~%p~~~~~~\n", current_dir);

        //p = current_dir;
        file_level = a->file_level + 1;
        i = 0;
    }

    for (; i < strlen(name); i++) {
        /*if (name[i] == '/' && i != 0) {
            file_level = file_level + 1;
            continue;
        }*/

        int ch = name[i] - 'a';
        if (ch < 0) {
            //ch = ch + 172;
            ch = ch + 75;
            
            //123
            //48 - 97 = -49
        }
        if (name[i] == '.') {
            ch = 36;
        }
        //printf("i: %d ch: %d name: %c\n", i, ch, name[i]);

        if (p->children[ch] != NULL) {
            printf("not today\n");
            //p = p->children[ch];
            if (i == strlen(name) - 1) {
                //printf("ultimul caracter: %d\n", ch);
                (p->children[ch])->is_final = 1;
                (p->children[ch])->type = 0;
                (p->children[ch])->file_level = file_level;
            }
        } else {
            //printf("yes today: ");
            //printf("i: %d ch: %d\n", i, ch);
            p->children[ch] = AllocTrie();
            
            if (!(p->children[ch])) {
                // free shit
                printf("free this shit\n");
                return -1;
            }

            (p->children[ch])->file_level = file_level;
            printf("---------parent: %p, child: %p\n", p, p->children[ch]);
            (p->children[ch])->parent = p;

            if (i == strlen(name) - 1) {
                //printf("ultimul caracter: %d\n", ch);
                (p->children[ch])->is_final = 1;
                (p->children[ch])->type = 0;
            }
            

        }
        p = p->children[ch];
    }
            //printf("fin~~~~%p~~~~~~\n", a);
    return 1;
}

void ls(Trie a, int is_root, int F, char* path, int level, int lvl_to_print, FILE* out) {
    unsigned int i;

    if (a == NULL) {
        printf("NU AR TRB SA VVEZI ASTA\n");
        return;
    }

    //printf("conditii: %d %d %d\n", a->is_final, a->file_level, lvl_to_print);

    if (a->is_final == 1 && a->file_level == lvl_to_print) {
        //printf("plmm\n");
        path[level] = '\0';
        if (is_root == 0) {
            fprintf(out, "%s ", path);
            printf("%s ", path);

        } else {
            printf("/%s ", path);
            fprintf(out, "/%s ", path);
        }
    }

    for (i = 0; i < MAX_SIZE; i++) {
            //printf("fin102~~~~%p~~~~~~\n", a);
    
        if (a->children[i] != NULL) {
            //printf("fin103~~~~%p~~~~~~\n", a);
    
            if (is_root == 1) {
                //printf("/");
                //strcat()
                //path[level] = 
            } else {

            }
            if (i > 25) {
                path[level] = i + 22;
            } else if (i == 36) {
                path[level] = '.';
            } else {
                path[level] = 97 + i;
            }
            //printf("lvl: %d %c\n", level, i + 'a');
            //print_shit(p->children[i], F);
            //printf(" ");
            ls(a->children[i], is_root, F, path, level + 1, lvl_to_print, out);
            //printf("fin1000004~~~~%p~~~~~~\n", a);

        }
    }
}

Trie cd(Trie a, Trie current_dir, char* path, char* abs_path, int is_abs) {
    Trie p = a;
    //printf("pathu puliiv2: %s\n", path);
    char original_path[100], *useless;
    int poz = -1;
    char* temp;
    unsigned int i;
    int file_level = -1;

    strcpy(original_path, abs_path);

/*    if (path == NULL) {
        strcpy(abs_path, "\0");
        return a;
    }*/

    if (path[0] == '/') {
        temp = strtok(path + 1, "/");
        is_abs = 1;
    } else {
        temp = strtok(path, "/");
        is_abs = 0;
    }

    printf("PULA: %s\n", temp);

    if (temp == NULL) {
        strcpy(abs_path, "\0");
        return a;
    }

    while (temp != NULL) {

        if (!strcmp(temp, "..")) {
            //printf("fa chioaro %s\n", abs_path);
            p = current_dir;

            for (i = 0; i < strlen(abs_path); i++) {
                if (abs_path[i] == '/') {
                    poz = i;
                }
            }

            if (poz == -1) {
                strcpy(abs_path, "/");
            } else {
                useless = strdup(abs_path);
                memset(abs_path, 0, 100);
                strncpy(abs_path, useless, poz);
                //strcpy(abs_path, useless);
            }
            
            /*if (param != NULL) {
                printf("NOOOOOOOOOOO %d\n", strlen(param));
            }
            printf("NEW PATH: %s\n", param);
            strcpy(abs_path, param);*/

            printf("NEW PATH: %s\n", abs_path);

            file_level = p->file_level;
            if (current_dir == a) {
                return NULL;
            } else {
                p = current_dir->parent;
            }

            printf("cd in sus boss %d\n", file_level);
            while (p != a && p->file_level != file_level - 1) {
                printf("up up and above, %d p: %p parent: %p\n", p->file_level, p, p->parent);
                p = p->parent;
            }

            current_dir = p;

            //return p;
            temp = strtok(NULL, "/");

            continue;

        }

        if (is_abs == 0) {
            p = current_dir;
            /*if (current_dir == a) {
                printf("ey boss, I found the root\n");
                strcpy(abs_path, "/");
            }*/
            strcat(abs_path, "/");
            strcat(abs_path, temp);
            printf("cale relativa, temp: %s\n", abs_path);



            for (i = 0; i < strlen(temp); i++) {
                printf("plm: %d\n", temp[i] - 'a');
                if (p->children[temp[i] - 'a'] != NULL) {
                    printf("exista %c\n", temp[i]);
                    p = p->children[temp[i] - 'a'];
                    current_dir = p;
                } else {
                    printf("nu exista, ciau pa %p %p\n", p, current_dir);
                    strcpy(abs_path, original_path);
                    return NULL;
                }
            }
            
            temp = strtok(NULL, "/");
            continue;
        }

        if (is_abs == 1) {
            p = a;
            is_abs = 0;
            
            strcpy(abs_path, "/");
            strcat(abs_path, temp);

            printf("incepe cu /, abs temp: %s\n", abs_path);


            for (i = 0; i < strlen(temp); i++) {
                printf("proasta satului: %c\n", temp[i]);
                if (p->children[temp[i] - 'a'] != NULL) {
                    p = p->children[temp[i] - 'a'];
                    current_dir = p;
                } else {
                    printf("nu exista, ciau pa\n");
                    strcpy(abs_path, original_path);
                    return NULL;
                    //break;
                }
            }

        
            temp = strtok(NULL, "/");

            continue;
        }


        //temp = strtok(NULL, "/");

    }

    return current_dir;
}

void destroy_trie(Trie* a) {
    if (a == NULL) {
        return;
    }

    int i = 0;

    for (i = 0; i < MAX_SIZE; i++) {
        if ((*a)->children[i] != NULL) {
            destroy_trie((*a)->children);
            free(*a);
        }
    }
}