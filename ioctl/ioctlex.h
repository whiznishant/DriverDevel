#ifndef MYIOCTL_H
#define MYIOCTL_H

#include <linux/ioctl.h>

//This is to generate unique integers for IOCTL commands.With Permissions.
//These are unsigned long ints.

#define CMD_SEND_MSG _IOW('1','1',char *)
#define CMD_RECV_MSG _IOR('2','2',char *)

#endif
