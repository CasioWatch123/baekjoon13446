#include<stdio.h>
#include<stdlib.h>

typedef struct {
	char* array;
	int length;
}CharArrayWrapper;

typedef struct {
	char* array;
	int cursor;
	int length;
}Substring;

typedef struct {
	Substring array[3];
}SubstringArrayWrapper;

typedef struct {
	int front;
	int rear;
}GammaIndex;


int multiStringCompare(SubstringArrayWrapper* Strings1, SubstringArrayWrapper* Strings2) {
	char array1[48] = {};
	char array2[48] = {};
	
	int i,j;
	
	int cursor1 = 0;
	for(i=0;i<3;i++) {
		Substring nowString = Strings1->array[i];
		for(j=0;j<nowString.length;j++) {
			array1[cursor1++] = nowString.array[nowString.cursor+j];
		}
	}
	
	int cursor2 = 0;
	for(i=0;i<3;i++) {
		Substring nowString = Strings2->array[i];
		for(j=0;j<nowString.length;j++) {
			array2[cursor2++] = nowString.array[nowString.cursor+j];
		}
	}
	
	if (cursor1 > cursor2)
		return 1;
	else if(cursor1 < cursor2)
		return 0;
	
	for(i=0;i<48;i++) {
		if (array1[i] == 0) 
			return 0;
		if (array1[i] < array2[i]) 
			return 1;
		if (array1[i] > array2[i]) 
			return 0;
	}
	
	return 0;
}


int singleStringCompare(Substring* New, Substring* Proto) {
	if (New->length <= 0) {
		return 0;
	}
	
	if (New->length > Proto->length) {
		return 1;
	}
	
	if (New->length == Proto->length) {
		int i;
		for(i=0;i<New->length;i++) {
			if (New->array[i] < Proto->array[i])
				return 1;
			if (New->array[i] > Proto->array[i])
				return 0;
		}
	}
	
	return 0;
}




Substring stringMerge(Substring* F, Substring* R) {
	Substring result;
	result.cursor = 0;
	result.length = F->length + R->length;
	
	result.array = (char*)malloc(sizeof(char) * (result.length + 1));
	
	int i;
	
	for(i=0;i<F->length;i++) {
		result.array[i] = F->array[F->cursor+i];
	}
	for(i=0;i<R->length;i++) {
		result.array[F->length+i] = R->array[R->cursor+i];
	}
	
	return result;
}




Substring searchAlpha(CharArrayWrapper* InputArray, int GammaCursor, GammaIndex* GammaInfo) {
	Substring topAlpha = {InputArray->array, 0, 0};
	
	int front = GammaCursor + GammaInfo->front;
	int rear = GammaCursor + GammaInfo->rear;
	
	if (GammaCursor > 0) {
		printf("dd\n");
		int alphaCursor = front - 1;
		
		int i,j;
		for(i=0;i<InputArray->length;i++) {
			if (
			(i < alphaCursor || i > rear) &&
			(InputArray->array[i] == InputArray->array[alphaCursor]) ) {
				for(j=1; ;j++) {
					if (
					!(i - j >= 0) || 
					!(i - j != rear) || 
					!(alphaCursor - j >= 0) || 
					!(alphaCursor - j != i) || 
					!(InputArray->array[i-j] == InputArray->array[alphaCursor-j]) )
						break;
				}
				
				Substring nowAlpha = {InputArray->array, i - j + 1, j};
				
				if (singleStringCompare(&nowAlpha, &topAlpha)) {
					topAlpha = nowAlpha;
				}
				
				i += j-1;
			}
		}
		
		if (topAlpha.length != 0) 
			return topAlpha;
	}
	
	
	//최대 길이 gamma에서 검색 실패 시 gamma 길이 줄이며 검색 시도 
	while(GammaInfo->front < GammaInfo->rear) {
		GammaInfo->front++;
		front = GammaCursor + GammaInfo->front;
		rear = GammaCursor + GammaInfo->rear;
		
		int nowAlphaCursor = front-1;
		
		int j;
		for(j=0;j<InputArray->length;j++) {
			if (
			(j < nowAlphaCursor || j > rear) && 
			InputArray->array[j] == InputArray->array[nowAlphaCursor]) {
				topAlpha.cursor = j;
				topAlpha.length = 1;
				
				return topAlpha;
			}
		}
	}
	
	return topAlpha;
}

Substring searchBeta(CharArrayWrapper* InputArray, int GammaCursor, GammaIndex* GammaInfo) {
	Substring topBeta = {InputArray->array, 0, 0};
	
	int front = GammaCursor + GammaInfo->front;
	int rear = GammaCursor + GammaInfo->rear;
	
	if (rear + 1 < InputArray->length) {
		int betaCursor = rear + 1;
		
		int i;
		for(i=0;i<InputArray->length;i++) {
			if (
			(i < front || i > betaCursor) &&
			(InputArray->array[i] == InputArray->array[betaCursor]) ) {//gamma, gamma 바로 뒤 betaCursor 범위 
				int j=0;
				for(j=1; ;j++) {
					if (
					!(i + j < InputArray->length) || 
					!(i + j != front) || 
					!(betaCursor + j < InputArray->length) || 
					!(betaCursor + j != i) || 
					!(InputArray->array[i+j] == InputArray->array[betaCursor+j]) )
					break;
				}
				Substring nowBeta = {InputArray->array, i, j};
				
				if (singleStringCompare(&nowBeta, &topBeta)) {
					topBeta = nowBeta;
				}
				
				i += j-1;
			}
		}
		
		if (topBeta.length != 0)
			return topBeta;
	}
	
	
	//최대 길이 gamma에서 검색 실패 시 gamma 길이 줄이며 검색 시도 
	while(GammaInfo->rear > GammaInfo->front) {
		GammaInfo->rear--;
		front = GammaCursor + GammaInfo->front;
		rear = GammaCursor + GammaInfo->rear;
		
		int nowBetaCursor = rear + 1;
		
		int j;
		for(j=0;j<InputArray->length;j++) {
			if (
			(j < front || j > nowBetaCursor) &&
			InputArray->array[j] == InputArray->array[nowBetaCursor]) {
				topBeta.cursor = j;
				topBeta.length = 1;
				
				return topBeta;
			}
		}
	}
	
	return topBeta;
}


SubstringArrayWrapper searchMain(CharArrayWrapper* InputArray1, CharArrayWrapper* InputArray2) {
	SubstringArrayWrapper top = {};
	
	int i,j,k;
	for(i=1;i<InputArray1->length;i++) {
		for(j=0;j<InputArray2->length-1;j++) {
			if (InputArray1->array[i] == InputArray2->array[j] /*&& 
			((i == 0 || j == 0) || InputArray1->array[i-1] != InputArray2->array[j-1])*/) {
				for(k=1; ;k++) {
					if (
					 !(i+k < InputArray1->length) ||
					 !(j+k < InputArray2->length) ||
					 !(InputArray1->array[i+k] == InputArray2->array[j+k]) )
						break;
				}
								
				GammaIndex gammaIndex = {0,k-1};
				printf("\n%d %d\n", gammaIndex.front, gammaIndex.rear);
				Substring nowAlpha = searchAlpha(InputArray1, i, &gammaIndex);
				printf("%d %d\n", gammaIndex.front, gammaIndex.rear);
				Substring nowBeta = searchBeta(InputArray2, j, &gammaIndex);
				printf("%d:%d %d, %d %d\n", i, gammaIndex.front, gammaIndex.rear, nowAlpha.length, nowBeta.length);
				
				Substring nowGamma = {InputArray1->array, i+gammaIndex.front, gammaIndex.rear-gammaIndex.front+1};
				
				SubstringArrayWrapper wrapper = {{nowAlpha, nowGamma, nowBeta}};
				printf("%d %d %d\n", nowAlpha.length, nowGamma.length, nowBeta.length);
				
				if (
				nowAlpha.length > 0 && 
				nowBeta.length > 0 &&
				multiStringCompare(&wrapper, &top) ) {
					top = wrapper;
					
					int a,b;
					for(a=0;a<3;a++) {
						Substring nowString = top.array[a];
						for(b=0;b<nowString.length;b++) {
							printf("%c", nowString.array[nowString.cursor+b]);
						}
						printf(" ");
					}
					
					printf("\n");
				}
			}
		}
	}
	
	return top;
}
int main(void) {
	char inputArray1[48] = {};
	char inputArray2[48] = {};
	
	scanf("%s %s", inputArray1, inputArray2);
	
	int i,j;
	for(i=0;inputArray1[i];i++);
	CharArrayWrapper input1 = {inputArray1, i};
	for(i=0;inputArray2[i];i++);
	CharArrayWrapper input2 = {inputArray2, i};
	
	SubstringArrayWrapper result = searchMain(&input1, &input2);
	
	for(i=0;i<3;i++) {
		Substring nowString = result.array[i];
		for(j=0;j<nowString.length;j++) {
			printf("%c", nowString.array[nowString.cursor+j]);
		}
	}
	
	return 0;
}
