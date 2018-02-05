#include <stdio.h>
#include <string.h>
#define MaxLine 100
#define MaxName 40
#define MaxRecords 100
#define MaxPhone 13

int readInput(char [][MaxName], char [][MaxPhone]);
int nameIndex(char [], char, char [][MaxName]);

int main () {
  char names[MaxRecords][MaxName];
  char phones[MaxRecords][MaxPhone];

  int numRecords = readInput(names, phones);
  if (numRecords == 0) {
        printf("Could not read the file phonebook.txt\n");
        return 1;
  }  
  
  printf("Enter first and last name:\n");

  char name[MaxName];
  while (fgets(name, MaxLine, stdin) != NULL) {
    name[strcspn(name, "\n")] = ' ';
    if (strlen(name) > 1) {
      int found = nameIndex(name, numRecords, names);
      printf("at %d\n", found);
      if (found < 0) {
        printf("Not found\n");
      } else {
        printf("%s %s\n", names[found], phones[found]);
      }
    } else {
      printf("Please enter a valid name\n");
    }
  }
  
}

int nameIndex(char name[], char numRecords, char names[][MaxName]) {
  int low = 0;
  int high = numRecords;

  while (low <= high) {
      int mid = (low + high) / 2;
      int cmp = strncmp(name, names[mid], MaxName);

      if (cmp == 0) return mid;
      if (cmp < 0) high = mid - 1;
      else low = mid + 1;
  }

  return -1;
}

int insertAt(int len, char name[MaxName], char names[][MaxName]) {
    int k = 0;
    while (k < len && strncmp(name, names[k], MaxName) > 0)
      k++;
    return k;
}

void insert(int len, int at,
            char name[MaxName], char names[][MaxName],
            char phone[MaxPhone], char phones[][MaxPhone]) {
    int i;
    for (i = len; i >= at && i < MaxRecords; i--) {
      strncpy(names[i + 1], names[i], MaxName);
      strncpy(phones[i + 1], phones[i], MaxPhone);
    }
    strncpy(names[at], name, MaxName);
    strncpy(phones[at], phone, MaxPhone);
}

int readInput(char names[][MaxName], char phones[][MaxPhone]) {
  FILE *phonebook;
  phonebook = fopen("phonebook.txt", "r");
  if (phonebook == NULL) {
    return 0;
  }
  
  int n = 0;
  char line[MaxLine];
  while (n < MaxRecords && fgets(line, MaxLine, phonebook) != NULL) {
    char name[MaxName];
    char phone[MaxPhone];
    sscanf(line, "%39[^0-9] %13[0-9-]", name, phone);

    insert(n, insertAt(n, name, names), name, names, phone, phones);
    
    n++;
  }

  return n;
}
