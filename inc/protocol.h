/*
 * protocol.h
 *
 *  Created on: May 7, 2019
 *      Author: sangbeom.lim@colubrismx.com
 */

#ifndef INC_PROTOCOL_H_
#define INC_PROTOCOL_H_

#define QUEUE "/queue_data" //file

#define EMPTY 0x00	//Queue Empty
#define SEND 0x01	//Queue Sending
#define FULL 0x02	//Queue Full

enum logLevelCommand{
	LOG_LEVEL_INFORMATION = 0x1001,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_CRITICAL,
	LOG_LEVEL_FATAL,
	LOG_LEVEL_DEBUG
	//...
};

enum logStringData{
	error001 = 0x2001,
	error002,
	error003,
	error004,
	error005,
	error006,
	error007,
	//...
};


#endif /* INC_PROTOCOL_H_ */
