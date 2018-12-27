#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int framesCount;
int *framesArray;
int *goblinArray;
int *useArray;
int pageFaultCount = 0;
char policyName[5];
int *referencedPages;
int referencedPagesCount = 0;


/// set all the element of the array by -1
void initializeByNegativeOne(int *array)
{
    for(int i = 0; i < framesCount; i++)
    {
        array[i] = -1;
    }
}

/// check if the frames array contain a page and return its index if exists, otherwise return -1
int isFramesArrayContainsPage(int pageNumber)
{
    for(int i = 0; i < framesCount; i++)
    {
        if(framesArray[i] == pageNumber)
            return i;
    }
    return -1;
}

/// print the content of the frames array
void printFramesContent()
{
    for(int i = 0; i < framesCount; i++)
    {
        if(framesArray[i] != -1)
            printf("%02d ",framesArray[i]);
    }
    printf("\n");
}
/// FIFO replacement policy works as follow:
/// . there is a pointer on the page to be replaced
/// . if a page fault happened, this page will be replaced and the pointer will be on the next page in the array
/// . repeat the previous steps in a circular way
/// . count the number of pages faults after all the frames array is filled
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

/// update the goblin array at specific index by the reference number
void updateGoblinArray(int index, int referenceNumber)
{
    goblinArray[index] = referenceNumber;
}

/// get the index of the minimum element in the goblin array so this is the index of the page will be replaced
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

/// LRU replacement policy work as follow:
/// . replace the page that has not been referenced for the longest time
/// . count the number of pages faults after all the frames array is filled
void startLRU()
{
    goblinArray = (int*) calloc(framesCount,sizeof(int));
    initializeByNegativeOne(goblinArray);
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
    free(goblinArray);
}

/// CLOCK replacement policy work as follow:
/// . require the association of an additional bit with each frame (use bit)
/// . when a page is firist loaded, the use bit is set to 1
/// . frames array will be treated as circular array
/// . the page that will be replaced, is the first page whose use bit = 0
/// . when searching for that page if the use bit of a page = 1, set it to 0
/// . if a page is existed in the frame array, set its use bit by 1
/// . count the number of pages faults after all the frames array is filled
void startCLOCK()
{
    useArray = (int*) calloc(framesCount,sizeof(int));
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
    free(useArray);
}

void startPolicy()
{
    /// scan the number of frames and the policy name
    scanf("%d",&framesCount);
    scanf("%s", policyName);
    /// allocate a space for frames array based on the frame then set all its element by -1
    framesArray = (int*) calloc(framesCount,sizeof(int));
    initializeByNegativeOne(framesArray);
    /// read the whole input until the user enters -1
    referencedPages = (int*) calloc(1,sizeof(int));
    int *temp;
    scanf("%d",&referencedPages[referencedPagesCount]);
    referencedPagesCount++;
    while(referencedPages[referencedPagesCount - 1] != -1)
    {
        temp = (int*) realloc(referencedPages, (referencedPagesCount + 1) *sizeof(int));
        referencedPages = temp;
        scanf("%d",&referencedPages[referencedPagesCount]);
        referencedPagesCount++;

    }
    referencedPagesCount--;
    /// print policy name & some of the output lines format then start the replacement policy specified by the user
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


    ///print the number of page faults happened
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n",pageFaultCount);
    free(framesArray);
    free(referencedPages);
    free(temp);
}

int main()
{
    startPolicy();
    return 0;
}
