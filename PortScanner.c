#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<ws2tcpip.h>
#include<string.h>
#include<pthread.h>

//reminder : gotta include -lws2_32 when compiling the program from terminal

struct range {
    int start;
    int end;
    char ip[64];
};
void* thread_scan(void* arg)
{
 struct range* r = (struct range*)arg;
 for(int i = r->start; i <= r->end; i++)
    {
        int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in route;
        route.sin_family = AF_INET;
        route.sin_port = htons(i);
        route.sin_addr.s_addr = inet_addr(r->ip);
        if (route.sin_addr.s_addr == INADDR_NONE) 
        {  printf("invalid IP address\n");}
        int connection = connect(tcp_socket, (struct sockaddr*) &route, sizeof(route));
        if (connection == 0) 
        {printf("Port %d is open\n", i); }

        closesocket(tcp_socket);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    WSADATA wsadata;
    int wsaresult = WSAStartup(MAKEWORD(2, 2), &wsadata );
    if (wsaresult != 0 )
    {printf("WSAStartup failed\n");}

    if (stricmp(argv[1], "TCP") == 0)
    {
        char *portrange = strchr(argv[3], '-');
        if (portrange == NULL)
        {
            char *portpointer;
            int port = strtol(argv[3], &portpointer, 10);
            if (*portpointer != '\0'){printf("invalid port or port range");}

            int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

            struct sockaddr_in route;
            route.sin_family = AF_INET;
            route.sin_port = htons(port);
            route.sin_addr.s_addr = inet_addr(argv[2]);
            if (route.sin_addr.s_addr == INADDR_NONE) 
            { printf("invalid IP address\n");}

            int connection = connect(tcp_socket, (struct sockaddr*) &route, sizeof(route));
            if (connection != 0) {printf("Port %d is closed or filtered\n", port);}
             else
            { printf("Port %d is open\n", port);}
            closesocket(tcp_socket);
        }

        else if (portrange != NULL)
        {
            size_t start_length = portrange - argv[3];
            char start_port[start_length + 1];
            strncpy(start_port, argv[3], start_length);
            start_port[start_length] = '\0';   

            char *pointer1;
            int startport = strtol(start_port, &pointer1, 10);
            if(*pointer1 != '\0'){printf("invalid start port");}

            char end_port[32];
            strcpy(end_port, portrange + 1);
            char *pointer2;
            int endport = strtol(end_port, &pointer2, 10);
            if(*pointer2 != '\0'){printf("invalid end port");}

            int total = endport - startport + 1;
            int chunksize = total / 10;

            pthread_t threads[10];
            struct range jobs[10];

            for (int t = 0; t < 10; t++)
            {
                jobs[t].start = startport + t * chunksize;
                
                if (t == 9)
                    jobs[t].end = endport; 
                else
                    jobs[t].end = startport + (t + 1) * chunksize - 1;

                strcpy(jobs[t].ip, argv[2]);

                pthread_create(&threads[t], NULL, thread_scan, &jobs[t]);
            }

            for (int t = 0; t < 10; t++)
            {
                pthread_join(threads[t], NULL);
            }
        }
    }
    else(printf("invalid command"));

    return 0;
}