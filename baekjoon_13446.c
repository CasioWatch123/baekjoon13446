#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define STANDARD_ARRAY InputArray1

typedef struct {
	char* array;
	int length;
}CharArrayWrapper;

typedef struct {
	CharArrayWrapper* target;
	int cursor;
	int length;
}Substring;

typedef struct {
	CharArrayWrapper* target;
	int front;
	int rear;
	int length;
	int gap;
}CursorsWrapper;

typedef struct {
	Substring gamma;
	Substring beta;
}GammaBetaWrapper;

CharArrayWrapper InputArray1;
CharArrayWrapper InputArray2;

int compareString(Substring* String, Substring* ProtoString) {
	if (String->target != NULL && String->length > ProtoString->length) {
//		printf("(compareString)method, return type 1\n"); //debug
		return 1;
	}
	else if(String->target != NULL && String->length == ProtoString->length) {
		int i;
		for(i=1;i<String->length;i++) {
			if (String->target->array[String->cursor+i] < ProtoString->target->array[ProtoString->cursor+i]) {
//				printf("(compareString)method, return type 2\n"); //debug
				return 1;
			}
			if (String->target->array[String->cursor+i] > ProtoString->target->array[ProtoString->cursor+i]) {
//				printf("(compareString)method, return type 3\n"); //debug
				return 0;
			}
		}
	}
//	printf("(compareString)method, return type 4\n");
	return 0;
}



//notice! return type is dynamically allocated array
GammaBetaWrapper searchBeta_BGB(CursorsWrapper* Cursors, CharArrayWrapper* Array2) { //반환 배열 구성 : gamma beta 순
	printf("(searchBeta_BGB)method entring Cursors front:rear %d:%d\n", Cursors->front, Cursors->rear);
 	GammaBetaWrapper result;
 	
 	result.gamma.target = NULL;
 	result.beta.target = NULL;
	
	Substring gamma;
	gamma.target = Cursors->target;
	gamma.cursor = Cursors->front+Cursors->length;
	gamma.length = -1;
// 	int gammaCursor = Cursors->front+Cursors->length;
 	CharArrayWrapper* Array1 = Cursors->target;
 	
 	printf("gamma.cursor : %d (Cursor->length : %d)\n", gamma.cursor, Cursors->length);
 	
 	if (Cursors->gap < 2) { //alpha-alpha case 		
 		return result;
	}
	 
	int i,j,k;
	for(i=1;i<Array2->length-1;i++) {
		printf("%c(%d):%c(%d)\n", Array2->array[i],i, gamma.target->array[gamma.cursor],gamma.cursor);
		if (Array2->array[i] == gamma.target->array[gamma.cursor]) {
			for(j=1; ;j++) {
				if (
				 !(i+j < Array2->length-1) ||
				 !(Cursors->length+j < Cursors->gap) || 
				 !(gamma.target->array[gamma.cursor+j] == Array2->array[i+j]) )
					break;
			}
			gamma.length = j;
			int array2GammaCursor = i;
			
			printf("tGL : %d\n",j);
			
			Substring topBeta = {Array2, -1, -1};
			
			for(j=0;j<array2GammaCursor;j++) {//gamma가 최대 길이일 때 beta 검색 시도 
				k = 0;
				int nowBetaCursor = array2GammaCursor + gamma.length;
				
				if(Array2->array[j] == Array2->array[nowBetaCursor]) {
					for(k=1; ;k++) {
						if (
						 !(j+k < array2GammaCursor) || 
						 !(Array2->array[j+k] == Array2->array[nowBetaCursor+k]) )
							break;
					}
				}
				
				Substring nowBeta = {Array2, j, k};
				
				if(compareString(&nowBeta, &topBeta)) {
					topBeta = nowBeta;
				}
				
				j += k; //마법의 최적화 로직 
			}
			
			if (topBeta.length > 0) { //최대 길이 gamma에서 beta 검색 성공
				printf("topBeta.length > 0 (%d)\n", topBeta.length);
				
				result.gamma = gamma;
				result.beta = topBeta;
				
				return result;
			}
			
			for(j=1;gamma.length-j>0;j++) {//gamma 길이 1씩 줄이며 beta 탐색 시도 
				int nowBetaCursor = array2GammaCursor + gamma.length - j;
				
				for(k=0;k<array2GammaCursor;k++) {
					if(Array2->array[k] == Array2->array[nowBetaCursor]) {
						Substring resultGamma = {Array2, array2GammaCursor, gamma.length-j};
						Substring resultBeta = {Array2, nowBetaCursor, 1};
						
						printf("(search_BGB)method gamma info | cursror : %d, length : %d\n", array2GammaCursor, resultGamma.length);
						result.gamma = resultGamma;
						result.beta = resultBeta;
						
						return result;
					}
				}
			}
		}
	}
	//탐색 실패로 for문 탈출 시 무효의 result 반환 
	return result;
}

CharArrayWrapper search_AGA(CharArrayWrapper* TargetArray, CharArrayWrapper* Array2) {
	int topLength = -1;
	CursorsWrapper topAlphaInfo;
	GammaBetaWrapper topGammaBetaInfo;
	
	int i,j,k;
	for(i=0;i<TargetArray->length-1;i++) {
		for(j=i+1;j<TargetArray->length;j++) {
			if (
			 TargetArray->array[i] == TargetArray->array[j] && 
			 (i == 0 || TargetArray->array[i-1] != TargetArray->array[j-1]) ) {//TargetArray 내 동일 문자 2개 발견
				for(k=1; ;k++) {//alpha substring의 최대 길이 탐색 
					if (
					 !(j+k < TargetArray->length) ||
					 !(i+k < j) ||
					 !(TargetArray->array[i+k] == TargetArray->array[j+k]) )
						break;
				}
				
				int alphaLength = k;
				
				CursorsWrapper alphaCursors = {TargetArray, i, j, alphaLength, j-i};
				
				//gamma beta search		
				//~ alpha-gamma ~ alpha ~ case
				GammaBetaWrapper nowGammaBeta = searchBeta_BGB(&alphaCursors, Array2);
				
				if (nowGammaBeta.gamma.target != NULL) {
					printf(" GAMMA : %d(%c) l : %d \n alpha : %d(%c) l : %d \n beta: %d(%c) l : %d\n", 
					nowGammaBeta.gamma.cursor, nowGammaBeta.gamma.target->array[nowGammaBeta.gamma.cursor], nowGammaBeta.gamma.length, 
					alphaCursors.front, alphaCursors.target->array[alphaCursors.front], alphaCursors.length,
					nowGammaBeta.beta.cursor, nowGammaBeta.beta.target->array[nowGammaBeta.beta.cursor], nowGammaBeta.beta.length);
					
					if (alphaCursors.length + nowGammaBeta.gamma.length + nowGammaBeta.beta.length > topLength) {
						topLength = alphaCursors.length + nowGammaBeta.gamma.length + nowGammaBeta.beta.length;
						topAlphaInfo = alphaCursors;
						topGammaBetaInfo = nowGammaBeta;
					}
				}
			}
		}
	}
	
	CharArrayWrapper result = {NULL};
	
	if (topLength > 0) {
		char* arr = (char*)malloc(sizeof(char) * topLength);
		result.array = arr;
		result.length = topLength;
		for(i=0;i<topAlphaInfo.length+topGammaBetaInfo.gamma.length;i++) {
			result.array[i] = topAlphaInfo.target->array[topAlphaInfo.front+i];
		}
		for(i=0;i<topGammaBetaInfo.beta.length;i++) {
			result.array[topAlphaInfo.length+topGammaBetaInfo.gamma.length+i] = topGammaBetaInfo.beta.target->array[topGammaBetaInfo.beta.cursor+i];
		}
	}
	
	return result;
}


int main(void) {
	long start, finish;

	char array1[47] = {};
	char array2[47] = {};
	
	scanf("%s %s",array1,array2);
	
	InputArray1.array = array1;
	InputArray2.array = array2;

	int i;
	for(i=0;InputArray1.array[i];i++);
	InputArray1.length = i;
	
	for(i=0;InputArray2.array[i];i++);
	InputArray2.length = i;
	start = clock();
	
	//main method block
	CharArrayWrapper result = search_AGA(&InputArray1,&InputArray2);
	if (result.array != NULL) {
		printf("result.length : %d\n",result.length);
		for(i=0;i<result.length;i++) {
			printf("%c", result.array[i]);
		}
		free(result.array);
	}

	
	finish = clock();
	
//	printf("\n\n");
	
	//debug
	printf("\n");
	printf("시작 시간: %6ld(ms)\n", start);
	printf("종료 시간: %6ld(ms)\n", finish);
	printf("소요 시간: %6ld(ms)\n", finish - start); 
}
