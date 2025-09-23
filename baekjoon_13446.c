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


Substring stringMerge(Substring* F, Substring* R) {
	printf("in (stringMerge)method\n");
	char* array = (char*)malloc(sizeof(char) * (F->length + R->length));
	int i;
	for(i=0;i<F->length;i++) {
		array[i] = F->target->array[F->cursor+i];
	}
	for(i=0;i<R->length;i++) {
		array[F->length+i] = R->target->array[R->cursor+i];
	}
	
	CharArrayWrapper wrapper = {array, F->length+R->length};
	Substring result = {&wrapper, 0, wrapper.length};
	
	printf("return (stringMerge)method\n");
	return result;
}
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
	GammaBetaWrapper top = {{NULL, 0, 0}, {NULL, 0, 0}};

	Substring array1Gamma;
	array1Gamma.target = Cursors->target;
	array1Gamma.cursor = Cursors->front + Cursors->length;
	array1Gamma.length = -1;

 	CharArrayWrapper* Array1 = Cursors->target;
 	
 	printf("gamma.cursor : %d (Cursor->length : %d)\n", array1Gamma.cursor, Cursors->length);
 	
 	if (Cursors->gap < 2) { //alpha-alpha case
 		return top;
	}
	
	
	printf("\n");
	int i,j,k;
	for(i=1;i<Array2->length-1;i++) {
		printf("%c(%d):%c(%d)\n", Array2->array[i],i, array1Gamma.target->array[array1Gamma.cursor],array1Gamma.cursor);
		if (Array2->array[i] == array1Gamma.target->array[array1Gamma.cursor]) {
			for(j=1; ;j++) {
				if (
				 !(i+j < Array2->length-1) ||
				 !(Cursors->length+j < Cursors->gap) || 
				 !(array1Gamma.target->array[array1Gamma.cursor+j] == Array2->array[i+j]) )
					break;
			}
			
			printf("  tGL : %d\n",j);
			
			Substring nowGamma = {Array2, i, j};
			Substring nowBeta = {Array2, 0, 0};
			
			for(j=0;j<nowGamma.length;j++) {//gamma가 최대 길이일 때 beta 검색 시도 
				k = 0;
				int nowBetaCursor = nowGamma.cursor + nowGamma.length;
				
				if(Array2->array[j] == Array2->array[nowBetaCursor]) {
					for(k=1; ;k++) {
						if (
						 !(j+k < nowGamma.cursor) || 
						 !(Array2->array[j+k] == Array2->array[nowBetaCursor+k]) )
							break;
					}
				}
				
				nowBeta.cursor = j;
				nowBeta.length = k;
				
				//현재 beta, gamma와 top beta, gamma 비교 
				if (nowGamma.length+nowBeta.length > top.gamma.length + top.beta.length) {
					top.gamma = nowGamma;
					top.beta = nowBeta;
				}
				else if (nowBeta.length > 0 && 
						 nowGamma.length+nowBeta.length == top.gamma.length + top.beta.length) {
					Substring nowBetaGamma = stringMerge(&nowBeta, &nowGamma);
					Substring topBetaGamma = stringMerge(&top.beta,&top.gamma);
					
					if (compareString(&nowBetaGamma, &topBetaGamma)) {
						top.gamma = nowGamma;
						top.beta = nowBeta;
					}
					free(nowBetaGamma.target->array);
					free(topBetaGamma.target->array);
				}
				
				j += k; //마법의 최적화 로직 
			}
			
			
			//최대 길이 gamma서 beta 검색 실패 시 gamma 길이 줄이며 검색 
			if (nowBeta.length == 0) { 
				for(j=1;nowGamma.length-j>0;j++) {
					int flag = 0;
					
					int nowBetaCursor = nowGamma.cursor + nowGamma.length - j;
					
					for(k=0;k<nowGamma.cursor;k++) {
						if (nowGamma.target->array[k] == nowGamma.target->array[nowBetaCursor]) {
							nowGamma.length -= j;
							
							nowBeta.cursor = k;
							nowBeta.length = 1;
							
							
							if (nowGamma.length+nowBeta.length > top.gamma.length + top.beta.length) {
								top.gamma = nowGamma;
								top.beta = nowBeta;
							}
							else if (nowBeta.length > 0 && 
									 nowGamma.length+nowBeta.length == top.gamma.length + top.beta.length) {
								Substring nowBetaGamma = stringMerge(&nowBeta, &nowGamma);
								Substring topBetaGamma = stringMerge(&top.beta,&top.gamma);
								
								if (compareString(&nowBetaGamma, &topBetaGamma)) {
									top.gamma = nowGamma;
									top.beta = nowBeta;
								}
								free(nowBetaGamma.target->array);
								free(topBetaGamma.target->array);
							}
							
							flag = 1;
							break;
						}
						if (flag) {
							break;
						}
					}
				}
			}
		}
	}
	return top;
}



GammaBetaWrapper searchBeta_GBB(CursorsWrapper* Cursors, CharArrayWrapper* Array2) {
	printf("(searchBeta_GBB)method entring Cursors front:rear %d:%d\n", Cursors->front, Cursors->rear);
	GammaBetaWrapper top = {{NULL, 0, 0}, {NULL, 0, 0}};
	
	Substring array1Gamma;
	array1Gamma.target = Cursors->target;
	array1Gamma.cursor = Cursors->rear + Cursors->length;
	array1Gamma.length = -1;
	
	CharArrayWrapper* Array1 = Cursors->target;
	
	printf("gamma.cursor : %d l:%d\n", array1Gamma.cursor, Cursors->length);
	
	if (Cursors->rear + Cursors->length >= Cursors->target->length) {
		return top;
	}
	
	
	printf("\n");
	int i,j,k;
	for(i=0;i<Array2->length-2;i++) {
		printf("%c(%d):%c(%d)\n", Array2->array[i],i, array1Gamma.target->array[array1Gamma.cursor],array1Gamma.cursor);
		if (Array2->array[i] == array1Gamma.target->array[array1Gamma.cursor]) {
			for(j=1; ;j++) {
				if (
				 !(i+j < Array2->length-2) ||
				 !(Cursors->rear+Cursors->length+j < Cursors->target->length) || 
				 !(array1Gamma.target->array[array1Gamma.cursor+j] == Array2->array[i+j]) )
					break;
			}
			
			printf("  tGL : %d\n",j);
			
			Substring nowGamma = {Array2, i, j};
			Substring nowBeta = {Array2, 0, 0};
			
			for(j=nowGamma.cursor+nowGamma.length+1;j<Array2->length-1;j++) {//gamma가 최대 길이일 때 beta 탐색 
				k=0;
				int nowBetaCursor = nowGamma.cursor + nowGamma.length;
				
				if (Array2->array[j] == Array2->array[nowBetaCursor]) {
					for(k=1; ;k++) {
						if (
						 !(nowBetaCursor+k < j) || 
						 !(Array2->array[j+k] == Array2->array[nowBetaCursor+k]) )
							break;
					}
					printf("found beta(%c), (%d, %d) l:%d\n", Array2->array[nowBetaCursor], nowBetaCursor, j, k);
				}
				
				nowBeta.cursor = j;
				nowBeta.length = k;
				
				//현재 beta, gamma와 top beta, gamma 비교 
				if (nowBeta.length > 0 && 
					nowGamma.length+nowBeta.length > top.gamma.length + top.beta.length) {
					printf("now gamma,beta length : %d, %d\n", nowGamma.length, nowBeta.length);
					top.gamma = nowGamma;
					top.beta = nowBeta;
				}
				else if (nowBeta.length > 0 && 
						 nowGamma.length+nowBeta.length == top.gamma.length + top.beta.length) {
					Substring nowBetaGamma = stringMerge(&nowBeta, &nowGamma);
					Substring topBetaGamma = stringMerge(&top.beta,&top.gamma);
					
					if (compareString(&nowBetaGamma, &topBetaGamma)) {
						top.gamma = nowGamma;
						top.beta = nowBeta;
					}
					free(nowBetaGamma.target->array);
					free(topBetaGamma.target->array);
				}
				
				j += k;
			}
			
			
			//최대 길이 gamma서 beta 검색 실패 시 gamma 길이 줄이며 검색 
			if (nowBeta.length == 0) {
				for(j=1;nowGamma.length-j>0;j++) {
					int flag = 0;
					
					int nowBetaCursor = nowGamma.cursor + nowGamma.length - j;
					
					for(k=nowBetaCursor+1;k<Array2->length;k++) {
						if (Array2->array[k] == Array2->array[nowBetaCursor]) {
							nowGamma.length -= j;
							
							nowBeta.cursor = k;
							nowBeta.length = 1;
							
							
							if (nowGamma.length+nowBeta.length > top.gamma.length + top.beta.length) {
								top.gamma = nowGamma;
								top.beta = nowBeta;
							}
							else if (nowBeta.length > 0 && 
									 nowGamma.length+nowBeta.length == top.gamma.length + top.beta.length) {
								Substring nowBetaGamma = stringMerge(&nowBeta, &nowGamma);
								Substring topBetaGamma = stringMerge(&top.beta,&top.gamma);
								
								if (compareString(&nowBetaGamma, &topBetaGamma)) {
									top.gamma = nowGamma;
									top.beta = nowBeta;
								}
								free(nowBetaGamma.target->array);
								free(topBetaGamma.target->array);
							}
							
							flag = 1;
							break;
						}
						if(flag) {
							break;
						}
					}
				}
			}
		}
	}
	return top;
}


void search(CharArrayWrapper* TargetArray, CharArrayWrapper* Array2) {
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
//				GammaBetaWrapper resultBGB = searchBeta_BGB(&alphaCursors, Array2);
				
				//~ alpha ~ gamma-alpha ~ case
				printf("\nalpha : (%d,%d) l:%d\n",i,j,alphaLength);
				GammaBetaWrapper result = searchBeta_GBB(&alphaCursors, Array2);
				printf("==result beta : %d, l:%d\n\n", result.beta.cursor, result.beta.length);
				
				//legacy code
//				if (nowGammaBeta.gamma.target != NULL) {
//					printf(" GAMMA : %d(%c) l : %d \n alpha : %d(%c) l : %d \n beta: %d(%c) l : %d\n", 
//					nowGammaBeta.gamma.cursor, nowGammaBeta.gamma.target->array[nowGammaBeta.gamma.cursor], nowGammaBeta.gamma.length, 
//					alphaCursors.front, alphaCursors.target->array[alphaCursors.front], alphaCursors.length,
//					nowGammaBeta.beta.cursor, nowGammaBeta.beta.target->array[nowGammaBeta.beta.cursor], nowGammaBeta.beta.length);
//					
//					if (alphaCursors.length + nowGammaBeta.gamma.length + nowGammaBeta.beta.length > topLength) {
//						topLength = alphaCursors.length + nowGammaBeta.gamma.length + nowGammaBeta.beta.length;
//						topAlphaInfo = alphaCursors;
//						topGammaBetaInfo = nowGammaBeta;
//					}
//				}
//				
				
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
	
//	return result;
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
	search(&InputArray1,&InputArray2);
//	if (result.array != NULL) {
//		printf("result.length : %d\n",result.length);
//		for(i=0;i<result.length;i++) {
//			printf("%c", result.array[i]);
//		}
//		free(result.array);
//	}

	
	finish = clock();
	
//	printf("\n\n");
	
	//debug
	printf("\n");
	printf("시작 시간: %6ld(ms)\n", start);
	printf("종료 시간: %6ld(ms)\n", finish);
	printf("소요 시간: %6ld(ms)\n", finish - start); 
}
