/*
 * linux_kbhit.h
 *
 *  Created on: Apr 4, 2019
 *      Author: sangbeom.lim@colubrismx.com
 */

#ifndef SRC_LINUX_KBHIT_H_
#define SRC_LINUX_KBHIT_H_


#include <stdio.h>
#include <termios.h> 	//터미널 속성을 변경하는 함수 사용
#include <unistd.h> 	//STDIN_FILENO와 같은 상수 값을 사용



char _kbhit(void)
{
    struct termios oldt, newt;	//oldt 기존 터미널 속성, newt 새로운 터미널 속성

    tcgetattr( STDIN_FILENO, &oldt );	//기존 터미널 속성 기반으로 몇가지만 변경해서 사용
    newt = oldt;						//기존 속성값 대입

    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;

    newt.c_lflag &= ~( ICANON | ECHO );	//정규화(Canonical)속성 제거

    tcsetattr( STDIN_FILENO, TCSANOW, &newt ); //변경된 설정값을 적용

    char ch = 0;

    if(read(STDIN_FILENO, &ch, 1) != 1)
    	ch = 0;
    else
    {
    	if(ch == 27) //press ESC
    	{
    		char dummy;
    		while(read(STDIN_FILENO, &dummy, 1) == 1);
    	}
    }
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

    return ch;
}

#endif /* SRC_LINUX_KBHIT_H_ */
