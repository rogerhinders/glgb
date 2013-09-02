#include <cstdio>
#include <cstring>
#include "disassembler.h"
#include "tables.h"

using namespace std;

Disassembler::Disassembler() {

}
u8 Disassembler::disassemble( u32 opcode, char* opstr, CPU* cpu ) {
    u8 op0 = opcode&0xFF;
    u8 op1 = ( opcode>>8 )&0xFF;
    u8 op2 = ( opcode>>16 )&0xFF;

    switch( op2 ) {
        case 0x00:
            strcpy( opstr,"nop" );
            break;
        case 0x01:
            sprintf( opstr,"ld bc,$%04x",( ( op0<<8 )|op1 ) );
            break;
        case 0x02:
            strcpy( opstr,"ld (bc),a" );
            break;
        case 0x03:
            strcpy( opstr,"inc bc" );
            break;
        case 0x04:
            strcpy( opstr,"inc b" );
            break;
        case 0x05:
            strcpy( opstr,"dec b" );
            break;
        case 0x06:
            sprintf( opstr,"ld b,$%02x",op1 );
            break;
        case 0x07:
            strcpy( opstr,"RLCA" );
            break;
        case 0x08:
            sprintf( opstr,"ld ($%04x),sp",( ( op0<<8 )|op1 ) );
            break;
        case 0x09:
            strcpy( opstr,"add hl,bc" );
            break;
        case 0x0a:
            strcpy( opstr,"ld a,(bc)" );
            break;
        case 0x0b:
            strcpy( opstr,"dec bc" );
            break;
        case 0x0c:
            strcpy( opstr,"inc c" );
            break;
        case 0x0d:
            strcpy( opstr,"dec c" );
            break;
        case 0x0e:
            sprintf( opstr,"ld c,$%02x",op1 );
            break;
        case 0x0f:
            strcpy( opstr,"rrca" );
            break;
        case 0x10:
            strcpy( opstr,"stop" );
            break;
        case 0x11:
            sprintf( opstr,"ld de,$%04x",( ( op0<<8 )|op1 ) );
            break;
        case 0x12:
            strcpy( opstr,"ld (de),a" );
            break;
        case 0x13:
            strcpy( opstr,"inc de" );
            break;
        case 0x14:
            strcpy( opstr,"inc d" );
            break;
        case 0x15:
            strcpy( opstr,"dec d" );
            break;
        case 0x16:
            sprintf( opstr,"ld d,$%02x",op1 );
            break;
        case 0x17:
            strcpy( opstr,"rla" );
            break;
        case 0x18:
            sprintf( opstr,"jr $%04x",cpu->PC.w+2+( s8 )op1 );
            break;
        case 0x19:
            strcpy( opstr,"add hl,de" );
            break;
        case 0x1a:
            strcpy( opstr,"ld a,(de)" );
            break;
        case 0x1b:
            strcpy( opstr,"dec de" );
            break;
        case 0x1c:
            strcpy( opstr,"inc e" );
            break;
        case 0x1d:
            strcpy( opstr,"dec e" );
            break;
        case 0x1e:
            sprintf( opstr,"ld e,$%02x",op1 );
            break;
        case 0x1f:
            strcpy( opstr,"rra" );
            break;
        case 0x20:
            sprintf( opstr,"jr nz,$%04x",cpu->PC.w+2+( s8 )op1 );
            break;
        case 0x21:
            sprintf( opstr,"ld hl,$%04x",( ( op0<<8 )|op1 ) );
            break;
        case 0x22:
            strcpy( opstr,"ldi (hl),a" );
            break;
        case 0x23:
            strcpy( opstr,"inc hl" );
            break;
        case 0x24:
            strcpy( opstr,"inc h" );
            break;
        case 0x25:
            strcpy( opstr,"dec h" );
            break;
        case 0x26:
            sprintf( opstr,"ld h,$%02x",op1 );
            break;
        case 0x27:
            strcpy( opstr,"daa" );
            break;
        case 0x28:
            sprintf( opstr,"jr z,$%04x",cpu->PC.w+2+( s8 )op1 );
            break;
        case 0x29:
            strcpy( opstr,"add hl,hl" );
            break;
        case 0x2a:
            strcpy( opstr,"ldi a,(hl)" );
            break;
        case 0x2b:
            strcpy( opstr,"dec hl" );
            break;
        case 0x2c:
            strcpy( opstr,"inc l" );
            break;
        case 0x2d:
            strcpy( opstr,"dec l" );
            break;
        case 0x2e:
            sprintf( opstr,"ld l,$%02x",op1 );
            break;
        case 0x2f:
            strcpy( opstr,"cpl" );
            break;
        case 0x30:
            sprintf( opstr,"jr nc,$%04x",cpu->PC.w+2+( s8 )op1 );
            break;
        case 0x31:
            sprintf( opstr,"ld sp,$%02x",op1 );
            break;
        case 0x32:
            strcpy( opstr,"ldd (hl),a" );
            break;
        case 0x33:
            strcpy( opstr,"inc sp" );
            break;
        case 0x34:
            strcpy( opstr,"inc (hl)" );
            break;
        case 0x35:
            strcpy( opstr,"dec (hl)" );
            break;
        case 0x36:
            sprintf( opstr,"ld hl,$%02x",op1 );
            break;
        case 0x37:
            strcpy( opstr,"scf" );
            break;
        case 0x38:
            sprintf( opstr,"jr c,$%04x",cpu->PC.w+2+( s8 )op1 );
            break;
        case 0x39:
            strcpy( opstr,"add hl,sp" );
            break;
        case 0x3a:
            strcpy( opstr,"ldd a,(hl)" );
            break;
        case 0x3b:
            strcpy( opstr,"dec sp" );
            break;
        case 0x3c:
            strcpy( opstr,"inc a" );
            break;
        case 0x3d:
            strcpy( opstr,"dec a" );
            break;
        case 0x3e:
            sprintf( opstr,"ld a,$%02x",op1 );
            break;
        case 0x3f:
            strcpy( opstr,"ccf" );
            break;
        case 0x40:
            strcpy( opstr,"ld b,b" );
            break;
        case 0x41:
            strcpy( opstr,"ld b,c" );
            break;
        case 0x42:
            strcpy( opstr,"ld b,d" );
            break;
        case 0x43:
            strcpy( opstr,"ld b,e" );
            break;
        case 0x44:
            strcpy( opstr,"ld b,h" );
            break;
        case 0x45:
            strcpy( opstr,"ld b,l" );
            break;
        case 0x46:
            strcpy( opstr,"ld b,(hl)" );
            break;
        case 0x47:
            strcpy( opstr,"ld b,a" );
            break;
        case 0x48:
            strcpy( opstr,"ld c,b" );
            break;
        case 0x49:
            strcpy( opstr,"ld c,c" );
            break;
        case 0x4a:
            strcpy( opstr,"ld c,d" );
            break;
        case 0x4b:
            strcpy( opstr,"ld c,e" );
            break;
        case 0x4c:
            strcpy( opstr,"ld c,h" );
            break;
        case 0x4d:
            strcpy( opstr,"ld c,l" );
            break;
        case 0x4e:
            strcpy( opstr,"ld c,(hl)" );
            break;
        case 0x4f:
            strcpy( opstr,"ld c,a" );
            break;
        case 0x50:
            strcpy( opstr,"ld d,b" );
            break;
        case 0x51:
            strcpy( opstr,"ld d,c" );
            break;
        case 0x52:
            strcpy( opstr,"ld d,d" );
            break;
        case 0x53:
            strcpy( opstr,"ld d,e" );
            break;
        case 0x54:
            strcpy( opstr,"ld d,h" );
            break;
        case 0x55:
            strcpy( opstr,"ld d,l" );
            break;
        case 0x56:
            strcpy( opstr,"ld d,(hl)" );
            break;
        case 0x57:
            strcpy( opstr,"ld d,a" );
            break;
        case 0x58:
            strcpy( opstr,"ld e,b" );
            break;
        case 0x59:
            strcpy( opstr,"ld e,c" );
            break;
        case 0x5a:
            strcpy( opstr,"ld e,d" );
            break;
        case 0x5b:
            strcpy( opstr,"ld e,e" );
            break;
        case 0x5c:
            strcpy( opstr,"ld e,h" );
            break;
        case 0x5d:
            strcpy( opstr,"ld e,l" );
            break;
        case 0x5e:
            strcpy( opstr,"ld e,(hl)" );
            break;
        case 0x5f:
            strcpy( opstr,"ld e,a" );
            break;
        case 0x60:
            strcpy( opstr,"ld h,b" );
            break;
        case 0x61:
            strcpy( opstr,"ld h,c" );
            break;
        case 0x62:
            strcpy( opstr,"ld h,d" );
            break;
        case 0x63:
            strcpy( opstr,"ld h,e" );
            break;
        case 0x64:
            strcpy( opstr,"ld h,h" );
            break;
        case 0x65:
            strcpy( opstr,"ld h,l" );
            break;
        case 0x66:
            strcpy( opstr,"ld h,(hl)" );
            break;
        case 0x67:
            strcpy( opstr,"ld h,a" );
            break;
        case 0x68:
            strcpy( opstr,"ld l,b" );
            break;
        case 0x69:
            strcpy( opstr,"ld l,c" );
            break;
        case 0x6a:
            strcpy( opstr,"ld l,d" );
            break;
        case 0x6b:
            strcpy( opstr,"ld l,e" );
            break;
        case 0x6c:
            strcpy( opstr,"ld l,h" );
            break;
        case 0x6d:
            strcpy( opstr,"ld l,l" );
            break;
        case 0x6e:
            strcpy( opstr,"ld l,(hl)" );
            break;
        case 0x6f:
            strcpy( opstr,"ld l,a" );
            break;
        case 0x70:
            strcpy( opstr,"ld (hl),b" );
            break;
        case 0x71:
            strcpy( opstr,"ld (hl),c" );
            break;
        case 0x72:
            strcpy( opstr,"ld (hl),d" );
            break;
        case 0x73:
            strcpy( opstr,"ld (hl),e" );
            break;
        case 0x74:
            strcpy( opstr,"ld (hl),h" );
            break;
        case 0x75:
            strcpy( opstr,"ld (hl),l" );
            break;
        case 0x76:
            strcpy( opstr,"halt" );
            break;
        case 0x77:
            strcpy( opstr,"ld  (hl),a" );
            break;
        case 0x78:
            strcpy( opstr,"ld a,b" );
            break;
        case 0x79:
            strcpy( opstr,"ld a,c" );
            break;
        case 0x7a:
            strcpy( opstr,"ld a,d" );
            break;
        case 0x7b:
            strcpy( opstr,"ld a,e" );
            break;
        case 0x7c:
            strcpy( opstr,"ld a,h" );
            break;
        case 0x7d:
            strcpy( opstr,"ld a,l" );
            break;
        case 0x7e:
            strcpy( opstr,"ld a,(hl)" );
            break;
        case 0x7f:
            strcpy( opstr,"ld a,a" );
            break;
        case 0x80:
            strcpy( opstr,"add b" );
            break;
        case 0x81:
            strcpy( opstr,"add c" );
            break;
        case 0x82:
            strcpy( opstr,"add d" );
            break;
        case 0x83:
            strcpy( opstr,"add e" );
            break;
        case 0x84:
            strcpy( opstr,"add h" );
            break;
        case 0x85:
            strcpy( opstr,"add l" );
            break;
        case 0x86:
            strcpy( opstr,"add (hl)" );
            break;
        case 0x87:
            strcpy( opstr,"add a" );
            break;
        case 0x88:
            strcpy( opstr,"adc b" );
            break;
        case 0x89:
            strcpy( opstr,"adc c" );
            break;
        case 0x8a:
            strcpy( opstr,"adc d" );
            break;
        case 0x8b:
            strcpy( opstr,"adc e" );
            break;
        case 0x8c:
            strcpy( opstr,"adc h" );
            break;
        case 0x8d:
            strcpy( opstr,"adc l" );
            break;
        case 0x8e:
            strcpy( opstr,"adc (hl)" );
            break;
        case 0x8f:
            strcpy( opstr,"adc a" );
            break;
        case 0x90:
            strcpy( opstr,"sub b" );
            break;
        case 0x91:
            strcpy( opstr,"sub c" );
            break;
        case 0x92:
            strcpy( opstr,"sub d" );
            break;
        case 0x93:
            strcpy( opstr,"sub e" );
            break;
        case 0x94:
            strcpy( opstr,"sub h" );
            break;
        case 0x95:
            strcpy( opstr,"sub l" );
            break;
        case 0x96:
            strcpy( opstr,"sub (hl)" );
            break;
        case 0x97:
            strcpy( opstr,"sub a" );
            break;
        case 0x98:
            strcpy( opstr,"sbc b" );
            break;
        case 0x99:
            strcpy( opstr,"sbc c" );
            break;
        case 0x9a:
            strcpy( opstr,"sbc d" );
            break;
        case 0x9b:
            strcpy( opstr,"sbc e" );
            break;
        case 0x9c:
            strcpy( opstr,"sbc h" );
            break;
        case 0x9d:
            strcpy( opstr,"sbc l" );
            break;
        case 0x9e:
            strcpy( opstr,"sbc (hl)" );
            break;
        case 0x9f:
            strcpy( opstr,"sbc a" );
            break;
        case 0xa0:
            strcpy( opstr,"and b" );
            break;
        case 0xa1:
            strcpy( opstr,"and c" );
            break;
        case 0xa2:
            strcpy( opstr,"and d" );
            break;
        case 0xa3:
            strcpy( opstr,"and e" );
            break;
        case 0xa4:
            strcpy( opstr,"and h" );
            break;
        case 0xa5:
            strcpy( opstr,"and l" );
            break;
        case 0xa6:
            strcpy( opstr,"and (hl)" );
            break;
        case 0xa7:
            strcpy( opstr,"and a" );
            break;
        case 0xa8:
            strcpy( opstr,"xor b" );
            break;
        case 0xa9:
            strcpy( opstr,"xor c" );
            break;
        case 0xaa:
            strcpy( opstr,"xor d" );
            break;
        case 0xab:
            strcpy( opstr,"xor e" );
            break;
        case 0xac:
            strcpy( opstr,"xor h" );
            break;
        case 0xad:
            strcpy( opstr,"xor l" );
            break;
        case 0xae:
            strcpy( opstr,"xor (hl)" );
            break;
        case 0xaf:
            strcpy( opstr,"xor a" );
            break;
        case 0xb0:
            strcpy( opstr,"or b" );
            break;
        case 0xb1:
            strcpy( opstr,"or c" );
            break;
        case 0xb2:
            strcpy( opstr,"or d" );
            break;
        case 0xb3:
            strcpy( opstr,"or e" );
            break;
        case 0xb4:
            strcpy( opstr,"or h" );
            break;
        case 0xb5:
            strcpy( opstr,"or l" );
            break;
        case 0xb6:
            strcpy( opstr,"or (hl)" );
            break;
        case 0xb7:
            strcpy( opstr,"or a" );
            break;
        case 0xb8:
            strcpy( opstr,"cp b" );
            break;
        case 0xb9:
            strcpy( opstr,"cp c" );
            break;
        case 0xba:
            strcpy( opstr,"cp d" );
            break;
        case 0xbb:
            strcpy( opstr,"cp e" );
            break;
        case 0xbc:
            strcpy( opstr,"cp h" );
            break;
        case 0xbd:
            strcpy( opstr,"cp l" );
            break;
        case 0xbe:
            strcpy( opstr,"cp (hl)" );
            break;
        case 0xbf:
            strcpy( opstr,"cp a" );
            break;
        case 0xc0:
            strcpy( opstr,"ret nz" );
            break;
        case 0xc1:
            strcpy( opstr,"pop bc" );
            break;
        case 0xc2:
            sprintf( opstr,"jp nz,$%04x",( ( op0<<8 )|op1 ) );
            break;
        case 0xc3:
            sprintf( opstr,"jp $%04x",( ( op0<<8 )|op1 ) );
            break;
        case 0xc4:
            sprintf( opstr,"call nz,$%04x",( ( op0<<8 )|op1 ) );
            break;
        case 0xc5:
            strcpy( opstr,"push bc" );
            break;
        case 0xc6:
            sprintf( opstr,"add $%02x",op1 );
            break;
        case 0xc7:
            strcpy( opstr,"rst 0" );
            break;
        case 0xc8:
            strcpy( opstr,"ret z" );
            break;
        case 0xc9:
            strcpy( opstr,"ret" );
            break;
        case 0xca:
            sprintf( opstr,"jp z,$%04x",( ( op0<<8 )|op1 ) );
            break;
        case 0xcb:
            disassembleCB( op1,opstr );
            break;
        case 0xcc:
            sprintf( opstr,"call z,$%04x",( ( op0<<8 )|op1 ) );
            break;
        case 0xcd:
            sprintf( opstr,"call $%04x",( ( op0<<8 )|op1 ) );
            break;
        case 0xce:
            sprintf( opstr,"adc $%02x",op1 );
            break;
        case 0xcf:
            strcpy( opstr,"rst 8" );
            break;
        case 0xd0:
            strcpy( opstr,"ret nc" );
            break;
        case 0xd1:
            strcpy( opstr,"pop de" );
            break;
        case 0xd2:
            sprintf( opstr,"jp nc,$%04x",( ( op0<<8 )|op1 ) );
            break;
        case 0xd3:
            strcpy( opstr,"-" );
            break;
        case 0xd4:
            sprintf( opstr,"call nc,$%04x",( ( op0<<8 )|op1 ) );
            break;
        case 0xd5:
            strcpy( opstr,"push de" );
            break;
        case 0xd6:
            sprintf( opstr,"sub $%02x",op1 );
            break;
        case 0xd7:
            strcpy( opstr,"rst 16" );
            break;
        case 0xd8:
            strcpy( opstr,"ret c" );
            break;
        case 0xd9:
            strcpy( opstr,"reti" );
            break;
        case 0xda:
            sprintf( opstr,"jp c,$%04x",( ( op0<<8 )|op1 ) );
            break;
        case 0xdb:
            strcpy( opstr,"-" );
            break;
        case 0xdc:
            sprintf( opstr,"call c,$%04x",( ( op0<<8 )|op1 ) );
            break;
        case 0xdd:
            strcpy( opstr,"-" );
            break;
        case 0xde:
            sprintf( opstr,"sbc $%02x",op1 );
            break;
        case 0xdf:
            strcpy( opstr,"rst 24" );
            break;
        case 0xe0:
            sprintf( opstr,"ld ($ff00+$%02x),a",op1 );
            break;
        case 0xe1:
            strcpy( opstr,"pop hl" );
            break;
        case 0xe2:
            strcpy( opstr,"ld ($ff00+c),a" );
            break;
        case 0xe3:
            strcpy( opstr,"-" );
            break;
        case 0xe4:
            strcpy( opstr,"-" );
            break;
        case 0xe5:
            strcpy( opstr,"push hl" );
            break;
        case 0xe6:
            sprintf( opstr,"and $%02x",op1 );
            break;
        case 0xe7:
            strcpy( opstr,"rst 32" );
            break;
        case 0xe8:
            sprintf( opstr,"ad sp,$%02x",op1 );
            break;
        case 0xe9:
            strcpy( opstr,"jp hl" );
            break;
        case 0xea:
            sprintf( opstr,"ld ($%04x),a",( ( op0<<8 )|op1 ) );
            break;
        case 0xeb:
            strcpy( opstr,"-" );
            break;
        case 0xec:
            strcpy( opstr,"-" );
            break;
        case 0xed:
            strcpy( opstr,"debug" );
            break;
        case 0xee:
            sprintf( opstr,"xor $%02x",op1 );
            break;
        case 0xef:
            strcpy( opstr,"rst 40" );
            break;
        case 0xf0:
            sprintf( opstr,"ld a,($ff00+$%02x)",op1 );
            break;
        case 0xf1:
            strcpy( opstr,"pop af" );
            break;
        case 0xf2:
            strcpy( opstr,"ld a,($ff00+c)" );
            break;
        case 0xf3:
            strcpy( opstr,"di" );
            break;
        case 0xf4:
            strcpy( opstr,"-" );
            break;
        case 0xf5:
            strcpy( opstr,"push af" );
            break;
        case 0xf6:
            sprintf( opstr,"or $%02x",op1 );
            break;
        case 0xf7:
            strcpy( opstr,"rst 48" );
            break;
        case 0xf8:
            sprintf( opstr,"ld hl,sp+$%02x",op1 );
            break;
        case 0xf9:
            strcpy( opstr,"ld sp,hl" );
            break;
        case 0xfa:
            sprintf( opstr,"ld a,($%04x)",( ( op0<<8 )|op1 ) );
            break;
        case 0xfb:
            strcpy( opstr,"ei" );
            break;
        case 0xfc:
            strcpy( opstr,"-" );
            break;
        case 0xfd:
            strcpy( opstr,"-" );
            break;
        case 0xfe:
            sprintf( opstr,"cp $%02x",op1 );
            break;
        case 0xff:
            strcpy( opstr,"rst 56" );
            break;

    }
    return op_l[op2];
}
void Disassembler::disassembleCB( u8 opcode, char *opstr ) {
    switch( opcode ) {
        case 0x00:
            strcpy( opstr,"rlc b" );
            break;
        case 0x01:
            strcpy( opstr,"rlc c" );
            break;
        case 0x02:
            strcpy( opstr,"rlc d" );
            break;
        case 0x03:
            strcpy( opstr,"rlc e" );
            break;
        case 0x04:
            strcpy( opstr,"rcl h" );
            break;
        case 0x05:
            strcpy( opstr,"rlc l" );
            break;
        case 0x06:
            strcpy( opstr,"rlc (hl)" );
            break;
        case 0x07:
            strcpy( opstr,"rlc a" );
            break;
        case 0x08:
            strcpy( opstr,"rrc b" );
            break;
        case 0x09:
            strcpy( opstr,"rrc c" );
            break;
        case 0x0a:
            strcpy( opstr,"rrc d" );
            break;
        case 0x0b:
            strcpy( opstr,"rrc e" );
            break;
        case 0x0c:
            strcpy( opstr,"rrc h" );
            break;
        case 0x0d:
            strcpy( opstr,"rrc l" );
            break;
        case 0x0e:
            strcpy( opstr,"rrc (hl)" );
            break;
        case 0x0f:
            strcpy( opstr,"rrc a" );
            break;
        case 0x10:
            strcpy( opstr,"rl b" );
            break;
        case 0x11:
            strcpy( opstr,"rl c" );
            break;
        case 0x12:
            strcpy( opstr,"rl d" );
            break;
        case 0x13:
            strcpy( opstr,"rl e" );
            break;
        case 0x14:
            strcpy( opstr,"rl h" );
            break;
        case 0x15:
            strcpy( opstr,"rl l" );
            break;
        case 0x16:
            strcpy( opstr,"rl (hl)" );
            break;
        case 0x17:
            strcpy( opstr,"rl a" );
            break;
        case 0x18:
            strcpy( opstr,"rr b" );
            break;
        case 0x19:
            strcpy( opstr,"rr c" );
            break;
        case 0x1a:
            strcpy( opstr,"rr d" );
            break;
        case 0x1b:
            strcpy( opstr,"rr e" );
            break;
        case 0x1c:
            strcpy( opstr,"rr h" );
            break;
        case 0x1d:
            strcpy( opstr,"rr l" );
            break;
        case 0x1e:
            strcpy( opstr,"rr (hl)" );
            break;
        case 0x1f:
            strcpy( opstr,"rr a" );
            break;
        case 0x20:
            strcpy( opstr,"sla b" );
            break;
        case 0x21:
            strcpy( opstr,"sla c" );
            break;
        case 0x22:
            strcpy( opstr,"sla d" );
            break;
        case 0x23:
            strcpy( opstr,"sla e" );
            break;
        case 0x24:
            strcpy( opstr,"sla h" );
            break;
        case 0x25:
            strcpy( opstr,"sla l" );
            break;
        case 0x26:
            strcpy( opstr,"sla (hl)" );
            break;
        case 0x27:
            strcpy( opstr,"sla a" );
            break;
        case 0x28:
            strcpy( opstr,"sra b" );
            break;
        case 0x29:
            strcpy( opstr,"sra c" );
            break;
        case 0x2a:
            strcpy( opstr,"sra d" );
            break;
        case 0x2b:
            strcpy( opstr,"sra e" );
            break;
        case 0x2c:
            strcpy( opstr,"sra h" );
            break;
        case 0x2d:
            strcpy( opstr,"sra l" );
            break;
        case 0x2e:
            strcpy( opstr,"sra (hl)" );
            break;
        case 0x2f:
            strcpy( opstr,"sra a" );
            break;
        case 0x30:
            strcpy( opstr,"swap b" );
            break;
        case 0x31:
            strcpy( opstr,"swap c" );
            break;
        case 0x32:
            strcpy( opstr,"swap d" );
            break;
        case 0x33:
            strcpy( opstr,"swap e" );
            break;
        case 0x34:
            strcpy( opstr,"swap h" );
            break;
        case 0x35:
            strcpy( opstr,"swap l" );
            break;
        case 0x36:
            strcpy( opstr,"swap (hl)" );
            break;
        case 0x37:
            strcpy( opstr,"swap a" );
            break;
        case 0x38:
            strcpy( opstr,"srl b" );
            break;
        case 0x39:
            strcpy( opstr,"srl c" );
            break;
        case 0x3a:
            strcpy( opstr,"srl d" );
            break;
        case 0x3b:
            strcpy( opstr,"srl e" );
            break;
        case 0x3c:
            strcpy( opstr,"srl h" );
            break;
        case 0x3d:
            strcpy( opstr,"srl l" );
            break;
        case 0x3e:
            strcpy( opstr,"srl (hl)" );
            break;
        case 0x3f:
            strcpy( opstr,"srl a" );
            break;
        case 0x40:
            strcpy( opstr,"bit 0,b" );
            break;
        case 0x41:
            strcpy( opstr,"bit 0,c" );
            break;
        case 0x42:
            strcpy( opstr,"bit 0,d" );
            break;
        case 0x43:
            strcpy( opstr,"bit 0,e" );
            break;
        case 0x44:
            strcpy( opstr,"bit 0,h" );
            break;
        case 0x45:
            strcpy( opstr,"bit 0,l" );
            break;
        case 0x46:
            strcpy( opstr,"bit 0,(hl)" );
            break;
        case 0x47:
            strcpy( opstr,"bit 0,a" );
            break;
        case 0x48:
            strcpy( opstr,"bit 1,b" );
            break;
        case 0x49:
            strcpy( opstr,"bit 1,c" );
            break;
        case 0x4a:
            strcpy( opstr,"bit 1,d" );
            break;
        case 0x4b:
            strcpy( opstr,"bit 1,e" );
            break;
        case 0x4c:
            strcpy( opstr,"bit 1,h" );
            break;
        case 0x4d:
            strcpy( opstr,"bit 1,l" );
            break;
        case 0x4e:
            strcpy( opstr,"bit 1,(hl)" );
            break;
        case 0x4f:
            strcpy( opstr,"bit 1,a" );
            break;
        case 0x50:
            strcpy( opstr,"bit 2,b" );
            break;
        case 0x51:
            strcpy( opstr,"bit 2,c" );
            break;
        case 0x52:
            strcpy( opstr,"bit 2,d" );
            break;
        case 0x53:
            strcpy( opstr,"bit 2,e" );
            break;
        case 0x54:
            strcpy( opstr,"bit 2,h" );
            break;
        case 0x55:
            strcpy( opstr,"bit 2,l" );
            break;
        case 0x56:
            strcpy( opstr,"bit 2,(hl)" );
            break;
        case 0x57:
            strcpy( opstr,"bit 2,a" );
            break;
        case 0x58:
            strcpy( opstr,"bit 3,b" );
            break;
        case 0x59:
            strcpy( opstr,"bit 3,c" );
            break;
        case 0x5a:
            strcpy( opstr,"bit 3,d" );
            break;
        case 0x5b:
            strcpy( opstr,"bit 3,e" );
            break;
        case 0x5c:
            strcpy( opstr,"bit 3,h" );
            break;
        case 0x5d:
            strcpy( opstr,"bit 3,l" );
            break;
        case 0x5e:
            strcpy( opstr,"bit 3,(hl)" );
            break;
        case 0x5f:
            strcpy( opstr,"bit 3,a" );
            break;
        case 0x60:
            strcpy( opstr,"bit 4,b" );
            break;
        case 0x61:
            strcpy( opstr,"bit 4,c" );
            break;
        case 0x62:
            strcpy( opstr,"bit 4,d" );
            break;
        case 0x63:
            strcpy( opstr,"bit 4,e" );
            break;
        case 0x64:
            strcpy( opstr,"bit 4,h" );
            break;
        case 0x65:
            strcpy( opstr,"bit 4,l" );
            break;
        case 0x66:
            strcpy( opstr,"bit 4,(hl)" );
            break;
        case 0x67:
            strcpy( opstr,"bit 4,a" );
            break;
        case 0x68:
            strcpy( opstr,"bit 5,b" );
            break;
        case 0x69:
            strcpy( opstr,"bit 5,c" );
            break;
        case 0x6a:
            strcpy( opstr,"bit 5,d" );
            break;
        case 0x6b:
            strcpy( opstr,"bit 5,e" );
            break;
        case 0x6c:
            strcpy( opstr,"bit 5,h" );
            break;
        case 0x6d:
            strcpy( opstr,"bit 5,l" );
            break;
        case 0x6e:
            strcpy( opstr,"bit 5,(hl)" );
            break;
        case 0x6f:
            strcpy( opstr,"bit 5,a" );
            break;
        case 0x70:
            strcpy( opstr,"bit 6,b" );
            break;
        case 0x71:
            strcpy( opstr,"bit 6,c" );
            break;
        case 0x72:
            strcpy( opstr,"bit 6,d" );
            break;
        case 0x73:
            strcpy( opstr,"bit 6,e" );
            break;
        case 0x74:
            strcpy( opstr,"bit 6,h" );
            break;
        case 0x75:
            strcpy( opstr,"bit 6,l" );
            break;
        case 0x76:
            strcpy( opstr,"bit 6,(hl)" );
            break;
        case 0x77:
            strcpy( opstr,"bit 6,a" );
            break;
        case 0x78:
            strcpy( opstr,"bit 7,b" );
            break;
        case 0x79:
            strcpy( opstr,"bit 7,c" );
            break;
        case 0x7a:
            strcpy( opstr,"bit 7,d" );
            break;
        case 0x7b:
            strcpy( opstr,"bit 7,e" );
            break;
        case 0x7c:
            strcpy( opstr,"bit 7,h" );
            break;
        case 0x7d:
            strcpy( opstr,"bit 7,l" );
            break;
        case 0x7e:
            strcpy( opstr,"bit 7,(hl)" );
            break;
        case 0x7f:
            strcpy( opstr,"bit 7,a" );
            break;
        case 0x80:
            strcpy( opstr,"res 0,b" );
            break;
        case 0x81:
            strcpy( opstr,"res 0,c" );
            break;
        case 0x82:
            strcpy( opstr,"res 0,d" );
            break;
        case 0x83:
            strcpy( opstr,"res 0,e" );
            break;
        case 0x84:
            strcpy( opstr,"res 0,h" );
            break;
        case 0x85:
            strcpy( opstr,"res 0,l" );
            break;
        case 0x86:
            strcpy( opstr,"res 0,(hl)" );
            break;
        case 0x87:
            strcpy( opstr,"res 0,a" );
            break;
        case 0x88:
            strcpy( opstr,"res 1,b" );
            break;
        case 0x89:
            strcpy( opstr,"res 1,c" );
            break;
        case 0x8a:
            strcpy( opstr,"res 1,d" );
            break;
        case 0x8b:
            strcpy( opstr,"res 1,e" );
            break;
        case 0x8c:
            strcpy( opstr,"res 1,h" );
            break;
        case 0x8d:
            strcpy( opstr,"res 1,l" );
            break;
        case 0x8e:
            strcpy( opstr,"res 1,(hl)" );
            break;
        case 0x8f:
            strcpy( opstr,"res 1,a" );
            break;
        case 0x90:
            strcpy( opstr,"res 2,b" );
            break;
        case 0x91:
            strcpy( opstr,"res 2,c" );
            break;
        case 0x92:
            strcpy( opstr,"res 2,d" );
            break;
        case 0x93:
            strcpy( opstr,"res 2,e" );
            break;
        case 0x94:
            strcpy( opstr,"res 2,h" );
            break;
        case 0x95:
            strcpy( opstr,"res 2,l" );
            break;
        case 0x96:
            strcpy( opstr,"res 2,(hl)" );
            break;
        case 0x97:
            strcpy( opstr,"res 2,a" );
            break;
        case 0x98:
            strcpy( opstr,"res 3,b" );
            break;
        case 0x99:
            strcpy( opstr,"res 3,c" );
            break;
        case 0x9a:
            strcpy( opstr,"res 3,d" );
            break;
        case 0x9b:
            strcpy( opstr,"res 3,e" );
            break;
        case 0x9c:
            strcpy( opstr,"res 3,h" );
            break;
        case 0x9d:
            strcpy( opstr,"res 3,l" );
            break;
        case 0x9e:
            strcpy( opstr,"res 3,(hl)" );
            break;
        case 0x9f:
            strcpy( opstr,"res 3,a" );
            break;
        case 0xa0:
            strcpy( opstr,"res 4,b" );
            break;
        case 0xa1:
            strcpy( opstr,"res 4,c" );
            break;
        case 0xa2:
            strcpy( opstr,"res 4,d" );
            break;
        case 0xa3:
            strcpy( opstr,"res 4,e" );
            break;
        case 0xa4:
            strcpy( opstr,"res 4,h" );
            break;
        case 0xa5:
            strcpy( opstr,"res 4,l" );
            break;
        case 0xa6:
            strcpy( opstr,"res 4,(hl)" );
            break;
        case 0xa7:
            strcpy( opstr,"res 4,a" );
            break;
        case 0xa8:
            strcpy( opstr,"res 5,b" );
            break;
        case 0xa9:
            strcpy( opstr,"res 5,c" );
            break;
        case 0xaa:
            strcpy( opstr,"res 5,d" );
            break;
        case 0xab:
            strcpy( opstr,"res 5,e" );
            break;
        case 0xac:
            strcpy( opstr,"res 5,h" );
            break;
        case 0xad:
            strcpy( opstr,"res 5,l" );
            break;
        case 0xae:
            strcpy( opstr,"res 5,(hl)" );
            break;
        case 0xaf:
            strcpy( opstr,"res 5,a" );
            break;
        case 0xb0:
            strcpy( opstr,"res 6,b" );
            break;
        case 0xb1:
            strcpy( opstr,"res 6,c" );
            break;
        case 0xb2:
            strcpy( opstr,"res 6,d" );
            break;
        case 0xb3:
            strcpy( opstr,"res 6,e" );
            break;
        case 0xb4:
            strcpy( opstr,"res 6,h" );
            break;
        case 0xb5:
            strcpy( opstr,"res 6,l" );
            break;
        case 0xb6:
            strcpy( opstr,"res 6,(hl)" );
            break;
        case 0xb7:
            strcpy( opstr,"res 6,a" );
            break;
        case 0xb8:
            strcpy( opstr,"res 7,b" );
            break;
        case 0xb9:
            strcpy( opstr,"res 7,c" );
            break;
        case 0xba:
            strcpy( opstr,"res 7,d" );
            break;
        case 0xbb:
            strcpy( opstr,"res 7,e" );
            break;
        case 0xbc:
            strcpy( opstr,"res 7,h" );
            break;
        case 0xbd:
            strcpy( opstr,"res 7,l" );
            break;
        case 0xbe:
            strcpy( opstr,"res 7,(hl)" );
            break;
        case 0xbf:
            strcpy( opstr,"res 7,a" );
            break;
        case 0xc0:
            strcpy( opstr,"set 0,b" );
            break;
        case 0xc1:
            strcpy( opstr,"set 0,c" );
            break;
        case 0xc2:
            strcpy( opstr,"set 0,d" );
            break;
        case 0xc3:
            strcpy( opstr,"set 0,e" );
            break;
        case 0xc4:
            strcpy( opstr,"set 0,h" );
            break;
        case 0xc5:
            strcpy( opstr,"set 0,l" );
            break;
        case 0xc6:
            strcpy( opstr,"set 0,(hl)" );
            break;
        case 0xc7:
            strcpy( opstr,"set 0,a" );
            break;
        case 0xc8:
            strcpy( opstr,"set 1,b" );
            break;
        case 0xc9:
            strcpy( opstr,"set 1,c" );
            break;
        case 0xca:
            strcpy( opstr,"set 1,d" );
            break;
        case 0xcb:
            strcpy( opstr,"set 1,e" );
            break;
        case 0xcc:
            strcpy( opstr,"set 1,h" );
            break;
        case 0xcd:
            strcpy( opstr,"set 1,l" );
            break;
        case 0xce:
            strcpy( opstr,"set 1,(hl)" );
            break;
        case 0xcf:
            strcpy( opstr,"set 1,a" );
            break;
        case 0xd0:
            strcpy( opstr,"set 2,b" );
            break;
        case 0xd1:
            strcpy( opstr,"set 2,c" );
            break;
        case 0xd2:
            strcpy( opstr,"set 2,d" );
            break;
        case 0xd3:
            strcpy( opstr,"set 2,e" );
            break;
        case 0xd4:
            strcpy( opstr,"set 2,h" );
            break;
        case 0xd5:
            strcpy( opstr,"set 2,l" );
            break;
        case 0xd6:
            strcpy( opstr,"set 2,(hl)" );
            break;
        case 0xd7:
            strcpy( opstr,"set 2,a" );
            break;
        case 0xd8:
            strcpy( opstr,"set 3,b" );
            break;
        case 0xd9:
            strcpy( opstr,"set 3,c" );
            break;
        case 0xda:
            strcpy( opstr,"set 3,d" );
            break;
        case 0xdb:
            strcpy( opstr,"set 3,e" );
            break;
        case 0xdc:
            strcpy( opstr,"set 3,h" );
            break;
        case 0xdd:
            strcpy( opstr,"set 3,l" );
            break;
        case 0xde:
            strcpy( opstr,"set 3,(hl)" );
            break;
        case 0xdf:
            strcpy( opstr,"set 3,a" );
            break;
        case 0xe0:
            strcpy( opstr,"set 4,b" );
            break;
        case 0xe1:
            strcpy( opstr,"set 4,c" );
            break;
        case 0xe2:
            strcpy( opstr,"set 4,d" );
            break;
        case 0xe3:
            strcpy( opstr,"set 4,e" );
            break;
        case 0xe4:
            strcpy( opstr,"set 4,h" );
            break;
        case 0xe5:
            strcpy( opstr,"set 4,l" );
            break;
        case 0xe6:
            strcpy( opstr,"set 4,(hl)" );
            break;
        case 0xe7:
            strcpy( opstr,"set 4,a" );
            break;
        case 0xe8:
            strcpy( opstr,"set 5,b" );
            break;
        case 0xe9:
            strcpy( opstr,"set 5,c" );
            break;
        case 0xea:
            strcpy( opstr,"set 5,d" );
            break;
        case 0xeb:
            strcpy( opstr,"set 5,e" );
            break;
        case 0xec:
            strcpy( opstr,"set 5,h" );
            break;
        case 0xed:
            strcpy( opstr,"set 5,l" );
            break;
        case 0xee:
            strcpy( opstr,"set 5,(hl)" );
            break;
        case 0xef:
            strcpy( opstr,"set 5,a" );
            break;
        case 0xf0:
            strcpy( opstr,"set 6,b" );
            break;
        case 0xf1:
            strcpy( opstr,"set 6,c" );
            break;
        case 0xf2:
            strcpy( opstr,"set 6,d" );
            break;
        case 0xf3:
            strcpy( opstr,"set 6,e" );
            break;
        case 0xf4:
            strcpy( opstr,"set 6,h" );
            break;
        case 0xf5:
            strcpy( opstr,"set 6,l" );
            break;
        case 0xf6:
            strcpy( opstr,"set 6,(hl)" );
            break;
        case 0xf7:
            strcpy( opstr,"set 6,a" );
            break;
        case 0xf8:
            strcpy( opstr,"set 7,b" );
            break;
        case 0xf9:
            strcpy( opstr,"set 7,c" );
            break;
        case 0xfa:
            strcpy( opstr,"set 7,d" );
            break;
        case 0xfb:
            strcpy( opstr,"set 7,e" );
            break;
        case 0xfc:
            strcpy( opstr,"set 7,h" );
            break;
        case 0xfd:
            strcpy( opstr,"set 7,l" );
            break;
        case 0xfe:
            strcpy( opstr,"set 7,(hl)" );
            break;
        case 0xff:
            strcpy( opstr,"set 7,a" );
            break;
    }
}
