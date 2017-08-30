#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <termios.h>

#define SERVER_IP           "10.27.6.40"
#define SERVER_PORT         54545
#define DATA_BUFFER_SIZE    1024

static struct termios old, new;

char getch(void);
char getch_(int echo);
char getch_(int echo);
void initTermios(int echo);

void handle_error(const char *error_string)
{
	//printf("Error: %s\nError code: %d\n", error_string, WSAGetLastError());
	//WSACleanup();
	printf("Press any key to exit from the program...");
	//while (!kbhit());
	//exit(EXIT_FAILURE);
}

/*void wsa_init()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
		handle_error("WSAStartup() ");
}*/

void connect_to_server(int *client_sock, unsigned int server_port, char *server_ip)
{
	// Creating client socket
	(*client_sock) = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	if (client_sock < 0)
		handle_error("socket() ");

	// Creating server address
	struct sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(server_port);
	addr_in.sin_addr.s_addr = inet_addr(server_ip);

	// Connecting the client socket to the server
	int connect_retval = connect(*client_sock, (struct sockaddr *)&addr_in, sizeof(addr_in));
	if (connect_retval < 0)
		handle_error("connect() ");

	printf("Connected to %s:%d\n", SERVER_IP, SERVER_PORT);
}

int send_message(int *socket)
{
	// Get the message from the user
	char msg[1];
	printf("\nEnter the message to send: ");
	msg[0] = (char)getch();
	// Send the message to the servers
	int sent_bytes = send(*socket, msg, strlen(msg), 0);
	if (sent_bytes < 0)
		handle_error("send() ");

	return sent_bytes;
}

int main()
{
	// Initialize the WSA
	//wsa_init();

	// Connect to server
	int client_socket;
    // Local variables used in the do-while loop
    int sent_bytes;
    int received_bytes;
    char recv_buff[DATA_BUFFER_SIZE];
	while(1){
        connect_to_server(&client_socket, SERVER_PORT, SERVER_IP);
        do {
            // Send data to the server
            sent_bytes = send_message(&client_socket);
            // Receive the answer if message was sent
            /*if (sent_bytes > 0) {
                received_bytes = recv(client_socket, recv_buff, DATA_BUFFER_SIZE, 0);
                // Error handling
                if (received_bytes < 0) {
                    handle_error("recv() ");
                } else {
                    // Printing out received string
                    recv_buff[received_bytes] = '\0';
                    printf("Received string: %s\n", recv_buff);
                }
            }*/
        } while (sent_bytes > 0);

        printf("Closing the socket...\n");
        close(client_socket);
        printf("Cleaning up memory...\n");
        //WSACleanup();
	}
	return 0;

}

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &old);
}
char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void)
{
  return getch_(0);
}
