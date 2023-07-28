#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
	struct RecordType* next; // for chaining in the hash table
};

// HashTable
struct HashType
{
	struct RecordType* head; // pointer to the head of the linked list at a particular index in the hash table
	int count; // number of records at a particular index
};

// Compute the hash function
int hash(int x, int size)
{
	return x % size;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
			pRecord->next = NULL; // initialize next pointer to NULL
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// Insert into HashTable
void insertIntoHashTable(struct HashType* hashTable, int hashSize, struct RecordType* record)
{
	int index = hash(record->id, hashSize);
	record->next = hashTable[index].head;
	hashTable[index].head = record;
	hashTable[index].count++;
}

// Create the hash table
struct HashType* createHashTable(int hashSize, struct RecordType* records, int recordSize)
{
	int i;
	struct HashType* hashTable = (struct HashType*) malloc(sizeof(struct HashType) * hashSize);
	
	for (i = 0; i < hashSize; i++)
	{
		hashTable[i].head = NULL;
		hashTable[i].count = 0;
	}

	for (i = 0; i < recordSize; i++)
	{
		insertIntoHashTable(hashTable, hashSize, &records[i]);
	}

	return hashTable;
}

// display records in the hash structure
void displayRecordsInHash(struct HashType* hashTable, int hashSize)
{
	int i;
	struct RecordType* temp;

	for (i = 0; i < hashSize; i++)
	{
		temp = hashTable[i].head;
		printf("\nBucket %d :", i);
		while (temp)
		{
			printf(" -> id: %d, name: %c, order: %d", temp->id, temp->name, temp->order);
			temp = temp->next;
		}
	}
}

int main(void)
{
	struct RecordType* records;
	int recordSize = 0;

	// Assuming hashSize is 10, it should be decided based on expected number of entries for efficient storage and lookup.
	int hashSize = 10;
	struct HashType* hashTable;

	recordSize = parseData("input.txt", &records);
	printRecords(records, recordSize);

	hashTable = createHashTable(hashSize, records, recordSize);

	displayRecordsInHash(hashTable, hashSize);

	// cleanup
	free(records);
	free(hashTable);

	return 0;
}
