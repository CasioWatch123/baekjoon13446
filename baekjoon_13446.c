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
	char* array = (char*)malloc(sizeof(char) * (F->length + R->length));
	int i;
	for(i=0;i<F->length;i++) {
		array[i] = F->target->array[F->cursor+i];
	}
	for(i=0;i<R->length;i++) {
		array[F->length+i] = R->target->array[R->cursor+i];
	}
	
	CharArrayWrapper* target = (CharArrayWrapper*)malloc(sizeof(CharArrayWrapper));
	target->array = array;
	target->length = F->length+R->length;
	
	Substring result = {target, 0, target->length};
	return result;
}

int compareString(Substring* String, Substring* ProtoString) {
	if (String->target != NULL && String->length > ProtoString->length) {
//		printf("(compareString)method, return type 1\n"); //debug
		return 1;
	}
	else if(String->target != NULL && String->length == ProtoString->length) {
		int i;
		for(i=0;i<String->length;i++) {
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




GammaBetaWrapper searchBeta_BB(CharArrayWrapper* Array2) {
	GammaBetaWrapper top = {{NULL, 0, 0}, {NULL, 0, 0}};
	
	int i,j,k;
	for(i=0;i<Array2->length-1;i++) {
		for(j=i+1;j<Array2->length;j++) {
			if (Array2->array[i] == Array2->array[j] && (i == 0 || Array2->array[i-1] != Array2->array[j-1])) {
				for(k=1; ;k++) {
					if (
					!(j+k < Array2->length) ||
					!(i+k < j) ||
					!(Array2->array[i+k] == Array2->array[j+k]) )
						break;
				}
				Substring nowBeta = {Array2, i, k};
				
				if (compareString(&nowBeta, &top.beta)) {
					top.beta = nowBeta;
				}
			}
		}
	}
	
	return top;
}

GammaBetaWrapper searchBeta_BGB(CursorsWrapper* Cursors, CharArrayWrapper* Array2) { //반환 배열 구성 : gamma beta 순
//	printf("(searchBeta_BGB)method entring Cursors front:rear %d:%d\n", Cursors->front, Cursors->rear);
	GammaBetaWrapper top = {{NULL, 0, 0}, {NULL, 0, 0}};

	Substring array1Gamma;
	array1Gamma.target = Cursors->target;
	array1Gamma.cursor = Cursors->front + Cursors->length;
	array1Gamma.length = -1;

 	CharArrayWrapper* Array1 = Cursors->target;
 	
// 	printf("gamma.cursor : %d (Cursor->length : %d)\n", array1Gamma.cursor, Cursors->length);
 	
 	if (Cursors->gap-Cursors->length < 1) { //alpha-alpha case
 		return top;
	}
	
	
//	printf("\n");
	int i,j,k;
	for(i=1;i<Array2->length-1;i++) {
//		printf("%c(%d):%c(%d)\n", Array2->array[i],i, array1Gamma.target->array[array1Gamma.cursor],array1Gamma.cursor);
		if (Array2->array[i] == array1Gamma.target->array[array1Gamma.cursor]) {
			for(j=1; ;j++) {
				if (
				 !(i+j < Array2->length-1) ||
				 !(Cursors->length+j < Cursors->gap) || 
				 !(array1Gamma.target->array[array1Gamma.cursor+j] == Array2->array[i+j]) )
					break;
			}
			
//			printf("  tGL : %d\n",j);
			
			Substring nowGamma = {Array2, i, j};
			Substring nowBeta = {Array2, 0, 0};
			int nowBetaCursor = nowGamma.cursor + nowGamma.length;
			
			for(j=0;j<nowGamma.cursor;j++) {//gamma가 최대 길이일 때 beta 검색 시도 
				k = 0;
				if(Array2->array[j] == Array2->array[nowBetaCursor]) {
					for(k=1; ;k++) {
						if (
						 !(j+k < nowGamma.cursor) || 
						 !(Array2->array[j+k] == Array2->array[nowBetaCursor+k]) )
							break;
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
						free(nowBetaGamma.target);
						
						free(topBetaGamma.target->array);
						free(topBetaGamma.target);
					}
					
					j += k; //마법의 최적화 로직 
				}
			}
			
			
			//최대 길이 gamma서 beta 검색 실패 시 gamma 길이 줄이며 검색 
			if (top.beta.length == 0) { 
				for(j=1;nowGamma.length-j>0;j++) {
					int flag = 0;
					
					int nowBetaCursor = nowGamma.cursor + nowGamma.length - j;
					
					for(k=0;k<nowGamma.cursor;k++) {
						if (nowGamma.target->array[k] == nowGamma.target->array[nowBetaCursor]) {
							nowGamma.length -= j;
							
							nowBeta.cursor = k;
							nowBeta.length = 1;
							
							top.gamma = nowGamma;
							top.beta = nowBeta;
							
							return top;
						}
					}
				}
			}
		}
	}
	return top;
}

GammaBetaWrapper searchBeta_GBB(CursorsWrapper* Cursors, CharArrayWrapper* Array2) {
//	printf("(searchBeta_GBB)method entring Cursors front:rear %d:%d\n", Cursors->front, Cursors->rear);
	GammaBetaWrapper top = {{NULL, 0, 0}, {NULL, 0, 0}};
	
	Substring array1Gamma;
	array1Gamma.target = Cursors->target;
	array1Gamma.cursor = Cursors->rear + Cursors->length;
	array1Gamma.length = -1;
	
	CharArrayWrapper* Array1 = Cursors->target;
	
//	printf("gamma.cursor : %d l:%d\n", array1Gamma.cursor, Cursors->length);
	
	if (Cursors->rear + Cursors->length >= Cursors->target->length) {
		return top;
	}
	
	
//	printf("\n");
	int i,j,k;
	for(i=0;i<Array2->length-2;i++) {
//		printf("%c(%d):%c(%d)\n", Array2->array[i],i, array1Gamma.target->array[array1Gamma.cursor],array1Gamma.cursor);
		if (Array2->array[i] == array1Gamma.target->array[array1Gamma.cursor]) {
			for(j=1; ;j++) {
				if (
				 !(i+j < Array2->length-2) ||
				 !(Cursors->rear+Cursors->length+j < Cursors->target->length) || 
				 !(array1Gamma.target->array[array1Gamma.cursor+j] == Array2->array[i+j]) )
					break;
			}
			
//			printf("  tGL : %d\n",j);
			
			Substring nowGamma = {Array2, i, j};
			Substring nowBeta = {Array2, 0, 0};
			int nowBetaCursor = nowGamma.cursor + nowGamma.length;
			
			for(j=nowBetaCursor+1;j<Array2->length;j++) {//gamma가 최대 길이일 때 beta 탐색 
				k=0;
				if (Array2->array[j] == Array2->array[nowBetaCursor]) {
					for(k=1; ;k++) {
						if (
						 !(nowBetaCursor+k < j) || 
						 !(Array2->array[j+k] == Array2->array[nowBetaCursor+k]) )
							break;
					}
					nowBeta.cursor = j;
					nowBeta.length = k;
					
					//현재 beta, gamma와 top beta, gamma 비교 
					if (nowBeta.length > 0 && 
						nowGamma.length+nowBeta.length > top.gamma.length + top.beta.length) {
	//					printf("now gamma,beta length : %d, %d\n", nowGamma.length, nowBeta.length);
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
						free(nowBetaGamma.target);
						
						free(topBetaGamma.target->array);
						free(topBetaGamma.target);
					}
					
					j += k;
				}
			}
			
			
			//최대 길이 gamma서 beta 검색 실패 시 gamma 길이 줄이며 검색 
			if (top.beta.length == 0) {
				for(j=1;nowGamma.length-j>0;j++) {
					int flag = 0;
					
					int nowBetaCursor = nowGamma.cursor + nowGamma.length - j;
					
					for(k=nowBetaCursor+1;k<Array2->length;k++) {
						if (Array2->array[k] == Array2->array[nowBetaCursor]) {
							nowGamma.length -= j;
							
							nowBeta.cursor = k;
							nowBeta.length = 1;
							
							top.gamma = nowGamma;
							top.beta = nowBeta;
							
							return top;
						}
					}
				}
			}
		}
	}
	return top;
}



Substring search(CharArrayWrapper* TargetArray, CharArrayWrapper* Array2) {
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
				
				GammaBetaWrapper resultArray[3];
				
				resultArray[0] = searchBeta_BGB(&alphaCursors, Array2);
				resultArray[1] = searchBeta_GBB(&alphaCursors, Array2);
				resultArray[2] = searchBeta_BB(Array2);
				
				for(k=0;k<3;k++) {
					if (resultArray[k].beta.length > 0) {
						if (alphaLength+resultArray[k].gamma.length+resultArray[k].beta.length > topLength) {
							topLength = alphaLength+resultArray[k].gamma.length+resultArray[k].beta.length;
							
							topAlphaInfo = alphaCursors;
							topGammaBetaInfo = resultArray[k];
//							printf("k:%d\n", k);
//							printf("Alpha Info : %d-l%d %d\n", alphaCursors.front, alphaCursors.length, alphaCursors.rear);
//							printf("GammaBeta Info : %d-l%d %d-l%d\n", topGammaBetaInfo.gamma.cursor, topGammaBetaInfo.gamma.length, topGammaBetaInfo.beta.cursor,topGammaBetaInfo.beta.length);
						}
						else if (alphaLength+resultArray[k].gamma.length+resultArray[k].beta.length == topLength) {
							Substring nowAlpha = {alphaCursors.target, alphaCursors.front, alphaCursors.length};
							Substring nowGammaBeta = stringMerge(&resultArray[k].gamma, &resultArray[k].beta);
							Substring nowString = stringMerge(&nowAlpha, &nowGammaBeta);
							
							Substring topAlpha = {topAlphaInfo.target, topAlphaInfo.front, topAlphaInfo.length};
							Substring topGammaBeta = stringMerge(&topGammaBetaInfo.gamma, &topGammaBetaInfo.beta);
							Substring topString = stringMerge(&topAlpha, &topGammaBeta);
							
							if (compareString(&nowString, &topString)) {
								topAlphaInfo = alphaCursors;
								topGammaBetaInfo = resultArray[k];
							}
							
							free(nowGammaBeta.target->array);
							free(nowGammaBeta.target);
							free(nowString.target->array);
							free(nowString.target);
							
							free(topGammaBeta.target->array);
							free(topGammaBeta.target);
							free(topString.target->array);
							free(topString.target);
						}
					}
				}
			}
		}
	}
	
	if (topLength > 0) {
		Substring alpha = {TargetArray, topAlphaInfo.front, topAlphaInfo.length};
		Substring gammaBeta = stringMerge(&topGammaBetaInfo.gamma, &topGammaBetaInfo.beta);
		Substring result = stringMerge(&alpha, &gammaBeta);
		
		free(gammaBeta.target->array);
		free(gammaBeta.target);
		
		return result;
	}
	
	Substring result = {NULL, 0, 0};
	return result;
}



int main(void) {
	long start, finish;

	char array1[48] = {};
	char array2[48] = {};
	
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
	Substring result = search(&InputArray1,&InputArray2);
	if (result.target != NULL) {
		for(i=0;i<result.length;i++) {
			printf("%c", result.target->array[i]);
		}
		printf("");
		free(result.target->array);
		free(result.target);
	}
	
	else {
		printf("-1");
	}
	
	finish = clock();
	
//	printf("\n\n");
//	
//	//debug
//	printf("\n");
//	printf("시작 시간: %6ld(ms)\n", start);
//	printf("종료 시간: %6ld(ms)\n", finish);
//	printf("소요 시간: %6ld(ms)\n", finish - start); 
}
