#include"serverheader.h"


/* getURL 함수를 테스트하는 함수
 * 2번째 파라미터 = getURL에 넣을 인풋
 * 3번째 파라미터 = 예상되는 답변
 */
int assertUrlTrue(char* (*getURL)(char*), char* input,  char* answer);

/*handleError를 테스트하는 함수
 * 2번째 파라미터 = handleError에 넣을 인풋
 * 3번째 파라미터 = 예상 코드 답변
 * 4번째 파라미터 = 예상 문장
 */
int assertCodeTrue(void (*handleError)(int, int*, char*), int e_code, 
								int c_answer, char* p_answer);

int main(int argc, char* argv[])
{


				if(assertCodeTrue(handleError, 200, CODE200, PHRASE200)) //200 코드를 넣고 200코드와 "OK"가 기대값으로 주어졌을 때
				{
								printf("Right answer\n");
				}else
				{
								printf("Wrong answer\n");
				}

				if(assertCodeTrue(handleError, 300, CODE404, PHRASE404))//300 코드를 넣고 404코드와 "FILE NOT FOUND"가 기대값으로 주어졌을 때
				{
								printf("Right answer\n");
				}else
				{
								printf("Wrong answer\n");
				}


				if(assertUrlTrue(getURL, "GET /index.html HTTP/1.1", "index.html"))//GET 헤더를 넣고 index.html이 반환되는 걸 기대
				{
								printf("Right answer\n");
				}else
				{
								printf("Wrong answer\n");
				}

				if(assertUrlTrue(getURL, "get", NULL))//get을 넣고 NULL을 기대하였을 때
				{
								printf("Right answer\n");
				}else
				{
								printf("Wrong answer\n");
				}
				return 0;
}

int assertCodeTrue(void (*handleError)(int, int*, char*), int e_code,
								int c_answer, char* p_answer)
{

				int code = 0;
				char phrase[BUFSIZ] = {'\0', };

				(*handleError)(e_code, &code, phrase);//handleError 함수를 호출하여 code와 pharse에 리턴값 저장


				if((code == c_answer) && (strcmp(phrase, p_answer) == 0))//기대 결과와 리턴값이 맞는지 판별
				{
								printf("handleError 함수, 기대 결과 %d, 아웃풋 %d\n", e_code, c_answer);
								return 1;
				}else
				{
								printf("handleError 함수, 기대 결과 %d, 아웃풋 %d\n", e_code, c_answer);
								return 0;
				}
}
int assertUrlTrue(char* (*getURL)(char* origindata), char* input ,char* answer)
{
				char buffer[BUFSIZ] = {'\0', };
			
				strcpy(buffer, input);

				if(getURL(buffer) != NULL)//getURL에 넣은 헤더를 분석한 값이 NULL이 아니면
				{
								if(strcmp(getURL(buffer), answer) == 0)//기대값과 비교하여 맞는지 확인
								{
												printf("getURL 함수, 기대 결과 %s, 아웃풋 %s\n", answer, getURL(buffer));
												return 1;
								}else
								{
												printf("getURL 함수, 기대 결과 %s, 아웃풋 %s\n", answer, getURL(buffer));
												return 0;
								}	
				}else
				{
								if(answer == NULL)//기대값도 NULL인이 확인
								{
												printf("getURL 함수, 기대 결과 %s, 아웃풋 %s\n", answer, getURL(buffer));
												return 1;
								}else
								{
												printf("getURL 함수, 기대 결과 %s, 아웃풋 %s\n", answer, getURL(buffer));
												return 0;
								}
				}
}
