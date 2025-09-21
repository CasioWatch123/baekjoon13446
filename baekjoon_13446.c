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
 	GammaBetaWrapper result;
 	
 	result.gamma.target = NULL;
 	result.beta.target = NULL;
 
 	int gammaCursor = Cursors->front+Cursors->length;
 	CharArrayWrapper* Array1 = Cursors->target;
 	
 	if (Cursors->gap < 2) { //alpha-alpha case 		
 		return result;
	}
	 
	int i,j,k;
	for(i=1;i<Array2->length-1;i++) {
		if (Array2->array[i] == Array1->array[gammaCursor]) {
			for(j=1; ;j++) {
				if (
				 !(i+j < Array2->length-1) ||
				 !(Cursors->length+j < Cursors->gap) || 
				 !(Cursors->target->array[gammaCursor+j] == Array2->array[i+j]) )
					break;
			}
			int topGammaLength = j;
			
			int betaCursor = gammaCursor + topGammaLength;
			
			int topBetaCursor = -1;
			int topBetaLength = -1;
			
			for(j=0;j<gammaCursor;j++) {//gamma가 최대 길이일 때 beta 검색 시도 
				k = 0;
				
				if(Array2->array[j] == Array2->array[betaCursor]) {
					for(k=1; ;k++) {
						if (
						 !(j+k < gammaCursor) || 
						 !(Array2->array[j+k] == Array2->array[betaCursor+k]) )
							break;
					}
				}
				
				if(k > topBetaLength) {
					topBetaCursor = j;
					topBetaLength = k;
				}
				j += k; //마법의 최적화 로직 
			}
			
			if (topBetaLength > 0) { //최대 길이 gamma에서 beta 검색 성공
				Substring gamma = {Array2, gammaCursor, topGammaLength};
				Substring beta = {Array2, topBetaCursor, topBetaLength};
				
				result.gamma = gamma;
				result.beta = beta;
				
				return result;
			}
			
			for(j=1;topGammaLength-j>0;j++) {//gamma 길이 1씩 줄이며 beta 탐색 시도 
				int nowBetaCursor = gammaCursor + topGammaLength - j;
				
				for(k=0;k<gammaCursor;k++) {
					if(Array2->array[k] == Array2->array[nowBetaCursor]) {
						Substring gamma = {Array2, gammaCursor, topGammaLength-j};
						Substring beta = {Array2, k, 1};
						
						result.gamma = gamma;
						result.beta = beta;
						
						return result;
					}
				}
			}
			//탐색 실패로 for문 탈출 시 무효의 result 반환 
			return result;
		}
	}
}

CursorsWrapper search_AGA(CharArrayWrapper* TargetArray, CharArrayWrapper* Array2) {
	int i,j,k;
	Substring topString = {TargetArray, -1, -1};
	CursorsWrapper topCursors = {NULL};
	
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
				
				CursorsWrapper cursors = {TargetArray, i, j, alphaLength, j-i};
				//gamma beta search		
				//~ alpha-gamma ~ alpha ~ case
				GammaBetaWrapper a = searchBeta_BGB(&cursors, Array2);
				printf("gamma : %d(%c) %d, beta: %d(%c) %d\n", a.gamma.cursor, a.gamma.target->array[a.gamma.cursor], a.beta.cursor, a.beta.target->array[a.beta.cursor]);
			}
		}
	}
	return topCursors;
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
	search_AGA(&InputArray1,&InputArray2);
	

	
	finish = clock();
//	printf("\n\n");
	
	//debug
	printf("\n");
	printf("시작 시간: %6ld(ms)\n", start);
	printf("종료 시간: %6ld(ms)\n", finish);
	printf("소요 시간: %6ld(ms)\n", finish - start); 
}
