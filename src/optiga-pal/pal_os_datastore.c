/**
* \copyright
* MIT License
*
* Copyright (c) 2020 Infineon Technologies AG
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*
* \endcopyright
*
* \author Infineon Technologies AG
*
* \file pal_os_datastore.c
*
* \brief   This file implements the platform abstraction layer APIs for data store.
*
* \ingroup  grPAL
*
* @{
*/

#include "optiga/pal/pal_os_datastore.h"
#include "memory/memory.h"
#include "hardfault.h"
/// @cond hidden

/// Size of length field (2 bytes)
#define LENGTH_SIZE                (0x02)
/// Size of data store buffer to hold the shielded connection manage context information (64 bytes context)
#define MANAGE_CONTEXT_BUFFER_SIZE      (0x40)

//Internal buffer to store the shielded connection manage context information (length field + Data)
uint8_t data_store_manage_context_buffer [LENGTH_SIZE + MANAGE_CONTEXT_BUFFER_SIZE];

//Internal buffer to store the optiga application context data during hibernate(length field + Data)
uint8_t data_store_app_context_buffer [LENGTH_SIZE + APP_CONTEXT_SIZE];

pal_status_t pal_os_datastore_write(uint16_t datastore_id,
                                    const uint8_t * p_buffer,
                                    uint16_t length)
{
    pal_status_t return_status = PAL_STATUS_FAILURE;
    uint8_t offset = 0;

    switch(datastore_id)
    {
        case OPTIGA_PLATFORM_BINDING_SHARED_SECRET_ID:
        {
            // Not implemented
            return_status = PAL_STATUS_FAILURE;
            break;
        }
        case OPTIGA_COMMS_MANAGE_CONTEXT_ID:
        {
            // !!!OPTIGA_LIB_PORTING_REQUIRED
            // This has to be enhanced by user only, in case of storing 
            // the manage context information in non-volatile memory 
            // to reuse for later during hard reset scenarios where the 
            // RAM gets flushed out.
            data_store_manage_context_buffer[offset++] = (uint8_t)(length>>8);
            data_store_manage_context_buffer[offset++] = (uint8_t)(length);
            memcpy(&data_store_manage_context_buffer[offset],p_buffer,length);
            return_status = PAL_STATUS_SUCCESS;
            break;
        }
        case OPTIGA_HIBERNATE_CONTEXT_ID:
        {
            // !!!OPTIGA_LIB_PORTING_REQUIRED
            // This has to be enhanced by user only, in case of storing 
            // the application context information in non-volatile memory 
            // to reuse for later during hard reset scenarios where the 
            // RAM gets flushed out.
            data_store_app_context_buffer[offset++] = (uint8_t)(length>>8);
            data_store_app_context_buffer[offset++] = (uint8_t)(length);
            memcpy(&data_store_app_context_buffer[offset],p_buffer,length);
            return_status = PAL_STATUS_SUCCESS;
            break;
        }
        default:
        {
            break;
        }
    }
    return return_status;
}


pal_status_t pal_os_datastore_read(uint16_t datastore_id, 
                                   uint8_t * p_buffer, 
                                   uint16_t * p_buffer_length)
{
    pal_status_t return_status = PAL_STATUS_FAILURE;
    uint16_t data_length;
    uint8_t offset = 0;

    switch(datastore_id)
    {
        case OPTIGA_PLATFORM_BINDING_SHARED_SECRET_ID:
        {
            if(*p_buffer_length < 32) {
                return_status = PAL_STATUS_FAILURE;
                break;
            }
            memory_get_io_protection_key(p_buffer);
            *p_buffer_length = 32;
            return_status = PAL_STATUS_SUCCESS;
            break;
        }
        case OPTIGA_COMMS_MANAGE_CONTEXT_ID:
        {
            // !!!OPTIGA_LIB_PORTING_REQUIRED
            // This has to be enhanced by user only,
            // if manage context information is stored in NVM during the hibernate, 
            // else this is not required to be enhanced.
            data_length = (uint16_t) (data_store_manage_context_buffer[offset++] << 8);
            data_length |= (uint16_t)(data_store_manage_context_buffer[offset++]);
            memcpy(p_buffer, &data_store_manage_context_buffer[offset], data_length);
            *p_buffer_length = data_length;
            return_status = PAL_STATUS_SUCCESS;
            break;
        }
        case OPTIGA_HIBERNATE_CONTEXT_ID:
        {
            // !!!OPTIGA_LIB_PORTING_REQUIRED
            // This has to be enhanced by user only,
            // if application context information is stored in NVM during the hibernate, 
            // else this is not required to be enhanced.
            data_length = (uint16_t) (data_store_app_context_buffer[offset++] << 8);
            data_length |= (uint16_t)(data_store_app_context_buffer[offset++]);
            memcpy(p_buffer, &data_store_app_context_buffer[offset], data_length);
            *p_buffer_length = data_length;
            return_status = PAL_STATUS_SUCCESS;
            break;
        }
        default:
        {
            *p_buffer_length = 0;
            break;
        }
    }

    return return_status;
}
/// @endcond
/**
* @}
*/
