

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/sem.h>
#include <pthread.h>
#include <sys/stat.h>
/*
 *  Port number for p2p server.
 *  Also the base for all port numbers allocated to peers.
 */
#define P_SRV_PORT 12345

/*
 *  Length of names of files
 */
#define P_NAME_LEN 128

/*
 *  Size of buffer to read and write files
 */
#define P_BUFF_SIZE 1024
#define MAX_QUEUE_SIZE 5


/*
 *  A structure for describing a shareable file.
 *  The structure contains the name of the file and the IP address
 *  of a peer willing to share the file, and the port number on which
 *  that peer is listening.
 */
typedef struct file_ent
{
    char               fe_name[P_NAME_LEN + 1];
    in_addr_t          fe_addr;
    in_port_t          fe_port;
}
    file_ent_t;

/*
 *  message types
 */
#define MSG_FINISH -3
#define MSG_DUMP 	-2
#define MSG_SHUTDOWN 	-1

#define MSG_ERROR     	 0

#define MSG_NOTIFY    	 1
#define MSG_ACK       	 2
#define MSG_DIRREQ    	 3
#define MSG_DIRHDR    	 4
#define MSG_DIRENT    	 5
#define MSG_FILEREQ   	 6
#define MSG_FILESRV   	 7

/*
 *  message formats: MSG_NOTIFY
 *  Used by clients to notify the server of a files they are willing to share.
 */
typedef struct msg_notify
{
    int         m_type;	 			// = MSG_NOTIFY
    char        m_name[P_NAME_LEN + 1];		// name of file the client has
    in_addr_t   m_addr;				// client's IP address
    in_port_t   m_port;				// client's perceived port
}
    msg_notify_t;

/*
 *  message formats: MSG_ACK
 *  The way the server responds to MSG_NOTIFY. It contains the client's port
 *  number, or, if the client did not specify a port, a new, unique number.
 */
typedef struct msg_ack
{
    int         m_type;				// = MSG_ACK
    in_port_t   m_port;				// client's assigned port
}
    msg_ack_t;

/*
 *  message formats: MSG_DIRREQ
 *  Used by clients to obtain a list of the files currently being shared.
 */
typedef struct msg_dirreq
{
    int         m_type;				// = MSG_DIRREQ
}
    msg_dirreq_t;

/*
 *  message formats: MSG_DIRHDR
 *  Used by the server to respond to MSG_DIRREQ.
 */
typedef struct msg_dirhdr
{
    int         m_type;				// = MSG_DIRHDR
    int         m_count;			// number of files in list
}
    msg_dirhdr_t;

/*
 *  message formats: MSG_DIRENT
 *  Used by the server to convey the name of one file being offered.
 */
typedef struct msg_dirent
{
    int         m_type;				// = MSG_DIRENT
    char        m_name[P_NAME_LEN + 1];		// name of the file requested
    in_addr_t   m_addr;		// IP address of client offering the file
    in_port_t   m_port;		// Port number of client offering the file
}
    msg_dirent_t;

/*
 *  message formats: MSG_FILEREQ
 *  Used by a peer to request a file from another peer.
 */
typedef struct msg_filereq
{
    int         m_type;				// = MSG_FILEREQ
    char        m_name[P_NAME_LEN + 1];		// name of the file requested
}
    msg_filereq_t;


/*
 *  message formats: MSG_FILESRV
 *  Used by a peer in response to MSG_FILEREQ message from a nother peer.
 */
typedef struct msg_filesrv
{
    int         m_type;				// = MSG_FILESRV
    int         m_file_size;			// size of file
}
    msg_filesrv_t;


/*
 *  message formats: MSG_SHUTDOWN
 *  Used by the server to bring down the system.
 *  Any client receiving this message should terminate.
 */
typedef struct msg_shutdown
{
    int         m_type;				// = MSG_SHUTDOWN
}
    msg_shutdown_t;

/*
 *  message formats: MSG_DUMP
 *  Used by the server to check about new requests.
 */
typedef struct msg_type{

        int         m_type;	 			// = MSG_DUMP

}msg_type_t;

/*
 *  message formats: MSG_FINISH
 *  Used by the client to notify about the finish of the connection
 *  with the server. Any server receiving this message should finish
 *  to handle the client request.
 */
typedef struct msg_finish{

        int         m_type;	 			// = MSG_FINISH

}msg_finish_t;
