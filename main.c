#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *inputFile;
int framesCount;
int *framesArray;
int *goblinArray;
int *useArray;
int pageFaultCount = 0;
char policyName[5];
int *referencedPages;
int referencedPagesCount = 0;

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
    for(int i = 0; i < referencedPagesCount; i++)
    {
        pageNumber = referencedPages[i];
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

    }
}

void updateGoblinArray(int index, int referenceNumber)
{
    goblinArray[index] = referenceNumber;
}
int LRUPageIndex()
{
    int min = 10000000, minIndex;
    for(int i = 0; i< framesCount; i++)
    {
        if(goblinArray[i] < min)
        {
            min = goblinArray[i];
            minIndex = i;
        }
    }
    return minIndex;
}
void startLRU()
{
    int pageNumber, nextAvilableFrameIndex = 0, referenceNumber = 0;
    for(int i = 0; i < referencedPagesCount; i++)
    {
        pageNumber = referencedPages[i];
        int isPageAvilable = isFramesArrayContainsPage(pageNumber);
        if(isPageAvilable != -1)
        {
            updateGoblinArray(isPageAvilable, referenceNumber++);
            printf("%02d     ",pageNumber);
        }
        else
        {
            if(nextAvilableFrameIndex < framesCount)
            {
                framesArray[nextAvilableFrameIndex] = pageNumber;
                updateGoblinArray(nextAvilableFrameIndex++, referenceNumber++);
                printf("%02d     ",pageNumber);
            }
            else
            {
                int index = LRUPageIndex();
                framesArray[index] = pageNumber;
                updateGoblinArray(index, referenceNumber++);
                pageFaultCount++;
                printf("%02d F   ",pageNumber);
            }

        }
        printFramesContent();
    }
}

void startCLOCK()
{
    int pageNumber, useArrayIndex = 0;
    for(int i = 0; i < referencedPagesCount; i++)
    {
        pageNumber = referencedPages[i];
        int isPageAvilable = isFramesArrayContainsPage(pageNumber);
        if(isPageAvilable != -1)
        {
            useArray[isPageAvilable] = 1;

            printf("%02d     ",pageNumber);
        }
        else
        {

            while(useArray[useArrayIndex % framesCount] != 0)
            {
                useArray[useArrayIndex % framesCount] = 0;
                useArrayIndex++;
            }
            framesArray[useArrayIndex % framesCount] = pageNumber;
            useArray[useArrayIndex % framesCount] = 1;
            useArrayIndex++;
            if(useArrayIndex > framesCount)
            {
                printf("%02d F   ",pageNumber);
                pageFaultCount++;
            }
            else printf("%02d     ",pageNumber);

        }
        printFramesContent();
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

    scanf("%d",&framesCount);
    scanf("%s", policyName);
    framesArray = (int*) calloc(framesCount,sizeof(int));
    goblinArray = (int*) calloc(framesCount,sizeof(int));
    useArray = (int*) calloc(framesCount,sizeof(int));
    initializeByNegativeOne();
    referencedPages = (int*) calloc(1,sizeof(int));
    int *temp;
    scanf("%d",&referencedPages[referencedPagesCount]);
    referencedPagesCount++;
    while(referencedPages[referencedPagesCount - 1] != -1)
    {
        temp = realloc(referencedPages, (referencedPagesCount + 1) *sizeof(int));
        referencedPages = temp;
        scanf("%d",&referencedPages[referencedPagesCount]);
        referencedPagesCount++;
    }
    referencedPagesCount--;
    printf("Replacement Policy = %s\n",policyName);
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    if(strcmp(policyName, "FIFO") == 0)
        startFIFO();

    else if (strcmp(policyName, "LRU") == 0)
        startLRU();
    else
        startCLOCK();

    printf("-------------------------------------\n");
    printf("Number of page faults = %d",pageFaultCount);

}

int main()
{
    startPolicy();
    return 0;
}
