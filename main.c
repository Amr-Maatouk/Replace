#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 500
int optArr[4] = { 0, 0, 0, 0 };
char* optArrChar[4] = { "-b", "-f", "-l", "-i" };
int countReplace = 0;

/**
 * Check if "opt" argument is an option or not and return the equivalent number of the option "
 * @param type:string opt in a string to be investigated.
 * return int for the equivalent option if valid, -1 otherwise.
 */
int isOption(const char* opt);
/**
 * Check the number of occurrence for "word" string in "source" string and return the number of occurrence
 * @param type:string word a string to look for.
 * @param type:string source a string to look into.
 * return int "the number of occurrence".
 */
int numOfOccurrence(char* word, const char* source);
/**
 * Replace "oldW" with "newW" from "oldSrc" to "newSrc".
 * @param type:string oldW a string to look for.
 * @param type:string newW a string to be replaced with.
 * @param type:string oldSrc a string to look into.
 * @param type:string newSrc a string to hold the new string.
 * @param type:int numOfFound "the number of occurrence".
 */
void replace(const char* oldW, const char* newW, const char* oldSrc, const char* newSrc, int numOfFound);
/**
 * Find string "s1" in string "s2".
 * @param type:string s1 a string to look into.
 * @param type:string s2 a string to look for.
 * return char*  to the position of "s2" in "s1" or NULL otherwise.
 */
char* stristr(char *s1, char *s2);

int main(int argc, char *argv[]) {
    int optState, fromPos; //optState: temp file to hold return value of isOption, fromPos: counter to go through argv[].
    const char* from, *to; //from: string to be replaced with string to, to: string that will replace string from.
    int numOfFiles = 0; //Number of files passed as arguments.
    FILE* sourceFile; //pointer to thecurrent source file.
    FILE* desFile; // pointer to the current destination file.
    char desFileName[20] = "Replaced.txt"; //Name of current destination file.
    char** src; //Array of strings (pointers to pointers) to hold source files names.
    char oldBuffer[BUFFER_SIZE]; //buffer to load data from source files
    char* newBuffer; //buffer holds the new data to be written in the new file.
    int srcFound = 0; //if there is any source files passed to the main function.
    int i, cnt;

    //check if the number of arguments is enough for the required arguments.
    if(argc < 5){
        printf("[ERROR]: Less Arguments Than Expected.\n");
        printf("[ERROR]: Use Replace.exe OPT FROM TO -- FILE SOURCE.\n");
        return -1;
    }
    //Display Arguments.
    else{
        for(i=1;i<argc;i++){
            printf("Arg[%d]: %s, ", i, argv[i]);
        }
        printf(".\n");
    }

    //check options in arguments.
    for (i = 1; i < argc; i++) {
        optState = isOption(argv[i]);
        if (optState >= 0) {
            optArr[optState] = 1;
        }
        else {
            fromPos = i;
            break;
        }
    }

    //Display options.
    for (i = 0; i < 4; i++) {
        if (optArr[i] == 1) {
            printf("Option %s enabled, %d. \n", optArrChar[i], i);
        }
    }

    if(fromPos >= argc){
        return -1;
    }
    from = argv[fromPos++];
    printf("From: %s\n", from);

    if(fromPos >= argc){
        return -1;
    }
    to = argv[fromPos++];
    printf("To: %s\n", to);

    if(fromPos >= argc){
        return -1;
    }
    //Look for "--" before fileNames.
    while(fromPos < argc){
        if(!strcmp(argv[fromPos++], "--")){
            srcFound = 1;
            break;
        }
    }

    //Check if there is at least one fileName.
    if(!srcFound){
        printf("[ERROR]: No File Name Found!\n");
        printf("[ERROR]: Enter -- before File Name.\n");
        return -1;
    }
    //Calculate number of files.
    numOfFiles = argc - fromPos;
    printf("%d source files\n", numOfFiles);

    src = (char **)malloc(numOfFiles * sizeof(char*));
    for (i = 0; i < numOfFiles; i++) {
        src[i] = argv[fromPos++];
        printf("src[%d]: %s\n", i, src[i]);
    }

    for(i=0 ; i < numOfFiles; i++){
        //open current File.
        sourceFile = fopen(src[i],"r+");
        //Check if File open successfully.
        if(sourceFile == NULL){
            printf("[ERROR]: Can Not Open Source File: %s, File Doesn't Exist!\n", src[i]);
            continue;
        }
        //Calculate Number of Occurance in the file
        cnt = 0;
        while(1){
            fgets(oldBuffer, BUFFER_SIZE, sourceFile);
            if(feof(sourceFile))
                break;
            cnt += numOfOccurrence(from, oldBuffer);
        }
        //Replace "From" string to "to" string in oldBudder and write the result to newBuffer
        countReplace =0;
        fseek ( sourceFile , 0 , SEEK_SET );
        sprintf(desFileName, "Replaced %d .txt", i);
        printf("%s\n",desFileName);
        desFile = fopen(desFileName,"w");
        if(desFile == NULL){
            printf("[ERROR]: Can Not Open Destination File: %s, File Doesn't Exist!\n", src[i]);
            continue;
        }
        while(1){
            fgets(oldBuffer, BUFFER_SIZE, sourceFile);
            if(feof(sourceFile))
                break;
            newBuffer = (char*)malloc(strlen(oldBuffer) + cnt * (strlen(to) - strlen(from) + 1) );
            replace(from, to, oldBuffer, newBuffer, cnt);
            printf("Old: %s, Length: %d\n",oldBuffer, strlen(oldBuffer));
            printf("New: %s, Length: %d \nCurrent Pos: %ld\n---------\n",newBuffer, strlen(newBuffer), ftell(sourceFile));

            fprintf(desFile, newBuffer);
            printf("Current Pos: %ld\n", ftell(sourceFile));
            free(newBuffer);
        }

        fflush(sourceFile);
        fflush(desFile);
        fclose(sourceFile);
        fclose(desFile);
        //If "-b" is 0, replace sourceFile with desFile.
        if(optArr[0] == 0){
            printf("trying to remove %s and rename %s\n", src[i], desFileName);
            if(remove(src[i]) == 0){
                printf("%s Removed Successfully\n", src[i]);
            }else{
                printf("Failed to remove &s\n", src[i]);
            }
            if(rename(desFileName, src[i]) == 0){
                printf("%s Renamed Successfully with %s\n", desFileName, src[i]);
            }else{
                printf("Failed to rename &s with %s\n", desFileName, src[i]);
            }
        }
        printf("----------\n");
    }

    free(src);
    return 0;
}

int isOption(const char* opt) {
    int option = -1;

    if (opt[0] == '-') {
        switch (opt[1])
        {
        case 'b':
            option = 0;
            break;

        case 'f':
            option = 1;
            break;

        case 'l':
            option = 2;
            break;

        case 'i':
            option = 3;
            break;

        default:
            option = -1;
            break;
        }
    }
    else {
        return -1;
    }

    return option;

}

int numOfOccurrence(char* word, const char* source) {
    int count = 0;

    char* foundPos = optArr[3] ? stristr(source, word) : strstr(source, word);
    while (foundPos != NULL) {
        count++;
        source = foundPos + strlen(word);
        foundPos = strstr(source, word);

    }
    return count;
}

void replace(const char* oldW, const char* newW, const char* oldSrc, const char* newSrc, int numOfFound) {
    char* foundPos = optArr[3] ? stristr(oldSrc, oldW) : strstr(oldSrc, oldW);
    int oldWLen = strlen(oldW);
    int newWLen = strlen(newW);
    strcpy(newSrc, "");

    while (foundPos != NULL) {
        countReplace++;
        printf("countReplace: %d, numOfFound: %d, optArr[2]: %d\n", countReplace, numOfFound, optArr[2]);
        if((countReplace > 1) && optArr[1]){break;}
        else if ( ( (countReplace < numOfFound) && (optArr[2] == 1) ) && !((countReplace > 1) && optArr[1]) ){
            strcpy(newSrc,oldSrc);
            oldSrc = foundPos + oldWLen;
            foundPos = optArr[3] ? stristr(oldSrc, oldW) : strstr(oldSrc, oldW);
            continue;
        }
        printf("Continue Replacing, %d\n", countReplace);
        strncat(newSrc, oldSrc, (strlen(oldSrc) - strlen(foundPos)) );
        strncat(newSrc, newW, newWLen);
        oldSrc = foundPos + oldWLen;
        foundPos = optArr[3] ? stristr(oldSrc, oldW) : strstr(oldSrc, oldW);
    }
    strcat(newSrc, oldSrc);

}

char* stristr(char *s1, char *s2){
    int i,j,k;

    for(i=0;s1[i];i++)
        for(j=i,k=0;tolower(s1[j])==tolower(s2[k]);j++,k++){
            if(!s2[k+1])
                return (s1+i);
        }


    return NULL;
}
