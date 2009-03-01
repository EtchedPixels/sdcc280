/* Select disassembly routine for specified architecture.
   Copyright (C) 1994, 1995, 1996, 1997, 1998, 1999 Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include "ansidecl.h"
#include "dis-asm.h"

#ifdef ARCH_all
#define ARCH_a29k
#define ARCH_alpha
#define ARCH_arc
#define ARCH_arm
#define ARCH_avr
#define ARCH_d10v
#define ARCH_d30v
#define ARCH_h8300
#define ARCH_h8500
#define ARCH_hppa
#define ARCH_i370
#define ARCH_i386
#define ARCH_i960
#define ARCH_fr30
#define ARCH_m32r
#define ARCH_m68k
#define ARCH_m88k
#define ARCH_mcore
#define ARCH_mips
#define ARCH_mn10200
#define ARCH_mn10300
#define ARCH_ns32k
#define ARCH_pj
#define ARCH_powerpc
#define ARCH_rs6000
#define ARCH_sh
#define ARCH_sparc
#define ARCH_tic30
#define ARCH_tic80
#define ARCH_v850
#define ARCH_vax
#define ARCH_w65
#define ARCH_z8k
#endif


disassembler_ftype
disassembler (abfd)
     bfd *abfd;
{
  enum bfd_architecture a = bfd_get_arch (abfd);
  disassembler_ftype disassemble;

  switch (a)
    {
      /* If you add a case to this table, also add it to the
	 ARCH_all definition right above this function.  */
#ifdef ARCH_a29k
    case bfd_arch_a29k:
      /* As far as I know we only handle big-endian 29k objects.  */
      disassemble = print_insn_big_a29k;
      break;
#endif
#ifdef ARCH_alpha
    case bfd_arch_alpha:
      disassemble = print_insn_alpha;
      break;
#endif
#ifdef ARCH_arc
    case bfd_arch_arc:
      {
	disassemble = arc_get_disassembler (bfd_get_mach (abfd),
					    bfd_big_endian (abfd));
	break;
      }
#endif
#ifdef ARCH_arm
    case bfd_arch_arm:
      if (bfd_big_endian (abfd))
	disassemble = print_insn_big_arm;
      else
	disassemble = print_insn_little_arm;
      break;
#endif
#ifdef ARCH_avr
    case bfd_arch_avr:
      disassemble = print_insn_avr;
      break;
#endif
#ifdef ARCH_d10v
    case bfd_arch_d10v:
      disassemble = print_insn_d10v;
      break;
#endif
#ifdef ARCH_d30v
    case bfd_arch_d30v:
      disassemble = print_insn_d30v;
      break;
#endif
#ifdef ARCH_h8300
    case bfd_arch_h8300:
      if (bfd_get_mach(abfd) == bfd_mach_h8300h)
	disassemble = print_insn_h8300h;
      else if (bfd_get_mach(abfd) == bfd_mach_h8300s)
	disassemble = print_insn_h8300s;
      else 
	disassemble = print_insn_h8300;
      break;
#endif
#ifdef ARCH_h8500
    case bfd_arch_h8500:
      disassemble = print_insn_h8500;
      break;
#endif
#ifdef ARCH_hppa
    case bfd_arch_hppa:
      disassemble = print_insn_hppa;
      break;
#endif
#ifdef ARCH_i370
    case bfd_arch_i370:
      disassemble = print_insn_i370;
      break;
#endif
#ifdef ARCH_i386
    case bfd_arch_i386:
      if (bfd_get_mach (abfd) == bfd_mach_i386_i386_intel_syntax)
        disassemble = print_insn_i386_intel;
      else
        disassemble = print_insn_i386_att;
      break;
#endif
#ifdef ARCH_i960
    case bfd_arch_i960:
      disassemble = print_insn_i960;
      break;
#endif
#ifdef ARCH_fr30
    case bfd_arch_fr30:
      disassemble = print_insn_fr30;
      break;
#endif
#ifdef ARCH_m32r
    case bfd_arch_m32r:
      disassemble = print_insn_m32r;
      break;
#endif
#ifdef ARCH_m68k
    case bfd_arch_m68k:
      disassemble = print_insn_m68k;
      break;
#endif
#ifdef ARCH_m88k
    case bfd_arch_m88k:
      disassemble = print_insn_m88k;
      break;
#endif
#ifdef ARCH_ns32k
    case bfd_arch_ns32k:
      disassemble = print_insn_ns32k;
      break;
#endif
#ifdef ARCH_mcore
    case bfd_arch_mcore:
      disassemble = print_insn_mcore;
      break;
#endif
#ifdef ARCH_mips
    case bfd_arch_mips:
      if (bfd_big_endian (abfd))
	disassemble = print_insn_big_mips;
      else
	disassemble = print_insn_little_mips;
      break;
#endif
#ifdef ARCH_mn10200
    case bfd_arch_mn10200:
      disassemble = print_insn_mn10200;
      break;
#endif
#ifdef ARCH_mn10300
    case bfd_arch_mn10300:
      disassemble = print_insn_mn10300;
      break;
#endif
#ifdef ARCH_pj
    case bfd_arch_pj:
      disassemble = print_insn_pj;
      break;
#endif
#ifdef ARCH_powerpc
    case bfd_arch_powerpc:
      if (bfd_big_endian (abfd))
	disassemble = print_insn_big_powerpc;
      else
	disassemble = print_insn_little_powerpc;
      break;
#endif
#ifdef ARCH_rs6000
    case bfd_arch_rs6000:
      disassemble = print_insn_rs6000;
      break;
#endif
#ifdef ARCH_sh
    case bfd_arch_sh:
      if (bfd_big_endian (abfd))
	disassemble = print_insn_sh;
      else
	disassemble = print_insn_shl;
      break;
#endif
#ifdef ARCH_sparc
    case bfd_arch_sparc:
      disassemble = print_insn_sparc;
      break;
#endif
#ifdef ARCH_tic30
    case bfd_arch_tic30:
      disassemble = print_insn_tic30;
      break;
#endif
#ifdef ARCH_tic80
    case bfd_arch_tic80:
      disassemble = print_insn_tic80;
      break;
#endif
#ifdef ARCH_v850
    case bfd_arch_v850:
      disassemble = print_insn_v850;
      break;
#endif
#ifdef ARCH_w65
    case bfd_arch_w65:
      disassemble = print_insn_w65;
      break;
#endif
#ifdef ARCH_z8k
    case bfd_arch_z8k:
      if (bfd_get_mach(abfd) == bfd_mach_z8001)
	disassemble = print_insn_z8001;
      else 
	disassemble = print_insn_z8002;
      break;
#endif
#ifdef ARCH_vax
    case bfd_arch_vax:
      disassemble = print_insn_vax;
      break;
#endif
    default:
      return 0;
    }
  return disassemble;
}
