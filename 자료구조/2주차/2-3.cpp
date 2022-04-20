#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 80

int main() {

	FILE  *fp1 = fopen("harry.txt", "r");
	FILE  *fp2 = fopen("aligned.txt", "w");

	char ch; //단어 입력
	char buffer[MAX]; //입력파일 한 줄
	char * words[MAX]; //출력파일 한 줄 -> 단어들
	char *token;  //토큰

	int words_i = 0, length = 0; //단어개수&글자수
	int index = 0; //입력파일 index
	int flag = 0;
	while (feof(fp1) == 0) //한단어씩 입력받는다
	{
		if (flag == 1) {
			index = 0;
			flag = 0;
		}
		fscanf(fp1, "%c", &ch);
		buffer[index++] = ch; //buffer라는 배열을 만들어서 저장함 -> 입력파일의 한 줄

		if (ch == '\n') { //줄바꿈 문자가 나올 때 
			flag = 1;
			buffer[index - 1] = '\0'; //입력파일에서의 한 줄을 구분해주기 위해 널문자를 넣어서 끝을 구분해줌

			token = strtok(buffer, " "); //-> 입력 파일의 한 줄이 끝났을 때 strtok 로 단어부터 나눠줌

			while (token != NULL) { //입력파일의 한 줄을 나눔
				length += strlen(token) + 1; //출력파일의 한 줄의 단어 개수 -> 마지막 단어에도 공백이 추가된 길이로 인식
				words[words_i++] = strdup(token); //출력파일의 한 줄 단어 복사

				if (length - 1 > 80) { //만약 length가 80보다 커지면
					length -= strlen(token) + 2; //아까 마지막에도 공백 + 앞글자의 공백 고려해서 -2함
					int L = (80 - length) / (words_i - 2);
					for (int i = 0; i < words_i - 2; i++) {
						fprintf(fp2, "%s", words[i]);
						if ((L <= 0 && i < 80 - length) || (L > 0 && i < (80 - length) % (words_i - 2))) {
							for (int j = 0; j <= L + 1; j++)
								fprintf(fp2, " ");
						}
						else
							for (int j = 0; j <= L; j++)
								fprintf(fp2, " ");
					}
					fprintf(fp2, "%s\n", words[words_i - 2]); // 마지막 글자는 띄어쓰기 XX
					length = strlen(token) + 1; //length 다시
					words_i = 0; //초기화
					words[words_i++] = strdup(token); //다시 복사
				}
				token = strtok(NULL, " "); //출력파일 한 줄 나눠줌
			}
		}
	}

	// 마지막줄
	for (int i = 0; i < words_i; i++)
		fprintf(fp2, "%s ", words[i]);

	fclose(fp1);
	fclose(fp2);

}