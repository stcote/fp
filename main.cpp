#include <QCoreApplication>
#include <QHostAddress>
#include <QUdpSocket>
#include <QDate>
#include <stdio.h>

//const QString FP_PIPE_NAME = "FP_Pipe";
const QString LOCALHOST_IP = "127.0.0.1";
const quint16 FP_PORT = 29457;

const int NAME_MAX = 127;

typedef struct
{
    int key;
    char name[NAME_MAX+1];
    int  numItems;
    qint64 day;
} t_CheckIn;

//*****************************************************************************
//*****************************************************************************
int main( int argc, char *argv[] )
{
QHostAddress hostAddr = QHostAddress::LocalHost;
QUdpSocket udp;
t_CheckIn checkIn;
int checkInSize = sizeof(checkIn);
bool sentOK = false;


    //*** check for valid # parameters ***
    if ( argc < 4 )
    {
        printf( "Invalid # arguments\n" );
        return -1;
    }

//    printf( "1: %s\n2: %s\n3: %s\n", argv[1], argv[2], argv[3] );

    //*** set up data to be sent to checkin server ***
    memset( &checkIn, 0, checkInSize );

    checkIn.key = atoi( argv[1] );
    qstrncpy( checkIn.name, argv[2], NAME_MAX );
    checkIn.numItems = atoi( argv[3] );
    checkIn.day = QDate::currentDate().toJulianDay();

    //*** send the data ***
    sentOK = (udp.writeDatagram( (const char*)&checkIn, checkInSize, hostAddr, FP_PORT ) == checkInSize );

    if ( !sentOK )
    {
        printf( "Error sending checkin data\n" );
        return -3;
    }

    printf( "Checkin data sent...\n" );

    return 0;
}
