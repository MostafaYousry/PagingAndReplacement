#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *inputFile;
int framesCount;
int *framesArray;
int *goblinArray;
int pageFaultCount = 0;
char policyName[5];

int isFramesArrayContainsPage(int pageNumber)
{
    for(int i = 0; i < framesCount; i++)
    {
        if(framesArray[i] == pageNumber)
            return i;
    }
    return -1;
}

void printFramesContent()
{
    for(int i = 0; i < framesCount; i++)
    {
        if(framesArray[i] != -1)
            printf("%02d ",framesArray[i]);
    }
    printf("\n");
}

void startFIFO()
{
    int index = 0;
    int pageNumber;
    //scanf("%d", &pageNumber);
    fscanf(inputFile,"%d", &pageNumber);
    while(pageNumber != -1)
    {
        int isPageAvilable = isFramesArrayContainsPage(pageNumber);
        if(isPageAvilable != -1)
        {
            printf("%02d     ",pageNumber);
        }
        else
        {
            if(framesArray[index % framesCount] != -1)
            {
                printf("%02d F   ",pageNumber);
                pageFaultCount++;
            }
            else
                printf("%02d     ",pageNumber);

            framesArray[index++ % framesCount] = pageNumber;
        }
        printFramesContent();
        //scanf("%d", &pageNumber);
        fscanf(inputFile,"%d", &pageNumber);
    }
}

void updateGoblinArray()
{
    for(int i = 0; i < framesCount; i++)
    {
        if(goblinArray[i] != -1)
            goblinArray[i]++;
    }
}
int LRUPageIndex()
{
    int max = -1, maxIndex;
    for(int i = 0; i< framesCount; i++)
    {
        if(goblinArray[i] > max)
        {
            max = goblinArray[i];
            maxIndex = i;
        }
    }
    return maxIndex;
}
void startLRU()
{
    int pageNumber, nextAvilableFrameIndex = 0;
    //scanf("%d", &pageNumber);
    fscanf(inputFile,"%d", &pageNumber);
    while(pageNumber != -1)
    {
        updateGoblinArray();
        int isPageAvilable = isFramesArrayContainsPage(pageNumber);
        if(isPageAvilable != -1)
        {
            goblinArray[isPageAvilable] = 0;
            printf("%02d     ",pageNumber);
        }
        else
        {
            if(nextAvilableFrameIndex < framesCount)
            {
                framesArray[nextAvilableFrameIndex] = pageNumber;
                goblinArray[nextAvilableFrameIndex++] = 0;
                printf("%02d     ",pageNumber);
            }
            else
            {
                int index = LRUPageIndex();
                framesArray[index] = pageNumber;
                goblinArray[index] = 0;
                pageFaultCount++;
                printf("%02d F   ",pageNumber);
            }

        }
        printFramesContent();
        //scanf("%d", &pageNumber);
        fscanf(inputFile,"%d", &pageNumber);
    }
}
void initializeByNegativeOne()
{
    for(int i = 0; i < framesCount; i++)
    {
        framesArray[i] = -1;
        goblinArray[i] = -1;
    }
}

void startPolicy()
{
    inputFile = fopen("inputLRU.txt","r");
    //scanf("%d",&framesCount);
    //scanf("%s", policyName);

    fscanf(inputFile,"%d", &framesCount);
    fscanf(inputFile, "%s", policyName);
    printf("Replacement Policy = %s\n",policyName);
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    framesArray = (int*) calloc(framesCount,sizeof(int));
    goblinArray = (int*) calloc(framesCount,sizeof(int));
    initializeByNegativeOne();
    if(strcmp(policyName, "FIFO") == 0)
        startFIFO();

    else if (strcmp(policyName, "LRU") == 0) {}
          startLRU();
//    else
//        startCLOCK();

    printf("-------------------------------------\n");
    printf("Number of page faults = %d",pageFaultCount);

    fclose(inputFile);
}

int main()
{
    startPolicy();
    return 0;
}
