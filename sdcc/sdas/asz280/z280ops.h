/* z280ops.h */

/*  Copyright (c) 2017  John R. Coffman
 *
 *  New approach to assembly for Z80/Z180/Z280 based on the work of
 *  Alan Baldwin.  The updates are made under the GPL 3 license
 *  agreement.
 *
 * John R. Coffman
 * <johninsd@gmail.com>
 * 27-Feb-2017
 *
 */

/*
 *  Copyright (C) 1989-2014  Alan R. Baldwin
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Alan R. Baldwin
 * 721 Berkeley St.
 * Kent, Ohio  44240
 */



struct a_table atable[] = {
    {	I_Z80,	ADC,	REG_A,	R_R8,	{ 0x88 | r_2s, 0, 0, 0 } },
    {	I_Z80,	ADC,	R_R8,	0,	{ 0x88 | r_1s, 0, 0, 0 } },
    {	I_Z80,	ADC,	REG_A,	A_IR, { 0x8E, 0, 0, 0 } },
    {	I_Z80,	ADC,	A_IR,	0,  { 0x8E, 0, 0, 0 } },
    {	I_Z80,	ADC,	REG_A,	A_SX, { phi2, 0x8E, disp2s, 0 } },
    {	I_Z80,	ADC,	A_SX, 0,	{ phi1, 0x8E, disp1s, 0 } },
    {	I_Z80,	ADC,	REG_A,	A_IM,	{ 0xCE, immed2b, 0, 0 } },
    {	I_Z80,	ADC,	A_IM,	0,	{ 0xCE, immed1b, 0, 0 } },
    {	I_Z80,	ADC,	REG_HL,	R_R16,	{ 0xED, 0112 | rr_2s, 0, 0 } },
   /* Z280  p. 5-13  */ 
    {	I_Z280,	ADC,	REG_A,	R_R8X,	{ phi2, 0214 | rx_2s, 0, 0 } },
    {	I_Z280,	ADC,	R_R8X,	0,	{ phi1, 0214 | rx_1s, 0, 0 } },
    {	I_Z280,	ADC,	REG_A,	A_DA,	{0335, 0217, addr2, 0 } },
    {	I_Z280,	ADC,	A_DA,	0,	{0335, 0217, addr1, 0 } },
    {	I_Z280, ADC,	REG_A,	A_LX,	{0375, 0210 | xxa_2, disp2lx, 0 } },
    {	I_Z280, ADC,	A_LX,	0,	{0375, 0210 | xxa_1, disp1lx, 0 } },
    {	I_Z280, ADC,	REG_A,	A_SR,	{0335, 0210, disp2lx, 0 } },
    {	I_Z280, ADC,	A_SR,	0,	{0335, 0210, disp1lx, 0 } },
    {	I_Z280,	ADC,	REG_A,	A_BX,	{0335, 0210 | bx_2, 0, 0 } },
    {	I_Z280,	ADC,	A_BX,	0,	{0335, 0210 | bx_1, 0, 0 } },
    {	I_Z280, ADC,	REG_A,	A_RA,	{0375, 0210, rdispw2, 0 } },
    {	I_Z280, ADC,	A_RA,	0,	{0375, 0210, rdispw1, 0 } },
   /* Z280  p. 5-14  */ 
    {	I_Z280,	ADC,	R_R16X, (REG_BC|REG_DE|REG_SP), {phi1, 0xED, 0112 | rr_2s, 0 } },
    {	I_Z280,	ADC,	REG_IX,	REG_IX, {phi1, 0xED, 0152, 0 } },
    {	I_Z280,	ADC,	REG_IY, REG_IY, {phi1, 0xED, 0152, 0 } },

    {	I_Z80,	ADD,	REG_A,	A_IM,	{0306, immed2b, 0, 0 } },
    {	I_Z80,	ADD,	REG_A,	R_R8,	{0200 | r_2s, 0, 0, 0 } },
    {	I_Z80,	ADD,	REG_HL,	R_R16,	{0011 | rr_2s, 0, 0 } },
    {	I_Z80,	ADD,	R_R16X, (REG_BC|REG_DE|REG_SP), {phi1, 0011 | rr_2s, 0, 0 } },
    {	I_Z80,	ADD,	REG_IX,	REG_IX, {phi1, 0051, 0, 0 } },
    {	I_Z80,	ADD,	REG_IY, REG_IY, {phi1, 0051, 0, 0 } },
    {	I_Z80,	ADD,	REG_A,	A_IR,	{0206, 0, 0, 0 } },
    {	I_Z80,	ADD,	REG_A,	A_SX,	{phi2, 0206, disp2s, 0 } },
   /* single argument 8-bit ADD to AC */
    {	I_Z80,	ADD,	A_IM,	0,	{0306, immed1b, 0, 0 } },
    {	I_Z80,	ADD,	R_R8,	0,	{0200 | r_1s, 0, 0, 0 } },
    {	I_Z80,	ADD,	A_IR,	0,	{0206, 0, 0, 0 } },
    {	I_Z80,	ADD,	A_SX,	0,	{phi1, 0206, disp1s, 0 } },
   /* Z280  p. 5-15  */    
    {	I_Z280, ADD,	R_R16HX, REG_A,	{phi1, 0355, 0155, 0 } },
   /* Z280  p. 5-16  */
    {	I_Z280,	ADD,	REG_A,	R_R8X,	{phi2, 0204 | rx_2s, 0, 0 } },
    {	I_Z280,	ADD,	REG_A,	A_DA,	{0335, 0207, addr2, 0 } },
    {	I_Z280,	ADD,	REG_A,	A_LX,	{0375, 0200 | xxa_2, disp2lx, 0 } },
    {	I_Z280,	ADD,	REG_A,	A_RA,	{0375, 0200, rdispw2, 0 } },
    {	I_Z280,	ADD,	REG_A,	A_SR,	{0335, 0200, disp2lx, 0 } },
    {	I_Z280,	ADD,	REG_A,	A_BX,	{0335, 0200 | bx_2, 0, 0 } },
   /* single argument 8-bit ADD to AC */
    {	I_Z280,	ADD,	R_R8X,	0,	{phi1, 0204 | rx_1s, 0, 0 } },
    {	I_Z280,	ADD,	A_DA,	0,	{0335, 0207, addr1, 0 } },
    {	I_Z280,	ADD,	A_LX,	0,	{0375, 0200 | xxa_1, disp1lx, 0 } },
    {	I_Z280,	ADD,	A_RA,	0,	{0375, 0200, rdispw1, 0 } },
    {	I_Z280,	ADD,	A_SR,	0,	{0335, 0200, disp1lx, 0 } },
    {	I_Z280,	ADD,	A_BX,	0,	{0335, 0200 | bx_1, 0, 0 } },
   /* Z280  p. 5-18  */
    {	I_Z280,	ADD,	REG_HL,	A_IM,	{0375, 0355, 0366, immed2w } },
    {	I_Z280,	ADD,	REG_HL,	A_DA,	{0335, 0355, 0326, addr2 } },
    {	I_Z280,	ADD,	REG_HL,	A_LXY|A_SX,	{0375, 0355, 0306 | xy_2m, disp2lx } },
/*    {	I_Z280,	ADD,	REG_HL,	A_SX,	{0375, 0355, 0306 | xy_2m, disp2lx } },		*/
    {	I_Z280,	ADD,	REG_HL, A_IR,	{0335, 0355, 0306, 0 } },
    {	I_Z280,	ADD,	REG_HL,	A_RA,	{0335, 0355, 0366, rdispw2 } },
    {	I_Z280,	ADDW,	REG_HL,	R_R16,	{0011 | rr_2s, 0, 0 } },
    {	I_Z280, ADDW,	REG_HL, R_R16X,	{phi2, 0355, 0346, 0 } },
    {	I_Z280,	ADDW,	REG_HL,	A_IM,	{0375, 0355, 0366, immed2w } },
    {	I_Z280,	ADDW,	REG_HL,	A_DA,	{0335, 0355, 0326, addr2 } },
    {	I_Z280,	ADDW,	REG_HL,	A_LXY|A_SX,	{0375, 0355, 0306 | xy_2m, disp2lx } },
/*    {	I_Z280,	ADDW,	REG_HL,	A_SX,	{0375, 0355, 0306 | xy_2m, disp2lx } },		*/
    {	I_Z280,	ADDW,	REG_HL,	A_RA,	{0335, 0355, 0366, rdispw2 } },
    {	I_Z280,	ADDW,	REG_HL, A_IR,	{0335, 0355, 0306, 0 } },
  /* ADDW [hl,] implied */
    {	I_Z280,	ADDW,	R_R16,	0,	{0011 | rr_1d, 0, 0 } },
    {	I_Z280, ADDW,	R_R16X,	0,	{phi1, 0355, 0346, 0 } },
    {	I_Z280,	ADDW,	A_IM,	0,	{0375, 0355, 0366, immed1w } },
    {	I_Z280,	ADDW,	A_DA,	0,	{0335, 0355, 0326, addr1 } },
    {	I_Z280,	ADDW,	A_LXY|A_SX,	0,	{0375, 0355, 0306 | xy_1m, disp1lx } },
/*    {	I_Z280,	ADDW,	A_SX,	0,	{0375, 0355, 0306 | xy_1m, disp1lx } },		*/
    {	I_Z280,	ADDW,	A_RA,	0,	{0335, 0355, 0366, rdispw1 } },
    {	I_Z280,	ADDW,	A_IR,	0,	{0335, 0355, 0306, 0 } },
    
    {	I_Z80,	AND,	REG_A,	R_R8,	{ 0240 | r_2s, 0, 0, 0 } },
    {	I_Z80,	AND,	R_R8,	0,	{ 0240 | r_1s, 0, 0, 0 } },
    {	I_Z80,	AND,	REG_A,	A_IR, { 0246, 0, 0, 0 } },
    {	I_Z80,	AND,	A_IR,	0,  { 0246, 0, 0, 0 } },
    {	I_Z80,	AND,	REG_A,	A_SX, { phi2, 0246, disp2s, 0 } },
    {	I_Z80,	AND,	A_SX, 0,	{ phi1, 0246, disp1s, 0 } },
    {	I_Z80,	AND,	REG_A,	A_IM,	{ 0346, immed2b, 0, 0 } },
    {	I_Z80,	AND,	A_IM,	0,	{ 0346, immed1b, 0, 0 } },
   /* Z280  p. 5-19  */
    {	I_Z280,	AND,	REG_A,	R_R8X,	{ phi2, 0244 | rx_2s, 0, 0 } },
    {	I_Z280,	AND,	REG_A,	A_DA,	{ 0335, 0247, addr2, 0 } },
    {	I_Z280,	AND,	REG_A,	A_LX,	{ 0375, 0240 | xxa_2, disp2lx, 0 } },
    {	I_Z280,	AND,	REG_A,	A_RA,	{ 0375, 0240, rdispw2, 0 } },
    {	I_Z280,	AND,	REG_A,	A_SR,	{ 0335, 0240, disp2lx, 0 } },
    {	I_Z280,	AND,	REG_A,	A_BX,	{ 0335, 0240 | bx_2, 0, 0 } },
    {	I_Z280,	AND,	R_R8X,	0,	{ phi1, 0244 | rx_1s, 0, 0 } },
    {	I_Z280,	AND,	A_DA,	0,	{ 0335, 0247, addr1, 0 } },
    {	I_Z280,	AND,	A_LX,	0,	{ 0375, 0240 | xxa_1, disp1lx, 0 } },
    {	I_Z280,	AND,	A_RA,	0,	{ 0375, 0240, rdispw1, 0 } },
    {	I_Z280,	AND,	A_SR,	0,	{ 0335, 0240, disp1lx, 0 } },
    {	I_Z280,	AND,	A_BX,	0,	{ 0335, 0240 | bx_1, 0, 0 } },
    
    {	I_Z80,	BIT,	A_COND, R_R8,	{ 0313, 0100 | r_2s | cond1, 0, 0 } },
    {	I_Z80,	BIT,	A_COND, A_IR,	{ 0313, 0106 | cond1, 0, 0 } },
    {	I_Z80,	BIT,	A_COND, A_SX,	{ phi2, 0313, disp2s, 0106 | cond1 } },

    {	I_Z80,	CALL,	A_DA,	0,	{0315, addr1, 0, 0 } },
    {	I_Z80,	CALL,	A_IM,	0,	{0315, addr1, 0, 0 } },
    {	I_Z80,	CALL,	A_COND,	A_DA,	{0304 | cond1, addr2, 0, 0 } },
    {	I_Z80,	CALL,	A_COND,	A_IM,	{0304 | cond1, addr2, 0, 0 } },
   /* Z280  p. 5-21  */
    {	I_Z280,	CALL,	A_IR,	0,	{0335, 0315, 0, 0 } },
    {	I_Z280,	CALL,	A_COND,	A_IR,	{0335, 0304 | cond1, 0, 0 } },
    {	I_Z280,	CALL,	A_RA,	0,	{0375, 0315, rdispw1, 0 } },
    {	I_Z280,	CALL,	A_COND,	A_RA,	{0375, 0304 | cond1, rdispw2, 0 } },
    
    {	I_Z80,	CP,	REG_A,	R_R8,	{ 0270 | r_2s, 0, 0, 0 } },
    {	I_Z80,	CP,	R_R8,	0,	{ 0270 | r_1s, 0, 0, 0 } },
    {	I_Z80,	CP,	REG_A,	A_IR, { 0276, 0, 0, 0 } },
    {	I_Z80,	CP,	A_IR,	0,  { 0276, 0, 0, 0 } },
    {	I_Z80,	CP,	REG_A,	A_SX, { phi2, 0276, disp2s, 0 } },
    {	I_Z80,	CP,	A_SX, 0,	{ phi1, 0276, disp1s, 0 } },
    {	I_Z80,	CP,	REG_A,	A_IM,	{ 0376, immed2b, 0, 0 } },
    {	I_Z80,	CP,	A_IM,	0,	{ 0376, immed1b, 0, 0 } },
   /* Z280  p. 5-23  */ 
    {	I_Z280,	CP,	REG_A,	R_R8X,	{ phi2, 0274 | rx_2s, 0, 0 } },
    {	I_Z280,	CP,	REG_A,	A_DA,	{ 0335, 0277, addr2, 0 } },
    {	I_Z280, CP,	REG_A,	A_LX,	{ 0375, 0270 | xxa_2, disp2lx, 0 } },
    {	I_Z280, CP,	REG_A,	A_RA,	{ 0375, 0270, rdispw2, 0 } },
    {	I_Z280, CP,	REG_A,	A_SR,	{ 0335, 0270, disp2lx, 0 } },
    {	I_Z280,	CP,	REG_A,	A_BX,	{ 0335, 0270 | bx_2, 0, 0 } },
    {	I_Z280,	CP,	R_R8X,	0,	{ phi1, 0274 | rx_1s, 0, 0 } },
    {	I_Z280,	CP,	A_DA,	0,	{ 0335, 0277, addr1, 0 } },
    {	I_Z280, CP,	A_LX,	0,	{ 0375, 0270 | xxa_1, disp1lx, 0 } },
    {	I_Z280, CP,	A_RA,	0,	{ 0375, 0270, rdispw1, 0 } },
    {	I_Z280, CP,	A_SR,	0,	{ 0335, 0270, disp1lx, 0 } },
    {	I_Z280,	CP,	A_BX,	0,	{ 0335, 0270 | bx_1, 0, 0 } },
   /* Z280  p. 5-29  */
    {	I_Z280,	CPW,	REG_HL,	R_R16,	{ 0355, 0307 | rr_2s, 0, 0 } },
    {	I_Z280,	CPW,	REG_HL,	R_R16X,	{ phi2, 0355, 0347, 0 } },
    {	I_Z280,	CPW,	REG_HL, A_IM,	{ 0375, 0355, 0367, immed2w } },
    {	I_Z280,	CPW,	REG_HL,	A_DA,	{ 0335, 0355, 0327, addr2 } },
    {	I_Z280,	CPW,	REG_HL,	A_LXY|A_SX,	{ 0375, 0355, 0307 | xy_2m, disp2lx } }, 
/*    {	I_Z280,	CPW,	REG_HL,	A_SX,	{ 0375, 0355, 0307 | xy_2m, disp2lx } },		*/
    {	I_Z280,	CPW,	REG_HL,	A_RA,	{ 0335, 0355, 0367, rdispw2 } },
    {	I_Z280,	CPW,	REG_HL, A_IR,	{ 0335, 0355, 0307, 0 } }, 
    {	I_Z280,	CP,	REG_HL,	R_R16,	{ 0355, 0307 | rr_2s, 0, 0 } },
    {	I_Z280,	CP,	REG_HL,	R_R16X,	{ phi2, 0355, 0347, 0 } },
    {	I_Z280,	CP,	REG_HL, A_IM,	{ 0375, 0355, 0367, immed2w } },
    {	I_Z280,	CP,	REG_HL,	A_DA,	{ 0335, 0355, 0327, addr2 } },
    {	I_Z280,	CP,	REG_HL,	A_LXY|A_SX,	{ 0375, 0355, 0307 | xy_2m, disp2lx } }, 
/*    {	I_Z280,	CP,	REG_HL,	A_SX,	{ 0375, 0355, 0307 | xy_2m, disp2lx } },		*/
    {	I_Z280,	CP,	REG_HL,	A_RA,	{ 0335, 0355, 0367, rdispw2 } },
    {	I_Z280,	CP,	REG_HL, A_IR,	{ 0335, 0355, 0307, 0 } }, 
    {	I_Z280,	CPW,	R_R16,	0,	{ 0355, 0307 | rr_1d, 0, 0 } },
    {	I_Z280,	CPW,	R_R16X,	0,	{ phi1, 0355, 0347, 0 } },
    {	I_Z280,	CPW,	A_IM,	0,	{ 0375, 0355, 0367, immed1w } },
    {	I_Z280,	CPW,	A_DA,	0,	{ 0335, 0355, 0327, addr1 } },
    {	I_Z280,	CPW,	A_LXY|A_SX,	0,	{ 0375, 0355, 0307 | xy_1m, disp1lx } }, 
/*    {	I_Z280,	CPW,	A_SX,	0,	{ 0375, 0355, 0307 | xy_1m, disp1lx } },		*/
    {	I_Z280,	CPW,	A_RA,	0,	{ 0335, 0355, 0367, rdispw1 } },
    {	I_Z280,	CPW,	A_IR,	0,	{ 0335, 0355, 0307, 0 } }, 

    
    {	I_Z80,	DEC,	R_R8,	0,	{ 0005 | r_1d, 0, 0, 0 } },
    {	I_Z80,	DEC,	R_R16,	0,	{ 0013 | rr_1d, 0, 0, 0 } },
    {	I_Z80,	DEC,	R_R16X,	0,	{ phi1, 0053, 0, 0 } },
    {	I_Z80,	DEC,	A_IR,	0,	{ 0065, 0, 0, 0 } },
    {	I_Z80,	DEC,	A_SX,	0,	{ phi1, 0065, disp1s, 0 } },
    {	I_Z80,	DECB,	A_IR,	0,	{ 0065, 0, 0, 0 } },
    {	I_Z80,	DECB,	A_SX,	0,	{ phi1, 0065, disp1s, 0 } },
    {	I_Z80,	DECW,	R_R16,	0,	{ 0013 | rr_1d, 0, 0, 0 } },
    {	I_Z80,	DECW,	R_R16X,	0,	{ phi1, 0053, 0, 0 } },
   /* Z280  p. 5-32  */
    {	I_Z280,	DEC,	R_R8X,	0,	{ phi1, 0045 | rx_1d, 0, 0 } },
    {	I_Z280,	DEC,	A_DA,	0,	{ 0335, 0075, addr1, 0 } },
    {	I_Z280,	DEC,	A_LX,	0,	{ 0375, 0005 | xxb_1, disp1lx, 0 } },
    {	I_Z280,	DEC,	A_RA,	0,	{ 0375, 0005, rdispw1, 0 } },
    {	I_Z280,	DEC,	A_SR,	0,	{ 0335, 0005, disp1lx, 0 } },
    {	I_Z280, DEC,	A_BX,	0,	{ 0335, 0005 | bx_1d, 0, 0 } },
    {	I_Z280,	DECB,	A_DA,	0,	{ 0335, 0075, addr1, 0 } },
    {	I_Z280,	DECB,	A_LX,	0,	{ 0375, 0005 | xxb_1, disp1lx, 0 } },
    {	I_Z280,	DECB,	A_RA,	0,	{ 0375, 0005, rdispw1, 0 } },
    {	I_Z280,	DECB,	A_SR,	0,	{ 0335, 0005, disp1lx, 0 } },
    {	I_Z280, DECB,	A_BX,	0,	{ 0335, 0005 | bx_1d, 0, 0 } },
   /* Z280  p. 5-33  */
    {	I_Z280,	DECW,	A_IR,	0,	{ 0335, 0013, 0, 0 } },
    {	I_Z280,	DECW,	A_DA,	0,	{ 0335, 0033, addr1, 0 } },
    {	I_Z280,	DECW,	A_LXY|A_SX,	0,	{ 0375, 0013 | xy_1m, disp1lx, 0 } },
    {	I_Z280,	DECW,	A_RA,	0,	{ 0335, 0073, rdispw1, 0 } },

   /* Z80, privileged on the Z280  p. 5-34  */
    {	I_Z80p,	DI,	0,	0,	{ 0363, 0, 0, 0 } },
    {	I_Z280p, DI,	A_IM,	0,	{ 0355, 0167, immed1b, 0 } },
    
   /*  p. 5-35   DIV(byte)  */
    {	I_Z280,	DIV,	REG_HL, R_R8,	{ 0355, 0304 | r_2d, 0, 0 } },
    {	I_Z280,	DIV,	REG_HL,	R_R8X,	{ phi2, 0355, 0344 | rx_2d, 0 } },
    {	I_Z280,	DIV,	REG_HL,	A_IM,	{ 0375, 0355, 0374, immed2b } },
    {	I_Z280,	DIV,	REG_HL,	A_DA,	{ 0335, 0355, 0374, addr2 } },
    {	I_Z280,	DIV,	REG_HL,	A_SX,	{ phi2, 0355, 0364, disp2s } },
    {	I_Z280,	DIV,	REG_HL,	A_LX,	{ 0375, 0355, 0304 | xxb_2, disp2lx } },
    {	I_Z280,	DIV,	REG_HL,	A_RA,	{ 0375, 0355, 0304, rdispw2 } },
    {	I_Z280,	DIV,	REG_HL,	A_SR,	{ 0335, 0355, 0304, disp2lx } },
    {	I_Z280,	DIV,	REG_HL,	A_BX,	{ 0335, 0355, 0304 | bx_2d, 0 } },
     /* misprint in the manual on p. 5-35 says the following should assemble as ED E4  (0344)
         Appendix C.  corrects this to ED F4  (0364)	*/
    {	I_Z280,	DIV,	REG_HL,	A_IR,	{ 0355, 0364, 0, 0 } },
    
    {	I_Z280,	DIV,	R_R8,	0,	{ 0355, 0304 | r_1d, 0, 0 } },
    {	I_Z280,	DIV,	R_R8X,	0,	{ phi1, 0355, 0344 | rx_1d, 0 } },
    {	I_Z280,	DIV,	A_IM,	0,	{ 0375, 0355, 0374, immed1b } },
    {	I_Z280,	DIV,	A_DA,	0,	{ 0335, 0355, 0374, addr1 } },
    {	I_Z280,	DIV,	A_SX,	0,	{ phi1, 0355, 0364, disp1s } },
    {	I_Z280,	DIV,	A_LX,	0,	{ 0375, 0355, 0304 | xxb_1, disp1lx } },
    {	I_Z280,	DIV,	A_RA,	0,	{ 0375, 0355, 0304, rdispw1 } },
    {	I_Z280,	DIV,	A_SR,	0,	{ 0335, 0355, 0304, disp1lx } },
    {	I_Z280,	DIV,	A_BX,	0,	{ 0335, 0355, 0304 | bx_1d, 0 } },
    {	I_Z280,	DIV,	A_IR,	0,	{ 0355, 0364, 0, 0 } },
    
   /*  p. 5-37   DIVU(byte)  */
    {	I_Z280,	DIVU,	REG_HL, R_R8,	{ 0355, 0305 | r_2d, 0, 0 } },
    {	I_Z280,	DIVU,	REG_HL,	R_R8X,	{ phi2, 0355, 0345 | rx_2d, 0 } },
    {	I_Z280,	DIVU,	REG_HL,	A_IM,	{ 0375, 0355, 0375, immed2b } },
    {	I_Z280,	DIVU,	REG_HL,	A_DA,	{ 0335, 0355, 0375, addr2 } },
    {	I_Z280,	DIVU,	REG_HL,	A_SX,	{ phi2, 0355, 0365, disp2s } },
    {	I_Z280,	DIVU,	REG_HL,	A_LX,	{ 0375, 0355, 0305 | xxb_2, disp2lx } },
    {	I_Z280,	DIVU,	REG_HL,	A_RA,	{ 0375, 0355, 0305, rdispw2 } },
    {	I_Z280,	DIVU,	REG_HL,	A_SR,	{ 0335, 0355, 0305, disp2lx } },
    {	I_Z280,	DIVU,	REG_HL,	A_BX,	{ 0335, 0355, 0305 | bx_2d, 0 } },
    {	I_Z280,	DIVU,	REG_HL,	A_IR,	{ 0355, 0365, 0, 0 } },

    {	I_Z280,	DIVU,	R_R8,	0,	{ 0355, 0305 | r_1d, 0, 0 } },
    {	I_Z280,	DIVU,	R_R8X,	0,	{ phi1, 0355, 0345 | rx_1d, 0 } },
    {	I_Z280,	DIVU,	A_IM,	0,	{ 0375, 0355, 0375, immed1b } },
    {	I_Z280,	DIVU,	A_DA,	0,	{ 0335, 0355, 0375, addr1 } },
    {	I_Z280,	DIVU,	A_SX,	0,	{ phi1, 0355, 0365, disp1s } },
    {	I_Z280,	DIVU,	A_LX,	0,	{ 0375, 0355, 0305 | xxb_1, disp1lx } },
    {	I_Z280,	DIVU,	A_RA,	0,	{ 0375, 0355, 0305, rdispw1 } },
    {	I_Z280,	DIVU,	A_SR,	0,	{ 0335, 0355, 0305, disp1lx } },
    {	I_Z280,	DIVU,	A_BX,	0,	{ 0335, 0355, 0305 | bx_1d, 0 } },
    {	I_Z280,	DIVU,	A_IR,	0,	{ 0355, 0365, 0, 0 } },

   /*  p. 5-41   DIVUW(word)  */
    {	I_Z280,	DIVUW,	REG_DEHL,	R_R16,	{ 0355, 0313 | rr_2s, 0, 0 } },
    {	I_Z280,	DIVUW,	REG_DEHL,	R_R16X,	{ phi2, 0355, 0353, 0 } },
    {	I_Z280,	DIVUW,	REG_DEHL,	A_IM,	{ 0375, 0355, 0373, immed2w } },
    {	I_Z280,	DIVUW,	REG_DEHL,	A_DA,	{ 0335, 0355, 0333, addr2 } },
    {	I_Z280,	DIVUW,	REG_DEHL,	A_LXY|A_SX,	{ 0375, 0355, 0313 | xy_2m, disp2lx } },
/*    {	I_Z280,	DIVUW,	REG_DEHL,	A_SX,	{ 0375, 0355, 0313 | xy_2m, disp2lx } },		*/
    {	I_Z280,	DIVUW,	REG_DEHL,	A_RA,	{ 0335, 0355, 0373, rdispw2 } },
    {	I_Z280,	DIVUW,	REG_DEHL,	A_IR,	{ 0335, 0355, 0313, 0 } },

    {	I_Z280,	DIVUW,	R_R16,	0,	{ 0355, 0313 | rr_1d, 0, 0 } },
    {	I_Z280,	DIVUW,	R_R16X,	0,	{ phi1, 0355, 0353, 0 } },
    {	I_Z280,	DIVUW,	A_IM,	0,	{ 0375, 0355, 0373, immed1w } },
    {	I_Z280,	DIVUW,	A_DA,	0,	{ 0335, 0355, 0333, addr1 } },
    {	I_Z280,	DIVUW,	A_LXY|A_SX,	0,	{ 0375, 0355, 0313 | xy_1m, disp1lx } },
/*    {	I_Z280,	DIVUW,	A_SX,	0,	{ 0375, 0355, 0313 | xy_1m, disp1lx } },		*/
    {	I_Z280,	DIVUW,	A_RA,	0,	{ 0335, 0355, 0373, rdispw1 } },
    {	I_Z280,	DIVUW,	A_IR,	0,	{ 0335, 0355, 0313, 0 } },

   /*  p. 5-41   DIVW(word)  */
    {	I_Z280,	DIVW,	REG_DEHL,	R_R16,	{ 0355, 0312 | rr_2s, 0, 0 } },
    {	I_Z280,	DIVW,	REG_DEHL,	R_R16X,	{ phi2, 0355, 0352, 0 } },
    {	I_Z280,	DIVW,	REG_DEHL,	A_IM,	{ 0375, 0355, 0372, immed2w } },
    {	I_Z280,	DIVW,	REG_DEHL,	A_DA,	{ 0335, 0355, 0332, addr2 } },
    {	I_Z280,	DIVW,	REG_DEHL,	A_LXY|A_SX,	{ 0375, 0355, 0312 | xy_2m, disp2lx } },
/*    {	I_Z280,	DIVW,	REG_DEHL,	A_SX,	{ 0375, 0355, 0312 | xy_2m, disp2lx } },		*/
    {	I_Z280,	DIVW,	REG_DEHL,	A_RA,	{ 0335, 0355, 0372, rdispw2 } },
    {	I_Z280,	DIVW,	REG_DEHL,	A_IR,	{ 0335, 0355, 0312, 0 } },

    {	I_Z280,	DIVW,	R_R16,	0,	{ 0355, 0312 | rr_1d, 0, 0 } },
    {	I_Z280,	DIVW,	R_R16X,	0,	{ phi1, 0355, 0352, 0 } },
    {	I_Z280,	DIVW,	A_IM,	0,	{ 0375, 0355, 0372, immed1w } },
    {	I_Z280,	DIVW,	A_DA,	0,	{ 0335, 0355, 0332, addr1 } },
    {	I_Z280,	DIVW,	A_LXY|A_SX,	0,	{ 0375, 0355, 0312 | xy_1m, disp1lx } },
/*    {	I_Z280,	DIVW,	A_SX,	0,	{ 0375, 0355, 0312 | xy_2m, disp1lx } },		*/
    {	I_Z280,	DIVW,	A_RA,	0,	{ 0335, 0355, 0372, rdispw1 } },
    {	I_Z280,	DIVW,	A_IR,	0,	{ 0335, 0355, 0312, 0 } },

    {	I_Z80,	DJNZ,	A_IM,	0,	{ 0020, rdispb1, 0, 0 } },
    {	I_Z80,	DJNZ,	A_DA,	0,	{ 0020, rdispb1, 0, 0 } },
    
   /* Z80, privileged on the Z280  p. 5-44  */
    {	I_Z80p,	EI,	0,	0,	{ 0373, 0, 0, 0 } },
    {	I_Z280p, EI,	A_IM,	0,	{ 0355, 0177, immed1b, 0 } },

    {	I_Z80,	EX,	REG_AF, REG_AFP,  {0010, 0, 0, 0 } },
    {	I_Z80,	EX,	REG_DE, REG_HL,	  {0353, 0, 0, 0 } },
    {	I_Z80,	EX,	A_IRSP,	R_R16BX,  {phi2, 0343, 0, 0 } },
   /*  p. 5-47  */
    {	I_Z280,	EX,	REG_H,	REG_L,	{ 0355, 0357, 0, 0 } },
   /*  p. 5-48  */
    {	I_Z280,	EX,	R_R16X, REG_HL,	{phi1, 0353, 0, 0 } },
    {	I_Z280,	EX,	REG_HL, R_R16X,	{phi2, 0353, 0, 0 } },
   /*  p. 5-49  */
    {	I_Z280,	EX,	REG_A,	R_R8,	{ 0355, 0007 | r_2d, 0, 0 } },
    {	I_Z280,	EX,	REG_A,	R_R8X,	{ phi2, 0355, 0047 | rx_2d, 0 } },
    {	I_Z280,	EX,	REG_A,	A_IR,	{ 0355, 0067, 0, 0 } },
    {	I_Z280,	EX,	REG_A,	A_DA,	{ 0335, 0355, 0077, addr2 } },
    {	I_Z280,	EX,	REG_A,	A_SX,	{ phi2, 0355, 0067, disp2s } },
    {	I_Z280,	EX,	REG_A,	A_LX,	{ 0375, 0355, 0007 | xxb_2, disp2lx } },
    {	I_Z280,	EX,	REG_A,	A_RA,	{ 0375, 0355, 0007, rdispw2 } },
    {	I_Z280,	EX,	REG_A,	A_SR,	{ 0335, 0355, 0007, disp2lx } },
    {	I_Z280,	EX,	REG_A,	A_BX,	{ 0335, 0355, 0007 | bx_2d, 0 } },
    
   /*  p. 5-50  */
    {	I_Z280,	EXTS,	REG_A,	0,	{ 0355, 0144, 0, 0 } },
    {	I_Z280,	EXTS,	0,	0,	{ 0355, 0144, 0, 0 } },
   /*  p. 5-51  */
    {	I_Z280,	EXTS,	REG_HL,	0,	{ 0355, 0154, 0, 0 } },
    
/*    {	I_Z80p,	EI,	A_COND,	0,	{ 0xFB, 0, 0, 0 } },		*/
/*    {	I_Z80p,	DI,	A_COND,	0,	{ 0xF3, 0, 0, 0 } },		*/
/*    {	I_Z80p,	HALT,	A_COND,	0,	{ 0x76, 0, 0, 0 } },		*/

   /*  p. 5-54  */
    {	I_Z80p,	IM,	A_COND,	0,	{ 0355, 0106 | cond1, 0, 0 } },
    
    {	I_Z80,	INC,	R_R8,	0,	{ 0004 | r_1d, 0, 0, 0 } },
    {	I_Z80,	INC,	R_R16,	0,	{ 0003 | rr_1d, 0, 0, 0 } },
    {	I_Z80,	INC,	R_R16X,	0,	{ phi1, 0043, 0, 0 } },
    {	I_Z80,	INC,	A_IR,	0,	{ 0064, 0, 0, 0 } },
    {	I_Z80,	INC,	A_SX,	0,	{ phi1, 0064, disp1s, 0 } },
   /*  p. 5-57  */
    {	I_Z280,	INC,	R_R8X,	0,	{ phi1, 0044 | rx_1d, 0, 0 } },
    {	I_Z280,	INC,	A_DA,	0,	{ 0335, 0074, addr1, 0 } },
    {	I_Z280,	INC,	A_LX,	0,	{ 0375, 0004 | xxb_1, disp1lx, 0 } },
    {	I_Z280,	INC,	A_RA,	0,	{ 0375, 0004, rdispw1, 0 } },
    {	I_Z280,	INC,	A_SR,	0,	{ 0335, 0004, disp1lx, 0 } },
    {	I_Z280,	INC,	A_BX,	0,	{ 0335, 0004 | bx_1d, 0, 0 } },
   /*  p. 5-58  */
    {	I_Z80,	INCW,	R_R16,	0,	{ 0003 | rr_1d, 0, 0, 0 } },
    {	I_Z80,	INCW,	R_R16X,	0,	{ phi1, 0043, 0, 0 } },
    {	I_Z280,	INCW,	A_IR,	0,	{ 0335, 0003, 0, 0 } },
    {	I_Z280,	INCW,	A_DA,	0,	{ 0335, 0023, addr1, 0 } },
    {	I_Z280,	INCW,	A_LXY|A_SX, 0,	{ 0375, 0003 | xy_1m, disp1lx, 0 } },
    {	I_Z280,	INCW,	A_RA,	0,	{ 0335, 0063, rdispw1, 0 } },

    {	I_Z80io, INN,	REG_A,	A_DA,	{0333, immed2b, 0, 0 } },
    {	I_Z80io, INN,	REG_A,	A_IM,	{0333, immed2b, 0, 0 } },
    {	I_Z80io, INN,	R_R8,	A_IRC,	{0355, 0100 | r_1d, 0, 0 } },
   /*  p. 5-55  */
    {	I_Z280io, INN,	R_R8X,	A_IRC,	{phi1, 0355, 0140 | rx_1d, 0 } },
    {	I_Z280io, INN,	A_DA,	A_IRC,	{0335, 0355, 0170, addr1 } },
    {	I_Z280io, INN,	A_LX|A_SX,	A_IRC,	{0375, 0355, 0100 | xxb_1, disp1lx } },
    {	I_Z280io, INN,	A_RA,	A_IRC,	{0375, 0355, 0100, rdispw1 } },
    {	I_Z280io, INN,	A_SR,	A_IRC,	{0335, 0355, 0100, disp1lx } },
    {	I_Z280io, INN,	A_BX,	A_IRC,	{0335, 0355, 0100 | bx_1d, 0 } },
   /*  p. 5-65  */
    {	I_Z280io, INN,	REG_HL,	A_IRC,	{ 0355, 0267, 0, 0 } },
    {	I_Z280io, INW,	REG_HL,	A_IRC,	{ 0355, 0267, 0, 0 } },
    
   /*  p. 5-66	*/
    {	I_Z280,	JAF,	A_IM,	0,	{ 0335, 0050, rdispb1, 0 } },
    {	I_Z280,	JAF,	A_DA,	0,	{ 0335, 0050, rdispb1, 0 } },
   /*  p. 5-67	*/
    {	I_Z280,	JAR,	A_IM,	0,	{ 0335, 0040, rdispb1, 0 } },
    {	I_Z280,	JAR,	A_DA,	0,	{ 0335, 0040, rdispb1, 0 } },

   /* INDW, INDIRW &c.  */
    {	I_Z280io,	INDW,	0, 0,	{ 0355, 0212, 0, 0 } },
    {	I_Z280io,	INDRW,	0, 0,	{ 0355, 0232, 0, 0 } },
    {	I_Z280io,	INIW,	0, 0,	{ 0355, 0202, 0, 0 } },
    {	I_Z280io,	INIRW,	0, 0,	{ 0355, 0222, 0, 0 } },
    
    {	I_Z80,	JP,	A_DA,	0,	{0303, addr1, 0, 0 } },
    {	I_Z80,	JP,	A_COND,	A_DA,	{0302 | cond1, addr2, 0, 0 } },
    {	I_Z80,	JP,	A_IM,	0,	{0303, addr1, 0, 0 } },
    {	I_Z80,	JP,	A_COND,	A_IM,	{0302 | cond1, addr2, 0, 0 } },
    {	I_Z80,	JP,	A_IRX,	0,	{phi1, 0351, 0, 0 } },
   /*  p. 5-68  */
    {	I_Z280,	JP,	A_COND,	A_IR,	{0335, 0302 | cond1, addr2, 0 } },
    {	I_Z280,	JP,	A_RA,	0,	{0375, 0303, rdispw1, 0 } },
    {	I_Z280,	JP,	A_COND,	A_RA,	{0375, 0302 | cond1, rdispw2, 0 } },
    
    {	I_Z80,	JR,	A_IM,	0,	{0030, rdispb1, 0, 0 } },
    {	I_Z80,	JR,	A_COND, A_IM,	{0040 | cond1, rdispb2, 0, 0 } },
    {	I_Z80,	JR,	A_DA,	0,	{0030, rdispb1, 0, 0 } },
    {	I_Z80,	JR,	A_COND, A_DA,	{0040 | cond1, rdispb2, 0, 0 } },
    
    {	I_Z80,	LD,	R_R8,	R_R8,	{0100 | r_1d | r_2s, 0, 0, 0 } },
    {	I_Z80,	LD,	R_R8,	A_IR,	{0106 | r_1d, 0, 0, 0 } },
    {	I_Z80,	LD,	R_R8,	A_SX,	{phi2, 0106 | r_1d, disp2s, 0 } },
    {	I_Z80,	LD,	A_IR,	R_R8,	{0160 | r_2s, 0, 0, 0 } },
    {	I_Z80,	LD,	A_SX,	R_R8,	{phi1, 0160 | r_2s, disp1s, 0 } },
   /* These two must occur before the A_IR and A_SX immediate loads */
    {	I_Z80,	LD,	R_R16,	A_IM,	{0001 | rr_1d, immed2w, 0, 0 } },
    {	I_Z80,	LD,	R_R16X,	A_IM,	{phi1, 0041, immed2w, 0 } },
   /***	the two above are repeated below for LDW		  ***/
    {	I_Z80,	LD,	R_R8,	A_IM,	{0006 | r_1d, immed2b, 0, 0 } },
    {	I_Z80,	LD,	A_IR,	A_IM,	{0066, immed2b, 0, 0 } },
    {	I_Z80,	LD,	A_SX,	A_IM,	{ phi1, 0066, disp1s, immed2b } },
    {	I_Z80,	LD,	REG_A,	A_IRBD, {0012 | rr_2s, 0, 0, 0 } },
    {	I_Z80,	LD,	A_IRBD, REG_A,  {0002 | rr_1d, 0, 0, 0 } },
    {	I_Z80,	LD,	REG_SP,	R_R16BX,  { phi2, 0371, 0, 0 } },
    {	I_Z80,	LDW,	REG_SP,	R_R16BX,  { phi2, 0371, 0, 0 } },		/* part of p. 5-92 */
    {	I_Z80,	LD,	A_DA,	REG_A,	{0062, addr1, 0, 0 } },
    {	I_Z80,	LD,	REG_A,	A_DA,	{0072, addr2, 0, 0 } },
    {	I_Z80,	LD,	R_R16BX, A_DA,	{phi1, 0052, addr2, 0 } },
    {	I_Z80,	LD,	A_DA,	R_R16BX,	{phi2, 0042, addr1, 0 } },
   /* These two must come after R_R16BX */
    {	I_Z80,	LD,	R_R16,	A_DA,	{0355, 0113 | rr_1d, addr2, 0 } },
    {	I_Z80,	LD,	A_DA,	R_R16,	{0355, 0103 | rr_2s, addr1, 0 } },
   /* Z280 p. 5-70  */
    {	I_Z280,	LD,	REG_A,	R_R8X,	{phi2, 0174 | rx_2s, 0, 0 } },
    {	I_Z280, LD,	REG_A,	A_LX,	{0375, 0170 | xxa_2, disp2lx, 0 } },
    {	I_Z280,	LD,	REG_A,	A_RA,	{0375, 0170, rdispw2, 0 } },
    {	I_Z280,	LD,	REG_A,	A_SR,	{0335, 0170, disp2lx, 0 } },
    {	I_Z280, LD,	REG_A,	A_BX,	{0335, 0170 | bx_2, 0, 0 } },
   /*  p. 5-71  */
    {	I_Z280,	LD,	R_R8X,	REG_A,	{phi1, 0147 | rx_1d, 0, 0 } },
    {	I_Z280,	LD,	A_LX,	REG_A,	{0355, 0043 | xxb_1, disp1lx, 0 } },
    {	I_Z280,	LD,	A_RA,	REG_A,	{0355, 0043, rdispw1, 0 } },
    {	I_Z280,	LD,	A_SR,	REG_A,	{0355, 0003, disp1lx, 0 } },
    {	I_Z280, LD,	A_BX,	REG_A,	{0355, 0003 | bx_1d, 0, 0 } },
   /*  p. 5-73  */
    {	I_Z280,	LD,	R_R8X,	A_IM,	{phi1, 0046 | rx_1d, immed2b, 0 } },
    {	I_Z280,	LD,	A_DA,	A_IM,	{0335, 0076, addr1, immed2b } },
    {	I_Z280, LD,	A_LX,	A_IM,	{0375, 0006 | xxb_1, disp1lx, immed2b } },
    {	I_Z280,	LD,	A_RA,	A_IM,	{0375, 0006, 1 | rdispw1, immed2b } },
    {	I_Z280,	LD,	A_SR,	A_IM,	{0335, 0006, disp1lx, immed2b } },
    {	I_Z280, LD,	A_BX,	A_IM,	{0335, 0006 | bx_1d, immed2b, 0 } },
   /*  p. 5-74  */
    {	I_Z280,	LD,	R_R8AE,	R_R8X,	{phi2, 0104 | r_1d | rx_2s, 0, 0 } },
    {	I_Z280,	LD,	R_R8HX, R_R8HX,	{phi1, 0144 | rx_1d | rx_2s, 0, 0 } },
    {	I_Z280,	LD,	R_R8HY, R_R8HY,	{phi1, 0144 | rx_1d | rx_2s, 0, 0 } },
    {	I_Z280,	LD,	R_R8X,	R_R8AE,	{phi1, 0140 | rx_1d | r_2s, 0, 0 } },
    {	I_Z280,	LD,	R_R8X,	A_IM,	{phi1, 0046 | rx_1d, immed2b, 0 } },
   /*  p. 5-76  */
    {	I_Z280,	LDA,	R_R16HX,  A_DA,	{phi1, 0041, addr2, 0 } },
/*    {	I_Z280,	LDA,	R_R16HX,  A_SX, {phi1, 0355, 0042 | xxb_2, disp2lx } },		*/
    {	I_Z280,	LDA,	R_R16HX,  A_LX|A_SX, {phi1, 0355, 0042 | xxb_2, disp2lx } },
    {	I_Z280,	LDA,	R_R16HX,  A_RA,	{phi1, 0355, 0042, rdispw2 } },
    {	I_Z280,	LDA,	R_R16HX,  A_SR,	{phi1, 0355, 0002, disp2lx } },
    {	I_Z280,	LDA,	R_R16HX,  A_BX,	{phi1, 0355, 0002 | bx_2d, 0 } },
   /*  p. 5-77  */
    {	I_Z280p, LDCTL,	R_R16HX,  A_IRC,	{ phi1, 0355, 0146, 0 } },
    {	I_Z280p, LDCTL,	A_IRC,	R_R16HX,	{ phi2, 0355, 0156, 0 } },
    {	I_Z280p, LDCTL,	R_R16HX,  REG_USP,	{ phi1, 0355, 0207, 0 } },
    {	I_Z280p, LDCTL,	REG_USP,  R_R16HX,	{ phi2, 0355, 0217, 0 } },
   /*  p. 5-84  */
    {	I_Z280p, LDUD,	REG_A,	A_IR,	{ 0355, 0206, 0, 0 } },
    {	I_Z280p, LDUD,	REG_A,	A_SX|A_LX,	{ phi2, 0355, 0206, disp2s } },		/* no LX form exists */
    {	I_Z280p, LDUD,	A_IR,	REG_A,	{ 0355, 0216, 0, 0 } },
    {	I_Z280p, LDUD,	A_SX|A_LX,	REG_A,	{ phi1, 0355, 0216, disp1s } },		/* no LX form exists */
   /*  p. 5-86  */
    {	I_Z280p, LDUP,	REG_A,	A_IR,	{ 0355, 0226, 0, 0 } },
   {	I_Z280p, LDUP,	REG_A,	A_SX|A_LX,	{ phi2, 0355, 0226, disp2s } },		/* no LX form exists */
    {	I_Z280p, LDUP,	A_IR,	REG_A,	{ 0355, 0236, 0, 0 } },
    {	I_Z280p, LDUP,	A_SX|A_LX,	REG_A,	{ phi1, 0355, 0236, disp1s } },		/* no LX form exists */
   /*  p. 5-88  */
   /* these two are the same as LD, since the size of the operand is known */
    {	I_Z80,	LDW,	R_R16,	A_IM,	{0001 | rr_1d, immed2w, 0, 0 } },
    {	I_Z80,	LDW,	R_R16X,	A_IM,	{phi1, 0041, immed2w, 0 } },
    {	I_Z280,	LDW,	A_IR,	A_IM,	{0335, 0001, immed2w, 0 } },
    {	I_Z280,	LDW,	A_DA,	A_IM,	{0335, 0021, addr1, immed2w } },
    {	I_Z280, LDW,	A_RA,	A_IM,	{0335, 0061, 2 | rdispw1, immed2w } },
   /*  p. 5-91       order in table is significant */
    {	I_Z280,	LDW,	R_R16,	A_IR,		{0355, 0006 | rr_1d, 0, 0 } },
    {	I_Z280, LDW,	A_IR,	R_R16,		{0355, 0016 | rr_2s, 0, 0 } },
    {	I_Z280,	LD,	R_R16,	A_IR,		{0355, 0006 | rr_1d, 0, 0 } },
    {	I_Z280, LD,	A_IR,	R_R16,		{0355, 0016 | rr_2s, 0, 0 } },
    {	I_Z280,	LDW,	R_R16,	A_SX,		{phi2, 0355, 0006 | rr_1d, disp2s } },	/* specific SX */
    {	I_Z280,	LDW,	A_SX,	R_R16,		{phi1, 0355, 0016 | rr_2s, disp1s } },	/* specific SX */
    {	I_Z280,	LD,	R_R16,	A_SX,		{phi2, 0355, 0006 | rr_1d, disp2s } },	/* specific SX */
    {	I_Z280,	LD,	A_SX,	R_R16,		{phi1, 0355, 0016 | rr_2s, disp1s } },	/* specific SX */
   /*  p. 5-89  */
    {	I_Z80,	LDW,	R_R16BX, A_DA,	{phi1, 0052, addr2, 0 } },		/* same as LD on Z80 */
    {	I_Z280,	LDW,	R_R16BX, A_LX|A_SX,	{phi1, 0355, 0044 | xxb_2, disp2lx } },		/* ARB */
    {	I_Z280,	LDW,	R_R16BX, A_RA,	{phi1, 0355, 0044, rdispw2 } },
    {	I_Z280,	LDW,	R_R16BX, A_SR,	{phi1, 0355, 0004, disp2lx } },
    {	I_Z280,	LDW,	R_R16BX, A_BX,	{phi1, 0355, 0004 | bx_2d, 0 } },
    {	I_Z280,	LD,	R_R16BX, A_LX|A_SX,	{phi1, 0355, 0044 | xxb_2, disp2lx } },		/* ARB & JRC */
    {	I_Z280,	LD,	R_R16BX, A_RA,	{phi1, 0355, 0044, rdispw2 } },
    {	I_Z280,	LD,	R_R16BX, A_SR,	{phi1, 0355, 0004, disp2lx } },
    {	I_Z280,	LD,	R_R16BX, A_BX,	{phi1, 0355, 0004 | bx_2d, 0 } },
   /*  p. 5-90  */
    {	I_Z80,	LDW,	A_DA,	R_R16BX,	{phi2, 0042, addr1, 0 } },
    {	I_Z280,	LDW,	A_LX|A_SX,	R_R16BX,	{phi2, 0355, 0045 | xxb_1, disp1lx } },
    {	I_Z280,	LDW,	A_RA,	R_R16BX,	{phi2, 0355, 0045, rdispw1 } },
    {	I_Z280,	LDW,	A_SR,	R_R16BX,	{phi2, 0355, 0005, disp1lx } },
    {	I_Z280,	LDW,	A_BX,	R_R16BX,	{phi2, 0355, 0005 | bx_1d, 0 } },   
    {	I_Z280,	LD,	A_LX|A_SX,	R_R16BX,	{phi2, 0355, 0045 | xxb_1, disp1lx } },
    {	I_Z280,	LD,	A_RA,	R_R16BX,	{phi2, 0355, 0045, rdispw1 } },
    {	I_Z280,	LD,	A_SR,	R_R16BX,	{phi2, 0355, 0005, disp1lx } },
    {	I_Z280,	LD,	A_BX,	R_R16BX,	{phi2, 0355, 0005 | bx_1d, 0 } },   
   /*  p. 5-91  */
/*    {	I_Z80,	LDW,	R_R16,	A_IM,		{0001 | rr_1d, immed2w, 0, 0 } },  p. 5-88 */
    {	I_Z280,	LDW,	(REG_BC | REG_DE | REG_SP),	A_DA,	{ 0355, 0113 | rr_1d, addr2 } },
    {	I_Z280,	LDW,	R_R16,	A_SX|A_LX,		{phi2, 0355, 0006 | rr_1d, disp2s } },	/* no LX form exists */
    {	I_Z280,	LDW,	A_DA,	(REG_BC | REG_DE | REG_SP),	{0355, 0103 | rr_2s, addr1, 0 } },
    {	I_Z280,	LDW,	A_SX|A_LX,	R_R16,		{phi1, 0355, 0016 | rr_2s, disp1s } },	/* no LX form exists */
    {	I_Z280,	LD,	(REG_BC | REG_DE | REG_SP),	A_DA,	{ 0355, 0113 | rr_1d, addr2 } },
    {	I_Z280,	LD,	R_R16,	A_SX|A_LX,		{phi2, 0355, 0006 | rr_1d, disp2s } },	/* no LX form exists */
    {	I_Z280,	LD,	A_DA,	(REG_BC | REG_DE | REG_SP),	{0355, 0103 | rr_2s, addr1, 0 } },
    {	I_Z280,	LD,	A_SX|A_LX,	R_R16,		{phi1, 0355, 0016 | rr_2s, disp1s } },	/* no LX form exists */
   /*  p. 5-92  */
/*    {	I_Z80,	LDW,	REG_SP,	R_R16BX,  { phi2, 0371, 0, 0 } },   moved upward */
    
    
   /* special register loads, excludable on the Z280 */ 
    {	I_Z80p,	LD,	REG_I,	REG_A,	{0355, 0107, 0, 0 } },
    {	I_Z80p,	LD,	REG_R,	REG_A,	{0355, 0117, 0, 0 } },
    {	I_Z80p,	LD,	REG_A,	REG_I,	{0355, 0127, 0, 0 } },
    {	I_Z80p,	LD,	REG_A,	REG_R,	{0355, 0137, 0, 0 } },
    
   /*  p. 5-93   MULT(byte)  */
    {	I_Z280,	MULT,	REG_A,	R_R8,	{ 0355, 0300 | r_2d, 0, 0 } },
    {	I_Z280,	MULT,	REG_A,	R_R8X,	{ phi2, 0355, 0340 | rx_2d, 0 } },
    {	I_Z280,	MULT,	REG_A,	A_IM,	{ 0375, 0355, 0370, immed2b } },
    {	I_Z280,	MULT,	REG_A,	A_DA,	{ 0335, 0355, 0370, addr2 } },
    {	I_Z280,	MULT,	REG_A,	A_SX,	{ phi2, 0355, 0360, disp2s } },
    {	I_Z280,	MULT,	REG_A,	A_LX,	{ 0375, 0355, 0300 | xxb_2, disp2lx } },
    {	I_Z280,	MULT,	REG_A,	A_RA,	{ 0375, 0355, 0300, rdispw2 } },
    {	I_Z280,	MULT,	REG_A,	A_SR,	{ 0335, 0355, 0300, disp2lx } },
    {	I_Z280,	MULT,	REG_A,	A_BX,	{ 0335, 0355, 0300 | bx_2d, 0 } },
    {	I_Z280,	MULT,	REG_A,	A_IR,	{ 0355, 0360, 0, 0 } },

    {	I_Z280,	MULT,	R_R8,	0,	{ 0355, 0300 | r_1d, 0, 0 } },
    {	I_Z280,	MULT,	R_R8X,	0,	{ phi1, 0355, 0340 | rx_1d, 0 } },
    {	I_Z280,	MULT,	A_IM,	0,	{ 0375, 0355, 0370, immed1b } },
    {	I_Z280,	MULT,	A_DA,	0,	{ 0335, 0355, 0370, addr1 } },
    {	I_Z280,	MULT,	A_SX,	0,	{ phi1, 0355, 0360, disp1s } },
    {	I_Z280,	MULT,	A_LX,	0,	{ 0375, 0355, 0300 | xxb_1, disp1lx } },
    {	I_Z280,	MULT,	A_RA,	0,	{ 0375, 0355, 0300, rdispw1 } },
    {	I_Z280,	MULT,	A_SR,	0,	{ 0335, 0355, 0300, disp1lx } },
    {	I_Z280,	MULT,	A_BX,	0,	{ 0335, 0355, 0300 | bx_1d, 0 } },
    {	I_Z280,	MULT,	A_IR,	0,	{ 0355, 0360, 0, 0 } },

   /*  p. 5-94   MULTU(byte)  */
    {	I_Z280,	MULTU,	REG_A,	R_R8,	{ 0355, 0301 | r_2d, 0, 0 } },
    {	I_Z280,	MULTU,	REG_A,	R_R8X,	{ phi2, 0355, 0341 | rx_2d, 0 } },
    {	I_Z280,	MULTU,	REG_A,	A_IM,	{ 0375, 0355, 0371, immed2b } },
    {	I_Z280,	MULTU,	REG_A,	A_DA,	{ 0335, 0355, 0371, addr2 } },
    {	I_Z280,	MULTU,	REG_A,	A_SX,	{ phi2, 0355, 0361, disp2s } },
    {	I_Z280,	MULTU,	REG_A,	A_LX,	{ 0375, 0355, 0301 | xxb_2, disp2lx } },
    {	I_Z280,	MULTU,	REG_A,	A_RA,	{ 0375, 0355, 0301, rdispw2 } },
    {	I_Z280,	MULTU,	REG_A,	A_SR,	{ 0335, 0355, 0301, disp2lx } },
    {	I_Z280,	MULTU,	REG_A,	A_BX,	{ 0335, 0355, 0301 | bx_2d, 0 } },
    {	I_Z280,	MULTU,	REG_A,	A_IR,	{ 0355, 0361, 0, 0 } },

    {	I_Z280,	MULTU,	R_R8,	0,	{ 0355, 0301 | r_1d, 0, 0 } },
    {	I_Z280,	MULTU,	R_R8X,	0,	{ phi1, 0355, 0341 | rx_1d, 0 } },
    {	I_Z280,	MULTU,	A_IM,	0,	{ 0375, 0355, 0371, immed1b } },
    {	I_Z280,	MULTU,	A_DA,	0,	{ 0335, 0355, 0371, addr1 } },
    {	I_Z280,	MULTU,	A_SX,	0,	{ phi1, 0355, 0361, disp1s } },
    {	I_Z280,	MULTU,	A_LX,	0,	{ 0375, 0355, 0301 | xxb_1, disp1lx } },
    {	I_Z280,	MULTU,	A_RA,	0,	{ 0375, 0355, 0301, rdispw1 } },
    {	I_Z280,	MULTU,	A_SR,	0,	{ 0335, 0355, 0301, disp1lx } },
    {	I_Z280,	MULTU,	A_BX,	0,	{ 0335, 0355, 0301 | bx_1d, 0 } },
    {	I_Z280,	MULTU,	A_IR,	0,	{ 0355, 0361, 0, 0 } },

   /*  p. 5-95   MULTUW(word)  */
    {	I_Z280,	MULTUW,	REG_HL,	R_R16,	{ 0355, 0303 | rr_2s, 0, 0 } },
    {	I_Z280,	MULTUW,	REG_HL,	R_R16X,	{ phi2, 0355, 0343, 0 } },
    {	I_Z280,	MULTUW,	REG_HL,	A_IM,	{ 0375, 0355, 0363, immed2w } },
    {	I_Z280,	MULTUW,	REG_HL,	A_DA,	{ 0335, 0355, 0323, addr2 } },
    {	I_Z280,	MULTUW,	REG_HL,	A_LXY|A_SX,	{ 0375, 0355, 0303 | xy_2m, disp2lx } },
    {	I_Z280,	MULTUW,	REG_HL,	A_RA,	{ 0335, 0355, 0363, rdispw2 } },
    {	I_Z280,	MULTUW,	REG_HL,	A_IR,	{ 0335, 0355, 0303, 0 } },
    
    {	I_Z280,	MULTUW,	R_R16,	0,	{ 0355, 0303 | rr_1d, 0, 0 } },
    {	I_Z280,	MULTUW,	R_R16X,	0,	{ phi1, 0355, 0343, 0 } },
    {	I_Z280,	MULTUW,	A_IM,	0,	{ 0375, 0355, 0363, immed1w } },
    {	I_Z280,	MULTUW,	A_DA,	0,	{ 0335, 0355, 0323, addr1 } },
    {	I_Z280,	MULTUW,	A_LXY|A_SX,	0,	{ 0375, 0355, 0303 | xy_1m, disp1lx } },
    {	I_Z280,	MULTUW,	A_RA,	0,	{ 0335, 0355, 0363, rdispw1 } },
    {	I_Z280,	MULTUW,	A_IR,	0,	{ 0335, 0355, 0303, 0 } },
   /*  p. 5-96   MULTW(word)  */
    {	I_Z280,	MULTW,	REG_HL,	R_R16,	{ 0355, 0302 | rr_2s, 0, 0 } },
    {	I_Z280,	MULTW,	REG_HL,	R_R16X,	{ phi2, 0355, 0342, 0 } },
    {	I_Z280,	MULTW,	REG_HL,	A_IM,	{ 0375, 0355, 0362, immed2w } },
    {	I_Z280,	MULTW,	REG_HL,	A_DA,	{ 0335, 0355, 0322, addr2 } },
    {	I_Z280,	MULTW,	REG_HL,	A_LXY|A_SX,	{ 0375, 0355, 0302 | xy_2m, disp2lx } },
    {	I_Z280,	MULTW,	REG_HL,	A_RA,	{ 0335, 0355, 0362, rdispw2 } },
    {	I_Z280,	MULTW,	REG_HL,	A_IR,	{ 0335, 0355, 0302, 0 } },
    
    {	I_Z280,	MULTW,	R_R16,	0,	{ 0355, 0302 | rr_1d, 0, 0 } },
    {	I_Z280,	MULTW,	R_R16X,	0,	{ phi1, 0355, 0342, 0 } },
    {	I_Z280,	MULTW,	A_IM,	0,	{ 0375, 0355, 0362, immed1w } },
    {	I_Z280,	MULTW,	A_DA,	0,	{ 0335, 0355, 0322, addr1 } },
    {	I_Z280,	MULTW,	A_LXY|A_SX,	0,	{ 0375, 0355, 0302 | xy_1m, disp1lx } },
    {	I_Z280,	MULTW,	A_RA,	0,	{ 0335, 0355, 0362, rdispw1 } },
    {	I_Z280,	MULTW,	A_IR,	0,	{ 0335, 0355, 0302, 0 } },
    
   /*  pp. 5-97,98  */
    {	I_Z80,	NEG,	REG_A,	0,	{ 0355, 0104, 0, 0 } },
    {	I_Z80,	NEG,	0,	0,	{ 0355, 0104, 0, 0 } },
    {	I_Z280,	NEG,	REG_HL,	0,	{ 0355, 0114, 0, 0 } },

    {	I_Z80,	OR,	REG_A,	R_R8,	{ 0260 | r_2s, 0, 0, 0 } },
    {	I_Z80,	OR,	R_R8,	0,	{ 0260 | r_1s, 0, 0, 0 } },
    {	I_Z80,	OR,	REG_A,	A_IR, { 0266, 0, 0, 0 } },
    {	I_Z80,	OR,	A_IR,	0,  { 0266, 0, 0, 0 } },
    {	I_Z80,	OR,	REG_A,	A_SX, { phi2, 0266, disp2s, 0 } },
    {	I_Z80,	OR,	A_SX, 0,	{ phi1, 0266, disp1s, 0 } },
    {	I_Z80,	OR,	REG_A,	A_IM,	{ 0366, immed2b, 0, 0 } },
    {	I_Z80,	OR,	A_IM,	0,	{ 0366, immed1b, 0, 0 } },
   /* Z280  p. 5-100  */
    {	I_Z280,	OR,	REG_A,	R_R8X,	{ phi2, 0264 | rx_2s, 0, 0 } },
    {	I_Z280,	OR,	REG_A,	A_DA,	{ 0335, 0267, addr2, 0 } },
    {	I_Z280,	OR,	REG_A,	A_LX,	{ 0375, 0260 | xxa_2, disp2lx, 0 } },
    {	I_Z280,	OR,	REG_A,	A_RA,	{ 0375, 0260, rdispw2, 0 } },
    {	I_Z280,	OR,	REG_A,	A_SR,	{ 0335, 0260, disp2lx, 0 } },
    {	I_Z280,	OR,	REG_A,	A_BX,	{ 0335, 0260 | bx_2, 0, 0 } },

    {	I_Z280,	OR,	R_R8X,	0,	{ phi1, 0264 | rx_1s, 0, 0 } },
    {	I_Z280,	OR,	A_DA,	0,	{ 0335, 0267, addr1, 0 } },
    {	I_Z280,	OR,	A_LX,	0,	{ 0375, 0260 | xxa_1, disp1lx, 0 } },
    {	I_Z280,	OR,	A_RA,	0,	{ 0375, 0260, rdispw1, 0 } },
    {	I_Z280,	OR,	A_SR,	0,	{ 0335, 0260, disp1lx, 0 } },
    {	I_Z280,	OR,	A_BX,	0,	{ 0335, 0260 | bx_1, 0, 0 } },

    {	I_Z80io, OUT,	A_DA,	REG_A,	{ 0323, immed1b, 0, 0 } },
    {	I_Z80io, OUT,	A_IM,	REG_A,	{ 0323, immed1b, 0, 0 } },
    {	I_Z80io, OUT,	A_IRC,	R_R8,	{ 0355, 0101 | r_2d, 0, 0 } },
   /* Z280  p. 5-105  */
    {	I_Z280io, OUT,	A_IRC,	R_R8X,	{ phi2, 0355, 0141 | rx_2d, 0 } },
    {	I_Z280io, OUT,	A_IRC,	A_DA,	{ 0335, 0355, 0171, addr2 } },
    {	I_Z280io, OUT,	A_IRC,	A_LX|A_SX,  {0375, 0355, 0101 | xxb_2, disp2lx } },
    {	I_Z280io, OUT,	A_IRC,	A_RA,	{ 0375, 0355, 0101, rdispw2 } },
    {	I_Z280io, OUT,	A_IRC,	A_SR,	{ 0335, 0355, 0101, disp2lx } },
    {	I_Z280io, OUT,	A_IRC,	A_BX,	{ 0335, 0355, 0101 | bx_2d, 0 } },
   /* Z280  pp. 5-101-109  */
    {	I_Z280io, OTDRW, 0,	0,	{ 0355, 0233, 0, 0 } },
    {	I_Z280io, OTIRW, 0,	0,	{ 0355, 0223, 0, 0 } },
    {	I_Z280io, OUTDW, 0,	0,	{ 0355, 0213, 0, 0 } },
    {	I_Z280io, OUTIW, 0,	0,	{ 0355, 0203, 0, 0 } },
   /* Z280  p. 5-110  */
    {	I_Z280io, OUT,	A_IRC, REG_HL,	{ 0355, 0277, 0, 0 } },
    {	I_Z280io, OUTW, A_IRC, REG_HL,	{ 0355, 0277, 0, 0 } },
    
   /* Z280  p. 5-111  NOT privileged!!!  */
    {	I_Z280,	PCACHE,	0,	0,	{ 0355, 0145, 0, 0 } },
    
    {	I_Z80,	POP,	R_R16PP, 0,	{ 0301 | rr_1d, 0, 0, 0 } },
    {	I_Z80,	POP,	R_R16X,	0,	{ phi1, 0341, 0, 0 } },
   /* Z280  p. 5-112  */
    {	I_Z280,	POP,	A_IR,	0,	{ 0335, 0301, 0, 0 } },
    {	I_Z280,	POP,	A_DA,	0,	{ 0335, 0321, addr1, 0 } },
    {	I_Z280,	POP,	A_RA,	0,	{ 0335, 0361, rdispw1, 0 } },

    {	I_Z80,	PUSH,	R_R16PP, 0,	{ 0305 | rr_1d, 0, 0, 0 } },
    {	I_Z80,	PUSH,	R_R16X,	0,	{ phi1, 0345, 0, 0 } },
   /* Z280  p. 5-113  */
    {	I_Z280,	PUSH,	A_IM,	0,	{ 0375, 0365, immed1w, 0 } },
    {	I_Z280,	PUSH,	A_IR,	0,	{ 0335, 0305, 0, 0 } },
    {	I_Z280,	PUSH,	A_DA,	0,	{ 0335, 0325, addr1, 0 } },
    {	I_Z280, PUSH,	A_RA,	0,	{ 0335, 0365, rdispw1, 0 } },

    {	I_Z80,	RES,	A_COND, R_R8,	{ 0313, 0200 | r_2s | cond1, 0, 0 } },
    {	I_Z80,	RES,	A_COND, A_IR,	{ 0313, 0206 | cond1, 0, 0 } },
    {	I_Z80,	RES,	A_COND, A_SX,	{ phi2, 0313, disp2s, 0206 | cond1 } },

    {	I_Z80,	RET,	0,	0,	{ 0311, 0, 0, 0 } },
    {	I_Z80,	RET,	A_COND, 0,	{ 0300 | cond1, 0, 0, 0 } },
    
   /* Z80p & Z280p  p. 5-116-118  */
    {	I_Z80p,	RETI,	0,	0,	{ 0355, 0115, 0, 0 } },
    {	I_Z80p,	RETN,	0,	0,	{ 0355, 0105, 0, 0 } },
    {	I_Z280p, RETIL,	0,	0,	{ 0355, 0125, 0, 0 } },

    {	I_Z80,	RST,	A_COND,	0,	{ 0307 | cond1, 0, 0, 0 } },
    
    {	I_Z80,	RL,	R_R8,	0,	{0313, 0020 | r_1s, 0, 0 } },
    {	I_Z80,	RL,	A_IR,	0,	{0313, 0026, 0, 0 } },
    {	I_Z80,	RL,	A_SX,	0,	{ phi1, 0313, disp1s, 0026 } },
    {	I_Z80,	RLC,	R_R8,	0,	{0313, 0000 | r_1s, 0, 0 } },
    {	I_Z80,	RLC,	A_IR,	0,	{0313, 0006, 0, 0 } },
    {	I_Z80,	RLC,	A_SX,	0,	{ phi1, 0313, disp1s, 0006 } },
    {	I_Z80,	RR,	R_R8,	0,	{0313, 0030 | r_1s, 0, 0 } },
    {	I_Z80,	RR,	A_IR,	0,	{0313, 0036, 0, 0 } },
    {	I_Z80,	RR,	A_SX,	0,	{ phi1, 0313, disp1s, 0036 } },
    {	I_Z80,	RRC,	R_R8,	0,	{0313, 0010 | r_1s, 0, 0 } },
    {	I_Z80,	RRC,	A_IR,	0,	{0313, 0016, 0, 0 } },
    {	I_Z80,	RRC,	A_SX,	0,	{ phi1, 0313, disp1s, 0016 } },
    {	I_Z80,	SLA,	R_R8,	0,	{0313, 0040 | r_1s, 0, 0 } },
    {	I_Z80,	SLA,	A_IR,	0,	{0313, 0046, 0, 0 } },
    {	I_Z80,	SLA,	A_SX,	0,	{ phi1, 0313, disp1s, 0046 } },
    {	I_Z80,	SRA,	R_R8,	0,	{0313, 0050 | r_1s, 0, 0 } },
    {	I_Z80,	SRA,	A_IR,	0,	{0313, 0056, 0, 0 } },
    {	I_Z80,	SRA,	A_SX,	0,	{ phi1, 0313, disp1s, 0056 } },
    {	I_Z80,	SRL,	R_R8,	0,	{0313, 0070 | r_1s, 0, 0 } },
    {	I_Z80,	SRL,	A_IR,	0,	{0313, 0076, 0, 0 } },
    {	I_Z80,	SRL,	A_SX,	0,	{ phi1, 0313, disp1s, 0076 } },
#if ODDBALL_SHIFT_ROTATE
    {	I_Z80,	RL,	REG_A,	R_R8,	{0313, 0020 | r_2s, 0, 0 } },
    {	I_Z80,	RL,	REG_A,	A_IR,	{0313, 0026, 0, 0 } },
    {	I_Z80,	RL,	REG_A,	A_SX,	{ phi2, 0313, disp2s, 0026 } },
    {	I_Z80,	RLC,	REG_A,	R_R8,	{0313, 0000 | r_2s, 0, 0 } },
    {	I_Z80,	RLC,	REG_A,	A_IR,	{0313, 0006, 0, 0 } },
    {	I_Z80,	RLC,	REG_A,	A_SX,	{ phi2, 0313, disp2s, 0006 } },
    {	I_Z80,	RR,	REG_A,	R_R8,	{0313, 0030 | r_2s, 0, 0 } },
    {	I_Z80,	RR,	REG_A,	A_IR,	{0313, 0036, 0, 0 } },
    {	I_Z80,	RR,	REG_A,	A_SX,	{ phi2, 0313, disp2s, 0036 } },
    {	I_Z80,	RRC,	REG_A,	R_R8,	{0313, 0010 | r_2s, 0, 0 } },
    {	I_Z80,	RRC,	REG_A,	A_IR,	{0313, 0016, 0, 0 } },
    {	I_Z80,	RRC,	REG_A,	A_SX,	{ phi2, 0313, disp2s, 0016 } },
    {	I_Z80,	SLA,	REG_A,	R_R8,	{0313, 0040 | r_2s, 0, 0 } },
    {	I_Z80,	SLA,	REG_A,	A_IR,	{0313, 0046, 0, 0 } },
    {	I_Z80,	SLA,	REG_A,	A_SX,	{ phi2, 0313, disp2s, 0046 } },
    {	I_Z80,	SRA,	REG_A,	R_R8,	{0313, 0050 | r_2s, 0, 0 } },
    {	I_Z80,	SRA,	REG_A,	A_IR,	{0313, 0056, 0, 0 } },
    {	I_Z80,	SRA,	REG_A,	A_SX,	{ phi2, 0313, disp2s, 0056 } },
    {	I_Z80,	SRL,	REG_A,	R_R8,	{0313, 0070 | r_2s, 0, 0 } },
    {	I_Z80,	SRL,	REG_A,	A_IR,	{0313, 0076, 0, 0 } },
    {	I_Z80,	SRL,	REG_A,	A_SX,	{ phi2, 0313, disp2s, 0076 } },
#endif
    
    {	I_Z80,	SET,	A_COND, R_R8,	{ 0313, 0300 | r_2s | cond1, 0, 0 } },
    {	I_Z80,	SET,	A_COND, A_IR,	{ 0313, 0306 | cond1, 0, 0 } },
    {	I_Z80,	SET,	A_COND, A_SX,	{ phi2, 0313, disp2s, 0306 | cond1 } },

    {	I_Z80,	SBC,	REG_A,	R_R8,	{ 0230 | r_2s, 0, 0, 0 } },
    {	I_Z80,	SBC,	R_R8,	0,	{ 0230 | r_1s, 0, 0, 0 } },
    {	I_Z80,	SBC,	REG_A,	A_IR, { 0236, 0, 0, 0 } },
    {	I_Z80,	SBC,	A_IR,	0,  { 0236, 0, 0, 0 } },
    {	I_Z80,	SBC,	REG_A,	A_SX, { phi2, 0236, disp2s, 0 } },
    {	I_Z80,	SBC,	A_SX, 0,	{ phi1, 0236, disp1s, 0 } },
    {	I_Z80,	SBC,	REG_A,	A_IM,	{ 0336, immed2b, 0, 0 } },
    {	I_Z80,	SBC,	A_IM,	0,	{ 0336, immed1b, 0, 0 } },
    {	I_Z80,	SBC,	REG_HL,	R_R16,	{ 0xED, 0102 | rr_2s, 0, 0 } },
   /* Z280  p. 5-130  */ 
    {	I_Z280,	SBC,	REG_A,	R_R8X,	{ phi2, 0234 | rx_2s, 0, 0 } },
    {	I_Z280,	SBC,	REG_A,	A_DA,	{0335, 0237, addr2, 0 } },
    {	I_Z280, SBC,	REG_A,	A_LX,	{0375, 0230 | xxa_2, disp2lx, 0 } },
    {	I_Z280, SBC,	REG_A,	A_SR,	{0335, 0230, disp2lx, 0 } },
    {	I_Z280,	SBC,	REG_A,	A_BX,	{0335, 0230 | bx_2, 0, 0 } },
    {	I_Z280, SBC,	REG_A,	A_RA,	{0375, 0230, rdispw2, 0 } },

    {	I_Z280,	SBC,	R_R8X,	0,	{ phi1, 0234 | rx_1s, 0, 0 } },
    {	I_Z280,	SBC,	A_DA,	0,	{0335, 0237, addr1, 0 } },
    {	I_Z280, SBC,	A_LX,	0,	{0375, 0230 | xxa_1, disp1lx, 0 } },
    {	I_Z280, SBC,	A_SR,	0,	{0335, 0230, disp1lx, 0 } },
    {	I_Z280,	SBC,	A_BX,	0,	{0335, 0230 | bx_1, 0, 0 } },
    {	I_Z280, SBC,	A_RA,	0,	{0375, 0230, rdispw1, 0 } },

   /* Z280  p. 5-131  */ 
    {	I_Z280,	SBC,	R_R16X, (REG_BC|REG_DE|REG_SP), {phi1, 0xED, 0102 | rr_2s, 0 } },
    {	I_Z280,	SBC,	REG_IX,	REG_IX, {phi1, 0xED, 0142, 0 } },
    {	I_Z280,	SBC,	REG_IY, REG_IY, {phi1, 0xED, 0142, 0 } },
    
   /* Z280  p. 5-132  system call */
    {	I_Z280, SC,	A_IM,	0,	{ 0355, 0161, immed1w, 0 } },
    {	I_Z280,	SC,	A_DA,	0,	{ 0355, 0161, immed1w, 0 } },

    {	I_Z80,	SUB,	REG_A,	R_R8,	{ 0220 | r_2s, 0, 0, 0 } },
    {	I_Z80,	SUB,	R_R8,	0,	{ 0220 | r_1s, 0, 0, 0 } },
    {	I_Z80,	SUB,	REG_A,	A_IR, { 0226, 0, 0, 0 } },
    {	I_Z80,	SUB,	A_IR,	0,  { 0226, 0, 0, 0 } },
    {	I_Z80,	SUB,	REG_A,	A_SX, { phi2, 0226, disp2s, 0 } },
    {	I_Z80,	SUB,	A_SX, 0,	{ phi1, 0226, disp1s, 0 } },
    {	I_Z80,	SUB,	REG_A,	A_IM,	{ 0326, immed2b, 0, 0 } },
    {	I_Z80,	SUB,	A_IM,	0,	{ 0326, immed1b, 0, 0 } },
   /* Z280  p. 5-139  */
    {	I_Z280,	SUB,	REG_A,	R_R8X,	{phi2, 0224 | rx_2s, 0, 0 } },
    {	I_Z280,	SUB,	REG_A,	A_DA,	{0335, 0227, addr2, 0 } },
    {	I_Z280,	SUB,	REG_A,	A_LX,	{0375, 0220 | xxa_2, disp2lx, 0 } },
    {	I_Z280,	SUB,	REG_A,	A_RA,	{0375, 0220, rdispw2, 0 } },
    {	I_Z280,	SUB,	REG_A,	A_SR,	{0335, 0220, disp2lx, 0 } },
    {	I_Z280,	SUB,	REG_A,	A_BX,	{0335, 0220 | bx_2, 0, 0 } },
 
    {	I_Z280,	SUB,	R_R8X,	0,	{phi1, 0224 | rx_1s, 0, 0 } },
    {	I_Z280,	SUB,	A_DA,	0,	{0335, 0227, addr1, 0 } },
    {	I_Z280,	SUB,	A_LX,	0,	{0375, 0220 | xxa_1, disp1lx, 0 } },
    {	I_Z280,	SUB,	A_RA,	0,	{0375, 0220, rdispw1, 0 } },
    {	I_Z280,	SUB,	A_SR,	0,	{0335, 0220, disp1lx, 0 } },
    {	I_Z280,	SUB,	A_BX,	0,	{0335, 0220 | bx_1, 0, 0 } },

  /* Z280  p. 5-140  */
    {	I_Z280,	SUB,	REG_HL, R_R16,	{0355, 0316 | rr_2s, 0, 0 } },
    {	I_Z280,	SUB,	REG_HL,	R_R16X,	{phi2, 0355, 0356, 0 } },
    {	I_Z280,	SUB,	REG_HL,	A_IM,	{0375, 0355, 0376, immed2w } },
    {	I_Z280,	SUB,	REG_HL,	A_DA,	{0335, 0355, 0336, addr2 } },
    {	I_Z280,	SUB,	REG_HL,	A_LXY|A_SX,	{0375, 0355, 0316 | xy_2m, disp2lx } },
    {	I_Z280,	SUB,	REG_HL, A_IR,	{0335, 0355, 0316, 0 } },
    {	I_Z280,	SUB,	REG_HL,	A_RA,	{0335, 0355, 0376, rdispw2 } },
    
    {	I_Z280,	SUBW,	REG_HL, R_R16,	{0355, 0316 | rr_2s, 0, 0 } },
    {	I_Z280,	SUBW,	REG_HL,	R_R16X,	{phi2, 0355, 0356, 0 } },
    {	I_Z280,	SUBW,	REG_HL,	A_IM,	{0375, 0355, 0376, immed2w } },
    {	I_Z280,	SUBW,	REG_HL,	A_DA,	{0335, 0355, 0336, addr2 } },
    {	I_Z280,	SUBW,	REG_HL,	A_LXY|A_SX,	{0375, 0355, 0316 | xy_2m, disp2lx } },
    {	I_Z280,	SUBW,	REG_HL, A_IR,	{0335, 0355, 0316, 0 } },
    {	I_Z280,	SUBW,	REG_HL,	A_RA,	{0335, 0355, 0376, rdispw2 } },
    
    {	I_Z280,	SUBW,	R_R16,	0,	{0355, 0316 | rr_1d, 0, 0 } },
    {	I_Z280,	SUBW,	R_R16X,	0,	{phi1, 0355, 0356, 0 } },
    {	I_Z280,	SUBW,	A_IM,	0,	{0375, 0355, 0376, immed1w } },
    {	I_Z280,	SUBW,	A_DA,	0,	{0335, 0355, 0336, addr1 } },
    {	I_Z280,	SUBW,	A_LXY|A_SX,	0,	{0375, 0355, 0316 | xy_1m, disp1lx } },
    {	I_Z280,	SUBW,	A_IR,	0,	{0335, 0355, 0316, 0 } },
    {	I_Z280,	SUBW,	A_RA,	0,	{0335, 0355, 0376, rdispw1 } },

   /* Z280  p. 5-141  */
    {	I_Z280,	TSET,	R_R8,	0,	{ 0313, 0060 | r_1s, 0, 0 } },
    {	I_Z280,	TSET,	A_IR,	0,	{ 0313, 0066, 0, 0 } },
    {	I_Z280,	TSET,	A_SX,	0,	{ phi1, 0313, disp1s, 0066 } },
    {	I_Z280,	TSET,	A_LX,	0,	{ phi1, 0313, disp1s, 0066 } },		/* no LX form exists */
    
   /* Z280  p. 5-142  */
    {	I_Z280io, TSTI,	A_IRC,	0,	{ 0355, 0160, 0, 0 } },

    {	I_Z80,	XOR,	REG_A,	R_R8,	{ 0250 | r_2s, 0, 0, 0 } },
    {	I_Z80,	XOR,	R_R8,	0,	{ 0250 | r_1s, 0, 0, 0 } },
    {	I_Z80,	XOR,	REG_A,	A_IR, { 0256, 0, 0, 0 } },
    {	I_Z80,	XOR,	A_IR,	0,  { 0256, 0, 0, 0 } },
    {	I_Z80,	XOR,	REG_A,	A_SX, { phi2, 0256, disp2s, 0 } },
    {	I_Z80,	XOR,	A_SX, 0,	{ phi1, 0256, disp1s, 0 } },
    {	I_Z80,	XOR,	REG_A,	A_IM,	{ 0356, immed2b, 0, 0 } },
    {	I_Z80,	XOR,	A_IM,	0,	{ 0356, immed1b, 0, 0 } },
   /* Z280  p. 5-143  */
    {	I_Z280,	XOR,	REG_A,	R_R8X,	{ phi2, 0254 | rx_2s, 0, 0 } },
    {	I_Z280,	XOR,	REG_A,	A_DA,	{ 0335, 0257, addr2, 0 } },
    {	I_Z280,	XOR,	REG_A,	A_LX,	{ 0375, 0250 | xxa_2, disp2lx, 0 } },
    {	I_Z280,	XOR,	REG_A,	A_RA,	{ 0375, 0250, rdispw2, 0 } },
    {	I_Z280,	XOR,	REG_A,	A_SR,	{ 0335, 0250, disp2lx, 0 } },
    {	I_Z280,	XOR,	REG_A,	A_BX,	{ 0335, 0250 | bx_2, 0, 0 } },

    {	I_Z280,	XOR,	R_R8X,	0,	{ phi1, 0254 | rx_1s, 0, 0 } },
    {	I_Z280,	XOR,	A_DA,	0,	{ 0335, 0257, addr1, 0 } },
    {	I_Z280,	XOR,	A_LX,	0,	{ 0375, 0250 | xxa_1, disp1lx, 0 } },
    {	I_Z280,	XOR,	A_RA,	0,	{ 0375, 0250, rdispw1, 0 } },
    {	I_Z280,	XOR,	A_SR,	0,	{ 0335, 0250, disp1lx, 0 } },
    {	I_Z280,	XOR,	A_BX,	0,	{ 0335, 0250 | bx_1, 0, 0 } },

/* Z180 instructions below */
    {	I_Z180,	IN0,	R_R8,	A_DA,	{ 0xED, 0x00 | r_1d, immed2b, 0 } },
    {	I_Z180,	OUT0,	A_DA,	R_R8,	{ 0xED, 0x01 | r_2d, immed1b, 0 } },
    {	I_Z180,	MLT,	R_R16,	0,	{ 0xED, 0x4C | rr_1d, 0, 0 } },
/* these are handled by X_INH2
    {	I_Z180, OTDM,	0,	0,	{ 0xED, 0x8B, 0, 0 } },
    {	I_Z180, OTDMR,	0,	0,	{ 0xED, 0x9B, 0, 0 } },
    {	I_Z180, OTIM,	0,	0,	{ 0xED, 0x83, 0, 0 } },
    {	I_Z180, OTIMR,	0,	0,	{ 0xED, 0x93, 0, 0 } },
    {	I_Z180, SLP,	0,	0,	{ 0xED, 0x76, 0, 0 } },
**/
    {	I_Z180,	TST,	REG_A,	R_R8,	{ 0xED, 0x04 | r_2d, 0, 0 } },
    {	I_Z180,	TST,	REG_A,	A_IR,	{ 0xED, 0x34, 0, 0 } },
    {	I_Z180,	TST,	REG_A,	A_IM,	{ 0xED, 0x64, immed2b, 0 } },
    {	I_Z180,	TST,	R_R8,	0,	{ 0xED, 0x04 | r_1d, 0, 0 } },
    {	I_Z180,	TST,	A_IR,	0,	{ 0xED, 0x34, 0, 0 } },
    {	I_Z180,	TST,	A_IM,	0,	{ 0xED, 0x64, immed1b, 0 } },
    {	I_Z180,	TSTIO,	A_IM,	0,	{ 0xED,	0x74, immed1b, 0 } },
    {	I_Z180,	TSTIO,	A_IM,	A_IRC,	{ 0xED, 0x74, immed1b, 0 } },
    {	I_Z180,	TSTIO,	A_IRC,	A_IM,	{ 0xED, 0x74, immed2b, 0 } },
    
/* Z80 undocumented instructions below */
    {	I_Z80u,	ADC,	REG_A,	R_R8X,	{ phi2, 0214 | rx_2s, 0, 0 } },
    {	I_Z80u,	ADD,	REG_A,	R_R8X,	 {phi2, 0204 | rx_2s, 0, 0 } },
    {	I_Z80u,	AND,	REG_A,	R_R8X,	{ phi2, 0244 | rx_2s, 0, 0 } },
    {	I_Z80u,	CP,	REG_A,	R_R8X,	{ phi2, 0274 | rx_2s, 0, 0 } },
    {	I_Z80u,	DEC,	R_R8X,	0,	{ phi1, 0045 | rx_1d, 0, 0 } },
    {	I_Z80u,	INC,	R_R8X,	0,	{ phi1, 0044 | rx_1d, 0, 0 } },
    {	I_Z80u, INN,	R_R8X,	A_IRC,	{ phi1, 0355, 0140 | rx_1d, 0 } },
    {	I_Z80u,	LD,	R_R8AE,	R_R8X,	{ phi2, 0104 | r_1d | rx_2s, 0, 0 } },
    {	I_Z80u,	LD,	R_R8X,	R_R8AE,	{ phi1, 0140 | rx_1d | r_2s, 0, 0 } },
    {	I_Z80u,	LD,	R_R8HX, R_R8HX,	{ phi1, 0144 | rx_1d | rx_2s, 0, 0 } },
    {	I_Z80u,	LD,	R_R8HY, R_R8HY,	{ phi1, 0144 | rx_1d | rx_2s, 0, 0 } },
    {	I_Z80u,	LD,	R_R8X,	A_IM,	{ phi1, 0046 | rx_1d, immed2b, 0 } },
    {	I_Z80u,	OR,	REG_A,	R_R8X,	{ phi2, 0264 | rx_2s, 0, 0 } },
    {	I_Z80u, OUT,	A_IRC,	R_R8X,	{ phi2, 0355, 0141 | rx_2d, 0 } },
    {	I_Z80u,	SBC,	REG_A,	R_R8X,	{ phi2, 0234 | rx_2s, 0, 0 } },
    {	I_Z80u,	SUB,	REG_A,	R_R8X,	{ phi2, 0224 | rx_2s, 0, 0 } },
    {	I_Z80u,	XOR,	REG_A,	R_R8X,	{ phi2, 0254 | rx_2s, 0, 0 } },
    {	I_Z80u,	SLL,	R_R8,	0,	{ 0313, 0060 | r_1s, 0, 0 } },
    {	I_Z80u,	SLL,	A_IR,	0,	{ 0313, 0066, 0, 0 } },
    {	I_Z80u,	SLL,	A_SX,	0,	{ phi1, 0313, disp1s, 0066 } },

    {	I_Z80u,	ADC,	R_R8X,	0,	{ phi1, 0214 | rx_1s, 0, 0 } },
    {	I_Z80u,	ADD,	R_R8X,	0,	{ phi1, 0204 | rx_1s, 0, 0 } },
    {	I_Z80u,	AND,	R_R8X,	0,	{ phi1, 0244 | rx_1s, 0, 0 } },
    {	I_Z80u,	CP,	R_R8X,	0,	{ phi1, 0274 | rx_1s, 0, 0 } },
    {	I_Z80u,	OR,	R_R8X,	0,	{ phi1, 0264 | rx_1s, 0, 0 } },
    {	I_Z80u,	SBC,	R_R8X,	0,	{ phi1, 0234 | rx_1s, 0, 0 } },
    {	I_Z80u,	SUB,	R_R8X,	0,	{ phi1, 0224 | rx_1s, 0, 0 } },
    {	I_Z80u,	XOR,	R_R8X,	0,	{ phi1, 0254 | rx_1s, 0, 0 } },
   
    {	0,	0,	0,	0,   {	0, 0, 0, 0  } },

};



