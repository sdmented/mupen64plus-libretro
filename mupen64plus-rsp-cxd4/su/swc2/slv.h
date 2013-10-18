/*
 * mupen64plus-rsp-cxd4 - RSP Interpreter
 * Copyright (C) 2012-2013  RJ 'Iconoclast' Swedlow
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/******************************************************************************\
* Project:  SP VU Emulation Table:  Store Longword from Vector Unit            *
* Authors:  Iconoclast                                                         *
* Release:  2013.05.06                                                         *
* License:  none (public domain)                                               *
\******************************************************************************/

void SLV(int vt, int element, signed int offset, int base)
{
    register unsigned int addr;

    addr  = SR[base] + (offset <<= 2);
    addr &= 0x00000FFF;
    element += 0x1;
    element &= ~01; /* advance adaptation to odd-indexed halfword entries */
    if (element > 0xC) /* We need an explicit `goto` for stupid compilers. */
        goto bitch; /* Blame M$ for their ineptitude with branch weighs. */
    switch (addr & 03)
    {
        case 00: /* word-aligned */
            *(short *)(RSP.DMEM + addr + HES(0x000)) = VR_H(vt, element+0x0);
            *(short *)(RSP.DMEM + addr + HES(0x002)) = VR_H(vt, element+0x2);
            return;
        case 02: /* F3DLX 0.95:  "Mario Kart 64" */
            *(short *)(RSP.DMEM + addr - HES(0x000)) = VR_H(vt, element+0x0);
            addr += 0x002 + HES(00);
            addr &= 0x00000FFF;
            *(short *)(RSP.DMEM + addr) = VR_H(vt, element+0x2);
            return;
        case 01:
        case 03:
            message("SLV\nOdd addr.", 3);
            return;
    }
bitch:
    message("SLV\nIllegal element.", 3);
    return;
}