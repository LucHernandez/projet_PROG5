/*
Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
	 Bâtiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/
#include "util.h"

/* We implement asr because shifting a signed is non portable in ANSI C */
uint32_t asr(uint32_t value, uint8_t shift) {
    return (value >> shift) | (get_bit(value, 31) ? ~0 << (32 - shift) : 0);
}

uint32_t ror(uint32_t value, uint8_t rotation) {
    return (value >> rotation) | (value << (32 - rotation));
}

int is_big_endian() {
    static uint32_t one = 1;
    return ((*(uint8_t *) & one) == 0);
}