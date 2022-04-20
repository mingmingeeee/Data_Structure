
#include <stdio.h>
#include <string.h>
#define MAX_WORD 4000
#define MAX_LEN 20

char dict[MAX_WORD][MAX_LEN];
char grid[MAX_LEN][MAX_LEN];
int n_words; //실제 사전에 저장된 단어의 수
int n;

void read_dictionary() {

	FILE *fp1 = fopen("dictionary.txt", "r");
	int i = 0;
	while (fscanf(fp1, "%s", dict[i]) != EOF)
		i++;
	fclose(fp1);
	n_words = i;
	return;
}
void read_grid() {
	FILE *fp2 = fopen("input.txt", "r");
	fscanf(fp2, "%d", &n);
	for (int i = 0; i < n; i++) {
		fscanf(fp2, "%s", grid[i]);
	}
	fclose(fp2);
	return;
}

char get_char(int i, int j, int dir, int dist) {

	int data[][8] = { { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 },{ 1,1 },{ 1,-1 },{ -1,1 },{ -1,-1 } };

	for (int k = 0; k < 8; k++) {
		if (dir == k) {
			for (int f = 0; f < dist; f++) {
				i += data[k][0];
				j += data[k][1];
			}
			break;
		}
	}

	return grid[i][j];

}

int search(int i) {

	for (int k = 0; k < n; k++) {
		for (int j = 0; j < n; j++) {
			int len = strlen(dict[i]);
			if (grid[k][j] != dict[i][0])
				continue;
			for (int dir = 0; dir < 8; dir++) {
				int dist = 1; //단어 길이
				for (; dist < len; dist++) { //작은 동안 살펴본다
					char ch = get_char(k, j, dir, dist); //i,j에서dir방향으로 dist칸 떨어진 그 단어 리턴해주는 함수
					if (ch == '\0' || ch != dict[i][dist]) //널 이거나 단어가 다르다면 볼 필요 X
						break;
				}
				if (dist >= len) { //빠져나가지 않고 같으면 찾았다 !!! 는 메세지 출력 
					return 1;
				}
			}
		}
	}

	return 0;
}

int main() {

	read_dictionary();
	read_grid();

	for (int i = 0; i < n_words; i++) {
		int result = search(i);
		if (result == 1)
			printf("%s\n", dict[i]);
	}


}
