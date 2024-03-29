#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

struct dirent *readdir(DIR *dirp);
char *count[10000];
char *clipboard;
int len = 0 , pos_line , pos_char , Have_Space = 0 , counter_new_LINE , size , first_cur , second_cur , size_clipboard , space_add = 0 , counter_path = 0 , dash = 0 , aculad = 0;
char bORf ;
char cORI; 
int counters = 0;

void help();
void cat(char * , char *);
int checking_validation(char *);
void create_file(char * , char *);
void cd(char *);
int count_char_word(char * ,int , int);
void filename(char *,char *);
void insert();
void position(char * , char * , char * );
void directory(char * , char **);
void dir_compare(char * , char **);
void go_to_directory(char **);
void go_to_root();
void remove_file();
void cursor(char * , int  , int , int );
void copy_file();
void cut();
void paste();
void grep();
void compare();
void say_my_dir();
void tree(char *, const int ,int);
void dirintry();
void tree_all();
void dir_grep(char );
void find();
void auto_indent();
void print_grep(char*  , int , char *);

int main() {    
    mkdir("root" , 0755);
    chdir("root");
    printf("Enter your command :\n");
    printf("(If you need help type the word HELP)\n");
    while(1){
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s ", cwd);
        } 
        else {
            perror("getcwd() error");
            return 1;
        }

        char cmd[10];
        char val[7];
        char address[1000];
        scanf("%s" , cmd);
        if(strcmp(cmd , "HELP") == 0) {
            help();
        }
        else if(strcmp(cmd , "CAT") == 0){
            scanf("%s" , val);
            scanf(" %[^\n]%*c" , address);
            if(checking_validation(val) == 1){
                continue;
            }
            char *file_name = (char*) calloc(2000 , sizeof(char));
            char *file_name2 = (char*) calloc(2000 , sizeof(char));
            int slash = '/';
            file_name = strrchr(address, slash);
            file_name2 = strtok(file_name, "/");
            filename(address , file_name2);
            cat(file_name2 , address);
        }
        else if(strcmp(cmd , "CRF") == 0){
            scanf("%s" , val);
            scanf(" %[^\n]%*c" , address);
            if(checking_validation(val) == 1){
                continue;
            }
            char *file_name;
            char *file_name2;
            int slash = '/';
            file_name = strrchr(address, slash);
            file_name2 = strtok(file_name, "/");
            filename(address , file_name2);
            create_file(address , file_name2);
            printf("File created successfully.\n");
            printf("\n--------------------\n");
        }
        else if(strcmp(cmd,"INF") == 0){
            scanf("%s" , val);
            if(checking_validation(val) == 1){
                continue;
            }
            insert();
        }
        else if(strcmp(cmd , "RMV") == 0){
            scanf("%s" , val);
            if(checking_validation(val) == 1){
                continue;
            }
            remove_file();
        }
        else if(strcmp(cmd , "CPY") == 0){
            scanf("%s" , val);
            if(checking_validation(val) == 1){
                continue;
            }
            copy_file();
        }
        else if(strcmp(cmd , "CUT") == 0){
            scanf("%s" , val);
            if(checking_validation(val) == 1){
                continue;
            }
            cut();
        }
        else if(strcmp(cmd , "PST") == 0){
            scanf("%s" , val);
            if(checking_validation(val) == 1){
                continue;
            }
            paste();
        }
        else if(strcmp(cmd , "grep") == 0){
            grep();
        }
        else if(strcmp(cmd , "compare") == 0){
            compare();
        }
        else if(strcmp(cmd , "tree") == 0){
            char buff[FILENAME_MAX];
            char path[FILENAME_MAX];
            GetCurrentDir( buff, FILENAME_MAX );
            GetCurrentDir( path, FILENAME_MAX );
            char *token = strtok(path , "/");
            char *folder;
            while(token != NULL){
                folder = token;
                token = strtok(NULL , "/");
            }
            int depth;
            scanf("%d" , &depth);
            printf("your current folder with %d depth is : \n%s\n" , depth , folder);
            if(depth > 0){
                printf("|\n");
                tree(buff, 0 , depth);
            }
            else if(depth == -1){
                printf("||\n");
                tree_all(buff , 1);
            }
            else if (depth < -1 ){
                printf("the depth you intered isn't valid :(\n");
                continue; 
            }
        }
        else if(strcmp(cmd , "find") == 0){
            find();
        }
        else if(strcmp(cmd , "auto_indent") == 0){
            auto_indent();
        }
        else{
            printf("Command does not exist");
            printf("\n--------------------\n");
        }
    }   
 
    return 0;
}

int checking_validation(char * val){
    if(strcmp(val , "--file") == 0){
        return 0;
    }
    else{
        printf("invalid command");
        printf("\n--------------------\n");
        return 1;
    }
}

void help(){
    printf("Here is a list of commands you can use : \n");
    printf("-CAT = print what is in a file\n");
    printf("-CRF = create a file\n");
    printf("-INF = insert a file\n");
    printf("-PLF = print list of files\n");
    printf("--------------------\n");
}

void cat(char *file_name , char *address){
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    char* token1 = strtok(buff, " / ");
    while(token1 != NULL) {
        token1 = strtok(NULL, " / ");
        if(strcmp(token1 , "root") == 0){
            while(token1 != NULL) {
                chdir("..");
                token1 = strtok(NULL, "/");
            }
        }
    }
    const char *temp;
    char *temp2;
    int counter_address = 0;
    char backup_address[10000];
    strcpy(backup_address , address);
    char *temp3 = strtok(backup_address, "/");
    while(temp3 != NULL){
        counter_address++;
        temp3  = strtok(NULL, "/");
    }
    char *token2 = strtok(address, "/");
    while(token2 != NULL){
        if(strcmp(token2 , "\"") == 0){ 
            temp = token2;
            token2 = strtok(NULL, "/");
        }
        else{
            const int a = chdir(token2);
            if(strcmp(token2 , file_name) == 0){
                FILE * fPtr; 

                fPtr = fopen(file_name , "r");
                char data[100000];
                if(fPtr != NULL){
                    while (fgets(data , 100000 , fPtr)){
                        printf("%s" , data);
                    }
                }
                else{
                    printf("File does not exist");
                }
                fclose(fPtr);
                printf("\n--------------------\n");
                break;
            }
            else if(a == 0){
                chdir(token2);
                token2 = strtok(NULL, "/");
            }
            else{
                printf("There is not such a directory or file\n");
                break;
            }
        }
    }
    
}

void create_file(char *address , char *file_name){
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    char* token1 = strtok(buff, " / ");
    while(token1 != NULL) {
        token1 = strtok(NULL, " / ");
        if(strcmp(token1 , "root") == 0){
            while(token1 != NULL) {
                chdir("..");
                token1 = strtok(NULL, "/");
            }
        }
    }
    GetCurrentDir( buff, FILENAME_MAX );
    const char *temp;
    char *temp2;
    char *temp3;
    char *token2 = strtok(address, "/");
    if(strcmp(token2 , "\"") == 0){
        while (token2 != NULL) {
            if(strcmp(token2 , "\"") == 0){
                temp = token2;
                token2 = strtok(NULL, "/");
            }
            else if(strcmp(token2 , file_name) == 0){
                FILE * a;
                a = fopen(file_name , "w");
                fclose(a);
                return;
            }
            else{
                cd(token2);
                temp = token2;
                token2 = strtok(NULL, "/");
            }
        }
        
    }
    else {
        while (strcmp(token2 , file_name)) {
            cd(token2);
            temp = token2;
            token2 = strtok(NULL, " / ");
        }
    FILE * a;
    a = fopen(file_name , "w");
    fclose(a);
    }
}

void cd(char *folder){
    mkdir(folder , 0755);
    chdir(folder);
}

int count_char_word(char *file_name , int pos1 , int pos2){
    FILE* fp;

    char filename[FILENAME_MAX];

    char c ;
    char q;
    counter_new_LINE = 0;
  
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Could not open file %s\n",file_name);
        return 0;
    }
    for (q = getc(fp); q != EOF; q = getc(fp)){
        if(q == '\n'){
           counter_new_LINE = counter_new_LINE + 1;
        }
    }
    counter_new_LINE++;
    if(pos1 > counter_new_LINE || pos1 < 1) {
        return -1;
    }
    int count[counter_new_LINE ][3] ;

    for(int x = 0 ; x < counter_new_LINE ; x++){
        for(int q = 0 ; q < 2 ; q++){
            count[x][q] = 0;
        }
    }
    for(int x = 0 ; x < counter_new_LINE ; x++){
        for(int q = 2 ; q < 3 ; q++){
            count[x][q] = 1;
        }
    }
    fclose(fp);
    fp = fopen(file_name, "r");
    int i = 0 , j =0;
    while(c != EOF){
        for (c = getc(fp); c != '\n' && c != EOF; c = getc(fp)){
            count[i][1]++;
        }
        count[i][0] = i + 1;
        i++;
    }
    fclose(fp);
    fp = fopen(file_name, "r");
    char ch;
    i = 0;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n' || ch == '\0')
            i++;

        if (ch == ' ' || ch == '\t')
            count[i][2]++;
    }

    fclose(fp);
    if(pos2 > count[pos1 - 1][1] || pos2 < 0) return -1;

    return 1;
}

void filename(char *address ,char *file_name2){
    if(address[0] == '\"'){
        int z ;
        for(z = 1 ; file_name2[strlen(file_name2) - z] != '\"' ; ){
            file_name2[strlen(file_name2) - z] = '\0';
        }
        file_name2[strlen(file_name2) - (z)] = '\0';
    }
}

void insert(){
    char dir1[1000];
    char *dir2[100];
    char valstr[20] = "12345678";
    char matn[10000];
    char temp;
    getchar();
    directory(dir1 , dir2);
    go_to_root();
    go_to_directory(dir2);
    if(Have_Space == 0){
        valstr[0] = '-';
        int i;
        for(i = 1 ; ; i++){
            valstr[i] = getchar();
            if(valstr[i] == ' ') break;
        }
        valstr[i] = '\0';
    }
    else{
        scanf("%s" , valstr);
        getchar();
    }
    if(strcmp(valstr , "--str") == 0){
        temp = getchar();
        if(temp != '\"'){
            Have_Space = 1;
            matn[0] = temp;
            temp = getchar();
            int x = 1;
            while(temp != ' '){
                matn[x] = temp;
                temp = getchar();
                x++;
            }
        }
        else{
            Have_Space = 0;
            int x = 0;
            while(1){
                scanf("%c" , &matn[x]);
                if(matn[x] == '-'){
                    if(matn[x-1] == ' '){
                        if(matn[x-2] == '\"'){
                            matn[x-2] = '\0';
                            break;
                        }
                    }
                }
                x++;
            }           
        }
        printf("your text is : %s\n" , matn);
        char posstr[20] ="1234567890";
        if(Have_Space == 0){
            posstr[0] = '-';
            int i;
            for(i = 1 ; ; i++){
                posstr[i] = getchar();
                // printf("posstr[%d] is : %c\n" , i , posstr[i]);
                if(posstr[i] == ' ') break;
            }
            posstr[i] = '\0';
        }
        else{
            scanf("%s" , posstr);
        }
        // printf("posstr is : %s\n" , posstr);
        if(strcmp(posstr , "--pos") == 0) {
            scanf("%d:%d" , &pos_line , &pos_char);
        }
        else{ printf("Invalid command!\n"); return;}
        FILE * fPtr; 
        printf("filename is : %s\n" , dir2[len - 1]);
        fPtr = fopen(dir2[len - 1] , "r");
        if(fPtr == NULL){
            printf("File does not exist\n");
            return;
        }
        else{
            int a = count_char_word(dir2[len - 1] , pos_line , pos_char);
            if(a == -1){
                printf("position does not exist\n");
                return;
            }
            else if(a == 1){
                FILE * newFile;
                newFile = fopen("nemidoonam" , "w");
                int row = 0 , character = 0;
                int fileChar;
                // printf("pos line is : %d\ncounter new line is : %d\n" , pos_line , counter_new_LINE);
                while (row < pos_line - 1){
                    fileChar = fgetc(fPtr);
                    // printf("filechar is : %c\n" , fileChar);
                    fputc(fileChar , newFile);
                    if(fileChar == '\n') row++;
                }

                while (character < pos_char ){
                    fileChar = fgetc(fPtr);
                    // printf("filechar is : %c\n" , fileChar);
                    fputc(fileChar , newFile);
                    character++;
                }
                for(int a = 0 ; matn[a] != '\0' ; a++){
                    int x = matn[a];
                    if(matn[a] == '\\'){ 
                        if(matn[a+1] == '\\'){ 
                            if(matn[a+2] == 'n') {
                                fprintf(newFile , "\\n"); 
                                a = a + 2;
                            }
                        }
                        else if(matn[a] == '\\') {
                            if(matn[a+1] == 'n') {
                                int y = '\n';
                                fputc(y , newFile);
                                a = a + 1;
                            }
                        }
                    }
                    else {
                        fputc(x , newFile);
                    }
                }
                while(fileChar != EOF){
                    fileChar = fgetc(fPtr);
                    if(fileChar == EOF) break;
                    fputc(fileChar , newFile);
                }
                fclose(newFile);
                fclose(fPtr);
                fPtr = fopen(dir2[len - 1] , "w");
                newFile = fopen("nemidoonam" , "r");
                fileChar = fgetc(newFile);
                // printf("filechar is : %c\n" , fileChar);
                while (fileChar != EOF)
                {
                    // printf("filechar is : %c\n" , fileChar);
                    fputc(fileChar,fPtr);
                    fileChar = fgetc(newFile);
                }
                fclose(newFile);
                fclose(fPtr);
                remove("nemidoonam");
            }
        }
    }
}

void position(char *address , char *pos1 , char *pos2){
    char *backup_address = (char*) calloc(2000 , sizeof(char));
    char *backup_address2 = (char*) calloc(2000 , sizeof(char));
    strcpy(backup_address , address);
    strcpy(backup_address2 , address);
    char *possition_second = strtok(backup_address2 , "--pos");
    printf("%s\n" , possition_second);
    char *temp;
    while(possition_second != NULL){
        temp = possition_second;
        possition_second = strtok(NULL , "--pos");
    }
    pos1 = temp[1];
    pos2 = temp[3];
}

void directory(char *dir1 , char **dir2){
    int i = 0  , flag = 0; 
    len = 0 ;
    Have_Space = 0;
    dash = 0;
    char c;
    scanf("%c" , &c);
    if(c != '\"' ) {
        Have_Space = 1;
    }
    while(flag == 0){
        scanf("%c" , &c);
        if(c != '\"' && c != '\n' && c != '-'){
            dir1[i] = c; 
            dir1[i + 1] = '\0';
            i++ ; 
        }
        if(Have_Space == 1 && c == ' '){
            dir1[i] = '\0' ; 
            flag = 1;
        }
        if(c == '-'){
            if(dir1[i - 1] == ' '){
                dash = 1;
                dir1[i - 1] = '\0';
            } 
            flag = 1;
        }
        if(c == '\n') {
            flag=1;
        }
    }
    char *token = strtok(dir1 , "/");
    int x = 0;
    while(token != NULL){
        dir2[x] = token;
        x++;
        len++;
        token = strtok(NULL , "/");
    }
}

void go_to_root(){
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    char* token1 = strtok(buff, " / ");
    while(token1 != NULL) {
        token1 = strtok(NULL, " / ");
        if(strcmp(token1 , "root") == 0){
            while(token1 != NULL) {
                chdir("..");
                token1 = strtok(NULL, "/");
            }
        }
    }
}

void go_to_directory(char **dir){
    for(int i = 0 ; i < len - 1 ; i++){
        printf("dir[%i] : %s\n" , i,dir[i]);
        const int a = chdir(dir[i]);
        if(a == 0){
            printf("raft to %s\n" , dir[i]);
            chdir(dir[i]);
        }
        else{
            printf("There is not such a directory or file\n");
            break;
        }
    }
}

void remove_file(){
    char dir1[1000];
    char *dir2[100];
    char posstr[20] = "12345678";
    char matn[10000];
    char temp;
    getchar();
    directory(dir1 , dir2);
    go_to_root();
    go_to_directory(dir2);
    printf("chera khara shod yeho\n");
    if(Have_Space == 0){
        posstr[0] = '-';
        int i;
        for(i = 1 ; ; i++){
            posstr[i] = getchar();
            if(posstr[i] == ' ') break;
        }
        posstr[i] = '\0';
    }
    else{
        scanf("%s" , posstr);
    }
    printf("posstr is : %s\n" , posstr);
    printf("1");
    if(strcmp(posstr , "--pos") == 0){
        printf("1");
        scanf("%d:%d" , &pos_line , &pos_char);
        printf("1");
        char sizestr[20];
        scanf("%s" , sizestr);
        printf("1");
        if(strcmp(sizestr , "-size") == 0){
            printf("1");
            scanf("%d" , &size);
            getchar();
            printf("1");
            scanf("-%c" , &bORf);
            if(size < 1 || (bORf != 'b' && bORf != 'f')){
                printf("Invalid command\n");
                return;
            }
            printf("1");
            // printf("pos %d:%d & size is : %d %c\n" , pos_line , pos_char , size , bORf);
            cursor(dir2[len - 1] , pos_line , pos_char , size);
            // printf("first cursor : %d\nsecond cursor : %d\n", first_cur ,second_cur);
            if(first_cur < 0){ 
                size = size + first_cur;
                first_cur = 0;
            }
            FILE * first;
            first = fopen(dir2[len -1] , "r");
            if(first == NULL){
                printf("File does not exist\n");
                return;
            }
            int a = count_char_word(dir2[len - 1] , pos_line , pos_char);
            if(a == -1){
                printf("Wrong position\n");
                return;
            }
            FILE * second;
            second = fopen("nemidoonam2" , "w");
            int shomaresh = 0;
            int shomaresh2 = 0;
            int c;
            while(shomaresh < first_cur){
                c = fgetc(first);
                // printf("first cs are : %c\n" , c);
                fputc(c , second);
                shomaresh++;
            }
            while(shomaresh2 < size){
                c = fgetc(first);
                // printf("rmv cs are : %c\n" , c);
                shomaresh2++;
            }
            while(1){
                c = fgetc(first);
                // printf("second cs are : %c\n" , c);
                if(c == EOF) break;
                fputc(c , second);
                shomaresh++;
            }
            fclose(first);
            fclose(second);
            first = fopen(dir2[len -1] , "w");
            second = fopen("nemidoonam2" , "r");
            c = fgetc(second);
            // printf("filechar is : %c\n" , fileChar);
            while (c != EOF)
            {
                // printf("filechar is : %c\n" , fileChar);
                fputc(c,first);
                c = fgetc(second);
            }
            fclose(first);
            fclose(second);
            remove("nemidoonam2");

        }
        else{
            printf("Invalid command\n");
            return;
        }
    }
    else{
        printf("Invalid command\n");
        return;
    }
}

void cursor(char *file_name , int pos1 , int pos2 , int size ){
     FILE* fp;

    char filename[FILENAME_MAX];

    char c ;
    char q;
    int counter = 0;
  
    fp = fopen(file_name, "r");
    int i = 1 , j =0;
    while(i <= pos1 || j < pos2){
        c = fgetc(fp);
        // printf("i is : %d\nj is : %d\n" , i , j);
        if(i == pos1){
            if(j> pos2){
                break;
            }
        }
        if (c == '\n' || c == '\0'){
            i++;
            j = 0;
        }
        j++;
        counter++;
    }
    // counter--;
    fclose(fp);
    if(bORf == 'f'){
        first_cur = counter;
        second_cur = counter + size;
    }
    else if(bORf == 'b'){
        first_cur = counter - size;
        second_cur = counter;
    }
}

void copy_file(){
    char dir1[1000];
    char *dir2[100];
    char posstr[20] = "12345678";
    char matn[10000];
    char temp;
    getchar();
    directory(dir1 , dir2);
    go_to_root();
    go_to_directory(dir2);
    if(Have_Space == 0){
        posstr[0] = '-';
        int i;
        for(i = 1 ; ; i++){
            posstr[i] = getchar();
            if(posstr[i] == ' ') break;
        }
        posstr[i] = '\0';
    }
    else{
        scanf("%s" , posstr);
    }
    if(strcmp(posstr , "--pos") == 0){
        scanf("%d:%d" , &pos_line , &pos_char);
        char sizestr[20];
        scanf("%s" , sizestr);
        if(strcmp(sizestr , "-size") == 0){
            scanf("%d" , &size);
            getchar();
            scanf("-%c" , &bORf);
            if(size < 1 || (bORf != 'b' && bORf != 'f')){
                printf("Invalid command\n");
                return;
            }
            // printf("pos %d:%d & size is : %d %c\n" , pos_line , pos_char , size , bORf);
            cursor(dir2[len - 1] , pos_line , pos_char , size);
            // printf("first cursor : %d\nsecond cursor : %d\n", first_cur ,second_cur);
            if(first_cur < 0){ 
                size = size + first_cur;
                first_cur = 0;
            }
            
            FILE * first;
            first = fopen(dir2[len -1] , "r");
            if(first == NULL){
                printf("File does not exist\n");
                return;
            }
            int a = count_char_word(dir2[len - 1] , pos_line , pos_char);
            if(a == -1){
                printf("Wrong position\n");
                return;
            }
            fseek(first, 0, SEEK_END); 
            int size_of_file = ftell(first); 
            fseek(first, 0, SEEK_SET); 
            // printf("size of file is : %d\n" , size_of_file);
            clipboard = (char *)malloc(size_of_file*sizeof(char));
            int shomaresh = 0;
            int shomaresh2 = 0;
            int c;
            while(shomaresh < first_cur){
                c = fgetc(first);
                // printf("first cs are : %c\n" , c);
                shomaresh++;
            }
            while(shomaresh2 < size){
                c = fgetc(first);
                clipboard[shomaresh2] = c;
                shomaresh2++;
            }
            size_clipboard = shomaresh2;
            printf("your clipboard is : %s\n" , clipboard);
            fclose(first);
        }
        else{
            printf("Invalid command\n");
            return;
        }
    }
    else{
        printf("Invalid command\n");
        return;
    }
}

void cut() {
    char dir1[1000];
    char *dir2[100];
    char posstr[20] = "12345678";
    char matn[10000];
    char temp;
    getchar();
    directory(dir1 , dir2);
    go_to_root();
    go_to_directory(dir2);
    if(Have_Space == 0){
        posstr[0] = '-';
        int i;
        for(i = 1 ; ; i++){
            posstr[i] = getchar();
            if(posstr[i] == ' ') break;
        }
        posstr[i] = '\0';
    }
    else{
        scanf("%s" , posstr);
    }
    if(strcmp(posstr , "--pos") == 0){
        scanf("%d:%d" , &pos_line , &pos_char);
        char sizestr[20];
        scanf("%s" , sizestr);
        if(strcmp(sizestr , "-size") == 0){
            scanf("%d" , &size);
            getchar();
            scanf("-%c" , &bORf);
            if(size < 1 || (bORf != 'b' && bORf != 'f')){
                printf("Invalid command\n");
                return;
            }
            // printf("pos %d:%d & size is : %d %c\n" , pos_line , pos_char , size , bORf);
            cursor(dir2[len - 1] , pos_line , pos_char , size);
            // printf("first cursor : %d\nsecond cursor : %d\n", first_cur ,second_cur);
            if(first_cur < 0){ 
                size = size + first_cur;
                first_cur = 0;
            }
            FILE * first;
            first = fopen(dir2[len -1] , "r");
            if(first == NULL){
                printf("File does not exist\n");
                return;
            }
            int a = count_char_word(dir2[len - 1] , pos_line , pos_char);
            if(a == -1){
                printf("Wrong position\n");
                return;
            }
            fseek(first, 0, SEEK_END); 
            int size_of_file = ftell(first); 
            fseek(first, 0, SEEK_SET); 
            // printf("size of file is : %d\n" , size_of_file);
            clipboard = (char *)malloc(size_of_file*sizeof(char));
            FILE * second;
            second = fopen("nemidoonam2" , "w");
            int shomaresh = 0;
            int shomaresh2 = 0;
            int c;
            while(shomaresh < first_cur){
                c = fgetc(first);
                // printf("first cs are : %c\n" , c);
                fputc(c , second);
                shomaresh++;
            }
            while(shomaresh2 < size){
                c = fgetc(first);
                clipboard[shomaresh2] = c;
                shomaresh2++;
            }
            size_clipboard = shomaresh2;
            while(1){
                c = fgetc(first);
                // printf("second cs are : %c\n" , c);
                if(c == EOF) break;
                fputc(c , second);
                shomaresh++;
            }
            fclose(first);
            fclose(second);
            printf("your clipboard is : %s\n" , clipboard);
            first = fopen(dir2[len -1] , "w");
            second = fopen("nemidoonam2" , "r");
            c = fgetc(second);
            // printf("filechar is : %c\n" , fileChar);
            while (c != EOF)
            {
                // printf("filechar is : %c\n" , fileChar);
                fputc(c,first);
                c = fgetc(second);
            }
            fclose(first);
            fclose(second);
            remove("nemidoonam2");

        }
        else{
            printf("Invalid command\n");
            return;
        }
    }
    else{
        printf("Invalid command\n");
        return;
    }
}

void paste(){
    char dir1[1000];
    char *dir2[100];
    char posstr[20] = "12345678";
    char matn[10000];
    char temp;
    getchar();
    directory(dir1 , dir2);
    go_to_root();
    go_to_directory(dir2);
    if(Have_Space == 0){
        posstr[0] = '-';
        int i;
        for(i = 1 ; ; i++){
            posstr[i] = getchar();
            if(posstr[i] == ' ') break;
        }
        posstr[i] = '\0';
    }
    else{
        scanf("%s" , posstr);
    }
    if(strcmp(posstr , "--pos") == 0){
        scanf("%d:%d" , &pos_line , &pos_char);
        FILE * fPtr; 
        printf("filename is : %s\n" , dir2[len - 1]);
        fPtr = fopen(dir2[len - 1] , "r");
        if(fPtr == NULL){
            printf("File does not exist\n");
            return;
        }
        else{
            int a = count_char_word(dir2[len - 1] , pos_line , pos_char);
            if(a == -1){
                printf("position does not exist\n");
                return;
            }
            else if(a == 1){
                FILE * newFile;
                newFile = fopen("nemidoonam" , "w");
                int row = 0 , character = 0;
                int fileChar;
                // printf("pos line is : %d\ncounter new line is : %d\n" , pos_line , counter_new_LINE);
                while (row < pos_line - 1){
                    fileChar = fgetc(fPtr);
                    // printf("filechar is : %c\n" , fileChar);
                    fputc(fileChar , newFile);
                    if(fileChar == '\n') row++;
                }

                while (character < pos_char ){
                    fileChar = fgetc(fPtr);
                    // printf("filechar is : %c\n" , fileChar);
                    fputc(fileChar , newFile);
                    character++;
                }
                int shomar = 0;
                while(shomar < size_clipboard){
                    fputc(clipboard[shomar] , newFile);
                    shomar++;
                }
                while(fileChar != EOF){
                    fileChar = fgetc(fPtr);
                    if(fileChar == EOF) break;
                    fputc(fileChar , newFile);
                }
                fclose(newFile);
                fclose(fPtr);
                fPtr = fopen(dir2[len - 1] , "w");
                newFile = fopen("nemidoonam" , "r");
                fileChar = fgetc(newFile);
                // printf("filechar is : %c\n" , fileChar);
                while (fileChar != EOF)
                {
                    // printf("filechar is : %c\n" , fileChar);
                    fputc(fileChar,fPtr);
                    fileChar = fgetc(newFile);
                }
                fclose(newFile);
                fclose(fPtr);
                remove("nemidoonam");
            }
        }

    }
    else{
        printf("Invalid command\n");
        return;
    }
}


void compare(){
    char dir_file1_1[1000];
    char dir_file2_1[1000];
    int line_first_file;
    int line_second_file;
    char *dir_file1_2[100];
    char *dir_file2_2[100];
    // getchar();
    dir_compare(dir_file1_1 , dir_file1_2);
    printf("chera\n");
    for(int i = 0 ; i < len  ; i++)
        printf("%s\n" , dir_file1_2[i]);
    go_to_root();
    go_to_directory(dir_file1_2);
    say_my_dir();
    count_char_word(dir_file1_2[len - 1] , 0 , 0);
    line_first_file = counter_new_LINE;
    printf("%d\n" , line_first_file);
    FILE * first;
    first = fopen(dir_file1_2[len - 1] , "r");
    if(first == NULL){
        printf("File does not exist\n");
        return;
    }
    printf("file opened\n");
    dir_compare(dir_file2_1 , dir_file2_2);
    for(int i = 0 ; i < len ; i++)
        printf("%s\n" , dir_file2_2[i]);
    go_to_root();
    go_to_directory(dir_file2_2);
    say_my_dir();
    count_char_word(dir_file2_2[len - 1] , 0 , 0);
    printf("your file name is : %s\n" , dir_file2_2[len - 1]);
    line_second_file = counter_new_LINE;
    // printf("%d\n" , line_second_file);
    FILE * second;
    second = fopen(dir_file2_2[len - 1] , "r");
    if(second == NULL){
        printf("File does not exist\n");
        return;
    }
    printf("file opened\n");
    fseek(first, 0, SEEK_END); 
    int size_of_file1 = ftell(first); 
    fseek(first, 0, SEEK_SET); 
    char *first_file = (char *)malloc(size_of_file1*sizeof(char));
    fseek(second, 0, SEEK_END); 
    int size_of_file2 = ftell(second); 
    fseek(second, 0, SEEK_SET); 
    char *second_file = (char *)malloc(size_of_file2*sizeof(char));
    int x;
    if(line_first_file > line_second_file){
        for(int i = 0 ; i < line_second_file ; i++){
            fgets(first_file , size_of_file1 , first);
            fgets(second_file , size_of_file2 , second);
            if(strcmp(first_file , second_file)) {
                printf("============= #%d =============\n" , i+1);
                printf("line %d of first file is : %sline %d of second file is : %s" ,i+1 ,first_file,i+1 , second_file);
            }
            x = i;
        }
        printf("\n");
        x++;
        for(int i = x ; i < line_first_file ; i++){
            if(i == x) {
                printf("========== #%d -- #%d ==========\n" , x+1 , line_first_file);
                printf("lines %d to %d of the first file are :\n" , x+1 , line_first_file);
            }
            fgets(first_file , size_of_file1 , first);
            printf("%s" , first_file);
        }
        printf("\n");
    }
    else if(line_first_file < line_second_file){
        for(int i = 0 ; i < line_first_file ; i++){
            fgets(first_file , size_of_file1 , first);
            fgets(second_file , size_of_file2 , second);
            if(strcmp(first_file , second_file)) {
                printf("============= #%d =============\n" , i+1);
                if(i == line_first_file - 1)
                    printf("line %d of first file is : %s\nline %d of second file is : %s" ,i+1 ,first_file,i+1 , second_file);
                else
                    printf("line %d of first file is : %sline %d of second file is : %s" ,i+1 ,first_file,i+1 , second_file);
            }
            x = i;
        }
        x++;
        for(int i = x ; i < line_second_file ; i++){
            if(i == x) {
                printf("========== #%d -- #%d ==========\n" , x+1 , line_second_file);
                printf("lines %d to %d of the second file are :\n" , x+1 , line_second_file);
            }
            fgets(second_file , size_of_file2 , second);
            printf("%s" , second_file);
        }
        printf("\n");
    }
    else{
        for(int i = 0 ; i < line_second_file ; i++){
            fgets(first_file , size_of_file1 , first);
            fgets(second_file , size_of_file2 , second);
            if(strcmp(first_file , second_file)) {
                printf("============= #%d =============\n" , i+1);
                if(i == line_first_file - 1)
                    printf("line %d of first file is : %s\nline %d of second file is : %s\n" ,i+1 ,first_file,i+1 , second_file);
                else
                    printf("line %d of first file is : %sline %d of second file is : %s" ,i+1 ,first_file,i+1 , second_file);
            }
            x = i;
        }
    }
    fclose(first);
    fclose(second);
}

void say_my_dir(){
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    printf("your current dir is : %s\n" , buff);
}

void dir_compare(char *dir1 , char **dir2){
    int i = 0  , flag = 0; 
    len = 0 ;
    Have_Space = 0;
    char c;
    scanf("%c" , &c);
    printf("c is %c\n" , c);
    if(c == ' ') c = getchar();
    if(c != '\"' ) {
        Have_Space = 1;
    }
    while(1){
        scanf("%c" , &c);
        if(c != '\"' && c != '\n' && c != '-'){
            dir1[i] = c; 
            dir1[i + 1] = '\0';
            i++ ; 
        }
        if(Have_Space == 1 && c == ' '){
            dir1[i] = '\0' ; 
            break;
        }
        if(c == '/'){
            printf("inja %c\n" , dir1[i - 2]);
            if(dir1[i - 2] == ' '){
                if(Have_Space == 1 && dir1[i - 3] == '\"'){
                    dir1[i - 3] = '\0';
                    break;
                }
                else if(Have_Space == 0){
                    dir1[i - 2] = '\0';
                    break;
                }
            } 
        }
        if(c == '/'){
            printf("inja %c\n" , dir1[i - 2]);
            if(dir1[i - 2] == '\"'){
                if(dir1[i - 3] == ' '){
                    if(Have_Space == 1 && dir1[i - 4] == '\"'){
                        dir1[i - 4] = '\0';
                        break;
                    }
                    else if(Have_Space == 0){
                        dir1[i - 4] = '\0';
                        break;
                    }
                } 
            }
        }
        if(c == '\n') {
            break;;
        }
    }
    char *token = strtok(dir1 , "/");
    int x = 0;
    while(token != NULL){
        dir2[x] = token;
        x++;
        len++;
        token = strtok(NULL , "/");
    }
}

// https://codeforwin.org/c-programming/c-program-to-list-all-files-in-a-directory-recursively
void tree(char *basePath, const int root , int depth)
{  
    int x = ceil(root/2);
    if(x >= depth){
      return ;
    }
    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            for (i=0; i<root; i++) 
            {
                if (i%2 == 0 || i == 0)
                    printf("|");
                else
                    printf(" ");

            }

            printf("|~%s\n", dp->d_name);

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            tree(path, root + 2 , depth);
        }
    }

    closedir(dir);
    return;
}

void tree_all(char *basePath, const int root){

    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            for (i=0; i<root; i++) 
            {
                if (i%2 == 0 || i == 0)
                    printf("|");
                else
                    printf(" ");

            }

            printf("└──%s\n", dp->d_name);

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            tree_all(path, root + 2);
        }
    }

    closedir(dir);
}
void grep(){
    char str[10];
    int flag = 0;
    scanf("%s" , str);
    if(strcmp(str , "--str") == 0) flag = 1;
    if(flag == 0){
        if(strcmp("-c" , str) == 0) {
            cORI = 'c';
            scanf("%s" , str);
        }
        else if(strcmp("-i" , str) == 0) {
            cORI = 'i';
            scanf("%s" , str);
        }
        else {
            gets(str);
            printf("Invalid command\n");
            return;
        }
        if(strcmp(str , "--str")){
            gets(str);
            printf("Invalid command\n");
            return;
        }
    }
    char temp;
    char matn[1000];
    getchar();
    temp = getchar();
    // printf("temp is : %c\n" , temp);
    if(temp != '\"'){
        Have_Space = 1;
        matn[0] = temp;
        temp = getchar();
        int x = 1;
        while(temp != ' '){
            matn[x] = temp;
            temp = getchar();
            x++;
        }
    }
    else{
        Have_Space = 0;
        scanf("%[^-]s" , matn);
        matn[strlen(matn) - 2] = '\0';

    }
    printf("your text is : %s\n" , matn);
    char c;
    char file[10];
    scanf("%s" , file);
    // if(Have_Space == 1){
    //     for(int i = 0 ; i < 7 ; i++){
    //         file[i] = file[i+1];
    //         file[i+1] = '\0';
    //     }
    // }
    // printf("filestr is : %s\n" , file);
    if(strcmp(file, "--files") != 0){
        printf("invalid command!\n");
        char chert[1000];
        gets(chert);
        return;
    }
    getchar();
    c = getchar();
    char address[1000];
    counters = 0;
    while(c != '\n'){
        if(c == '\"'){
            scanf("%[^\"]s" , address);
            getchar();
            c = getchar();
        }
        else if(c == '/'){
            scanf("%s" , address);
            c = getchar();
        }
        else{
            c = getchar();
            continue;
        }
        print_grep(address , flag , matn);
    }
    if((flag == 0)){
        if(cORI == 'c'){
            printf("The number of lines which include the text is : %d\n" , counters);
        }
    }
}

void print_grep(char *address , int flag , char *matn){
    char backup_address1[1000];
    char backup_address2[1000];
    strcpy(backup_address1 , address);
    strcpy(backup_address2 , address);
    int count_add = 0;
    char *token2 = strtok(backup_address1 , "/");
    while(token2 != NULL){
        count_add++;
        token2 = strtok(NULL , "/");
    }
    go_to_root();
    char nameoffile[1000];
    char * token = strtok(backup_address2 , "/");
    for(int i = 0 ; i < count_add - 1 ; i++){
        const int a = chdir(token);
        if(a == 0){
            chdir(token);
            token = strtok(NULL , "/");
        }
        else{
            printf("Directory doesn't exist\n");
            return;
        }
    }
    strcpy(nameoffile , token);
    FILE * first;
    first = fopen(token , "r");
    if(first == NULL){
        printf("File «%s» doesn't exist\n" , nameoffile);
        return ;
    }
    if(flag == 1){
        count_char_word(token , 0 , 0);
        char line[counter_new_LINE][1] ;
        for(int i = 0 ; i < counter_new_LINE ; i++){
            line[i][0] = 0;
        }
        char* c;
        char data[1000];
        c = fgets(data , 1000 , first);
        while(c != NULL){
            if(strstr(data , matn) != NULL){
                printf("%s : %s" , nameoffile , data);
            }
            c = fgets(data , 1000 , first);
        }
        printf("\n");
    }
    else{
        if(cORI == 'c'){
            count_char_word(token , 0 , 0);
            char line[counter_new_LINE][1] ;
            for(int i = 0 ; i < counter_new_LINE ; i++){
                line[i][0] = 0;
            }
            char* c;
            char data[1000];
            c = fgets(data , 1000 , first);
            while(c != NULL){
                if(strstr(data , matn) != NULL){
                    counters++;
                }
                c = fgets(data , 1000 , first);
            }
        }
        else if (cORI == 'i'){
            count_char_word(token , 0 , 0);
            char line[counter_new_LINE][1] ;
            for(int i = 0 ; i < counter_new_LINE ; i++){
                line[i][0] = 0;
            }
            char* c;
            char data[1000];
            c = fgets(data , 1000 , first);
            while(c != NULL){
                if(strstr(data , matn) != NULL){
                    printf("%s\n" ,nameoffile);
                    break;
                }
                c = fgets(data , 1000 , first);
            }
        }
        
    }
    fclose(first);
}

void dir_grep(char c){
    int i = 0  , flag = 0; 
    len = 0 ;
    Have_Space = 0;
    scanf("%c" , &c);
    if(c == ' ') c = getchar();
    if(c != '\"' ) {
        Have_Space = 1;
    }
    while(flag == 0){
        scanf("%c" , &c);
        if(c != '\"' && c != '\n'){

        }
    }
}

void find(){
    char str[10];
    char matn[1000];
    scanf("%s" , str);
    if(strcmp(str , "--str") != 0){
        char chert[1000];
        gets(chert);
        printf("Invalid command\n");
        return;
    }
    else{
        getchar();
        char c = getchar();
        if(c != '\"'){
            Have_Space = 1;
            matn[0] = c;
            c = getchar();
            int x = 1;
            while(c != ' '){
                matn[x] = c;
                c = getchar();
                x++;
            }
        }
        else{
            Have_Space = 0;
            int x = 0;
            while(1){
                scanf("%c" , &matn[x]);
                if(matn[x] == '-'){
                    if(matn[x-1] == ' '){
                        if(matn[x-2] == '\"'){
                            matn[x-2] = '\0';
                            break;
                        }
                    }
                }
                x++;
            }           
        }
        char filestr[20] ="1234567890";
        if(Have_Space == 0){
            filestr[0] = '-';
            int i;
            for(i = 1 ; ; i++){
                filestr[i] = getchar();
                // printf("posstr[%d] is : %c\n" , i , posstr[i]);
                if(filestr[i] == ' ') break;
            }
            filestr[i] = '\0';
        }
        else{
            scanf("%s" , filestr);
        }
        if(strcmp(filestr , "--file")){
            char chert[1000];
            gets(chert);
            printf("Invalid command\n");
            return;
        }
        else{
            char dir1[1000];
            char *dir2[100];
            directory(dir1 , dir2);
            for(int i = 0 ; i < len ; i++)
                printf("/%s" , dir2[i]);
            printf("\n");
            int find_flag = 0;
            int at ;
            if(dash == 1){
                char option[10];
                scanf("%s" , option);
                if(strcmp("count" , option) == 0) find_flag = 1;
                else if(strcmp("at" , option) == 0) find_flag = 2;
                else if(strcmp("byword" , option) == 0) find_flag = 3;
            }
        }
    }
}

void auto_indent(){
    getchar();
    char add[1000];
    scanf("%[^\n]s" , add);
    if(add[0] == '\"'){
        int i ;
        for( i = 0 ; add[i+1] != NULL ; i++){
            add[i] = add[i+1];
            add[i+1] = '\0';
        }
        add[i-1] = '\0';
    }
    char *t;
    int counter_address = 0;
    char backup_address[10000];
    strcpy(backup_address , add);
    char *temp3 = strtok(backup_address, "/");
    while(temp3 != NULL){
        counter_address++;
        temp3  = strtok(NULL, "/");
    }
    go_to_root();
    char *token = strtok(add, "/");
    for(int i = 0 ; i < counter_address - 1 ;i++){
        int a = chdir(token);
        if(a != 0){
            printf("wrong address\n");
            return;
        }
        chdir(token);
        t = token;
        printf("filename is %s\n" ,token);
        token = strtok(NULL , "/");
        printf("filename is %s\n" ,token);
    }
    // token = strtok(NULL , "/");
    printf("counter is %d\n" , counter_address);
    printf("filename is %s\n" ,token);

    FILE * fptr;
    fptr = fopen( token , "r");
    if(fptr == NULL){
        printf("file does not exist\n");
        return;
    }
    FILE * second;
    second = fopen("a" , "w");
    char c = getc(fptr);
    while(c == ' ') c = getc(fptr);
    while(c != EOF && c != NULL && c != '\n'){
        if(c == '{'){
            aculad++;
            int x = aculad - 1;
            if(t == '{' || t =='}'){
                fputc('\n' , second);
                for(int z = 0; z < x;z++){
                    fputs("    " , second);
                }
            }
            else{
                fputc(' ' , second);
            }
            fputc('{' , second);
            t = c;
            c = getc(fptr);
            if(c != '{' && c != '}'){
                fputc('\n' , second);
                for(int z = 0; z < x+1;z++){
                    fputs("    " , second);
                }
            }
        }
        else if(c == '}'){
            fputc('\n' , second);
            aculad--;
            if(aculad < 0){
                printf("invalid\n");
                char chert[1000];
                gets(chert);
                return;
            }
            int x = aculad;
            for(int z = 0; z < x;z++){
                fputs("    " , second);
            }
            fputc('}' , second);
            t = c;
            c = getc(fptr);
            if(c != '{' && c != '}'){
                fputc('\n' , second);
                for(int z = 0; z < x;z++){
                    fputs("    " , second);
                }
            }
        }
        else{
            fputc(c , second);
            t = c;
            c = getc(fptr);
        }
    }
    fputc('\n' , second);
    fclose(fptr);
    fclose(second);
    if(aculad != 0){
        printf("invalid number of {}\n");
        remove("a");
        return;
    }
    fptr = fopen( token , "w");
    second = fopen("a" , "r");
    c = fgetc(second);
    while (c != EOF)
    {
        // printf("filechar is : %c\n" , fileChar);
        fputc(c,fptr);
        c = fgetc(second);
    }
    fclose(second);
    fclose(fptr);
    remove("a");
}
