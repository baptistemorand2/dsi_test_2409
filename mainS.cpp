////////////////////////////////////////////////////////////////
// Include
////////////////////////////////////////////////////////////////
#include <iostream>

#include "socket_server_tcp.h"
////////////////////////////////////////////////////////////////
// Macro
////////////////////////////////////////////////////////////////


/**
@brief maximum cmd in a message
**/
#define NB_MAX_CMD 10
/**
@brief maximum length of a cmd
**/
#define CMD_LENGTH_MAX 50

/**
@brief maximum LENGTH of a message
**/
#define MSG_LENGTH_MAX CMD_LENGTH_MAX*NB_MAX_CMD


/**
@brief cmd delimiter 
**/
#define CMD_DELIMITER '#'

/**
@brief arg delimiter 
**/
#define ARG_DELIMITER ';'



////////////////////////////////////////////////////////////////
// Global
////////////////////////////////////////////////////////////////
/**
@brief structure main 
**/
using namespace std;

/**
@brief structure cmd
@param cmd cmd send
@param param parameter of the cmd
**/

typedef struct{
    char cmd[CMD_LENGTH_MAX];
    char param[CMD_LENGTH_MAX];
}Cmd_t;

////////////////////////////////////////////////////////////////
// Function
////////////////////////////////////////////////////////////////
/**
@brief parse 
@param cmd
@return struct Cmd_t
**/

Cmd_t parse(char* msg){
	Cmd_t cmd;
	unsigned int i;
	bool argF=false ;
	for(i=0;i<strlen(msg);i++){//parse cmd
		cmd.cmd[i]=msg[i];
		if(msg[i]==ARG_DELIMITER){
			argF=true;
			break;
		}
	}
	cmd.cmd[i]='\0';
	if(argF==true){
		cmd.param[0]=' ';
		cmd.param[1]='\0';
	}
	else{
		cmd.param[0]='\0';
		return cmd;
	}
		
	unsigned int index=0;
	i++;
	for(i;i<strlen(msg);i++){//parse cmd
		cmd.param[index]=msg[i];
		index++;
	}
	if(index!=0){
		cmd.param[index]='\0';
	}
	
	return cmd;//if no cmd or no arg
}

/**
@brief set function callback
@param server tcp serzver
@param callback function to send callback
**/
void setCallback(Socket_server_tcp* server,void (*callback)(Cmd_t)){
	char msg[MSG_LENGTH_MAX];
	
	
	int socket=server->acceptCon();// accept con of client
	if(socket>0){//check if there is a client
			
		server->recvM(socket,msg,MSG_LENGTH_MAX);
		char msgC[NB_MAX_CMD][CMD_LENGTH_MAX];//copy to not be delete by strtok
		//split CMD
		unsigned int index=0;
		unsigned int indexCmd=0;
		unsigned int i;
		for(i=0;i<strlen(msg);i++){
			
			msgC[index][indexCmd]=msg[i];
			indexCmd++;
			if(msg[i]==CMD_DELIMITER){
				msgC[index][indexCmd-1]='\0';//end of char
				indexCmd=0;
				index++;
			}

			
		}
		//msgC[index][indexCmd-1]='\0';//add \0 if no cmd delimiter
			
		
		Cmd_t cmd;
		if(index!=0){//no command
		
			for(i=0;i<index;i++){
				cout<<"sub :"<<msgC[i]<<"\n";
				cmd=parse(msgC[i]);
				callback(cmd);//call callback
			}
			
		}
		
		
		close(socket);//close client sockets
	}
	
	
}
/**
@brief callback event 
@param message message to parse
**/
void eventCb(Cmd_t message){
	if(message.cmd!=NULL){
		if(message.param[0]!='\0'){
		
			cout<<"\n\t Got a command\n\t\tsubsystem:"<<message.cmd<<"\n\t\tPARAMETERS: ";
			if((message.param[0]==' ')&&(message.param[1]=='\0')){
				cout<<"(void)";
			}
			else{
				cout<<message.param;
			}
			cout<<"\n";
		}
		else{
			cout<<"Invalid command \""<<message.cmd<<"\"\n";
		}
		
	}
	
}


//void connect()
/*
@brief main function 
**/

int main(int argc, char *argv[]){
	
	cout<<"main Server start\n";
	
	// TCP
	
	Socket_server_tcp server(9876);// create server on default port 
	server.listenS(1);// accept 5 connection 
	while(1){
		
		setCallback(&server,eventCb);//set server callback
		
	}
	
	return 0;
}