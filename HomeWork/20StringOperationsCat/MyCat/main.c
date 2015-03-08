#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1000

int main(int argc, char **argv)
{
    FILE *filePointer;
    char *strTemp = NULL;

    strTemp = (char *)malloc(SIZE * sizeof(char));
    if(strTemp == NULL)
    {
        fprintf(stderr, "Memory error.\n");
        exit(1);
    }

    if(argc > 1)
    {
        if(strcmp(argv[1], "--number") == 0 || strcmp(argv[1], "-n") == 0)
        {
            int counter = 1;

            for(int i = 2; argv[i] != NULL; ++i)
            {
                filePointer = fopen(argv[i], "r");
                if(filePointer == NULL)
                {
                    fprintf(stderr, "%s: No such file or directory.\n", argv[i]);
                }
                else
                {
                    while(fgets(strTemp, SIZE * sizeof(strTemp), filePointer))
                    {
                        printf("%6d  %s", counter++, strTemp);
                    }
                    fclose(filePointer);
                }
            }
        }
        else if(strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "--number-nonblank") == 0 || strcmp(argv[1], "-nb") == 0 || strcmp(argv[1], "-bn") == 0)
        {
            int counter = 1;

            for(int i = 2; argv[i] != NULL; ++i)
            {
                filePointer = fopen(argv[i], "r");
                if(filePointer == NULL)
                {
                    fprintf(stderr, "%s: No such file or directory.\n", argv[i]);
                }
                else
                {
                    while(fgets(strTemp, SIZE * sizeof(strTemp), filePointer))
                    {
                        if(strlen(strTemp) > 1)
                        {
                            printf("%6d  %s", counter++, strTemp);
                        }
                        else
                        {
                            printf("%8s%s", "", strTemp);
                        }
                    }
                    fclose(filePointer);
                }
            }
        }
        else if(strcmp(argv[1], "--help") == 0)
        {
            filePointer = fopen("/home/troyan/andrewtroyan/troyandz/HomeWork/20StringOperationsCat/MyCat/CatInstructions.txt", "r");
            if(filePointer == NULL)
            {
                fprintf(stderr, "Access error.\n");
                exit(1);
            }
            while(fgets(strTemp, SIZE * sizeof(strTemp), filePointer))
            {
                printf("%s", strTemp);
            }
            fclose(filePointer);
        }
        else
        {
            for(int i = 1; argv[i] != NULL; ++i)
            {
                filePointer = fopen(argv[i], "r");
                if(filePointer == NULL)
                {
                    fprintf(stderr, "%s: No such file or directory.\n", argv[i]);
                }
                else
                {
                    while(fgets(strTemp, SIZE * sizeof(strTemp), filePointer))
                    {
                        printf("%s", strTemp);
                    }
                    fclose(filePointer);
                }
            }
        }
    }

    free(strTemp);
    strTemp = NULL;
    return 0;
}
