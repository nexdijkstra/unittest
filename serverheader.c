#include"serverheader.h"

/* 에러에 따라 에러 코드와 문구를 저장하는 함수*/
void handleError(int Errorcode, int* code, char* phrase)
{
				switch(Errorcode)
				{
								case 200: //승인 일 때
												*code = CODE200;
												strcpy(phrase, PHRASE200);
												break;
								case 400://bad request일 때
												*code = CODE400;
												strcpy(phrase, PHRASE400);
												break;
								case 404://없는 파일 요청시
												*code = CODE404;
												strcpy(phrase, PHRASE404);
												break;
				}
}

char* getURL(char* origindata)
{
				int n;
				char *p;
				char rcvBuf[BUFSIZ+1];
				char *url = (char*)malloc(sizeof(char) * URLSIZ);

				memset(rcvBuf, 0, BUFSIZ+1);

				strcpy(rcvBuf, origindata); //메게 변수 원본을 보호하기 위해 복사

				p = strtok(rcvBuf, " "); //GET 분리

				/*NULL 값 반환시 bad request로 취급 GET이 아니면 bad request*/
				if(strcmp(p, "GET"))
				{
								return NULL;
				}

				/*파일 주소 짜르기*/
				p = strtok(NULL, " ");

				if(!strcmp(p, "/"))//아무것도 없는 /이면 기본 주소
				{
								strcpy(url, "index.html");
				}else{
								char strBuffer[URLSIZ+1] = {0, };
								int i = 0, flag = 0;

								strcpy(strBuffer, p);

								for(i = 0; i < URLSIZ; i++)
								{
												if(strBuffer[i] == '/')//문자열중 /제거
												{
																strcpy(url, strBuffer+(i+1));
																flag = 1;
												}
								}

								if(flag == 0)
								{
												strcpy(url, strBuffer);
								}
				}

				p = strtok(NULL, " ");

				return url;
}

