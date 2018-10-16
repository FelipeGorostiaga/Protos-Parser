#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>


#define MAXSPACES 10
#define CONTINUE 1
#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2


enum states {START,ERROR,WORD,GUION,HELP,QUIT,END,SPACE,LETTER};


int getLine(char *prmpt, char *buff, size_t sz);
int readCommands();
void welcome();
void getProxyVersion();
void clearScreen();
void printErrorMessage(int errorCode);
void handleCommandProxy(char *buff, int cmd, size_t  buffSize);

int main(void) {

    int option;
    welcome();
    while((option =readCommands()) ==  CONTINUE) {

    }

    return 0;
}

void clearScreen() {
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

void printHelp() {
    printf("HELP SCREEN \n");
    printf("Write one command per line \n");
    printf("Available commands: \n ");
    printf("-f \"filename\" Redirect metrics to filename \n");
    printf("-t \"command\" Configure proxy server to execute command on incoming mails from origin server \n");
    printf("-e \"errorfilename\" Redirect stderr to errorfilename \n");
    printf("-v Proxy server version \n");

    clearScreen();
}

void welcome() {
    printf("Welcome to proxy server administrator! \n");
    printf("Please insert command followed by <ENTER> \n");
}

void printErrorMessage(int errorCode) {
    switch(errorCode) {
        case 0: printf("Invalid format command.\n");break;
        case 1: printf("Invalid command, missing arguments.\n");break;
        case 2: printf("Invalid command, too many blank spaces.\n");break;
        case 3: printf("Invalid command, parameters can´t exceed 40 characters long.\n");break;
    }
}

int readCommands() {

    char buff[50];
    int errorNum;
    int invalidLetter;
    int strlen = 0;
    int spaceCount = 0;
    enum states state = START;
    int c,cmd,rt;

    while(state != END && (c = getchar()) != EOF &&  c != '\n' ) {

        switch(state) {

            case START:
                if (c == '-' ) state = GUION;
                else if (isspace(c)) {
                    state = SPACE;
                }
                else {
                    state = ERROR;
                    errorNum = 0;
                }
                break;

            case SPACE:
                if(isspace(c)) {
                    if(spaceCount < MAXSPACES)
                        spaceCount++;
                    else
                        state = ERROR;
                        errorNum = 2;
                }
                else if (c == '-')
                    state = GUION;
                else {
                    state = ERROR;
                    errorNum = 0;
                }
                break;

            case ERROR:
                //fflush(STDIN_FILENO);
                printErrorMessage(errorNum);
                state = END;
                break;

            case HELP:
                if(isspace(c)) {
                    printHelp();
                    state = START;
                    fflush(STDIN_FILENO);
                }
                else {
                    state = ERROR;
                    errorNum = 1;
                }
            case LETTER:
                if(!isspace(c)) {
                    state = ERROR;
                    errorNum = 1;
                }
                else
                    state = WORD;
                break;

            case WORD:
                rt = getLine(NULL,buff, sizeof(buff));
                if(rt == NO_INPUT ) {
                    state = ERROR;
                    errorNum = 1;
                }
                else if(rt == TOO_LONG) {
                    state = ERROR;
                    errorNum = 3;
                }
                else {
                    handleCommandProxy(buff,cmd,sizeof(buff));
                }

                break;
            case QUIT:
                if(!isspace(c)) {
                    state = ERROR;
                    errorNum = 0;
                }
                else {
                    return 0;
                }
            case GUION:
                if(!(isalpha(c))) {
                    state = ERROR;
                    errorNum = 0;
                }
                else {
                    if(c == 'h')state = HELP;
                    else if(c == 't' || c == 'v' || c == 'e' || c == 'f' || c == 'm' ) {
                        state = LETTER;
                        cmd = c;
                    }
                    else if(c == 'q') state = QUIT;
                    else {
                        state = ERROR;
                        errorNum = 1;
                        invalidLetter = c;
                    }
                }
                break;

        }
    }
    if(state == QUIT) {
        printf("Closing program...");
        sleep(2);
        return 0;
    }
    else if (state == END) {
        printf("\n\n");
        printHelp();
    }
    else {
        //socket magic
        //TODO
    }
}

int getLine(char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }
    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

void getProxyVersion() {
    //TODO
    return;
}

void handleCommandProxy(char *buff, int cmd, size_t  buffSize) {
    int sock;
    //sock = socket(AF_LOCAL,SCTP,);
    createProtocolPackage(cmd,buff,buffSize);


    printf("%s",buff);
    printf("%d",strlen(buff));


    return;
}

void createProtocolPackage(int command, char * buff, size_t buffsize) {
    return;
}