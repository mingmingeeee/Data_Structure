#include "library.h"
#include "string_tools.h"
#include <windows.h>

#define NUM_CHARS 256 // 2^9 = 256 -> 한 바이트가 가질 수 있는 모든 값의 경우
#define BUFFER_LENGTH 200
#define SIZE_INDEX_TABLE 100
// 배열은 타입이 아닌 변수이기 때문에 header 파일에는 넣지 않는답
// 다른 소스코드랑은 공유하지 않는 것이기 때문,,,

Artist * artist_directory[NUM_CHARS];
SNode * index_directory[SIZE_INDEX_TABLE];

int num_index = 0;

void insert_node(Artist *ptr_artist, SNode *ptr_snode);
void print_song(Song *);
void print_artist(Artist *p);
Artist *find_artist(char *name);
SNode *find_snode(Artist *ptr_artist, char *title);
void insert_to_index_directory(Song *ptr_song);
SNode *find_song(int index);
void save_song(Song *ptr_song, FILE *fp);
void save_artist(Artist *p, FILE *fp);
void destroy_song(Song *ptr_song);
void remove_snode(Artist *ptr_artist, SNode *ptr_snode);

void initialize() {
	// 안에 값이 없을 수도 있기 때문에 먼저 모두 초기화를 한다.
	for (int i = 0; i < NUM_CHARS; i++)
		artist_directory[i] = NULL;

	for (int i = 0; i < SIZE_INDEX_TABLE; i++)
		index_directory[i] = NULL;
}

void load(FILE *fp) {
	char buffer[BUFFER_LENGTH];
	char *name, *title, *path;

	while (1) {		// 무한 루프
		if (read_line(fp, buffer, BUFFER_LENGTH) <= 0)		// get a line 라인단위로 받기
			break; // 아무 명령 XXX

		name = strtok(buffer, "#");
		if (strcmp(name, " ") == 0)
			name = NULL;
		else
			name = strdup(name);

		title = strtok(NULL, "#");
		if (strcmp(title, " ") == 0)
			title = NULL;
		else
			title = strdup(title);

		path = strtok(NULL, "#");
		if (strcmp(path, " ") == 0)
			path = NULL;
		else
			path = strdup(path);

		//		printf("%s %s %s\n", name, title, path);
		add_song(name, title, path);
	}
}

void search_song(char *artist, char *title) {
	Artist *ptr_artist = find_artist(artist);
	if (ptr_artist == NULL) {
		printf("No such artist exists.\n");
		return;
	}
	SNode *ptr_snode = find_snode(ptr_artist, title);
	if (ptr_snode != NULL) {
		printf("Found:\n");
		print_song(ptr_snode->song);
	}
	else {
		printf("No such artist exists.\n");
		return;
	}
}

SNode *find_snode(Artist *ptr_artist, char *title) {
	SNode *ptr_snode = ptr_artist->head;
	while (ptr_snode != NULL && strcmp(ptr_snode->song->title, title) < 0)
		ptr_snode = ptr_snode->next;

	if (ptr_snode != NULL && strcmp(ptr_snode->song->title, title) == 0)
		return ptr_snode;
	else
		return NULL;
}

void search_song(char *artist) {
	Artist *ptr_artist = find_artist(artist);
	if (ptr_artist == NULL) {
		printf("No such artist exists.\n");
		return;
	}
	printf("Found:\n");
	print_artist(ptr_artist);
}

Artist *create_artist_instance(char *name) {
	// malloc 하는 함수 따로 만들기
	Artist * ptr_artist = (Artist *)malloc(sizeof(Artist));
	ptr_artist->name = name;
	ptr_artist->head = NULL;
	ptr_artist->tail = NULL;
	ptr_artist->next = NULL;

	return ptr_artist;
}

Artist *add_artist(char *name) {

	Artist *ptr_artist = create_artist_instance(name);

	Artist *p = artist_directory[(unsigned char)name[0]]; // first node

	/* 알파벳 정렬 */
	Artist *q = NULL;

	while (p != NULL && strcmp(p->name, name) < 0) {
		q = p;
		p = p->next;
	}
	if (p == NULL && q == NULL) { // unique node
		artist_directory[(unsigned char)name[0]] = ptr_artist;
	}
	else if (q == NULL) { // add at the front
		ptr_artist->next = artist_directory[(unsigned char)name[0]];
		artist_directory[(unsigned char)name[0]] = ptr_artist;
	}
	else { // add after(q)
		ptr_artist->next = p;
		q->next = ptr_artist;
	}

	return ptr_artist;
}

Song *create_song_instance(Artist *ptr_artist, char *title, char *path) {
	Song *ptr_song = (Song *)malloc(sizeof(Song));
	ptr_song->artist = ptr_artist;
	ptr_song->title = title;
	ptr_song->path = path;
	ptr_song->index = num_index;
	num_index++;
	// 노래를 삭제하면 중간에 빈 번호가 생길 수 있으나
	// 우리는 그냥 노래를 구분하기 위해 만든 것이므로
	// 그거까지 신경쓸 필요는 없다. 
	return ptr_song;
}

void add_song(char *artist, char *title, char *path) {

	// find if the artist already exists
	// return NULL if not
	Artist *ptr_artist = find_artist(artist);

	// 있으면 찾고 없으면 ptr_artist에 다시 만듬

	if (ptr_artist == NULL) { // 없으니 새로 만듦
		ptr_artist = add_artist(artist);
	}
	//  add the song to the artist pointed by ptr_artist

	Song *ptr_song = create_song_instance(ptr_artist, title, path);
	SNode *ptr_snode = (SNode *)malloc(sizeof(SNode));
	ptr_snode->song = ptr_song;
	ptr_snode->next = NULL;
	ptr_snode->prev = NULL;

	// insert node
	// ptr_artist가 가리키고 있는 노드 안에 ptr_snode 넣기
	insert_node(ptr_artist, ptr_snode);
	insert_to_index_directory(ptr_song);

}

void insert_to_index_directory(Song *ptr_song) {
	SNode *ptr_snode = (SNode *)malloc(sizeof(SNode));
	ptr_snode->song = ptr_song;
	ptr_snode->next = NULL;
	ptr_snode->prev = NULL;		// unused -> 단방향으로 해줄 것이기 때문에 사용 X

	int index = ptr_song->index % SIZE_INDEX_TABLE;

	// insert add the snode into the single linked list at index_table[index]

	SNode *p = index_directory[index];
	SNode *q = NULL;
	while (p != NULL && strcmp(p->song->title, ptr_song->title) < 0) {
		q = p;
		p = p->next;
	}
	if (q == NULL) {	// add_first
		ptr_snode->next = p;
		index_directory[index] = ptr_snode;
	}
	else {				// add_after_q
		ptr_snode->next = p;
		q->next = ptr_snode;
	}

}

void insert_node(Artist *ptr_artist, SNode *ptr_snode) {
	SNode *p = ptr_artist->head;
	while (p != NULL && strcmp(p->song->title, ptr_snode->song->title) < 0)
		p = p->next;

	// add ptr_snode before p
	// 와타시가 들어갈 자리는 p 앞이다.
	// 이중 연결 리스트에서는 조건을 다 나눈다.
	// 1. empty 2, at the front, 3. at the end, 5. in the middle

	if (ptr_artist->head == NULL) { // 1 -> 연결리스트 처음에서 빠져나옴
		ptr_artist->head = ptr_snode;
		ptr_artist->tail = ptr_snode;
	}
	else if (p == ptr_artist->head) { // 2
		ptr_snode->next = ptr_artist->head;
		ptr_artist->head->prev = ptr_snode;
		ptr_artist->head = ptr_snode;
	}
	else if (p == NULL) { // 3 -> 연결리스트를 끝까지 돌았을 때
		ptr_snode->prev = ptr_artist->tail;
		ptr_artist->tail->next = ptr_snode;
		ptr_artist->tail = ptr_snode;
	}
	else { // before p
		ptr_snode->next = p;
		ptr_snode->prev = p->prev;
		p->prev->next = ptr_snode;
		p->prev = ptr_snode;
	}
}

Artist *find_artist(char *name) {
	Artist *p = artist_directory[(unsigned char)name[0]];	// first artist with initial name[0]

	/* name[0] : 00000000 ~ 11111111 */
	/* 써버리면 이거를 하나의 정수로 전환해서 그 정수를 배열 인덱스로 써버린다는 것 */
	/* 11001111 -> 1111111111111111001111 -> 첫비트가 1이면 음수로 간주함 */
	/* 그러므로 (unsigned char)로 변환해주어야 한다. */

	while (p != NULL && strcmp(p->name, name) < 0) // -> 다 돌 필요 없이 내가 찾는 이름보다 커지면 멈춤 어차피 그 뒤엔 없으니까...
		p = p->next;

	if (p != NULL && strcmp(p->name, name) == 0)
		return p;
	// if (p == NULL)
	// return NULL; // 이름 발견 x
	else
		return NULL;
}

void status() { // 화면에 출력
	for (int i = 0; i < NUM_CHARS; i++) {
		Artist *p = artist_directory[i];
		while (p != NULL) {
			print_artist(p);
			p = p->next;
		}
	}
}

void print_artist(Artist *p) {
	printf("%s\n", p->name);
	SNode *ptr_snode = p->head;
	while (ptr_snode != NULL) {
		print_song(ptr_snode->song);
		ptr_snode = ptr_snode->next;
	}
}

void print_song(Song *ptr_song) {
	printf("	%d: %s, %s\n", ptr_song->index, ptr_song->title, ptr_song->path);
}

SNode *find_song(int index) {	// 노래 번호로 song 찾기
	SNode *ptr_snode = index_directory[index % SIZE_INDEX_TABLE];
	while (ptr_snode != NULL && ptr_snode->song->index != index)
		ptr_snode = ptr_snode->next;

	return ptr_snode;
}

void play(int index) {
	SNode *ptr_snode = find_song(index);

	if (ptr_snode == NULL) {
		printf("No such song exists.\n");
		return;
	}

	printf("found the song %s \n", ptr_snode->song->title);

	// ShellExecute(GetDesktopWindow(), "open", ptr_snode->song->path, NULL, NULL, SW_SHOW);
	// 강의랑 똑같은데 왜 오류 생기는지,,,ㅠ ㅠ ㅠ
}

void save(FILE *fp) { // 파일에 출력하는 것
	for (int i = 0; i < NUM_CHARS; i++) {
		Artist *p = artist_directory[i];
		while (p != NULL) {
			save_artist(p, fp);
			p = p->next;
		}
	}
}

void save_artist(Artist *p, FILE *fp) {
	SNode *ptr_snode = p->head;
	while (ptr_snode != NULL) {
		save_song(ptr_snode->song, fp);
		ptr_snode = ptr_snode->next;
	}
}

void save_song(Song *ptr_song, FILE *fp) {
	if (ptr_song->artist != NULL)
		fprintf(fp, "%s#", ptr_song->artist->name);
	else
		fprintf(fp, " #\n");
	if (ptr_song->title != NULL)
		fprintf(fp, "%s#", ptr_song->title);
	else
		fprintf(fp, " #");
	if (ptr_song->path != NULL)
		fprintf(fp, "%s#\n", ptr_song->path);
	else
		fprintf(fp, " #\n");
}

void remove(int index) {
	SNode *q = NULL;										// previous to p
	SNode *p = index_directory[index % SIZE_INDEX_TABLE];	// head node
	
	// 정렬이 안되어있다고 가정하고,,,
	while (p != NULL && p->song->index != index) {
		q = p;
		p = p->next;
	}

	if (p == NULL) {	// either empty list or not exist
		printf("No such song exists.\n");
		return;
	}

	Song *ptr_song = p->song; 
	// 하나의 song을 삭제하려면 inde_directory, artist에 달려있는 snode 모두 삭제해야 한다.

	if (q == NULL) {	// reomve list
		index_directory[index % SIZE_INDEX_TABLE] = p->next;
	}
	else {				// remove after q
		q->next = p->next; // q->next = q->next->next
	}
	free(p);

	Artist *ptr_artist = ptr_song->artist;

	// find the snode in the double linked list of ptr_artist

	SNode *ptr_snode = find_snode(ptr_artist, ptr_song->title);
	if (ptr_snode == NULL) {
		printf("Not found snode - something worng.\n");
		return;
	}

	remove_snode(ptr_artist, ptr_snode);
	destroy_song(ptr_song);

}

// 에러 왜 나는 지 모르겠음
void destroy_song(Song *ptr_song) { // strdup로 동적메모리할당 받아줬던 것들 free시키기
	if (ptr_song->artist != NULL)
		free(ptr_song->artist);
	if (ptr_song->title != NULL)
		free(ptr_song->title);
	if (ptr_song->path != NULL)
		free(ptr_song->path);
	free(ptr_song);
}

void remove_snode(Artist *ptr_artist, SNode *ptr_snode) {
	SNode *first = ptr_artist->head;
	SNode *last = ptr_artist->tail;

	if (first == ptr_snode && last == ptr_snode) {	// unique node
		ptr_artist->head = NULL;
		ptr_artist->tail = NULL;
	}
	else if (first == ptr_snode) { // remove first
		ptr_artist->head = ptr_snode->next;
		ptr_snode->next->prev = ptr_snode->prev;
	}
	else if (last == ptr_snode) { // remove last
		ptr_snode->prev->next = ptr_snode->next;
		ptr_artist->tail = ptr_snode->prev;
	}
	else  { // in the middle
		ptr_snode->prev->next = ptr_snode->next;
		ptr_snode->next->prev = ptr_snode->prev;
	}
	free(ptr_snode);
}