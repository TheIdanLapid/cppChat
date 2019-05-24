/*
 * FileBase.h
 *
 *  Created on: Jun 12, 2017
 *      Author: user
 */

#ifndef SRC_FILEBASE_H_
#define SRC_FILEBASE_H_

class FileBase {
protected:
	int fd;
public:
	FileBase();

	/*
	 * reads from the fd to the given buffer
	 * @param buffer the buffer to read to it
	 * @param length to amount of bytes to read
	 */
	virtual int read(char* buffer, int length);

	/*
	 * writes to the fd from the given buffer
	 * @param buffer the buffer to write from it
	 * @param length to amount of bytes to write
	 */
	virtual int write(const char* buffer, int length);

	/*
	 * closes the fd
	 */
	virtual void close()=0;
	virtual ~FileBase();

	int inline getFd() const {
		return fd;
	}
};

#endif /* SRC_FILEBASE_H_ */
