/*
 * GStreamer splitter + decoder, adapted from parser.c
 *
 * Copyright 2010 Maarten Lankhorst for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef __GST_PRIVATE_INCLUDED__
#define __GST_PRIVATE_INCLUDED__

#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define COBJMACROS
#define NONAMELESSSTRUCT
#define NONAMELESSUNION
#include "dshow.h"
#include "mfidl.h"
#include "wine/debug.h"
#include "wine/strmbase.h"

#include "unixlib.h"

static inline const char *debugstr_time(REFERENCE_TIME time)
{
    ULONGLONG abstime = time >= 0 ? time : -time;
    unsigned int i = 0, j = 0;
    char buffer[23], rev[23];

    while (abstime || i <= 8)
    {
        buffer[i++] = '0' + (abstime % 10);
        abstime /= 10;
        if (i == 7) buffer[i++] = '.';
    }
    if (time < 0) buffer[i++] = '-';

    while (i--) rev[j++] = buffer[i];
    while (rev[j-1] == '0' && rev[j-2] != '.') --j;
    rev[j] = 0;

    return wine_dbg_sprintf("%s", rev);
}

#define MEDIATIME_FROM_BYTES(x) ((LONGLONG)(x) * 10000000)

extern const struct unix_funcs *unix_funcs;

HRESULT avi_splitter_create(IUnknown *outer, IUnknown **out) DECLSPEC_HIDDEN;
HRESULT decodebin_parser_create(IUnknown *outer, IUnknown **out) DECLSPEC_HIDDEN;
HRESULT mpeg_splitter_create(IUnknown *outer, IUnknown **out) DECLSPEC_HIDDEN;
HRESULT wave_parser_create(IUnknown *outer, IUnknown **out) DECLSPEC_HIDDEN;

BOOL init_gstreamer(void) DECLSPEC_HIDDEN;

extern HRESULT mfplat_get_class_object(REFCLSID rclsid, REFIID riid, void **obj) DECLSPEC_HIDDEN;
extern HRESULT mfplat_DllRegisterServer(void) DECLSPEC_HIDDEN;

IMFMediaType *mf_media_type_from_wg_format(const struct wg_format *format) DECLSPEC_HIDDEN;
void mf_media_type_to_wg_format(IMFMediaType *type, struct wg_format *format) DECLSPEC_HIDDEN;

HRESULT winegstreamer_stream_handler_create(REFIID riid, void **obj) DECLSPEC_HIDDEN;

HRESULT audio_converter_create(REFIID riid, void **ret) DECLSPEC_HIDDEN;

#endif /* __GST_PRIVATE_INCLUDED__ */
