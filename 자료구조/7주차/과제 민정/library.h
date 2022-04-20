#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// ������ Ÿ�� ����

// ����ü�� ���� ����
typedef struct song Song;
typedef struct snode SNode;
typedef struct artist Artist;

// artist, title, path, index (�뷡 ��ȣ)
struct song {
	Artist *artist;
	char *title;
	char *path;
	int index;
};

// ����� �����̸鼭 song�� �Ŵ����� ����
// next, prev, song
struct snode {
	struct snode *next, *prev;
	Song *song;
};

// name, next, head, tail 
struct artist {
	char *name;
	struct artist *next; // �̸��� ������ ������ �ܹ������� ����

	SNode *head, *tail;
};

// �ڿ� ������ ����ü�� �տ��� �� ���ۿ� ����

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
