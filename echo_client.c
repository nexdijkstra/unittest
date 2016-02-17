#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>



int main(int argc, char* argv[])
{
				int c_socket, s_socket;
				int c_fd;
				int c_n, s_n,  len, n_left = 0, n_recv, one = 1;
				struct sockaddr_in s_addr, c_addr;
				char s_Buffer[BUFSIZ] = {'\0', }, c_Buffer[BUFSIZ] = {'\0', };
				socklen_t sin_len = sizeof(c_addr);

								if((s_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
								{
												printf("can't open socket\n");
												return 0;
								}
							
								setsockopt(s_socket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

								if((c_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)//소켓 생성
								{
												printf("can't open socket\n");
												return -1;
								}

								memset(&c_addr, 0, sizeof(c_addr));
								memset(&s_addr, 0, sizeof(s_addr));

								/*AF_INET IPv4 주소
								 *inet_addr = IPv4를 포함한 string을 알맞은 주소로 변환
								 * */
				
								c_addr.sin_addr.s_addr = inet_addr("52.192.132.151");
								c_addr.sin_family = AF_INET;
								c_addr.sin_port = htons(atoi(argv[1]));

								s_addr.sin_family = AF_INET;
								s_addr.sin_addr.s_addr = INADDR_ANY;
								s_addr.sin_port = htons(atoi(argv[1]));

								if(bind(s_socket, (struct sockaddr*)&s_addr, sizeof(s_addr)) == -1)
								{
												close(s_socket);
												return 0;
								}

								listen(s_socket, 10);

				
								if((connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr)) == -1)  &&
																((c_fd = accept(s_socket, (struct sockaddr*)&c_addr, &sin_len)) == -1)) 
								{
												printf("Can't connect\n");
												close(c_socket);
												return -1;
								}

								//표준입출력에서 키보드 입력 받는다.
								//read의 return value = 실패 -1, 성공  읽어들인 바이트 수
								if((c_n = read(0, c_Buffer, BUFSIZ)) > 0)
								{
												c_Buffer[c_n] = '\0';//null문자 삽입
												
												if(!strcmp(c_Buffer, "quit\n"))
												{
															printf("Exit program\n");
															return 0;
												}

												printf("original Data : %s", c_Buffer);
								}

								if(((c_n = write(c_socket, c_Buffer, BUFSIZ)) < 0) &&
																(((s_n = read(c_fd, s_Buffer, BUFSIZ)) < 0)))
								{

												printf("Can't trans\n");
												return 0;
								}


								if(((s_n = write(c_fd, s_Buffer, BUFSIZ) < 0)) &&
																((c_n = read(c_socket, c_Buffer, BUFSIZ)) < 0))
								{

								}

								printf("echoed data : %s", c_Buffer);

								return 0;
}


