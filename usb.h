/* -*- Mode: C++; c-file-style: "stroustrup" -*- */

/*
 *  Copyright 2016 Karl Anders Oygard. All rights reserved.
 *  Use of this source code is governed by a BSD-style license that can be
 *  found in the LICENSE file.
 */

#ifndef _USB_H_
#define _USB_H_

#include <libusb-1.0/libusb.h>

#include "ckoz0014.h"

#define INTR_RECV_LENGTH	32
#define INTR_SEND_LENGTH	32

// This class implements the USB communication layer with the stick.

class USB
{
public:

    USB();

    virtual bool Init(int epoll_fd);
    virtual void Stop();

    virtual void Poll(const epoll_event& event);

    bool CanSend() const { return !message_in_transit; }
    int Send(const char* buffer, size_t length);

protected:

    virtual void Error(const char* fmt, ...) = 0;
    virtual void Info(const char* fmt, ...) = 0;

    int epoll_fd;

private:

    static void relno(void* user_data,
		      int status,
		      unsigned int rf_major,
		      unsigned int rf_minor,
		      unsigned int usb_major,
		      unsigned int usb_minor);

    static void message_received(void* user_data,
				 mci_rx_event event,
				 int datapoint,
				 mci_rx_datatype data_type,
				 int value,
				 int signal,
				 mgw_rx_battery battery,
				 int seq_no);

    static void ack_received(void* user_data,
			     int success,
			     int seq_no,
			     int extra);

    virtual void Relno(int status,
		       unsigned int rf_major,
		       unsigned int rf_minor,
		       unsigned int usb_major,
		       unsigned int usb_minor) {}

    virtual void MessageReceived(mci_rx_event event,
				 int datapoint,
				 mci_rx_datatype data_type,
				 int value,
				 int signal,
				 mgw_rx_battery battery,
				 int seq_no) {}

    virtual void AckReceived(int success,
			     int seq_no,
			     int extra) {}

    static void sent(struct libusb_transfer* transfer);
    static void received(struct libusb_transfer* transfer);

    void Sent(struct libusb_transfer* transfer);
    void Received(struct libusb_transfer* transfer);

    static void fd_added(int fd, short fd_events, void* source);
    static void fd_removed(int fd, void* source);

    void FDAdded(int fd, short fd_events);
    void FDRemoved(int fd);

    bool init_fds();

    bool message_in_transit;

    xc_parse_data data;

    libusb_context* context;
    libusb_device_handle* handle;

    unsigned char recvbuf[INTR_RECV_LENGTH];
    libusb_transfer* recv_transfer;

    unsigned char sendbuf[INTR_SEND_LENGTH];
    libusb_transfer* send_transfer;
};

#endif
