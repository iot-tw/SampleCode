/*
https://en.wikibooks.org/wiki/Serial_Programming/Serial_Linux
http://stackoverflow.com/questions/9076494/how-to-convert-from-utc-to-local-time-in-c
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>

time_t GetUTCTime() 
{
        time_t abs_ts,loc_ts,gmt_ts;
        struct tm loc_time_info,gmt_time_info;

        /*Absolute time stamp.*/
        time(&abs_ts);

        /*Now get once the local time for this time stamp,
        **and once the GMT (UTC without summer time) time stamp.*/
        localtime_r(&abs_ts,&loc_time_info);
        gmtime_r(&abs_ts,&gmt_time_info);

        /*Convert them back.*/
        loc_ts=mktime(&loc_time_info);
        gmt_ts=mktime(&gmt_time_info);

        /*Unfortunately, GMT still has summer time. Get rid of it:*/
        if(gmt_time_info.tm_isdst==1) {
            gmt_ts-=3600;
        }
        return gmt_ts;
}


int main(int argc,char** argv)
{
        struct termios tio;
        struct termios stdio;
        struct termios old_stdio;
        int tty_fd;
        int count = 0;
        char sendBuff[32];
        time_t time1,time2;
        time1 = time(NULL);

        unsigned char c = 'D';
        tcgetattr(STDOUT_FILENO,&old_stdio);

        printf("Start demo\n");
        memset(&stdio,0,sizeof(stdio));
        stdio.c_iflag = 0;
        stdio.c_oflag = 0;
        stdio.c_cflag = 0;
        stdio.c_lflag = 0;
        stdio.c_cc[VMIN] = 1;
        stdio.c_cc[VTIME] = 0;
        tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
        tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking

        memset(&tio,0,sizeof(tio));
        tio.c_iflag = 0;
        tio.c_oflag = 0;
        tio.c_cflag = CS8|CREAD|CLOCAL;        
        tio.c_lflag = 0;
        tio.c_cc[VMIN] = 1;
        tio.c_cc[VTIME] = 5;

        tty_fd = open("/dev/ttyAMA0", O_RDWR | O_NONBLOCK);      
        if(tty_fd == -1) {
            return -1;
        }
        cfsetospeed(&tio,B9600);            
        cfsetispeed(&tio,B9600);            

        tcsetattr(tty_fd, TCSANOW,&tio);
        
        while (c != 'q') {
            if((count%10) == 0) {
                count = 0;
                memset(sendBuff,'\0',32);
                sprintf(sendBuff,"AT+DTX=10,%lu\r\n", GetUTCTime());
                write(STDOUT_FILENO, sendBuff, 21);
                write(tty_fd, sendBuff, 21); 
            }
            count++;
            sleep(1);
            read(tty_fd,&c, 1);
        }

        close(tty_fd);
        tcsetattr(STDOUT_FILENO,TCSANOW,&old_stdio);
        return EXIT_SUCCESS;
}