;--------------------------------------------------------------------------
;  _mulint.s
;
;  Copyright (C) 2014, Krzysztof Nikiel, Ben Shi
;
;  This library is free software; you can redistribute it and/or modify it
;  under the terms of the GNU General Public License as published by the
;  Free Software Foundation; either version 2, or (at your option) any
;  later version.
;
;  This library is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License 
;  along with this library; see the file COPYING. If not, write to the
;  Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
;   MA 02110-1301, USA.
;
;  As a special exception, if you link this library with other files,
;  some of which are compiled with SDCC, to produce an executable,
;  this library does not by itself cause the resulting executable to
;  be covered by the GNU General Public License. This exception does
;  not however invalidate any other reasons why the executable file
;   might be covered by the GNU General Public License.
;--------------------------------------------------------------------------

	.globl __mulint

	.area CODE
__mulint:
	pushw x
	ld a, (#6,sp)
	ld xl, a
	ld a, (#8,sp)
	mul x, a
	ldw (#1,sp), x
	ld a, (#6,sp)
	ld xl, a
	ld a, (#7,sp)
	mul x, a
	ld a, xl
	add a, (#1,sp)
	ld (#1,sp), a
	ld a, (#5,sp)
	ld xl, a
	ld a, (#8,sp)
	mul x, a
	ld a, xl
	add a, (#1,sp)
	ld (#1,sp), a
	popw x
	ret