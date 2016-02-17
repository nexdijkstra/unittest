#include "constheader.h"
#include "serverheader.h"

/*파일 확장자 찾기 위한 구조체 
 * ext = 확장자
 * filetype = 헤더에 보내기 위한 타입
 */
struct {
				char* ext;
				char* filetype;
}extensions[] = {
				{"gif", "image/gif"},
				{"jpg", "image/jpeg"},
				{"jpeg", "image/jpeg"},
				{"png", "image/png"},
				{"zip", "image/zip"},
				{"gz", "image/gz"},
				{"tar", "image/tar"},
				{"html", "text/html"},
				{"htm", "text/html"},
				{0, 0} };


int main(int argc, char* argv[])
{
				FILE* indexfile;
				int client_fd, fd;
				int sock;
				int one=1;
				int code = 200;
				char phrase[20] = "OK";
				char sendBuf[BUFSIZ+1] = {'\0', }, url[URLSIZ] = {'\0', };
				struct sockaddr_in svr_addr, cli_addr;
				
				socklen_t sin_len = sizeof(cli_addr);

				if(argc < 2) //port number가 없으면
				{
								printf("Enter a port number\n");
								return 0;
				}else{ //port number를 받아왔다면 소켓에 주소를 입력시킨다.
								svr_addr.sin_family=AF_INET;
								svr_addr.sin_addr.s_addr=INADDR_ANY;
								svr_addr.sin_port=htons(atoi(argv[1]));
				}

				if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) //소켓을 생성한다.
				{
								printf("can't open socket\n");
								return 0;
				}

				setsockopt(sock , SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

				if(bind(sock, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) == -1) //생성한 소켓을 바운드한다.
				{
								close(sock);
								printf("can't bind\n");
								return 0;
				}

				listen(sock, 10);

				while(1)
				{
								char buf[BUFSIZ+1];
								char* tempURL; //URL이 NULL인지 판별(wrong request 판별)
								int n = 0;
								memset(buf, 0, BUFSIZ+1); //버프 초기

								if((client_fd = accept(sock, (struct sockaddr *)&cli_addr, &sin_len)) == -1) //접속 판별
								{
												printf("connect error\n");
												continue;
								}

								if((n = read(client_fd, buf, BUFSIZ+1)) <= 0)
								{
												printf("Can't receive data from web\n");
												return 0;
								}

								/*get URL 은 NULL이나 URL을 반환하는데 NULL은 wrong request, 올바른 request일때 URL을 반환*/
								tempURL = getURL(buf); 

								if(tempURL == NULL)  //만약 클라이언트한테서 잘못된 request를 받으면
								{
												handleError(400, &code, phrase);//에러 400을 출력
												sprintf(sendBuf, "HTTP/1.1 %d %s\r\n\r\n", code, phrase);
												printf("%s\n",sendBuf);
												n = write(client_fd, sendBuf, strlen(sendBuf));
								}else
								{
												char Temptype[BUFSIZ+1] = {0, };
												char* t = NULL;
												int i = 0;

												strcpy(Temptype, tempURL);
												strcpy(url, tempURL);

												if((fd = open(url, O_RDONLY)) < 0) //없는 파일이면 404 에러
												{
																handleError(404, &code, phrase);
																sprintf(sendBuf, "HTTP/1.1 %d %s\r\n", code, phrase);
																n = write(client_fd, sendBuf, strlen(sendBuf));
												}else{
																t = strtok(Temptype, ".");//확장자 구하기
																t = strtok(NULL, " ");
																if(t != NULL)//확장자가 있으면
																{
																				for(i = 0; extensions[i].ext != 0; i++)
																				{
																								if(strcmp(t, extensions[i].ext) == 0)
																								{
																												sprintf(sendBuf, "HTTP/1.1 %d %s\r\n", code, phrase);
																												n = write(client_fd, sendBuf, strlen(sendBuf));

																												sprintf(Temptype, "Content-type: %s; charset=ko-KR\r\n", extensions[i].filetype);
																												n = write(client_fd, Temptype, strlen(Temptype));

																												strcpy(Temptype, "\r\n");
																												n = write(client_fd, Temptype, strlen(Temptype));
																												
																												while((n = read(fd, buf, BUFSIZ)) > 0)
																												{
																																write(client_fd, buf, n);
																												}
																								}
																				}
																}else//확장자가 없으면
																{
																				handleError(404,&code, phrase);
																				sprintf(sendBuf, "HTTP/1.1 %d %s\r\n", code, phrase);
																				n = write(client_fd, sendBuf, strlen(sendBuf));

																}
												}
								}
								/*초기화*/
								memset(sendBuf, '\0', strlen(sendBuf));
								memset(url, '\0', strlen(url));

								handleError(200, &code, phrase);
								close(client_fd);
								close(fd);
				}
				close(sock);
				return 0;
}

