#include<stdio.h>
#include<stdlib.h>

typedef struct {
	char* array;
	int length;
}CharArrayWrapper;

typedef struct {
	CharArrayWrapper target;
	int cursor;
	int length;
}Substring;

typedef struct {
	Substring strarr[3];
	int length;
}SubstringArrayWrapper;

typedef struct {
	int front;
	int rear;
}GammaIndex;


int multiStringCompare(SubstringArrayWrapper* Strings1, SubstringArrayWrapper* Strings2) {
	char array1[48] = {};
	char array2[48] = {};
	Substring string1 = {{array1},0,0};
	Substring string2 = {{array2},0,0};
	
	int i,j,cursor;
	
	for(i=0;i<Strings1->length;i++) {
		Substring nowString = Strings1->strarr[i];
		for(j=0;j<Strings1->strarr[i].length;j++) {
			string1.target.array[string1.length++] = nowString.target.array[nowString.cursor+j];
		}
	}

	for(i=0;i<Strings1->length;i++) {
		Substring nowString = Strings2->strarr[i];
		for(j=0;j<Strings2->strarr[i].length;j++) {
			string2.target.array[string2.length++] = nowString.target.array[nowString.cursor+j];
		}
	}
	
	
	if (string1.length > string2.length) {
		return 1;
	}
	else if (string1.length == string2.length) {
		for(i=0;i<string1.length;i++) {
			if (string1.target.array[string1.cursor+i] < string2.target.array[string2.cursor+i]) {
				return 1;
			}
			else if (string1.target.array[string1.cursor+i] > string2.target.array[string2.cursor+i]) {
				return 0;
			}
		}
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
			if (New->target.array[i] < Proto->target.array[i])
				return 1;
			if (New->target.array[i] > Proto->target.array[i])
				return 0;
		}
	}
	
	return 0;
}


Substring stringMerge(Substring* F, Substring* R) {
	Substring result;
	result.cursor = 0;
	result.length = F->length + R->length;
	
	result.target.length = result.length;
	result.target.array = (char*)malloc(sizeof(char) * (result.length + 1));
	
	int i;
	
	for(i=0;i<F->length;i++) {
		result.target.array[i] = F->target.array[F->cursor+i];
	}
	for(i=0;i<R->length;i++) {
		result.target.array[F->length+i] = R->target.array[R->cursor+i];
	}
	
	return result;
}




Substring searchAlpha(CharArrayWrapper* InputArray, int GammaCursor, GammaIndex* GammaInfo) {
	
}

Substring searchBeta(CharArrayWrapper* InputArray, int GammaCursor, GammaIndex* GammaInfo) {
	int betaCursor = GammaCursor + GammaInfo->rear + 1;
	
	Substring topBeta = {*InputArray, 0, 0};
	
	
	int i,j;
	for(i=0;i<InputArray->length;i++) {
		if (
		(i < GammaCursor || i > betaCursor) &&
		(InputArray->array[i] == InputArray->array[betaCursor]) ) {//gamma, gamma 바로 뒤 betaCursor 범위 
			for(j=1; ;j++) {
				if (
				!(i+j < GammaCursor || betaCursor+j < InputArray->length) || 
				!(InputArray->array[i+j] == InputArray->array[betaCursor+j]) )
					break;
			}
			
			Substring nowBeta = {*InputArray, i, j};
			
			if (singleStringCompare(&nowBeta,&topBeta)) {
				topBeta = nowBeta;
			}
		}
	}
	
	if (topBeta.length != 0)
		return topBeta;
	
	//최대 길이 gamma에서 검색 실패 시 gamma 길이 줄이며 검색 시도 
	while(GammaInfo->rear-- > GammaInfo->front){
		int front = GammaCursor + GammaInfo->front;
		int rear = GammaCursor + GammaInfo->rear;
		
		int nowBetaCursor = rear + 1;
		
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
Substring searchMain(CharArrayWrapper* InputArray1, CharArrayWrapper* InputArray2) {
	int i,j,k;
	for(i=1;i<InputArray1->length;i++) {
		for(j=0;j<InputArray2->length-1;j++) {
			if (InputArray1->array[i] == InputArray2->array[j] && 
			((i == 0 || j == 0) || InputArray1->array[i-1] == InputArray2->array[j-1])) {
				//alpha, beta 검색 로직 
				for(k=1; ;k++) {
					if (
					 !(i+k < InputArray1->length) ||
					 !(j+k < InputArray2->length) ||
					 !(InputArray1->array[i+k] == InputArray2->array[j+k]) )
						break;
				}
				
				GammaIndex gammaIndex = {0,k-1};
				
				Substring alpha = searchAlpha(InputArray1, i, &gammaIndex);
				Substring beta = searchBeta(InputArray2, j, &gammaIndex);
				
			}
		}
	}
}
int main(void) {
//	char a1[5];
//	char a2[5];
//	char a3[5];
//	char a4[5];
//	scanf("%s",a1);
//	scanf("%s",a2);
//	scanf("%s",a3);
//	scanf("%s",a4);
//	SubstringArrayWrapper wrapper1 = {{{{a1, 4}, 0, 4}, {{a2, 4}, 0, 4}}, 2};
//	SubstringArrayWrapper wrapper2 = {{{{a3, 4}, 0, 4}, {{a4, 4}, 0, 4}}, 2};
	char InputArray[48] = {};
	scanf("%s", InputArray);
	int i;
	for(i=0;InputArray[i];i++);
	
	CharArrayWrapper wrapper = {InputArray, i};
	
	GammaIndex idx = {0,2};
	Substring a = searchBeta(&wrapper, 2, &idx);
	
	for(i=0;i<a.length;i++)
		printf("%c", a.target.array[a.cursor+i]);
	return 0;
}
