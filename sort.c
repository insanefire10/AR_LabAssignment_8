//Adit Rajkumar
//Professor T
//3_14_2023
//Lab Assignment 8

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

void merger(int pData[], int l, int m, int r)//creates temp arrays for each split, 
{
	int *larray = Alloc((m-l+1)*sizeof(int));//creating 2 new arrays to split
	int *rarray = Alloc((r-m)*sizeof(int));
	for(int i = 0; i < (m-l+1); i++) //filling new arrays with data
	{
		larray[i] = pData[l+i];
	}
	for(int i = 0; i < (r-m); i++)
	{
		rarray[i] = pData[m+i+1];
	}
	int i = 0;
	int j = 0;
	int k = l;
	while(i < (m-l+1) && j < (r-m))//compares values at each split and puts it in the correct place in the main array
	{
		if(larray[i] <= rarray[j])
		{
			pData[k] = larray[i];
			i++;
		}
		else
		{
			pData[k] = rarray[j];
			j++;
		}
		k++;
	}

	while(i < (m-l+1)) //fills rest of data in main array
	{
		pData[k] = larray[i];
		i++;
		k++;
	}

	while(j < (r-m))
	{
		pData[k] = rarray[j];
		j++;
		k++;
	}

	DeAlloc(larray);
	DeAlloc(rarray);

}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	
	if (l < r) {//terminating condition
        int m = l + (r - l) / 2; //"Middle" integer between 0 and n
        mergeSort(pData, l, m); //run merge sort recursively each time splitting the merge into 1/2 and onward
        mergeSort(pData, m + 1, r);
 
        merger(pData, l, m, r);
    }

	

}



// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
    int i, sz = (dataSz > 100 ? dataSz - 100 : 0);
    int firstHundred = (dataSz < 100 ? dataSz : 100);
    printf("\tData:\n\t");
    for (i=0;i<firstHundred;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\t");
    
    for (i=sz;i<dataSz;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}