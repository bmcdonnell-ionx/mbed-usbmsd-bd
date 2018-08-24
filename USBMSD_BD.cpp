/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "USBMSD_BD.h"
#include "mbed_debug.h"

#define SD_COMMAND_TIMEOUT 5000

#define SD_DBG             0

USBMSD_BD::USBMSD_BD(BlockDevice* bd) {
    _bd = bd;
    //no init
    _status = 0x01;
    
//    connect();
}

int USBMSD_BD::disk_initialize() {
    _bd->init();
    _ssize = _bd->get_erase_size();
    _sectors = _bd->size() / _ssize;
    
    // OK
    _status = 0x00;
    
    return 0;
}

int USBMSD_BD::disk_write(const uint8_t* buff, uint64_t sector, uint8_t count)
{
    bd_addr_t addr = (bd_addr_t)sector*_ssize;
    bd_size_t size = (bd_size_t)count*_ssize;
    int err = _bd->erase(addr, size);
    err = _bd->program(buff, addr, size);
    return err;
}

int USBMSD_BD::disk_read(uint8_t* buff, uint64_t sector, uint8_t count)
{
    bd_addr_t addr = (bd_addr_t)sector*_ssize;
    bd_size_t size = (bd_size_t)count*_ssize;
    int err = _bd->read(buff, addr, size);
    return err;
}

int USBMSD_BD::disk_status() { return _status; }
int USBMSD_BD::disk_sync() { return 0; }
uint64_t USBMSD_BD::disk_sectors() { return _sectors; }
uint64_t USBMSD_BD::disk_size() { return _bd->size(); }


// PRIVATE FUNCTIONS

