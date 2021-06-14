#include "p2p.h"

/****global variables****/
bool isShutDown;
typedef enum {handleSeed,handleLeech,handleShutDown} HandleTheInput;
//pthread_mutex_t mutexC;

/***********************functions declerations**************************/
in_port_t seed(int fdSocket, int argc, char* argv[], struct sockaddr_in *myaddr);
in_port_t leech(int fdServer, int argc, char* argv[], struct sockaddr_in* serverAddr);

int checkIfThisIsTheFile(char* name, char *argv[],int argc);

bool ThePortIsntExist(struct sockaddr_in sockAdderPeers[], msg_dirent_t msg_dirent,int size) ;

void getFileFromClient(int fdServer, struct sockaddr_in *serverAddr,msg_dirent_t* msg_dirent, in_port_t *portNum);
void updatingServer(int fdServer, struct sockaddr_in *serverAddr,msg_dirent_t* msg_dirent, in_port_t *portNum);
void shutDown(int fdSocket);
void peer2Peer(int fdConnected);
void handleRequest(int fdConnected);
void actingLikeAServer(in_port_t *portNumber, struct sockaddr_in *sockAdderServer);
void actingLikeAClient(HandleTheInput handle,int argc,char *argv[],in_port_t *portNumber, struct sockaddr_in *sockAdderServer);
void run(int argc, char* argv[]);

/************main function*************/
int main(int argc, char* argv[]) {
  run(argc,argv);
  return EXIT_SUCCESS;
}

/************run function*************/
void run(int argc, char* argv[]){
  HandleTheInput handle = handleShutDown;
  struct sockaddr_in sockAdderServer;
  in_port_t portNumber;


  if(argc < 2){
      perror("Invalid Input: Usage: <name of the program> <seed/leech> <arguments> ||\n <name of the program> <shutdown>");
      exit(EXIT_FAILURE);
  }

  if(strcmp(argv[1],"seed") == 0) handle = handleSeed;
  else if(strcmp(argv[1],"leech") == 0) handle = handleLeech;
  else if(strcmp(argv[1],"shutdown") == 0) handle = handleShutDown;
  else{
    perror("Invalid Input: Usage: <name of the program> <seed/leech> <arguments> ||\n <name of the program> <shutdown>");
    exit(EXIT_FAILURE);
  }

  //actiong like a client => 3 options - seed,leech or shutdown
  actingLikeAClient(handle,argc,argv,&portNumber,&sockAdderServer);

  //changeing from being a client to act like a server
  actingLikeAServer(&portNumber,&sockAdderServer);

}

/************actingLikeAClient function*************/
void actingLikeAClient(HandleTheInput handle,int argc,char *argv[],in_port_t *portNumber, struct sockaddr_in *sockAdderServer){

  int fdSocketClient;

  fdSocketClient = socket(AF_INET,SOCK_STREAM,0);

  if(fdSocketClient == -1){
    perror("Client socket() Failed!");
    exit(EXIT_FAILURE);
  }

  sockAdderServer->sin_family = AF_INET;
  sockAdderServer->sin_port = htons(P_SRV_PORT);
  sockAdderServer->sin_addr.s_addr = INADDR_ANY;

  if(inet_aton("127.0.0.1",&sockAdderServer->sin_addr) != 1){
    close(fdSocketClient);
    perror("Client inet_aton() Failed!");
    exit(EXIT_FAILURE);
  }
  if(connect(fdSocketClient,(struct sockaddr*)sockAdderServer,sizeof(*sockAdderServer)) != 0 ) {
    close(fdSocketClient);
    perror("Client connect() Failed!");
    exit(EXIT_FAILURE);
  }

  switch(handle) {
    case handleSeed:
      if (argc < 3) {
        close(fdSocketClient);
        perror("Invalid Input: Usage: <program name> <seed> <arguments(file names)>");
        exit(EXIT_FAILURE);
      }
      *portNumber = seed(fdSocketClient, argc-2, argv+2,sockAdderServer);
      break;
    case handleLeech:
      if (argc < 3) {
        close(fdSocketClient);
        perror("Invalid Input: Usage: <program name> <leech> <arguments(file names)>");
        exit(EXIT_FAILURE);
      }
      *portNumber = leech(fdSocketClient, argc-2,argv+2, sockAdderServer);
      break;
    case handleShutDown:
      shutDown(fdSocketClient);
      exit(EXIT_SUCCESS);
      break;
    default:
      break;
  }
}

/************actingLikeAServer function*************/
void actingLikeAServer(in_port_t *portNumber, struct sockaddr_in *sockAdderServer){
  int fdSocketServer;
  int fdConnected;
  //pthread_t pthread;
  //pthread_mutex_init( &mutexC, NULL );

  printf("Peer - start_server: starting peer server\n");

  fdSocketServer = socket(AF_INET,SOCK_STREAM,0);
  if(fdSocketServer == -1) {
      perror("Server socket() Failed!");
      exit(EXIT_FAILURE);
  }

  sockAdderServer->sin_port =  *portNumber ;

  printf("Peer - start_server: opened socket\n");
  if((bind(fdSocketServer,(struct sockaddr*)sockAdderServer,sizeof(*sockAdderServer))) == -1) {
    close(fdSocketServer);
    perror("actingLikeAServer  bind() Failed!");
    exit(EXIT_FAILURE);
  }
  printf("Peer - start_server: bound socket to port %d\n",(int)sockAdderServer->sin_port);
  if ((listen(fdSocketServer, MAX_QUEUE_SIZE)) != 0) {
      printf("Server listen() Failed!");
      close(fdSocketServer);
      exit(EXIT_FAILURE);
  }
  printf("Peer - start_server: listning on socket\n");
  //mutex
  //pthread_mutex_lock( &mutexC );
  isShutDown = false;
  //pthread_mutex_unlock( &mutexC );

  while(!isShutDown) {
    //connect to the request
    fdConnected = accept(fdSocketServer, NULL, NULL);

    if(fdConnected < 0) {
      close(fdSocketServer);
      perror("Client accept() Failed!");
      exit(EXIT_FAILURE);
    }
    //the chiled starting conversation with the client
    /*
    if(pthread_create(&pthread,NULL,handleRequest,(void*)&fdConnected)!=0){
      close(fdSocketServer);
      perror("Server pthread_create() Failed!");
      exit(EXIT_FAILURE);
   }*/
   handleRequest( fdConnected);
   printf("Peer - finished to handle the client request\n");
    //else, the father continue handle the other requests in the queue
  }
}

/************handleRequest function*************/
void handleRequest(int fdConnected){
  msg_shutdown_t msg_shutdown;
  msg_type_t msg_type;
  msg_type.m_type = MSG_DUMP;

  while (1) {
    if(recv(fdConnected, &msg_type, sizeof(msg_type), MSG_PEEK) < 0)continue;
    if(msg_type.m_type == MSG_FILEREQ){
       peer2Peer(fdConnected);
       return;
     }
    if(msg_type.m_type == MSG_SHUTDOWN){
      if(recv(fdConnected,&msg_shutdown, sizeof(msg_shutdown), 0) < 0) {
        close(fdConnected);
        perror("Server recv() Failed!");
        exit(EXIT_FAILURE);
      }
      printf("Peer - shut_down: receiving Shutting down,Good Bye!\n");
      //close(*(int*)fdConnected);
      //pthread_mutex_lock( &mutexC );
      isShutDown = true;
      //pthread_mutex_unlock( &mutexC );
      exit(EXIT_SUCCESS);
    }
  }
}

/************seed function*************/
in_port_t seed(int fdSocket, int argc, char* argv[], struct sockaddr_in *myaddr) {

  msg_notify_t msg_notify;
  msg_ack_t msg_ack;

  msg_notify.m_type = MSG_NOTIFY;
  msg_notify.m_addr = myaddr->sin_addr.s_addr;
  msg_notify.m_port = 0; // initialized to 0
  int i = 0;
  for (; i < argc; i++) {

    strcpy(msg_notify.m_name, argv[i]);
    //sending to the server notify
    if(send(fdSocket,&msg_notify, sizeof(msg_notify),0) < 0 ) {
        perror("Client send() Failed!");
        exit(EXIT_FAILURE);
    }

    printf("\nClient - share: sending MSG_NOTIFY for %s @ 127.0.0.1:%d\n",msg_notify.m_name,msg_notify.m_port);
    //receiving ack from the server
    if(recv(fdSocket,&msg_ack, sizeof(msg_ack),0) < 0) {
        perror("Cleint - recv() in seed Failed!");
        exit(EXIT_FAILURE);
    }

    printf("Client - share: recieving MSG_ACK\n");
    // updating new port number that received from the server
    if(i == 0) {
        msg_notify.m_port = msg_ack.m_port;
        printf("Client - share: set port to %d\n", msg_notify.m_port);
    }
  }
  myaddr->sin_port = (in_port_t)msg_notify.m_port;

  msg_finish_t msg_finish;
  msg_finish.m_type = MSG_FINISH;

  if(send(fdSocket,&msg_finish, sizeof(msg_finish),0) < 0 ) {
      perror("Client send() Failed!");
      exit(EXIT_FAILURE);
  }
  return (in_port_t)msg_notify.m_port;
}

/************peer2Peer function*************/
void peer2Peer(int fdConnected) {
  msg_filereq_t msg_filereq;
  msg_filesrv_t msg_filesrv;
  ssize_t ret = 0;

  struct stat st;
  int fd;
  int numOfBlocks;
  char buff[P_BUFF_SIZE];

  //recieving from other peer the name of the file he want to get
  if(recv(fdConnected,&msg_filereq, sizeof(msg_filereq),0) < 0) {
    close(fdConnected);
    perror("Peer recv() Failed!");
    exit(EXIT_FAILURE);
  }
  msg_filesrv.m_type = MSG_FILESRV;
  fd = open( msg_filereq.m_name , O_RDONLY );
  if(fd == -1) {
    //couldn't find the file
    printf("Peer - start_server: peeked at msg type: -1\n");
     //put a negative size of the file to note that the peer does not have the file
    msg_filesrv.m_file_size = -1;

    if((send(fdConnected,&msg_filesrv, sizeof(msg_filesrv),0)) < 0 ) {
      close(fdConnected);
      perror("Peer send() Failed!");
      exit(EXIT_FAILURE);
    }
    printf("Peer - start_server: The file does not exist\n");
    return;
  }

  //need to find the size of the file for the msg_filesrv
  if(fstat(fd, &st) < 0) {
    close(fdConnected);
    close(fd);
    perror("Peer fstat() Failed!");
    exit(EXIT_FAILURE);
  }
  msg_filesrv.m_type = MSG_FILESRV;
  msg_filesrv.m_file_size = (int)st.st_size;

  printf("Peer - sending_file: sending MSG_FILESRV.\n");
  printf("Peer - sending_file: The file name is : %s.\n",msg_filereq.m_name);
  printf("Peer - sending_file: The size of the file is : %d.\n",msg_filesrv.m_file_size );


  if((send(fdConnected,&msg_filesrv, sizeof(msg_filesrv),0)) < 0 ) {
    close(fdConnected);
    perror("Peer send() Failed!");
    exit(EXIT_FAILURE);
  }

  printf("Peer - sending_file: sending file\n");


  numOfBlocks = (msg_filesrv.m_file_size / P_BUFF_SIZE) + 1;
  int i = 1;
  //while is not EOF
  while((ret = read(fd,buff,sizeof(buff))) != 0){

    if(ret == -1){
      perror("Peer read() Failed!");
      close(fdConnected);
      exit(EXIT_FAILURE);
    }

    if((send(fdConnected,buff, ret,0)) < 0 ) {
      close(fdConnected);
      perror("Peer send() Failed!");
      exit(EXIT_FAILURE);
    }
    printf("Peer - sending_file: sending file to client. part: %d/%d\n",i++,numOfBlocks);
  }
  printf("Peer - sending_file: The file has been sent!\n");

  close(fd);

}


/************leech function*************/
in_port_t leech(int fdServer, int argc, char* argv[], struct sockaddr_in* serverAddr) {

  msg_dirreq_t msg_dirreq;
  msg_dirhdr_t msg_dirhdr;

  in_port_t portNum = 0;
  int ret;
  msg_dirreq.m_type = MSG_DIRREQ;

  //sent request for the list of shareable files
  if(send(fdServer,&msg_dirreq, sizeof(msg_dirreq),0) < 0 ){
    close(fdServer);
    perror("Client send() msg_dirreq Failed!");
    exit(EXIT_FAILURE);
  }
  printf("Client - get_list: sending MSG_DIRREQ\n");
  //recieving the numbers of the shareable files
  if(recv(fdServer,&msg_dirhdr, sizeof(msg_dirhdr),0) < 0){
    close(fdServer);
    perror("Cleint recv() msg_dirhdr Failed!");
    exit(EXIT_FAILURE);
  }

  msg_dirent_t msg_dirent_temp;
  msg_dirent_t msg_dirent[msg_dirhdr.m_count];
  printf("Client - get_list: received MSG_DIRHDR with %d items\n",msg_dirhdr.m_count);
  int k = 0;
  int i = 0;
  for(; i < msg_dirhdr.m_count; i++) {
    //receiving the details of each file

    if(recv(fdServer,&msg_dirent_temp, sizeof(msg_dirent_temp),0) < 0){
      close(fdServer);
      perror("Cleint recv() msg_dirent Failed!");
      exit(EXIT_FAILURE);
    }

    printf("Client - get_list: received MSG_DIRENT for %s @ 127.0.0.1:%d\n",msg_dirent_temp.m_name,(int)msg_dirent_temp.m_port);
    //checking if the file the client ask for is the actual file

    ret = checkIfThisIsTheFile(msg_dirent_temp.m_name,argv,argc);

    if(ret!=-1){
      //pthread_mutex_lock(&mutexC);
          getFileFromClient(fdServer,serverAddr,&msg_dirent_temp,&portNum);
            msg_dirent[k++] = msg_dirent_temp;
          //  pthread_mutex_unlock(&mutexC);
    }


  }
  //pthread_mutex_lock(&mutexC);
    for(i = 0; i < k; i++)
      updatingServer(fdServer,serverAddr,&msg_dirent[i],&portNum);
  //pthread_mutex_unlock(&mutexC);

  msg_finish_t msg_finish;
  msg_finish.m_type = MSG_FINISH;

  if(send(fdServer,&msg_finish, sizeof(msg_finish),0) < 0 ) {
      perror("Client send() Failed!");
      exit(EXIT_FAILURE);
  }
  return portNum;
}


/************getFileFromClient function*************/
void getFileFromClient(int fdServer, struct sockaddr_in *serverAddr,msg_dirent_t* msg_dirent, in_port_t *portNum) {
  msg_filereq_t msg_filereq;
  msg_filesrv_t msg_filesrv;


  int fdPeer;
  int fd;
  int numOfBlocks;
  int ret = P_BUFF_SIZE;
  struct sockaddr_in clientAddr;
  char buff[P_BUFF_SIZE] = " ";


  clientAddr.sin_family = AF_INET;
  clientAddr.sin_port = msg_dirent->m_port; // the peer port
  clientAddr.sin_addr.s_addr = msg_dirent->m_addr;

  printf("Client - get_file_from_client: getting file %s from client on port %d\n",msg_dirent->m_name,(int)msg_dirent->m_port );
 //opening socket with tcp connection(SOCK_STREAM)
  fdPeer = socket(AF_INET,SOCK_STREAM,0);
  if(fdPeer == -1){
    close(fdServer);
    perror("Client socket() Failed!");
    exit(EXIT_FAILURE);
  }

  if(connect(fdPeer,(struct sockaddr*)&clientAddr,sizeof(clientAddr)) < 0 ) {
    close(fdServer);
    close(fdPeer);
    perror("Client connect() Failed!");
    exit(EXIT_FAILURE);
  }
  printf("Client - get_file_from_client: established connection\n");

  msg_filereq.m_type = MSG_FILEREQ;
  strcpy(msg_filereq.m_name,msg_dirent->m_name);
 //sending file request to the peer

  if(send(fdPeer,&msg_filereq, sizeof(msg_filereq),0) < 0 ){
    close(fdServer);
    close(fdPeer);
    perror("Client send() Failed!");
    exit(EXIT_FAILURE);
  }
  printf("Client - get_file_from_client: sent MSG_FILEREQ\n");
   //recieving the size of the file from thr peer

  if(recv(fdPeer,&msg_filesrv, sizeof(msg_filesrv),0) < 0){
    close(fdServer);
    close(fdPeer);
    perror("Cleint recv() Failed!");
    exit(EXIT_FAILURE);
  }
  printf("Client - get_file_from_client: received MSG_FILESRV: file length %d\n",msg_filesrv.m_file_size);

  if(msg_filesrv.m_file_size < 0) {
    close(fdPeer);
    printf("Client - The file does not Exist.\n");
    return;
}

  printf("Client - get_file_from_client: opened file %s\n", msg_dirent->m_name);
  fd = open(msg_dirent->m_name, O_WRONLY | O_CREAT, 0666);
  if(fd == -1){
    close(fdServer);
    close(fdPeer);
    perror("Client open() Failed!");
    exit(EXIT_FAILURE);
  }

  numOfBlocks = (msg_filesrv.m_file_size / P_BUFF_SIZE) + 1;
  int i = 0;
  for (; i < numOfBlocks; i++) {

    if((ret = recv(fdPeer,buff, sizeof(buff),0)) < 0) {
      close(fdServer);
      close(fdPeer);
      perror("Cleint recv() Failed!");
      exit(EXIT_FAILURE);
    }

    if(write(fd,buff, ret) == -1){
      close(fdServer);
      close(fdPeer);
      perror("Client write() Failed!");
      exit(EXIT_FAILURE);
    }
    printf("Client - get_file_from_client: receiving file from client. part: %d/%d\n",i+1,numOfBlocks);
  }

  printf("Client - get_file_from_client: obtained file %s from client on port %d\n",msg_dirent->m_name, msg_dirent->m_port );
  close(fd);
  close(fdPeer);
  //the peer finish the connection
}

/************updatingServer function*************/
void updatingServer(int fdServer, struct sockaddr_in *serverAddr,msg_dirent_t* msg_dirent, in_port_t *portNum){

  //client sending notify to the server for updating his library of files
  msg_notify_t msg_notify;
  msg_ack_t msg_ack;


  msg_notify.m_type = MSG_NOTIFY;
  strcpy(msg_notify.m_name,msg_dirent->m_name);
  msg_notify.m_addr = serverAddr->sin_addr.s_addr;
  msg_notify.m_port = *portNum;

  if(send(fdServer,&msg_notify, sizeof(msg_notify),0) < 0 ) {
    close(fdServer);
    perror("Client send() Failed!");
    exit(EXIT_FAILURE);
  }

  printf("Client - get_file_from_client: sending MSG_NOTIFY to updating\n");
  printf("the Server that the client holds now the file: %s\n", msg_notify.m_name);

  if(recv(fdServer,&msg_ack, sizeof(msg_ack),0) < 0) {
    close(fdServer);
    perror("Cleint recv() Failed!");
    exit(EXIT_FAILURE);
  }

    printf("Client - receiving MSG_ACK from the Server\n");
  //updating the port number if this is the first connection
  if(*portNum == 0) *portNum = msg_ack.m_port;

}

/************checkIfThisIsTheFile function*************/
int checkIfThisIsTheFile(char* name, char *argv[],int argc) {
  int i = 0;
  for(; i < argc; i++)
      if((strcmp(name,argv[i]) == 0)) return i;
  return -1;
}

/************ThePortIsntExist function*************/
bool ThePortIsntExist(struct sockaddr_in sockAdderPeers[], msg_dirent_t msg_dirent,int size) {
  int i = 0;
  for (; i < size; i++){
    //the port is already exist
    if(sockAdderPeers[i].sin_port == msg_dirent.m_port) return false;
  }
  return true;
}

/************shutDown function*************/
void shutDown(int fdSocket){
  int tempSocket;
  int index = 0;
  msg_dirreq_t msg_dirreq;
  msg_dirhdr_t msg_dirhdr;
  msg_dirent_t msg_dirent;
  msg_shutdown_t msg_shutdown;
  struct sockaddr_in tempAddr;

  printf("Client - shut_down: receiving MSG_SHUTDOWN\n");

  msg_dirreq.m_type = MSG_DIRREQ;

  if(send(fdSocket,&msg_dirreq, sizeof(msg_dirreq),0) < 0 ){
    close(fdSocket);
    perror("Client send() Failed!");
    exit(EXIT_FAILURE);
  }
    printf("Client - shut_down: sending MSG_DIRREQ\n");
  if(recv(fdSocket,&msg_dirhdr, sizeof(msg_dirhdr),0) < 0){
    close(fdSocket);
    perror("Client recv() Failed!");
    exit(EXIT_FAILURE);
  }
  printf("Client - shut_down: receiving MSG_DIRHDR with %d items\n",msg_dirhdr.m_count);

  struct sockaddr_in sockAdderPeers[msg_dirhdr.m_count];
  int i = 0;
  for (; i < msg_dirhdr.m_count; i++) {
    if(recv(fdSocket,&msg_dirent, sizeof(msg_dirent), 0) < 0) {
      close(fdSocket);
      free(sockAdderPeers);
      perror("Client recv() Failed!");
      exit(EXIT_FAILURE);
    }
    printf("Client - shut_down: received MSG_DIRENT for %s @ 127.0.0.1:%d\n",msg_dirent.m_name, (int)msg_dirent.m_port);
    //each peer have a uniqe port and i
    //dont want to close connection again with the port i already shutdown
    if(ThePortIsntExist(sockAdderPeers,msg_dirent,index)){
      sockAdderPeers[index].sin_family = AF_INET;
      sockAdderPeers[index].sin_port = msg_dirent.m_port;
      sockAdderPeers[index].sin_addr.s_addr = msg_dirent.m_addr;
      index++;
    }
  }

  tempAddr.sin_family = AF_INET;
  msg_shutdown.m_type = MSG_SHUTDOWN;

  for (i = 0; i < index; i++) {
    tempAddr.sin_port = sockAdderPeers[i].sin_port;
    tempAddr.sin_addr.s_addr = sockAdderPeers[i].sin_addr.s_addr;

    tempSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tempSocket < 0 ) {
      close(fdSocket);
      perror("Client socket() Failed!");
      exit(EXIT_FAILURE);
    }

    if(connect(tempSocket, (struct sockaddr *) &tempAddr, sizeof(tempAddr)) != 0) {
      close(tempSocket);
      close(fdSocket);
      perror("Client connect() Failed!");
      exit(EXIT_FAILURE);
    }
    printf("Client - shut_down: sending MSG_SHUTDOWN to peer at 127.0.0.1:%d\n",sockAdderPeers[i].sin_port);
    //sending shutdown to each peer
    if(send(tempSocket,&msg_shutdown, sizeof(msg_shutdown), 0) < 0) {
      close(tempSocket);
      close(fdSocket);
      perror("Client send() Failed!");
      exit(EXIT_FAILURE);
    }
     //closing the connection with the peer
     close(tempSocket);
  }

  printf("Client - shut_down: sending MSG_SHUTDOWN to server at 127.0.0.1:%d\n",P_SRV_PORT);
  //sending shutdown to the server
  if(send(fdSocket,&msg_shutdown, sizeof(msg_shutdown),0) < 0 ){
    close(fdSocket);
    perror("Client send() Failed!");
    exit(EXIT_FAILURE);
  }

  close(fdSocket);
  ///pthread_mutex_lock( &mutexC );
  isShutDown = true;
  //pthread_mutex_unlock( &mutexC );


}
