#include <stdio.h>
#include <string.h>
#define MaxArtists 10
#define MaxLine 1000
#define MaxName 31
#define MaxVote 10

int artist_cmp(char [MaxName], char [MaxName], int, int);
void parallel_sort(int, int, int, char [][MaxName], int []);
int read_vote_file(FILE *, char [][MaxName], int [MaxArtists]);

int main() {
  FILE *votes_fp;
  votes_fp = fopen("votes.txt", "r");

  if (votes_fp == NULL) {
    printf("Could not open file votes.txt\n");
    return 0;
  }

  char artists[MaxArtists][MaxName];
  int votes[MaxArtists];

  int lines_read =  read_vote_file(votes_fp, artists, votes);
  parallel_sort(0, lines_read - 1, MaxName, artists, votes);
  
  int i;

  for (i = 0; i < lines_read; i++) {
      printf("%s %d\n", artists[i], votes[i]);
  }
}

void parallel_sort(int lo, int hi, int max, char artists[][max], int votes[]) {
  char key[max];
  for (int h = lo + 1; h <= hi; h++) {
    strncpy(key, artists[h], MaxName);
    int m = votes[h];
    int k = h - 1;
    
    while (k >= lo && artist_cmp(key, artists[k], m, votes[k]) < 0) {
      strcpy(artists[k + 1], artists[k]);
      votes[k + 1] = votes[k];
      --k;
    }
    
    strncpy(artists[k + 1], key, MaxName);
    votes[k + 1] = m;
  }
}

int artist_cmp(char a1[MaxName], char a2[MaxName], int v1, int v2) {
  int cmp = strcmp(a1, a2);
  if (cmp == 0) {
    if (v1 > v2)
      return -1;
    if (v1 == v2)
      return 0;
    if (v1 < v2)
      return 1;
  }
  return cmp;
}

int read_vote_file(FILE *votes_fp, char artists[][MaxName], int votes[]) {
  int i;
  char line[MaxLine];
  
  for (i = 0; i < MaxArtists && fgets(line, MaxLine, votes_fp) != NULL; i++) {
    char name[MaxName];
    int vote = 0;
    int offset = 0, readCharCount;
    sscanf(line, "%[^0-9]%n", name, &readCharCount);
    offset += readCharCount;

    int intermediate_vote;
    int n = 0;
    sscanf(line + offset, "%d%n", &intermediate_vote, &readCharCount);
    offset += readCharCount;
    while (n < MaxVote && intermediate_vote != 0) {
      vote += intermediate_vote;
      sscanf(line + offset, "%d%n", &intermediate_vote, &readCharCount);
      offset += readCharCount;
      n++;
    }

    strncpy(artists[i], name, MaxName);
    votes[i] = vote;
  }

  return i;
}
