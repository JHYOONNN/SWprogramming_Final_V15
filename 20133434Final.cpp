#include "stdafx.h"
#include <stdio.h>
#include<stdlib.h>

#include <io.h>
#include <string>

using namespace std;

void fOpenGetC(char filename[][_MAX_PATH], int fileNum, char *findWord, char *keyWord, int point[][2], float similarPoint[][2], float accuracy);
void compareString(char *content, int fileNum, char *findWord, char *keyWord, int point[][2], float similarPoint[][2], float accuracy);
float similarity(char *c1, char *c2);
void Ranking(char fileRank[][_MAX_PATH], int pointArray[][2], float similarPointArray[][2], int arrayCount);

void fOpenGetC(char filename[][_MAX_PATH], int fileNum, char *findWord, char *keyWord, int point[][2], float similarPoint[][2], float accuracy) {
	FILE *fp;
	int word_count = 0, index = 0, sub_index = 0;
	char content[100000];
	char word[64];
	char filepath[100] = ".\\dataset-2nd\\";

	_strlwr(findWord);
	_strlwr(keyWord);

	strcat(filepath, filename[fileNum]);

	fp = fopen(filepath, "rt");

	if (fp != NULL) {
		while (fgets(content, 100000, fp)) {
			_strlwr(content);
			compareString(content, fileNum, findWord, keyWord, point, similarPoint, accuracy);
			}
		fclose(fp);
		}
	
}

void compareString(char *content, int fileNum, char *findWord, char *keyWord, 
	int point[][2], float similarPoint[][2], float accuracy) {
	bool hasFindWord = false;
	bool hasFindKeyword = false;

	char *pPos = NULL;
	char *ptr = strtok(content, ".,()'!\"\n ");

	while (ptr != NULL) {
		while (1) {
			pPos = strstr(ptr, findWord);
			if (pPos == NULL)
				break;
			if (pPos) {
				ptr = pPos + 1;
				point[fileNum][0]++;
				pPos = NULL;
				hasFindWord = true;
			}
		}

		if (hasFindWord == false) {
			while (1) {
				pPos = strstr(ptr, keyWord);
				if (pPos == NULL)
					break;
				if (pPos) {
					ptr = pPos + 1;
					point[fileNum][1]++;
					pPos = NULL;
				}
			}
			if (hasFindKeyword == false) {
				if (similarity(ptr, findWord) >= accuracy) {
					similarPoint[fileNum][0] += similarity(ptr, findWord);
					similarPoint[fileNum][1] ++;
					//printf("%d %d, %s  %.2f\n", fileNum, strlen(ptr), ptr, similarity(ptr, findWord));
				}
			}
		}

		hasFindWord = false;
		hasFindKeyword = false;
		ptr = strtok(NULL, ".,()'!\"\n ");
	}
}

float similarity(char *c1, char *c2) {
	int finalPoint = 0;
	int pointForCorrespondenc = 0;
	int pointForConnect = 1;
	int pointForConnectTemp = 1;
	int pointForConnectTemp2 = 1;
	int locationTemp = 0;
	int locationTemp2 = 0;
	int locationTemp3 = 0;
	int findWordLength = strlen(c2);
	int pointForWordLength = strlen(c1) - strlen(c2);

	char *ptr = c1;
	char *ptrTemp = ptr;
	char *ptrTemp2 = ptr;

	
	pointForWordLength = findWordLength - abs(pointForWordLength);

	while (c2[locationTemp + 1] != NULL) {
		if (strchr(c1, c2[locationTemp]) != NULL) {
			ptr = strchr(c1, c2[locationTemp]);
			while (strchr(ptr, c2[locationTemp]) != NULL) {
				ptr = strchr(ptr, c2[locationTemp]);
				ptrTemp = ptr;
				locationTemp2 = locationTemp;
				pointForConnectTemp = 1;
				while (*(ptrTemp + 1) == c2[locationTemp2 + 1] && *(ptrTemp + 1) != NULL 
					&& c2[locationTemp2 + 1] != NULL) {
					locationTemp2++;
					ptrTemp++;
					pointForConnectTemp++;
				}
				if (pointForConnect < pointForConnectTemp)
					pointForConnect = pointForConnectTemp;
				ptr = ptr + 1;
			}
		}
		locationTemp++;
	}

	while (c2[locationTemp3] != NULL) {
		ptrTemp2 = strchr(c1, c2[locationTemp3]);
		if (ptrTemp2 != NULL)
			pointForCorrespondenc++;
		locationTemp3++;
	}

	finalPoint = (3*(float)pointForConnect + 2*(float)pointForCorrespondenc 
		+ (float)pointForWordLength) / (6* (float)findWordLength) * 100;
	
	return finalPoint;
}

void Ranking(char fileRank[][_MAX_PATH], int pointArray[][2], float similarPointArray[][2],
	int arrayCount) {

	int temp[4];
	float tempForSim[2];
	float average = 0;

	char temp2[_MAX_PATH];
	for (int i = 3; i < arrayCount; i++) {
		
		for (int j = 3; j < arrayCount-1; j++) {
			if (pointArray[j][0] < pointArray[j + 1][0]) {
			for (int k = 0; k < 2; k++) {
				temp[k] = pointArray[j][k];
				pointArray[j][k] = pointArray[j + 1][k];
				pointArray[j + 1][k] = temp[k];
			}
			for (int k = 0; k < 2; k++) {
				tempForSim[k] = similarPointArray[j][k];
				similarPointArray[j][k] = similarPointArray[j + 1][k];
				similarPointArray[j + 1][k] = tempForSim[k];
			}
			strcpy(temp2, fileRank[j]);
			strcpy(fileRank[j], fileRank[j + 1]);
			strcpy(fileRank[j + 1], temp2);
		}

			else if (pointArray[j][0] == pointArray[j + 1][0]) {
				if (pointArray[j][1] < pointArray[j + 1][1]) {
					for (int k = 0; k < 2; k++) {
						temp[k] = pointArray[j][k];
						pointArray[j][k] = pointArray[j + 1][k];
						pointArray[j + 1][k] = temp[k];
					}
					for (int k = 0; k < 2; k++) {
						tempForSim[k] = similarPointArray[j][k];
						similarPointArray[j][k] = similarPointArray[j + 1][k];
						similarPointArray[j + 1][k] = tempForSim[k];
					}

					strcpy(temp2, fileRank[j]);
					strcpy(fileRank[j], fileRank[j + 1]);
					strcpy(fileRank[j + 1], temp2);
				}

				else if (pointArray[j][1] == pointArray[j + 1][1]) {
					if (similarPointArray[j][0] < similarPointArray[j + 1][0]) {
						for (int k = 0; k < 2; k++) {
							temp[k] = pointArray[j][k];
							pointArray[j][k] = pointArray[j + 1][k];
							pointArray[j + 1][k] = temp[k];
						}
						for (int k = 0; k < 2; k++) {
							tempForSim[k] = similarPointArray[j][k];
							similarPointArray[j][k] = similarPointArray[j + 1][k];
							similarPointArray[j + 1][k] = tempForSim[k];
						}

						strcpy(temp2, fileRank[j]);
						strcpy(fileRank[j], fileRank[j + 1]);
						strcpy(fileRank[j + 1], temp2);
					}
				}
			}
		}
	}

	for (int i = 3; i < arrayCount; i++) {
		if (similarPointArray[i][1] != 0)
			average = similarPointArray[i][0] / similarPointArray[i][1];
		else
			average = 0;

		if (pointArray[i][0] != 0 || pointArray[i][1] != 0 || similarPointArray[i][0] != 0) {
			printf("파일명 : %s                          랭킹 : %d \n완전 일치 단어 개수 : %d \n키워드 일치 단어 개수 : %d \n유사단어 분포 점수 : %.2f \n유사단어 개수 : %.0f \n유사도 평균 : %0.2f \n\n\n", fileRank[i], i - 2, pointArray[i][0], pointArray[i][1], similarPointArray[i][0], similarPointArray[i][1], average);
			printf("**************************************************************************\n");
		}
	}
	printf("외의 값들은 단어, 키워드, 유사단어가 아예 없어 랭킹에서 제외됩니다. \n");
}

void main() {
	char filename[1200][_MAX_PATH];
	char searchWord[100];
	char mainSearchWord[100];
	char fileRanking[1200][_MAX_PATH];

	_finddata_t fd;
	long handle;

	int result = 1;
	int tmpForArray = 1;
	float accuracy = 0;
	int findWordLength = 0;
	int findPoint[1200][2] = { 0 };

	float similarPoint[1200][2] = { 0 };

	
	handle = _findfirst(".\\dataset-2nd\\*.*", &fd);

	for (int i = 0; i<strlen(fd.name); i++)
		filename[0][i] = fd.name[i];
	filename[0][strlen(fd.name)] = '\0';
	if (handle == -1) {
		printf("There were no files.\n");
		return;
	}
	while (result != -1)
	{


		for (int i = 0; i<strlen(fd.name); i++)
			filename[tmpForArray][i] = fd.name[i];
		
		filename[tmpForArray][strlen(fd.name)] = '\0';
		result = _findnext(handle, &fd);

		tmpForArray++;

	}

	printf("검색할 문자열을 입력하세요 :       ");
	scanf("%s", searchWord);
	findWordLength = strlen(searchWord);

	printf("중요 문자를 입력하세요 :       ");
	scanf("%s", mainSearchWord);

	printf("유사 단어 정확도를 입력하세요.(퍼센트 기준) \n( (낮음) 1 ~ 100 (완전 일치 단어만 찾기) )          ");
	scanf("%f", &accuracy);

	printf("\n\n\n");
	for (int i = 0; i < tmpForArray - 3; i++) {
		fOpenGetC(filename, i + 3, searchWord, mainSearchWord, findPoint, similarPoint, accuracy);
	}
	Ranking(filename, findPoint, similarPoint, tmpForArray);
	
	_findclose(handle);
	return;
}
