#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 데이터 타입 정의

// 구조체들 먼저 정의
typedef struct song Song;
typedef struct snode SNode;
typedef struct artist Artist;

// artist, title, path, index (노래 번호)
struct song {
	Artist *artist;
	char *title;
	char *path;
	int index;
};

// 양방향 연결이면서 song을 거느리고 있음
// next, prev, song
struct snode {
	struct snode *next, *prev;
	Song *song;
};

// name, next, head, tail 
struct artist {
	char *name;
	struct artist *next; // 이름ㅇ 동일한 가수들 단방향으로 연결

	SNode *head, *tail;
};

// 뒤에 나오는 구조체를 앞에서 쓸 수밖에 없음

void add_song(char *artist, char *title, char *path);
void initialize();
void status();
void load(FILE *fp);
void search_song(char *artist, char *title);
void search_song(char *artist);
void play(int index);
void save(FILE *fp);
void remove(int index);

#endif
